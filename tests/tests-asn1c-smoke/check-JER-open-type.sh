#!/usr/bin/env sh
#
# Test JER Open Type encoding compliance with ITU-T X.697 Clause 41
# Validates that Open Type values are encoded without type name wrapper
#

set -e

top_builddir=${top_builddir:-../..}
top_srcdir=${top_srcdir:-../..}

path_from_workdir() {
    case "$1" in
        /*) printf '%s\n' "$1" ;;
        *) printf '../%s\n' "$1" ;;
    esac
}

ASN1C="$(path_from_workdir "${top_builddir}")/asn1c/asn1c"
SKELETONS_DIR="$(path_from_workdir "${top_srcdir}")/skeletons"

WORKDIR="test-JER-open-type"

# Clean up from previous runs
rm -rf "${WORKDIR}"
mkdir -p "${WORKDIR}"
cd "${WORKDIR}"

# Create ASN.1 schema with Open Type structure
cat > test-open-type.asn1 << 'EOF'
TestModule DEFINITIONS AUTOMATIC TAGS ::= BEGIN
    TEST-CLASS ::= CLASS {
        &id    INTEGER UNIQUE,
        &Type
    } WITH SYNTAX { ID &id TYPE &Type }

    TestSet TEST-CLASS ::= {
        { ID 42 TYPE TestMessage },
        ...
    }

    TestFrame ::= SEQUENCE {
        msgId   TEST-CLASS.&id({TestSet}),
        value   TEST-CLASS.&Type({TestSet}{@msgId})
    }

    TestMessage ::= SEQUENCE {
        msgCount INTEGER(0..127),
        msgData  OCTET STRING(SIZE(1..32))
    }
END
EOF

# Generate C code from ASN.1 schema
if [ ! -x "${ASN1C}" ]; then
    echo "ERROR: asn1c executable not found at ${ASN1C}" >&2
    exit 1
fi

"${ASN1C}" -fcompound-names -findirect-choice -gen-JER -S "${SKELETONS_DIR}" test-open-type.asn1 || {
    echo "ERROR: Failed to generate C code from ASN.1 schema" >&2
    exit 1
}

# Create test program
cat > test_program.c << 'EOF'
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "TestFrame.h"
#include "TestMessage.h"
#include "jer_encoder.h"

static int
write_to_string(const void *buffer, size_t size, void *app_key) {
    char **str = (char **)app_key;
    size_t current_len = *str ? strlen(*str) : 0;
    char *new_str = realloc(*str, current_len + size + 1);
    if(!new_str) return -1;
    memcpy(new_str + current_len, buffer, size);
    new_str[current_len + size] = '\0';
    *str = new_str;
    return 0;
}

int main() {
    TestFrame_t *frame = NULL;
    TestMessage_t *msg = NULL;
    asn_enc_rval_t er;
    char *output = NULL;
    int result = 0;
    
    /* Allocate TestFrame */
    frame = calloc(1, sizeof(TestFrame_t));
    assert(frame != NULL);
    frame->msgId = 42;
    
    /* Allocate TestMessage */
    msg = calloc(1, sizeof(TestMessage_t));
    assert(msg != NULL);
    msg->msgCount = 100;
    OCTET_STRING_fromBuf(&msg->msgData, "TestData", 8);
    
    /* Set Open Type value using CHOICE structure */
    frame->value.present = TestFrame__value_PR_TestMessage;
    frame->value.choice.TestMessage = msg;
    
    /* Encode to JER */
    er = jer_encode(&asn_DEF_TestFrame, frame, 0, write_to_string, &output);
    
    if(er.encoded == -1) {
        fprintf(stderr, "ERROR: Failed to encode\n");
        result = 1;
        goto cleanup;
    }
    
    /* Verify that output does NOT contain "TestMessage" wrapper */
    if(strstr(output, "\"TestMessage\"")) {
        fprintf(stderr, "FAIL: Output incorrectly contains 'TestMessage' wrapper\n");
        fprintf(stderr, "This violates ITU-T X.697 Clause 41\n");
        fprintf(stderr, "Output: %s\n", output);
        result = 1;
        goto cleanup;
    }
    
    /* Verify that output contains the expected fields */
    if(!strstr(output, "\"msgCount\"") || !strstr(output, "\"msgData\"")) {
        fprintf(stderr, "FAIL: Output missing expected fields\n");
        fprintf(stderr, "Output: %s\n", output);
        result = 1;
        goto cleanup;
    }
    
    /* Test round-trip decode to ensure decoder can handle encoder output */
    {
        TestFrame_t *decoded_frame = NULL;
        asn_dec_rval_t rval;
        
        rval = jer_decode(NULL, &asn_DEF_TestFrame, (void **)&decoded_frame, output, strlen(output));
        
        if(rval.code != RC_OK) {
            fprintf(stderr, "FAIL: Failed to decode JER that we just encoded\n");
            fprintf(stderr, "This indicates OPEN TYPE decoder doesn't handle encoder output\n");
            fprintf(stderr, "Decode result: %d, consumed: %zu\n", rval.code, rval.consumed);
            fprintf(stderr, "Output was: %s\n", output);
            result = 1;
            if(decoded_frame) ASN_STRUCT_FREE(asn_DEF_TestFrame, decoded_frame);
            goto cleanup;
        }
        
        /* Verify decoded data matches original */
        if(decoded_frame->msgId != 42) {
            fprintf(stderr, "FAIL: Decoded msgId doesn't match (got %ld, expected 42)\n", decoded_frame->msgId);
            result = 1;
            ASN_STRUCT_FREE(asn_DEF_TestFrame, decoded_frame);
            goto cleanup;
        }
        
        ASN_STRUCT_FREE(asn_DEF_TestFrame, decoded_frame);
    }
    
cleanup:
    if(output) free(output);
    if(frame) ASN_STRUCT_FREE(asn_DEF_TestFrame, frame);
    
    return result;
}
EOF

# Build the test program
${MAKE:-make} -f converter-example.mk || {
    echo "ERROR: Failed to build library" >&2
    exit 1
}

${CC:-cc} -DASN_PDU_COLLECTION -I. -o test_program test_program.c libasncodec.a -lm || {
    echo "ERROR: Failed to compile test program" >&2
    exit 1
}

# Run the test
./test_program || {
    echo "ERROR: Test failed" >&2
    exit 1
}

echo "JER Open Type test passed (including round-trip decode)"
cd ..
rm -rf "${WORKDIR}"
exit 0
