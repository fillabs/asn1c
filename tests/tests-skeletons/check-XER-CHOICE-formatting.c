/*
 * Test XER formatting for CHOICE types to ensure proper newlines after closing tags
 * (regression test for issue with mashed tags in CHOICE encoding).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <asn_internal.h>
#include <xer_encoder.h>
#include <INTEGER.h>
#include <IA5String.h>
#include <constr_CHOICE.h>
#include <constr_SEQUENCE.h>

/* Test CHOICE structure */
typedef struct TestChoice {
    enum {
        TestChoice_PR_NOTHING,
        TestChoice_PR_intValue,
        TestChoice_PR_strValue
    } present;
    union TestChoice_u {
        INTEGER_t intValue;
        IA5String_t strValue;
    } choice;
} TestChoice_t;

/* Test container with CHOICE */
typedef struct TestContainer {
    TestChoice_t myChoice;
} TestContainer_t;

/* ASN.1 type descriptor for TestChoice */
static asn_TYPE_member_t asn_MBR_TestChoice_1[] = {
    {
        .flags = 0,
        .name = "intValue",
        .type = &asn_DEF_INTEGER,
        .memb_offset = offsetof(TestChoice_t, choice.intValue),
    },
    {
        .flags = 0,
        .name = "strValue",
        .type = &asn_DEF_IA5String,
        .memb_offset = offsetof(TestChoice_t, choice.strValue),
    },
};

static const asn_TYPE_tag2member_t asn_MAP_TestChoice_tag2el_1[] = {
    { (ASN_TAG_CLASS_UNIVERSAL | (2 << 2)), 0, 0, 0 },
    { (ASN_TAG_CLASS_UNIVERSAL | (22 << 2)), 1, 0, 0 }
};

static asn_CHOICE_specifics_t asn_SPC_TestChoice_specs_1 = {
    .struct_size = sizeof(TestChoice_t),
    .ctx_offset = 0,
    .pres_offset = offsetof(TestChoice_t, present),
    .pres_size = sizeof(((TestChoice_t *)0)->present),
    .tag2el = asn_MAP_TestChoice_tag2el_1,
    .tag2el_count = 2,
    .to_canonical_order = NULL,
    .from_canonical_order = NULL,
    .ext_start = -1
};

static const ber_tlv_tag_t asn_DEF_TestChoice_tags_1[] = {
    (ASN_TAG_CLASS_CONTEXT | (0 << 2))
};

asn_TYPE_descriptor_t asn_DEF_TestChoice = {
    .name = "TestChoice",
    .xml_tag = "TestChoice",
    .op = &asn_OP_CHOICE,
    .tags = asn_DEF_TestChoice_tags_1,
    .tags_count = 1,
    .elements = asn_MBR_TestChoice_1,
    .elements_count = 2,
    .specifics = &asn_SPC_TestChoice_specs_1
};

/* ASN.1 type descriptor for TestContainer */
static asn_TYPE_member_t asn_MBR_TestContainer_1[] = {
    {
        .flags = 0,
        .name = "myChoice",
        .type = &asn_DEF_TestChoice,
        .memb_offset = offsetof(TestContainer_t, myChoice),
    },
};

static const ber_tlv_tag_t asn_DEF_TestContainer_tags_1[] = {
    (ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};

static const asn_TYPE_tag2member_t asn_MAP_TestContainer_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }
};

static asn_SEQUENCE_specifics_t asn_SPC_TestContainer_specs_1 = {
    .struct_size = sizeof(TestContainer_t),
    .ctx_offset = 0,
    .tag2el = asn_MAP_TestContainer_tag2el_1,
    .tag2el_count = 1,
};

asn_TYPE_descriptor_t asn_DEF_TestContainer = {
    .name = "TestContainer",
    .xml_tag = "TestContainer",
    .op = &asn_OP_SEQUENCE,
    .tags = asn_DEF_TestContainer_tags_1,
    .tags_count = 1,
    .elements = asn_MBR_TestContainer_1,
    .elements_count = 1,
    .specifics = &asn_SPC_TestContainer_specs_1
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

/* Check that XER output doesn't have mashed tags (two closing tags on same line) */
static void
test_xer_choice_formatting(void) {
    TestContainer_t container;
    struct xer_buffer xb = {0, 0, 0};
    asn_enc_rval_t er;
    
    /* Initialize test structure */
    memset(&container, 0, sizeof(container));
    
    /* Set CHOICE to intValue = 42 */
    container.myChoice.present = TestChoice_PR_intValue;
    asn_long2INTEGER(&container.myChoice.choice.intValue, 42);
    
    /* Encode to XER */
    er = xer_encode(&asn_DEF_TestContainer, &container, XER_F_BASIC, xer_buffer_append, &xb);
    
    assert(er.encoded > 0);
    assert(xb.buffer != NULL);
    
    printf("XER output:\n%s\n", xb.buffer);
    
    /* Check for mashed tags - closing tags should NOT be on the same line */
    /* BAD pattern: </intValue></myChoice> or </myChoice></TestContainer> */
    
    char *mashed1 = strstr(xb.buffer, "</intValue></myChoice>");
    char *mashed2 = strstr(xb.buffer, "</myChoice></TestContainer>");
    
    if(mashed1 || mashed2) {
        fprintf(stderr, "FAIL: Found mashed closing tags in CHOICE output!\n");
        if(mashed1) fprintf(stderr, "  Found: </intValue></myChoice>\n");
        if(mashed2) fprintf(stderr, "  Found: </myChoice></TestContainer>\n");
        FREEMEM(xb.buffer);
        exit(1);
    }
    
    /* Check that closing tags appear on the same line as simple content (like INTEGER values) */
    /* Pattern should be: "42</intValue>" not "42\n        </intValue>" */
    char *intval_content = strstr(xb.buffer, "42</intValue>");
    if(!intval_content) {
        fprintf(stderr, "FAIL: </intValue> not on same line as content '42'!\n");
        fprintf(stderr, "Expected pattern: '42</intValue>' in output\n");
        FREEMEM(xb.buffer);
        exit(1);
    }
    
    /* Check that each closing tag is followed by a newline */
    char *intval_close = strstr(xb.buffer, "</intValue>");
    char *choice_close = strstr(xb.buffer, "</myChoice>");
    
    assert(intval_close != NULL);
    assert(choice_close != NULL);
    
    /* Check that </intValue> is followed by \n */
    size_t intval_close_len = strlen("</intValue>");
    if(intval_close[intval_close_len] != '\n') {
        fprintf(stderr, "FAIL: </intValue> not followed by newline! Next char is: 0x%02x\n",
                (unsigned char)intval_close[intval_close_len]);
        FREEMEM(xb.buffer);
        exit(1);
    }
    
    /* Check that </myChoice> is followed by \n */
    size_t choice_close_len = strlen("</myChoice>");
    if(choice_close[choice_close_len] != '\n') {
        fprintf(stderr, "FAIL: </myChoice> not followed by newline! Next char is: 0x%02x\n",
                (unsigned char)choice_close[choice_close_len]);
        FREEMEM(xb.buffer);
        exit(1);
    }
    
    /* Check that there are no extra blank lines after CHOICE member closing tags */
    /* We expect one newline after each closing tag, not two (which would create a blank line) */
    if(strstr(xb.buffer, "</intValue>\n\n")) {
        fprintf(stderr, "FAIL: Found extra blank line after </intValue>!\n");
        FREEMEM(xb.buffer);
        exit(1);
    }
    
    /* Note: A blank line before </TestContainer> is expected - that's standard SEQUENCE behavior */
    
    /* Cleanup */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_INTEGER, &container.myChoice.choice.intValue);
    FREEMEM(xb.buffer);
    
    printf("PASS: XER CHOICE formatting test passed\n");
}

int
main(void) {
    test_xer_choice_formatting();
    return 0;
}
