#!/usr/bin/env bash
#
# Regression: inline-constrained types in IOC TYPE cells (X.681 §9.3).
#
# An IOC entry like  EXTENSION OCTET STRING (SIZE(3))  is an AMT_TYPE with a
# subtype constraint (X.680 §49).  The code generator must reference the base
# built-in descriptor (asn_DEF_OCTET_STRING), not a suffixed symbol that is
# forward-declared but never defined.  Without the fix in asn1c_ioc.c, the
# OPEN TYPE codec dereferences a zero-initialized descriptor and segfaults.
#
# Real-world trigger: 3GPP F1AP RRC-Version-ExtIEs (TS 38.473).
#

set -e
set -o pipefail

top_builddir=${top_builddir:-../..}
top_srcdir=${top_srcdir:-../..}

path_from_testdir() {
    case "$1" in
        /*) printf '%s\n' "$1" ;;
        *) printf '../%s\n' "$1" ;;
    esac
}

ASN1C="$(path_from_testdir "${top_builddir}")/asn1c/asn1c"
SKELETONS_DIR="$(path_from_testdir "${top_srcdir}")/skeletons"

testdir=test-APER-inline-constrained-ioc
cleanup() {
    rm -rf "${testdir}"
}
trap cleanup EXIT

rm -rf "${testdir}"
mkdir "${testdir}"
cd "${testdir}"

# Minimal CLASS with an inline-constrained OPEN TYPE field.
cat > test-inline-ioc.asn1 << 'EOF'
TestModule DEFINITIONS AUTOMATIC TAGS ::= BEGIN
    TESTCLASS ::= CLASS {
        &id        INTEGER UNIQUE,
        &Extension
    } WITH SYNTAX { ID &id EXTENSION &Extension }

    MyExtIEs TESTCLASS ::= {
        { ID 1 EXTENSION OCTET STRING (SIZE(3)) },
        ...
    }

    MyContainer ::= SEQUENCE {
        id        TESTCLASS.&id({MyExtIEs}),
        extension TESTCLASS.&Extension({MyExtIEs}{@id})
    }
END
EOF

"${ASN1C}" \
    -fcompound-names \
    -findirect-choice \
    -flink-skeletons \
    -S "${SKELETONS_DIR}" \
    test-inline-ioc.asn1

# APER encode + decode round-trip; segfaults (exit 139) without the fix.
cat > test_program.c << 'EOF'
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MyContainer.h"
#include "OCTET_STRING.h"
#include "aper_decoder.h"
#include "aper_encoder.h"

int main(void) {
    MyContainer_t pdu;
    MyContainer_t *decoded = NULL;
    asn_enc_rval_t er;
    asn_dec_rval_t dr;
    uint8_t encoded[128];
    size_t encoded_len;

    memset(&pdu, 0, sizeof(pdu));
    pdu.id = 1;

    pdu.extension.present = MyContainer__extension_PR_OCTET_STRING_SIZE_3_;
    if(OCTET_STRING_fromBuf(&pdu.extension.choice.OCTET_STRING_SIZE_3_, "\x01\x02\x03", 3) != 0) {
        fprintf(stderr, "OCTET_STRING_fromBuf failed\n");
        return 1;
    }

    er = aper_encode_to_buffer(&asn_DEF_MyContainer, NULL, &pdu, encoded, sizeof(encoded));
    if(er.encoded < 0) {
        fprintf(stderr, "APER encode failed\n");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_MyContainer, &pdu);
        return 1;
    }

    encoded_len = (size_t)((er.encoded + 7) / 8);
    dr = aper_decode_complete(NULL, &asn_DEF_MyContainer, (void **)&decoded, encoded, encoded_len);
    if(dr.code != RC_OK) {
        fprintf(stderr, "APER decode failed (code=%d consumed=%zu)\n", dr.code, dr.consumed);
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_MyContainer, &pdu);
        if(decoded) ASN_STRUCT_FREE(asn_DEF_MyContainer, decoded);
        return 1;
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_MyContainer, &pdu);
    ASN_STRUCT_FREE(asn_DEF_MyContainer, decoded);
    return 0;
}
EOF

${MAKE:-make} -f converter-example.mk

${CC:-cc} -DASN_PDU_COLLECTION -I. -o test_program test_program.c libasncodec.a -lm

./test_program
