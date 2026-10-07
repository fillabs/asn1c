/*
 * Test XER formatting to ensure proper newlines after closing tags
 * and correct indentation (regression test for issue with mashed tags).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <asn_internal.h>
#include <xer_encoder.h>
#include <INTEGER.h>
#include <constr_SEQUENCE.h>

/* Test structure for XER formatting */
typedef struct TestSeq {
    INTEGER_t field1;
    INTEGER_t field2;
    INTEGER_t field3;
} TestSeq_t;

/* ASN.1 type descriptor for TestSeq */
static asn_TYPE_member_t asn_MBR_TestSeq_1[] = {
    {
        .flags = 0,
        .name = "field1",
        .type = &asn_DEF_INTEGER,
        .memb_offset = offsetof(TestSeq_t, field1),
    },
    {
        .flags = 0,
        .name = "field2",
        .type = &asn_DEF_INTEGER,
        .memb_offset = offsetof(TestSeq_t, field2),
    },
    {
        .flags = 0,
        .name = "field3",
        .type = &asn_DEF_INTEGER,
        .memb_offset = offsetof(TestSeq_t, field3),
    },
};

static const ber_tlv_tag_t asn_DEF_TestSeq_tags_1[] = {
    (ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};

static const asn_TYPE_tag2member_t asn_MAP_TestSeq_tag2el_1[] = {
    { (ASN_TAG_CLASS_UNIVERSAL | (2 << 2)), 0, 0, 2 },
    { (ASN_TAG_CLASS_UNIVERSAL | (2 << 2)), 1, -1, 1 },
    { (ASN_TAG_CLASS_UNIVERSAL | (2 << 2)), 2, -2, 0 }
};

static asn_SEQUENCE_specifics_t asn_SPC_TestSeq_specs_1 = {
    .struct_size = sizeof(TestSeq_t),
    .ctx_offset = 0,
    .tag2el = asn_MAP_TestSeq_tag2el_1,
    .tag2el_count = 3,
    .first_extension = -1
};

asn_TYPE_descriptor_t asn_DEF_TestSeq = {
    .name = "TestSeq",
    .xml_tag = "TestSeq",
    .op = &asn_OP_SEQUENCE,
    .tags = asn_DEF_TestSeq_tags_1,
    .tags_count = 1,
    .elements = asn_MBR_TestSeq_1,
    .elements_count = 3,
    .specifics = &asn_SPC_TestSeq_specs_1
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
test_xer_formatting(void) {
    TestSeq_t seq;
    struct xer_buffer xb = {0, 0, 0};
    asn_enc_rval_t er;
    
    /* Initialize test structure */
    memset(&seq, 0, sizeof(seq));
    asn_long2INTEGER(&seq.field1, 42);
    asn_long2INTEGER(&seq.field2, 100);
    asn_long2INTEGER(&seq.field3, 200);
    
    /* Encode to XER */
    er = xer_encode(&asn_DEF_TestSeq, &seq, XER_F_BASIC, xer_buffer_append, &xb);
    
    assert(er.encoded > 0);
    assert(xb.buffer != NULL);
    
    printf("XER output:\n%s\n", xb.buffer);
    
    /* Check for mashed tags - closing tags should be followed by newline */
    /* Pattern we're looking for (BAD): </field1></field2> or </field1>    <field2> */
    /* Pattern we want (GOOD): </field1>\n    <field2> */
    
    /* Search for problematic patterns */
    char *mashed1 = strstr(xb.buffer, "</field1></field2>");
    char *mashed2 = strstr(xb.buffer, "</field2></field3>");
    char *mashed3 = strstr(xb.buffer, "</field3></TestSeq>");
    
    if(mashed1 || mashed2 || mashed3) {
        fprintf(stderr, "FAIL: Found mashed closing tags!\n");
        if(mashed1) fprintf(stderr, "  Found: </field1></field2>\n");
        if(mashed2) fprintf(stderr, "  Found: </field2></field3>\n");
        if(mashed3) fprintf(stderr, "  Found: </field3></TestSeq>\n");
        FREEMEM(xb.buffer);
        exit(1);
    }
    
    /* Check that each closing tag is followed by a newline */
    char *field1_close = strstr(xb.buffer, "</field1>");
    char *field2_close = strstr(xb.buffer, "</field2>");
    char *field3_close = strstr(xb.buffer, "</field3>");
    
    assert(field1_close != NULL);
    assert(field2_close != NULL);
    assert(field3_close != NULL);
    
    /* Lengths of closing tags, used to locate the following character */
    {
        size_t field1_close_len = strlen("</field1>");
        size_t field2_close_len = strlen("</field2>");
        size_t field3_close_len = strlen("</field3>");
    
        /* Check that </field1> is followed by \n */
        if(field1_close[field1_close_len] != '\n') {
            fprintf(stderr, "FAIL: </field1> not followed by newline! Next char is: 0x%02x\n",
                    (unsigned char)field1_close[field1_close_len]);
            FREEMEM(xb.buffer);
            exit(1);
        }
    
        /* Check that </field2> is followed by \n */
        if(field2_close[field2_close_len] != '\n') {
            fprintf(stderr, "FAIL: </field2> not followed by newline! Next char is: 0x%02x\n",
                    (unsigned char)field2_close[field2_close_len]);
            FREEMEM(xb.buffer);
            exit(1);
        }
    
        /* Check that </field3> is followed by \n */
        if(field3_close[field3_close_len] != '\n') {
            fprintf(stderr, "FAIL: </field3> not followed by newline! Next char is: 0x%02x\n",
                    (unsigned char)field3_close[field3_close_len]);
            FREEMEM(xb.buffer);
            exit(1);
        }
    }
    
    /* Check for double newlines (which would indicate improper fix) */
    if(strstr(xb.buffer, "\n\n    <field")) {
        fprintf(stderr, "FAIL: Found double newlines between fields!\n");
        FREEMEM(xb.buffer);
        exit(1);
    }
    
    /* Cleanup */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_INTEGER, &seq.field1);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_INTEGER, &seq.field2);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_INTEGER, &seq.field3);
    FREEMEM(xb.buffer);
    
    printf("PASS: XER formatting test passed\n");
}

int
main(void) {
    test_xer_formatting();
    return 0;
}
