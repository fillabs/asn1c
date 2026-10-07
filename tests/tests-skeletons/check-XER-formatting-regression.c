/*
 * Test XER formatting regression tests for:
 * 1. Nested SEQUENCE closing tag indentation (should align with opening tag)
 * 2. Empty SEQUENCE_OF/SET_OF spacing (no spaces between tags)
 * 3. Primitive type closing tags (no extra spaces before closing tag)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <asn_internal.h>
#include <xer_encoder.h>
#include <INTEGER.h>
#include <OCTET_STRING.h>
#include <constr_SEQUENCE.h>
#include <constr_SEQUENCE_OF.h>

/* Inner SEQUENCE structure */
typedef struct InnerSeq {
    INTEGER_t innerField;
} InnerSeq_t;

static asn_TYPE_member_t asn_MBR_InnerSeq_1[] = {
    {
        .flags = 0,
        .name = "innerField",
        .type = &asn_DEF_INTEGER,
        .memb_offset = offsetof(InnerSeq_t, innerField),
    },
};

static const ber_tlv_tag_t asn_DEF_InnerSeq_tags_1[] = {
    (ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};

static const asn_TYPE_tag2member_t asn_MAP_InnerSeq_tag2el_1[] = {
    { (ASN_TAG_CLASS_UNIVERSAL | (2 << 2)), 0, 0, 0 }
};

static asn_SEQUENCE_specifics_t asn_SPC_InnerSeq_specs_1 = {
    .struct_size = sizeof(InnerSeq_t),
    .ctx_offset = 0,
    .tag2el = asn_MAP_InnerSeq_tag2el_1,
    .tag2el_count = 1,
    .first_extension = -1
};

static asn_TYPE_descriptor_t asn_DEF_InnerSeq = {
    .name = "InnerSeq",
    .xml_tag = "InnerSeq",
    .op = &asn_OP_SEQUENCE,
    .tags = asn_DEF_InnerSeq_tags_1,
    .tags_count = 1,
    .elements = asn_MBR_InnerSeq_1,
    .elements_count = 1,
    .specifics = &asn_SPC_InnerSeq_specs_1
};

/* SEQUENCE OF INTEGER */
typedef struct IntList {
    struct {
        INTEGER_t **array;
        int count;
        int size;
        void (*free)(INTEGER_t *);
    } list;
    asn_struct_ctx_t _asn_ctx;
} IntList_t;

static asn_TYPE_member_t asn_MBR_IntList_1[] = {
    {
        .flags = 0,
        .name = "",
        .type = &asn_DEF_INTEGER,
    },
};

static const ber_tlv_tag_t asn_DEF_IntList_tags_1[] = {
    (ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};

static asn_SET_OF_specifics_t asn_SPC_IntList_specs_1 = {
    .struct_size = sizeof(IntList_t),
    .ctx_offset = offsetof(IntList_t, _asn_ctx),
};

static asn_TYPE_descriptor_t asn_DEF_IntList = {
    .name = "IntList",
    .xml_tag = "IntList",
    .op = &asn_OP_SEQUENCE_OF,
    .tags = asn_DEF_IntList_tags_1,
    .tags_count = 1,
    .elements = asn_MBR_IntList_1,
    .elements_count = 1,
    .specifics = &asn_SPC_IntList_specs_1
};

/* Outer test structure */
typedef struct TestStruct {
    INTEGER_t primitiveField;
    InnerSeq_t *nestedSeq;
    IntList_t *emptyList;
    IntList_t *filledList;
} TestStruct_t;

static asn_TYPE_member_t asn_MBR_TestStruct_1[] = {
    {
        .flags = 0,
        .name = "primitiveField",
        .type = &asn_DEF_INTEGER,
        .memb_offset = offsetof(TestStruct_t, primitiveField),
    },
    {
        .flags = ATF_POINTER,
        .optional = 0,
        .name = "nestedSeq",
        .type = &asn_DEF_InnerSeq,
        .memb_offset = offsetof(TestStruct_t, nestedSeq),
    },
    {
        .flags = ATF_POINTER,
        .optional = 1,
        .name = "emptyList",
        .type = &asn_DEF_IntList,
        .memb_offset = offsetof(TestStruct_t, emptyList),
    },
    {
        .flags = ATF_POINTER,
        .optional = 1,
        .name = "filledList",
        .type = &asn_DEF_IntList,
        .memb_offset = offsetof(TestStruct_t, filledList),
    },
};

static const ber_tlv_tag_t asn_DEF_TestStruct_tags_1[] = {
    (ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};

static const asn_TYPE_tag2member_t asn_MAP_TestStruct_tag2el_1[] = {
    { (ASN_TAG_CLASS_UNIVERSAL | (2 << 2)), 0, 0, 0 },
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 1, 0, 2 },
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 2, -1, 1 },
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 3, -2, 0 }
};

static asn_SEQUENCE_specifics_t asn_SPC_TestStruct_specs_1 = {
    .struct_size = sizeof(TestStruct_t),
    .ctx_offset = 0,
    .tag2el = asn_MAP_TestStruct_tag2el_1,
    .tag2el_count = 4,
    .first_extension = -1
};

static asn_TYPE_descriptor_t asn_DEF_TestStruct = {
    .name = "TestStruct",
    .xml_tag = "TestStruct",
    .op = &asn_OP_SEQUENCE,
    .tags = asn_DEF_TestStruct_tags_1,
    .tags_count = 1,
    .elements = asn_MBR_TestStruct_1,
    .elements_count = 4,
    .specifics = &asn_SPC_TestStruct_specs_1
};

/* Buffer to capture XER output */
struct xer_buffer {
    char *buffer;
    size_t buffer_size;
    size_t allocated_size;
};

static int
xer_buffer_append(const void *buffer, size_t size, void *app_key) {
    struct xer_buffer *xb = app_key;
    
    while(xb->buffer_size + size + 1 > xb->allocated_size) {
        size_t new_size = 2 * (xb->allocated_size ? xb->allocated_size : 256);
        char *new_buf = MALLOC(new_size);
        if(!new_buf) return -1;
        if(xb->buffer) {
            memcpy(new_buf, xb->buffer, xb->buffer_size);
            FREEMEM(xb->buffer);
        }
        xb->buffer = new_buf;
        xb->allocated_size = new_size;
    }
    
    memcpy(xb->buffer + xb->buffer_size, buffer, size);
    xb->buffer_size += size;
    xb->buffer[xb->buffer_size] = '\0';
    return 0;
}

/* Test 1: Primitive field should have no spaces before closing tag */
static void
test_primitive_no_spaces(void) {
    printf("Test 1: Primitive field closing tag formatting...\n");
    
    TestStruct_t test;
    InnerSeq_t inner;
    struct xer_buffer xb = {0, 0, 0};
    asn_enc_rval_t er;
    
    memset(&test, 0, sizeof(test));
    memset(&inner, 0, sizeof(inner));
    
    asn_long2INTEGER(&test.primitiveField, 42);
    asn_long2INTEGER(&inner.innerField, 99);
    test.nestedSeq = &inner;
    
    er = xer_encode(&asn_DEF_TestStruct, &test, XER_F_BASIC, xer_buffer_append, &xb);
    assert(er.encoded > 0);
    
    printf("Output:\n%s\n", xb.buffer);
    
    /* Check that primitive field closing tag has no spaces before it */
    /* Looking for pattern: <primitiveField>42</primitiveField> (no spaces between 42 and </) */
    char *primitive = strstr(xb.buffer, "42</primitiveField>");
    if(!primitive) {
        fprintf(stderr, "FAIL: Primitive field has extra spaces before closing tag!\n");
        fprintf(stderr, "Expected: <primitiveField>42</primitiveField>\n");
        FREEMEM(xb.buffer);
        exit(1);
    }
    
    /* Also check that primitive closing tag is followed by newline */
    char *primitive_close = strstr(xb.buffer, "</primitiveField>");
    assert(primitive_close != NULL);
    if(primitive_close[strlen("</primitiveField>")] != '\n') {
        fprintf(stderr, "FAIL: Primitive closing tag not followed by newline!\n");
        FREEMEM(xb.buffer);
        exit(1);
    }
    
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_INTEGER, &test.primitiveField);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_INTEGER, &inner.innerField);
    FREEMEM(xb.buffer);
    
    printf("PASS: Primitive field correctly formatted\n\n");
}

/* Test 2: Nested SEQUENCE closing tag should be indented to align with opening tag */
static void
test_nested_indentation(void) {
    printf("Test 2: Nested SEQUENCE closing tag indentation...\n");
    
    TestStruct_t test;
    InnerSeq_t inner;
    struct xer_buffer xb = {0, 0, 0};
    asn_enc_rval_t er;
    
    memset(&test, 0, sizeof(test));
    memset(&inner, 0, sizeof(inner));
    
    asn_long2INTEGER(&test.primitiveField, 42);
    asn_long2INTEGER(&inner.innerField, 99);
    test.nestedSeq = &inner;
    
    er = xer_encode(&asn_DEF_TestStruct, &test, XER_F_BASIC, xer_buffer_append, &xb);
    assert(er.encoded > 0);
    
    printf("Output:\n%s\n", xb.buffer);
    
    /* Check that nested SEQUENCE closing tag is indented correctly */
    /* Should be:    <nestedSeq>\n        <innerField>99</innerField>\n    </nestedSeq> */
    /* NOT:          <nestedSeq>\n        <innerField>99</innerField>\n        </nestedSeq> */
    
    /* Find the nestedSeq closing tag */
    char *nested_close = strstr(xb.buffer, "</nestedSeq>");
    assert(nested_close != NULL);
    
    /* Check indentation: should have 4 spaces before </nestedSeq> (1 level) */
    /* The pattern should be: \n    </nestedSeq> */
    /* Find the newline before the closing tag */
    char *newline_before = nested_close - 1;
    while(newline_before > xb.buffer && *newline_before != '\n') {
        newline_before--;
    }
    
    if(*newline_before != '\n') {
        fprintf(stderr, "FAIL: No newline before nested SEQUENCE closing tag!\n");
        FREEMEM(xb.buffer);
        exit(1);
    }
    
    /* Count spaces between newline and closing tag */
    int spaces = nested_close - newline_before - 1;
    if(spaces != 4) {
        fprintf(stderr, "FAIL: Nested SEQUENCE closing tag has %d spaces, expected 4!\n", spaces);
        fprintf(stderr, "Got: '");
        for(char *p = newline_before; p < nested_close + 13; p++) {
            if(*p == '\n') fprintf(stderr, "\\n");
            else if(*p == ' ') fprintf(stderr, "_");
            else fprintf(stderr, "%c", *p);
        }
        fprintf(stderr, "'\n");
        FREEMEM(xb.buffer);
        exit(1);
    }
    
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_INTEGER, &test.primitiveField);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_INTEGER, &inner.innerField);
    FREEMEM(xb.buffer);
    
    printf("PASS: Nested SEQUENCE closing tag correctly indented\n\n");
}

/* Test 3: Empty SEQUENCE_OF should have no spaces between tags */
static void
test_empty_list_no_spaces(void) {
    printf("Test 3: Empty SEQUENCE_OF spacing...\n");
    
    TestStruct_t test;
    InnerSeq_t inner;
    IntList_t empty_list;
    struct xer_buffer xb = {0, 0, 0};
    asn_enc_rval_t er;
    
    memset(&test, 0, sizeof(test));
    memset(&inner, 0, sizeof(inner));
    memset(&empty_list, 0, sizeof(empty_list));
    
    asn_long2INTEGER(&test.primitiveField, 42);
    asn_long2INTEGER(&inner.innerField, 99);
    test.nestedSeq = &inner;
    
    /* Set empty list */
    empty_list.list.count = 0;
    empty_list.list.array = NULL;
    test.emptyList = &empty_list;
    
    er = xer_encode(&asn_DEF_TestStruct, &test, XER_F_BASIC, xer_buffer_append, &xb);
    assert(er.encoded > 0);
    
    printf("Output:\n%s\n", xb.buffer);
    
    /* Check that empty list has no spaces between tags */
    /* Should be: <emptyList></emptyList> */
    /* NOT: <emptyList>        </emptyList> or <emptyList>\n    </emptyList> */
    char *empty_tag = strstr(xb.buffer, "<emptyList></emptyList>");
    if(!empty_tag) {
        fprintf(stderr, "FAIL: Empty list has spaces between tags!\n");
        fprintf(stderr, "Expected: <emptyList></emptyList>\n");
        
        /* Show what we have */
        char *start = strstr(xb.buffer, "<emptyList>");
        if(start) {
            char *end = strstr(start, "</emptyList>");
            if(end) {
                fprintf(stderr, "Got: '");
                for(char *p = start; p <= end + strlen("</emptyList>") - 1; p++) {
                    if(*p == '\n') fprintf(stderr, "\\n");
                    else if(*p == ' ') fprintf(stderr, "_");
                    else fprintf(stderr, "%c", *p);
                }
                fprintf(stderr, "'\n");
            }
        }
        FREEMEM(xb.buffer);
        exit(1);
    }
    
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_INTEGER, &test.primitiveField);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_INTEGER, &inner.innerField);
    FREEMEM(xb.buffer);
    
    printf("PASS: Empty list has no spaces between tags\n\n");
}

/* Test 4: Filled SEQUENCE_OF should have proper indentation */
static void
test_filled_list_indentation(void) {
    printf("Test 4: Filled SEQUENCE_OF indentation...\n");
    
    TestStruct_t test;
    InnerSeq_t inner;
    IntList_t filled_list;
    INTEGER_t int1, int2;
    INTEGER_t *int_array[2];
    struct xer_buffer xb = {0, 0, 0};
    asn_enc_rval_t er;
    
    memset(&test, 0, sizeof(test));
    memset(&inner, 0, sizeof(inner));
    memset(&filled_list, 0, sizeof(filled_list));
    memset(&int1, 0, sizeof(int1));
    memset(&int2, 0, sizeof(int2));
    
    asn_long2INTEGER(&test.primitiveField, 42);
    asn_long2INTEGER(&inner.innerField, 99);
    test.nestedSeq = &inner;
    
    /* Set filled list */
    asn_long2INTEGER(&int1, 10);
    asn_long2INTEGER(&int2, 20);
    int_array[0] = &int1;
    int_array[1] = &int2;
    filled_list.list.count = 2;
    filled_list.list.array = int_array;
    test.filledList = &filled_list;
    
    er = xer_encode(&asn_DEF_TestStruct, &test, XER_F_BASIC, xer_buffer_append, &xb);
    assert(er.encoded > 0);
    
    printf("Output:\n%s\n", xb.buffer);
    
    /* Check that filled list closing tag is indented correctly */
    /* The pattern should be: \n    </filledList> (4 spaces) */
    char *filled_close = strstr(xb.buffer, "</filledList>");
    assert(filled_close != NULL);
    
    /* Find the newline before the closing tag */
    char *newline_before = filled_close - 1;
    while(newline_before > xb.buffer && *newline_before != '\n') {
        newline_before--;
    }
    
    if(*newline_before != '\n') {
        fprintf(stderr, "FAIL: No newline before filled list closing tag!\n");
        FREEMEM(xb.buffer);
        exit(1);
    }
    
    /* Count spaces between newline and closing tag */
    int spaces = filled_close - newline_before - 1;
    if(spaces != 4) {
        fprintf(stderr, "FAIL: Filled list closing tag has %d spaces, expected 4!\n", spaces);
        FREEMEM(xb.buffer);
        exit(1);
    }
    
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_INTEGER, &test.primitiveField);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_INTEGER, &inner.innerField);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_INTEGER, &int1);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_INTEGER, &int2);
    FREEMEM(xb.buffer);
    
    printf("PASS: Filled list closing tag correctly indented\n\n");
}

int
main(void) {
    test_primitive_no_spaces();
    test_nested_indentation();
    test_empty_list_no_spaces();
    test_filled_list_indentation();
    
    printf("========================================\n");
    printf("All XER formatting regression tests PASSED\n");
    printf("========================================\n");
    
    return 0;
}
