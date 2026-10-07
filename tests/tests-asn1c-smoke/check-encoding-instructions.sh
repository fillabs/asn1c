#!/usr/bin/env sh

set -e

top_builddir=${top_builddir:-../..}
top_srcdir=${top_srcdir:-../..}
WORKDIR="test-encoding-instructions"

path_from_workdir() {
    case "$1" in
        /*) printf '%s\n' "$1" ;;
        *) printf '../%s\n' "$1" ;;
    esac
}

ASN1C="$(path_from_workdir "${top_builddir}")/asn1c/asn1c"
SKELETONS_DIR="$(path_from_workdir "${top_srcdir}")/skeletons"

rm -rf "${WORKDIR}"
mkdir -p "${WORKDIR}"
cd "${WORKDIR}"

cat > test.asn1 << 'ENDOFASN1'
EncodingInstructions DEFINITIONS AUTOMATIC TAGS ::=
BEGIN

Flag ::= [TEXT] BOOLEAN
Mode ::= [XER:TEXT] ENUMERATED { idle(0), busy(1) }
Count ::= [TEXT] INTEGER { one(1), two(2) }
Bits ::= [TEXT] BIT STRING { alpha(0), beta(2) }
Blob ::= [JER:BASE64] OCTET STRING
Ratio ::= REAL

Packet ::= SEQUENCE {
    payload Blob,
    mode Mode,
    count Count
}

ENCODING-CONTROL XER
    GLOBAL-DEFAULTS MODIFIED-ENCODINGS
    DECIMAL Ratio
    TEXT Count.one AS "uno"
END

ENCODING-CONTROL JER
    NAME Packet.payload AS "payload64"
    TEXT Mode.busy AS "occupied"
END

END
ENDOFASN1

"${ASN1C}" -fcompound-names -gen-JER -no-gen-OER -no-gen-UPER -no-gen-APER -no-gen-CBOR -S "${SKELETONS_DIR}" test.asn1

cat > test_program.c << 'ENDOFTEST'
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Flag.h"
#include "Mode.h"
#include "Count.h"
#include "Bits.h"
#include "Ratio.h"
#include "Packet.h"
#include "xer_encoder.h"
#include "xer_decoder.h"
#include "jer_encoder.h"
#include "jer_decoder.h"

static int
append_cb(const void *buffer, size_t size, void *app_key) {
    char **out = (char **)app_key;
    size_t old = *out ? strlen(*out) : 0;
    char *tmp = realloc(*out, old + size + 1);
    if(!tmp) return -1;
    memcpy(tmp + old, buffer, size);
    tmp[old + size] = 0;
    *out = tmp;
    return 0;
}

static char *
encode_xer_value(const asn_TYPE_descriptor_t *td, const void *sptr) {
    asn_enc_rval_t er;
    char *out = 0;
    er = xer_encode(td, sptr, XER_F_BASIC, append_cb, &out);
    assert(er.encoded >= 0);
    assert(out);
    return out;
}

static char *
encode_jer_value(const asn_TYPE_descriptor_t *td, const void *sptr) {
    asn_enc_rval_t er;
    char *out = 0;
    er = jer_encode(td, sptr, JER_F_MINIFIED, append_cb, &out);
    assert(er.encoded >= 0);
    assert(out);
    return out;
}

static void
expect_xer_decode_failure(const asn_TYPE_descriptor_t *td, const char *xml) {
    void *decoded = 0;
    asn_dec_rval_t rv = xer_decode(0, td, &decoded, xml, strlen(xml));
    if(decoded) ASN_STRUCT_FREE(*td, decoded);
    assert(rv.code != RC_OK);
}

static void
expect_jer_decode_failure(const char *json) {
    Packet_t *decoded = 0;
    asn_dec_rval_t rv = jer_decode(0, &asn_DEF_Packet, (void **)&decoded,
                                   json, strlen(json));
    if(decoded) ASN_STRUCT_FREE(asn_DEF_Packet, decoded);
    assert(rv.code != RC_OK);
}

int
main(void) {
    Flag_t flag = 1;
    Mode_t mode = Mode_busy;
    Count_t count = Count_one;
    Bits_t bits;
    Ratio_t ratio = 12.5;
    Packet_t packet;
    Packet_t *decoded = 0;
    asn_dec_rval_t rv;
    char *xer = 0;
    char *jer = 0;
    const char payload[] = { 'M', 'a' };

    memset(&packet, 0, sizeof(packet));
    memset(&bits, 0, sizeof(bits));
    assert(OCTET_STRING_fromBuf((OCTET_STRING_t *)&bits, "\240", 1) == 0);
    bits.bits_unused = 5;
    assert(OCTET_STRING_fromBuf(&packet.payload, payload, sizeof(payload)) == 0);
    packet.mode = Mode_busy;
    packet.count = Count_one;

    xer = encode_xer_value(&asn_DEF_Flag, &flag);
    assert(strstr(xer, "true"));
    assert(!strstr(xer, "<true/>"));
    free(xer);

    xer = encode_xer_value(&asn_DEF_Mode, &mode);
    assert(strstr(xer, "occupied"));
    assert(!strstr(xer, "<busy/>"));
    free(xer);

    xer = encode_xer_value(&asn_DEF_Count, &count);
    assert(strstr(xer, "uno"));
    free(xer);

    xer = encode_xer_value(&asn_DEF_Bits, &bits);
    assert(strstr(xer, "alpha beta"));
    assert(!strstr(xer, "101"));
    free(xer);

    xer = encode_xer_value(&asn_DEF_Ratio, &ratio);
    assert(strstr(xer, "12.5"));
    free(xer);
    expect_xer_decode_failure(&asn_DEF_Ratio, "<Ratio>1abc</Ratio>");
    expect_xer_decode_failure(&asn_DEF_Ratio, "<Ratio><PLUS-INFINITY/></Ratio>");

    jer = encode_jer_value(&asn_DEF_Packet, &packet);
    assert(strstr(jer, "\"payload64\":\"TWE=\""));
    assert(!strstr(jer, "\"payload\""));
    assert(strstr(jer, "\"mode\":\"occupied\""));

    rv = jer_decode(0, &asn_DEF_Packet, (void **)&decoded, jer, strlen(jer));
    assert(rv.code == RC_OK);
    assert(decoded);
    assert(decoded->payload.size == 2);
    assert(memcmp(decoded->payload.buf, payload, 2) == 0);
    assert(decoded->mode == Mode_busy);
    assert(decoded->count == Count_one);
    ASN_STRUCT_FREE(asn_DEF_Packet, decoded);
    decoded = 0;

    expect_jer_decode_failure("{\"payload64\":\"!!!!\",\"mode\":\"occupied\",\"count\":1}");
    expect_jer_decode_failure("{\"payload64\":\"TWE=\",\"mode\":\"missing\",\"count\":1}");
    expect_jer_decode_failure("{\"payload\":\"TWE=\",\"mode\":\"occupied\",\"count\":1}");

    free(jer);
    ASN_STRUCT_RESET(asn_DEF_Bits, &bits);
    ASN_STRUCT_RESET(asn_DEF_Packet, &packet);
    return 0;
}
ENDOFTEST

${MAKE:-make} -f converter-example.mk
${CC:-cc} -DASN_PDU_COLLECTION -I. -o test_program test_program.c libasncodec.a -lm
./test_program

expect_fail() {
    name=$1
    body=$2
    printf '%s\n' "${body}" > "${name}.asn1"
    if "${ASN1C}" -S "${SKELETONS_DIR}" -P "${name}.asn1" > "${name}.out" 2> "${name}.err"; then
        echo "ERROR: ${name}.asn1 unexpectedly compiled" >&2
        exit 1
    fi
}

expect_fail bad-xer-text-octet 'Bad DEFINITIONS ::= BEGIN
S ::= OCTET STRING
ENCODING-CONTROL XER
    TEXT S
END
END'

expect_fail bad-xer-decimal-type 'Bad DEFINITIONS ::= BEGIN
S ::= INTEGER
ENCODING-CONTROL XER
    GLOBAL-DEFAULTS MODIFIED-ENCODINGS
    DECIMAL S
END
END'

expect_fail bad-xer-decimal-default 'Bad DEFINITIONS ::= BEGIN
S ::= REAL
ENCODING-CONTROL XER
    DECIMAL S
END
END'

expect_fail bad-jer-base64-type 'Bad DEFINITIONS ::= BEGIN
S ::= INTEGER
ENCODING-CONTROL JER
    BASE64 S
END
END'

expect_fail bad-jer-text-type 'Bad DEFINITIONS ::= BEGIN
S ::= INTEGER { one(1) }
ENCODING-CONTROL JER
    TEXT S.one AS "uno"
END
END'

expect_fail bad-jer-name-duplicate 'Bad DEFINITIONS ::= BEGIN
S ::= SEQUENCE { a INTEGER, b INTEGER }
ENCODING-CONTROL JER
    NAME S.a AS "x"
    NAME S.b AS "x"
END
END'

cd ..
rm -rf "${WORKDIR}"
exit 0
