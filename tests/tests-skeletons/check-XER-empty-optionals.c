#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*
 * This test requires compiling with -DXER_ALLOW_EMPTY_OPTIONALS to enable
 * empty optional field handling. Without the flag, empty optional fields
 * will cause decoder failures.
 */

#if XER_EMPTY_OPTIONALS_ENABLED

/* Include generated types from test-xer-empty-optionals.asn1 */
/* We'll test the feature using simple inline structures */

#include <asn_internal.h>
#include <INTEGER.h>
#include <NativeInteger.h>
#include <NativeEnumerated.h>
#include <OCTET_STRING.h>
#include <BIT_STRING.h>
#include <constr_SEQUENCE.h>
#include <xer_decoder.h>

/* Simple test structure definition */
typedef struct TestOptionalInt {
    long mandatoryInt;
    long *optionalInt;  /* OPTIONAL */
} TestOptionalInt_t;

static asn_TYPE_member_t asn_MBR_TestOptionalInt_1[] = {
    { ATF_NOFLAGS, 0, offsetof(struct TestOptionalInt, mandatoryInt),
        .tag = (ASN_TAG_CLASS_CONTEXT | (0 << 2)),
        .tag_mode = 0,
        .type = &asn_DEF_NativeInteger,
        .type_selector = 0,
        .memb_constraints = 0,
        .oer_constraints = 0,
        .per_constraints = 0,
        .default_value = 0,
        .name = "mandatoryInt"
    },
    { ATF_POINTER, 1, offsetof(struct TestOptionalInt, optionalInt),
        .tag = (ASN_TAG_CLASS_CONTEXT | (1 << 2)),
        .tag_mode = 0,
        .type = &asn_DEF_NativeInteger,
        .type_selector = 0,
        .memb_constraints = 0,
        .oer_constraints = 0,
        .per_constraints = 0,
        .default_value = 0,
        .name = "optionalInt"
    },
};
static const ber_tlv_tag_t asn_DEF_TestOptionalInt_tags_1[] = {
    (ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_TestOptionalInt_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 },
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }
};
static asn_SEQUENCE_specifics_t asn_SPC_TestOptionalInt_specs_1 = {
    sizeof(struct TestOptionalInt),
    offsetof(struct TestOptionalInt, _asn_ctx),
    asn_MAP_TestOptionalInt_tag2el_1,
    2,
    0, 0, 0,
    -1,
    2
};
asn_TYPE_descriptor_t asn_DEF_TestOptionalInt = {
    "TestOptionalInt",
    "TestOptionalInt",
    &asn_OP_SEQUENCE,
    asn_DEF_TestOptionalInt_tags_1,
    sizeof(asn_DEF_TestOptionalInt_tags_1)
        /sizeof(asn_DEF_TestOptionalInt_tags_1[0]),
    asn_DEF_TestOptionalInt_tags_1,
    sizeof(asn_DEF_TestOptionalInt_tags_1)
        /sizeof(asn_DEF_TestOptionalInt_tags_1[0]),
    0, 0,
    asn_MBR_TestOptionalInt_1,
    2,
    &asn_SPC_TestOptionalInt_specs_1
};

static void
test_empty_optional_self_closing(void) {
    const char *xer_data = 
        "<TestOptionalInt>\n"
        "  <mandatoryInt>42</mandatoryInt>\n"
        "  <optionalInt/>\n"
        "</TestOptionalInt>\n";
    
    TestOptionalInt_t *st = NULL;
    asn_dec_rval_t rval;
    
    printf("Test 1: Self-closing empty optional INTEGER...\n");
    rval = xer_decode(NULL, &asn_DEF_TestOptionalInt, (void **)&st,
                      xer_data, strlen(xer_data));
    
    if(rval.code != RC_OK) {
        fprintf(stderr, "FAILED: Decoding failed with code %d\n", rval.code);
        if(st) ASN_STRUCT_FREE(asn_DEF_TestOptionalInt, st);
        exit(1);
    }
    
    assert(st->mandatoryInt == 42);
    assert(st->optionalInt == NULL);  /* Should be absent */
    printf("PASS: Self-closing empty optional treated as absent\n");
    
    ASN_STRUCT_FREE(asn_DEF_TestOptionalInt, st);
}

static void
test_empty_optional_separate_tags(void) {
    const char *xer_data = 
        "<TestOptionalInt>\n"
        "  <mandatoryInt>99</mandatoryInt>\n"
        "  <optionalInt></optionalInt>\n"
        "</TestOptionalInt>\n";
    
    TestOptionalInt_t *st = NULL;
    asn_dec_rval_t rval;
    
    printf("Test 2: Separate empty tags for optional INTEGER...\n");
    rval = xer_decode(NULL, &asn_DEF_TestOptionalInt, (void **)&st,
                      xer_data, strlen(xer_data));
    
    if(rval.code != RC_OK) {
        fprintf(stderr, "FAILED: Decoding failed with code %d\n", rval.code);
        if(st) ASN_STRUCT_FREE(asn_DEF_TestOptionalInt, st);
        exit(1);
    }
    
    assert(st->mandatoryInt == 99);
    assert(st->optionalInt == NULL);  /* Should be absent */
    printf("PASS: Separate empty tags treated as absent\n");
    
    ASN_STRUCT_FREE(asn_DEF_TestOptionalInt, st);
}

static void
test_present_optional(void) {
    const char *xer_data = 
        "<TestOptionalInt>\n"
        "  <mandatoryInt>77</mandatoryInt>\n"
        "  <optionalInt>123</optionalInt>\n"
        "</TestOptionalInt>\n";
    
    TestOptionalInt_t *st = NULL;
    asn_dec_rval_t rval;
    
    printf("Test 3: Present optional with value...\n");
    rval = xer_decode(NULL, &asn_DEF_TestOptionalInt, (void **)&st,
                      xer_data, strlen(xer_data));
    
    if(rval.code != RC_OK) {
        fprintf(stderr, "FAILED: Decoding failed with code %d\n", rval.code);
        if(st) ASN_STRUCT_FREE(asn_DEF_TestOptionalInt, st);
        exit(1);
    }
    
    assert(st->mandatoryInt == 77);
    assert(st->optionalInt != NULL);
    assert(*st->optionalInt == 123);
    printf("PASS: Present optional decoded correctly\n");
    
    ASN_STRUCT_FREE(asn_DEF_TestOptionalInt, st);
}

static void
test_absent_optional(void) {
    const char *xer_data = 
        "<TestOptionalInt>\n"
        "  <mandatoryInt>55</mandatoryInt>\n"
        "</TestOptionalInt>\n";
    
    TestOptionalInt_t *st = NULL;
    asn_dec_rval_t rval;
    
    printf("Test 4: Absent optional (no tag)...\n");
    rval = xer_decode(NULL, &asn_DEF_TestOptionalInt, (void **)&st,
                      xer_data, strlen(xer_data));
    
    if(rval.code != RC_OK) {
        fprintf(stderr, "FAILED: Decoding failed with code %d\n", rval.code);
        if(st) ASN_STRUCT_FREE(asn_DEF_TestOptionalInt, st);
        exit(1);
    }
    
    assert(st->mandatoryInt == 55);
    assert(st->optionalInt == NULL);  /* Should be absent */
    printf("PASS: Absent optional handled correctly\n");
    
    ASN_STRUCT_FREE(asn_DEF_TestOptionalInt, st);
}

static void
test_mandatory_field_empty_should_fail(void) {
    const char *xer_data = 
        "<TestOptionalInt>\n"
        "  <mandatoryInt></mandatoryInt>\n"
        "</TestOptionalInt>\n";
    
    TestOptionalInt_t *st = NULL;
    asn_dec_rval_t rval;
    
    printf("Test 5: Empty mandatory field (should fail)...\n");
    rval = xer_decode(NULL, &asn_DEF_TestOptionalInt, (void **)&st,
                      xer_data, strlen(xer_data));
    
    if(rval.code == RC_OK) {
        fprintf(stderr, "FAILED: Empty mandatory field should have failed but succeeded\n");
        if(st) ASN_STRUCT_FREE(asn_DEF_TestOptionalInt, st);
        exit(1);
    }
    
    printf("PASS: Empty mandatory field correctly rejected\n");
    if(st) ASN_STRUCT_FREE(asn_DEF_TestOptionalInt, st);
}

int
main() {
    printf("Running XER empty optional field tests (XER_ALLOW_EMPTY_OPTIONALS enabled)...\n\n");
    
    test_empty_optional_self_closing();
    test_empty_optional_separate_tags();
    test_present_optional();
    test_absent_optional();
    test_mandatory_field_empty_should_fail();
    
    printf("\nAll tests PASSED!\n");
    return 0;
}

#else

int
main() {
    printf("Test skipped: XER_ALLOW_EMPTY_OPTIONALS not enabled\n");
    printf("Compile with -DXER_ALLOW_EMPTY_OPTIONALS to run this test\n");
    return 0;
}

#endif
