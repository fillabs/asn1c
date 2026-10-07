/* Test for APER INTEGER semi-constrained encoding (INTEGER (1..MAX) etc.) */

#include <stdio.h>
#include <assert.h>
#include <string.h>

#include <INTEGER.h>
#include <INTEGER.c>
#include <INTEGER_aper.c>
#include <per_support.c>
#include <per_support.h>
#include <aper_support.c>
#include <aper_support.h>

static int FailOut(const void *data, size_t size, void *op_key) {
    (void)data;
    (void)size;
    (void)op_key;
    assert(!"UNREACHABLE");
    return 0;
}

static void normalize(asn_per_outp_t *po) {
    if(po->nboff >= 8) {
        po->buffer += (po->nboff >> 3);
        po->nbits  -= (po->nboff & ~0x07);
        po->nboff  &= 0x07;
    }
}

static void
test_semiconstrained_signed_encode_decode(int lineno, long value, long lbound) {
    INTEGER_t st;
    INTEGER_t *reconstructed_st = 0;
    struct asn_INTEGER_specifics_s specs;
    struct asn_per_constraints_s cts;
    asn_enc_rval_t enc_rval;
    asn_dec_rval_t dec_rval;
    asn_per_outp_t po;
    asn_per_data_t pd;

    printf("%d: Testing semi-constrained signed INTEGER (%ld..MAX), value=%ld\n",
           lineno, lbound, value);

    memset(&st, 0, sizeof(st));
    memset(&po, 0, sizeof(po));
    memset(&pd, 0, sizeof(pd));
    memset(&cts, 0, sizeof(cts));
    memset(&specs, 0, sizeof(specs));

    /* Semi-constrained: only lower bound, range_bits = -1 */
    cts.value.flags = APC_SEMI_CONSTRAINED;
    cts.value.range_bits = -1;
    cts.value.effective_bits = -1;
    cts.value.lower_bound = lbound;
    cts.value.upper_bound = 0;  /* Not used for semi-constrained */

    asn_long2INTEGER(&st, value);

    po.buffer = po.tmpspace;
    po.nboff = 0;
    po.nbits = 8 * sizeof(po.tmpspace);
    po.output = FailOut;

    specs.field_width = sizeof(long);
    specs.field_unsigned = 0;

    asn_DEF_INTEGER.specifics = &specs;
    enc_rval = INTEGER_encode_aper(&asn_DEF_INTEGER, &cts, &st, &po);
    if(enc_rval.encoded < 0) {
        fprintf(stderr, "%d: Encoding failed for value %ld with lower bound %ld\n",
                lineno, value, lbound);
        assert(!"Encoding should succeed");
    }

    /* Normalize the output buffer */
    normalize(&po);

    /* Calculate the encoded size */
    size_t encoded_bytes = po.buffer - po.tmpspace;
    printf("  Encoded %zu bytes\n", encoded_bytes);

    /* Decode the value back */
    pd.buffer = po.tmpspace;
    pd.nboff = 0;
    pd.nbits = 8 * encoded_bytes + po.nboff;
    pd.moved = 0;

    dec_rval = INTEGER_decode_aper(0, &asn_DEF_INTEGER, &cts, 
                                   (void **)&reconstructed_st, &pd);
    if(dec_rval.code != RC_OK) {
        fprintf(stderr, "%d: Decoding failed for value %ld\n", lineno, value);
        assert(!"Decoding should succeed");
    }

    long reconstructed_value = 0;
    asn_INTEGER2long(reconstructed_st, &reconstructed_value);
    
    if(reconstructed_value != value) {
        fprintf(stderr, "%d: Value mismatch: expected %ld, got %ld\n",
                lineno, value, reconstructed_value);
        assert(!"Values should match");
    }
    
    printf("  PASS: value %ld encoded/decoded correctly\n", value);

    ASN_STRUCT_RESET(asn_DEF_INTEGER, &st);
    ASN_STRUCT_FREE(asn_DEF_INTEGER, reconstructed_st);
}

static void
test_semiconstrained_unsigned_encode_decode(int lineno, unsigned long value, long lbound) {
    INTEGER_t st;
    INTEGER_t *reconstructed_st = 0;
    struct asn_INTEGER_specifics_s specs;
    struct asn_per_constraints_s cts;
    asn_enc_rval_t enc_rval;
    asn_dec_rval_t dec_rval;
    asn_per_outp_t po;
    asn_per_data_t pd;

    printf("%d: Testing semi-constrained unsigned INTEGER (%ld..MAX), value=%lu\n",
           lineno, lbound, value);

    memset(&st, 0, sizeof(st));
    memset(&po, 0, sizeof(po));
    memset(&pd, 0, sizeof(pd));
    memset(&cts, 0, sizeof(cts));
    memset(&specs, 0, sizeof(specs));

    /* Semi-constrained: only lower bound, range_bits = -1 */
    cts.value.flags = APC_SEMI_CONSTRAINED;
    cts.value.range_bits = -1;
    cts.value.effective_bits = -1;
    cts.value.lower_bound = lbound;
    cts.value.upper_bound = 0;  /* Not used for semi-constrained */

    asn_ulong2INTEGER(&st, value);

    po.buffer = po.tmpspace;
    po.nboff = 0;
    po.nbits = 8 * sizeof(po.tmpspace);
    po.output = FailOut;

    specs.field_width = sizeof(long);
    specs.field_unsigned = 1;  /* Unsigned */

    asn_DEF_INTEGER.specifics = &specs;
    enc_rval = INTEGER_encode_aper(&asn_DEF_INTEGER, &cts, &st, &po);
    if(enc_rval.encoded < 0) {
        fprintf(stderr, "%d: Encoding failed for value %lu with lower bound %ld\n",
                lineno, value, lbound);
        assert(!"Encoding should succeed");
    }

    /* Normalize the output buffer */
    normalize(&po);

    /* Calculate the encoded size */
    size_t encoded_bytes = po.buffer - po.tmpspace;
    printf("  Encoded %zu bytes\n", encoded_bytes);

    /* Decode the value back */
    pd.buffer = po.tmpspace;
    pd.nboff = 0;
    pd.nbits = 8 * encoded_bytes + po.nboff;
    pd.moved = 0;

    dec_rval = INTEGER_decode_aper(0, &asn_DEF_INTEGER, &cts, 
                                   (void **)&reconstructed_st, &pd);
    if(dec_rval.code != RC_OK) {
        fprintf(stderr, "%d: Decoding failed for value %lu\n", lineno, value);
        assert(!"Decoding should succeed");
    }

    unsigned long reconstructed_value = 0;
    asn_INTEGER2ulong(reconstructed_st, &reconstructed_value);
    
    if(reconstructed_value != value) {
        fprintf(stderr, "%d: Value mismatch: expected %lu, got %lu\n",
                lineno, value, reconstructed_value);
        assert(!"Values should match");
    }
    
    printf("  PASS: value %lu encoded/decoded correctly\n", value);

    ASN_STRUCT_RESET(asn_DEF_INTEGER, &st);
    ASN_STRUCT_FREE(asn_DEF_INTEGER, reconstructed_st);
}

#define TEST_SIGNED(value, lbound) \
    test_semiconstrained_signed_encode_decode(__LINE__, value, lbound)

#define TEST_UNSIGNED(value, lbound) \
    test_semiconstrained_unsigned_encode_decode(__LINE__, value, lbound)

static void
test_zero_length_body_aper(int lineno, int semi_constrained, long lbound) {
    INTEGER_t *st = NULL;
    struct asn_per_constraints_s cts;
    asn_dec_rval_t dec_rval;
    asn_per_data_t pd;
    /*
     * Craft a zero-length INTEGER body: in APER unconstrained short-form,
     * the length byte is read after alignment; 0x00 means length = 0.
     */
    uint8_t buf[1] = { 0x00 };

    printf("%d: Testing zero-length APER body (%s, lbound=%ld)\n",
           lineno, semi_constrained ? "semi-constrained" : "unconstrained", lbound);

    memset(&pd, 0, sizeof(pd));
    pd.buffer = buf;
    pd.nboff = 0;
    pd.nbits = 8;

    if(semi_constrained) {
        memset(&cts, 0, sizeof(cts));
        cts.value.flags = APC_SEMI_CONSTRAINED;
        cts.value.range_bits = -1;
        cts.value.effective_bits = -1;
        cts.value.lower_bound = lbound;
        dec_rval = INTEGER_decode_aper(0, &asn_DEF_INTEGER, &cts, (void **)&st, &pd);
    } else {
        dec_rval = INTEGER_decode_aper(0, &asn_DEF_INTEGER, NULL, (void **)&st, &pd);
    }

    if(dec_rval.code != RC_FAIL) {
        fprintf(stderr, "%d: Expected RC_FAIL for zero-length body, got %d\n",
                lineno, (int)dec_rval.code);
        assert(!"Zero-length INTEGER body must be rejected");
    }
    printf("  PASS: zero-length body correctly rejected\n");
    ASN_STRUCT_FREE(asn_DEF_INTEGER, st);
}

#define TEST_ZERO_LENGTH(semi_constrained, lbound) \
    test_zero_length_body_aper(__LINE__, semi_constrained, lbound)

int main() {
    printf("=== Testing APER semi-constrained INTEGER encoding ===\n\n");

    printf("--- Test INTEGER (1..MAX) ---\n");
    /* The issue from the bug report: INTEGER (1..MAX) with serialNumber=1 */
    TEST_SIGNED(1, 1);
    TEST_SIGNED(2, 1);
    TEST_SIGNED(100, 1);
    TEST_SIGNED(256, 1);
    TEST_SIGNED(65535, 1);
    TEST_SIGNED(1000000, 1);

    printf("\n--- Test INTEGER (0..MAX) ---\n");
    TEST_SIGNED(0, 0);
    TEST_SIGNED(1, 0);
    TEST_SIGNED(255, 0);
    TEST_SIGNED(65536, 0);

    printf("\n--- Test INTEGER (100..MAX) ---\n");
    TEST_SIGNED(100, 100);
    TEST_SIGNED(101, 100);
    TEST_SIGNED(200, 100);
    TEST_SIGNED(1000, 100);

    printf("\n--- Test INTEGER (-10..MAX) ---\n");
    TEST_SIGNED(-10, -10);
    TEST_SIGNED(0, -10);
    TEST_SIGNED(100, -10);

    printf("\n--- Test unsigned INTEGER (1..MAX) ---\n");
    TEST_UNSIGNED(1, 1);
    TEST_UNSIGNED(100, 1);
    TEST_UNSIGNED(65535, 1);
    TEST_UNSIGNED(1000000, 1);

    printf("\n--- Test zero-length APER INTEGER body (malformed input) ---\n");
    /* Zero-length body must be rejected for both unconstrained and semi-constrained */
    TEST_ZERO_LENGTH(0, 0); /* unconstrained (ct=NULL) */
    TEST_ZERO_LENGTH(1, 0); /* semi-constrained, lbound=0 */
    TEST_ZERO_LENGTH(1, 1); /* semi-constrained, lbound=1 */

    printf("\n=== All semi-constrained INTEGER tests passed! ===\n");
    return 0;
}
