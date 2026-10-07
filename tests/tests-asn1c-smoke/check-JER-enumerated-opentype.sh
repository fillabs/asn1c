#!/usr/bin/env sh
#
# Test JER encoding of ENUMERATED and CHOICE types in OPEN TYPE contexts
# Validates that:
#   1. ENUMERATED values are encoded as strings, not integers
#   2. CHOICE types are properly encoded with their selected alternative
#   3. ENUMERATED within CHOICE are encoded as strings
# This is a regression test for the F1AP JER encoding issue
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

WORKDIR="test-JER-enumerated-opentype"

# Clean up from previous runs
rm -rf "${WORKDIR}"
mkdir -p "${WORKDIR}"
cd "${WORKDIR}"

# Create ASN.1 schema similar to F1AP structure with ENUMERATED and CHOICE in IOC
cat > test-enum-ioc.asn1 << 'ENDOFASN1'
TestModule DEFINITIONS AUTOMATIC TAGS ::= BEGIN

-- Simulates F1AP-style protocol IE structure with both ENUMERATED and CHOICE types
TEST-PROTOCOL-IES ::= CLASS {
    &id         INTEGER UNIQUE,
    &criticality    Criticality,
    &Value
} WITH SYNTAX {
    ID &id
    CRITICALITY &criticality
    TYPE &Value
}

Criticality ::= ENUMERATED { reject(0), ignore(1), notify(2) }

ResetType ::= ENUMERATED {
    resetAll(0),
    partOfInterface(1)
}

Cause ::= CHOICE {
    radioNetwork    CauseRadioNetwork,
    transport       INTEGER(0..255),
    protocol        INTEGER(0..255)
}

CauseRadioNetwork ::= ENUMERATED {
    unspecified(0),
    rlFailure(1),
    unknownCell(2)
}

-- Define protocol IEs: mix of ENUMERATED and CHOICE types for comprehensive testing
TestIEs TEST-PROTOCOL-IES ::= {
    { ID 1 CRITICALITY reject TYPE ResetType } |
    { ID 2 CRITICALITY ignore TYPE Cause },
    ...
}

-- Protocol IE container
ProtocolIE ::= SEQUENCE {
    id          TEST-PROTOCOL-IES.&id({TestIEs}),
    criticality Criticality,
    value       TEST-PROTOCOL-IES.&Value({TestIEs}{@id})
}

-- Test message structure
TestMessage ::= SEQUENCE {
    protocolIEs SEQUENCE OF ProtocolIE
}

END
ENDOFASN1

# Generate C code from ASN.1 schema
if [ ! -x "${ASN1C}" ]; then
    echo "ERROR: asn1c executable not found at ${ASN1C}" >&2
    exit 1
fi

"${ASN1C}" -fcompound-names -findirect-choice -gen-JER -S "${SKELETONS_DIR}" test-enum-ioc.asn1 || {
    echo "ERROR: Failed to generate C code from ASN.1 schema" >&2
    exit 1
}

# Create test program
cat > test_program.c << 'ENDOFTEST'
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "TestMessage.h"
#include "ProtocolIE.h"
#include "ResetType.h"
#include "Cause.h"
#include "CauseRadioNetwork.h"
#include "Criticality.h"
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
    TestMessage_t *msg = NULL;
    ProtocolIE_t *ie1 = NULL, *ie2 = NULL;
    ResetType_t *reset_val = NULL;
    Cause_t *cause_val = NULL;
    asn_enc_rval_t er;
    char *output = NULL;
    int result = 0;
    int test_failures = 0;
    
    /* Allocate TestMessage */
    msg = calloc(1, sizeof(TestMessage_t));
    assert(msg != NULL);
    
    /* Test 1: ENUMERATED in Open Type (ResetType) */
    ie1 = calloc(1, sizeof(ProtocolIE_t));
    assert(ie1 != NULL);
    ie1->id = 1;
    ie1->criticality = Criticality_reject;
    
    reset_val = calloc(1, sizeof(ResetType_t));
    assert(reset_val != NULL);
    *reset_val = ResetType_partOfInterface;  /* value 1 */
    
    ie1->value.present = ProtocolIE__value_PR_ResetType;
    ie1->value.choice.ResetType = reset_val;
    
    ASN_SEQUENCE_ADD(&msg->protocolIEs, ie1);
    
    /* Test 2: CHOICE containing ENUMERATED (Cause with CauseRadioNetwork) */
    ie2 = calloc(1, sizeof(ProtocolIE_t));
    assert(ie2 != NULL);
    ie2->id = 2;
    ie2->criticality = Criticality_ignore;
    
    cause_val = calloc(1, sizeof(Cause_t));
    assert(cause_val != NULL);
    cause_val->present = Cause_PR_radioNetwork;
    cause_val->choice.radioNetwork = CauseRadioNetwork_rlFailure;  /* value 1 */
    
    ie2->value.present = ProtocolIE__value_PR_Cause;
    ie2->value.choice.Cause = cause_val;
    
    ASN_SEQUENCE_ADD(&msg->protocolIEs, ie2);
    
    /* Encode to JER */
    er = jer_encode(&asn_DEF_TestMessage, msg, 0, write_to_string, &output);
    
    if(er.encoded == -1) {
        fprintf(stderr, "ERROR: Failed to encode\n");
        result = 1;
        goto cleanup;
    }
    
    printf("=== Encoded JER ===\n%s\n", output);
    printf("===================\n\n");
    
    /* Verification 1: Check that ResetType is encoded as string, not integer 
     * Note: We check for "value": 1 specifically to avoid matching "id": 1
     * which is a legitimate integer field in the protocol IE structure */
    if(strstr(output, "\"partOfInterface\"")) {
        printf("PASS: ResetType encoded as string \"partOfInterface\"\n");
    } else if(strstr(output, "\"value\": 1") || strstr(output, "\"value\":1")) {
        fprintf(stderr, "FAIL: ResetType value encoded as integer 1 instead of \"partOfInterface\"\n");
        test_failures++;
    } else {
        fprintf(stderr, "FAIL: ResetType value \"partOfInterface\" not found in output\n");
        test_failures++;
    }
    
    /* Verification 2: Check that CHOICE (Cause) is properly encoded with selected alternative */
    if(strstr(output, "\"radioNetwork\"") && strstr(output, "\"rlFailure\"")) {
        printf("PASS: CHOICE encoded correctly with alternative \"radioNetwork\": \"rlFailure\"\n");
    } else if(strstr(output, "\"radioNetwork\": 1") || strstr(output, "\"radioNetwork\":1")) {
        fprintf(stderr, "FAIL: CHOICE alternative encoded as integer 1 instead of \"rlFailure\"\n");
        test_failures++;
    } else if(!strstr(output, "\"radioNetwork\"")) {
        fprintf(stderr, "FAIL: CHOICE alternative name \"radioNetwork\" not found\n");
        test_failures++;
    } else {
        fprintf(stderr, "FAIL: Could not find proper CHOICE encoding\n");
        test_failures++;
    }
    
    /* Verification 3: Check that Criticality is also encoded as string */
    if(strstr(output, "\"reject\"") && strstr(output, "\"ignore\"")) {
        printf("PASS: Criticality values encoded as strings\n");
    } else {
        fprintf(stderr, "FAIL: Criticality values not properly encoded as strings\n");
        test_failures++;
    }
    
cleanup:
    if(output) free(output);
    if(msg) ASN_STRUCT_FREE(asn_DEF_TestMessage, msg);
    
    if(test_failures > 0) {
        fprintf(stderr, "\n%d test(s) failed\n", test_failures);
        return 1;
    }
    
    printf("\nAll tests passed\n");
    return result;
}
ENDOFTEST

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

echo "JER ENUMERATED in Open Type test passed"
cd ..
rm -rf "${WORKDIR}"
exit 0
