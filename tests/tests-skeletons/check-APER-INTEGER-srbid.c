/*
 * Regression test for GitHub issue #555:
 * F1AP SRBID APER encoding with extensible INTEGER constraint.
 *
 * The SRBID type in 3GPP 38.473 v19.3.0 is INTEGER (0..3, ...).
 * Values 0..3 (root range) must be encoded as 3 bits:
 *   [extension-bit=0][2-bit value]
 * NOT as 2 bits without the extension bit.
 *
 * Without the CPR_ignore_extension_additions fix, INTEGER (0..3,...,4..255)
 * would generate range_bits=8 (root widened to 0..255).  A value of 1
 * encoded in 8 bits + extension bit = 9 bits would be misread by a
 * spec-conformant decoder expecting 3 bits, returning SRBID=0 instead of 1.
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <INTEGER.h>
#include <INTEGER.c>
#include <INTEGER_aper.c>
#include <aper_support.c>
#include <aper_support.h>
#include <per_support.c>
#include <per_support.h>

static int
FailOut(const void *data, size_t size, void *op_key) {
    (void)data;
    (void)size;
    (void)op_key;
    assert(!"UNREACHABLE");
    return 0;
}

/*
 * For SRBID ::= INTEGER (0..3, ...) the PER constraint is:
 *   { APC_CONSTRAINED | APC_EXTENSIBLE, range_bits=2, effective_bits=2,
 *     lower_bound=0, upper_bound=3 }
 *
 * APER encoding of a root value v (0..3):
 *   [extension-bit=0 (1 bit)][v in 2 bits] = 3 bits total
 *
 * APER encoding of an extension value v (>=4):
 *   [extension-bit=1 (1 bit)][7 alignment bits][length (8 bits)][value bytes]
 */
static const asn_per_constraints_t srbid_constraint = {
    /* value: APC_CONSTRAINED | APC_EXTENSIBLE, range_bits=2, eff=2, lb=0, ub=3 */
    { APC_CONSTRAINED | APC_EXTENSIBLE, 2, 2, 0, 3 },
    /* size: unconstrained */
    { APC_UNCONSTRAINED, -1, -1, 0, 0 },
    0, 0
};

struct test_case {
    long value;
    const uint8_t *expected_bytes;
    size_t expected_nbits;  /* total bits in the encoding */
};

static void
check_srbid(int lineno, const struct test_case *tc) {
    INTEGER_t st;
    INTEGER_t *dec_st = 0;
    struct asn_INTEGER_specifics_s specs;
    asn_enc_rval_t enc_rval;
    asn_dec_rval_t dec_rval;
    asn_per_outp_t po;
    asn_per_data_t pd;
    size_t encoded_nbits;
    size_t encoded_nbytes;

    memset(&st, 0, sizeof(st));
    memset(&specs, 0, sizeof(specs));
    memset(&po, 0, sizeof(po));
    memset(&pd, 0, sizeof(pd));

    asn_long2INTEGER(&st, tc->value);

    po.buffer = po.tmpspace;
    po.nboff = 0;
    po.nbits = 8 * (int)sizeof(po.tmpspace);
    po.output = FailOut;

    specs.field_width = sizeof(long);
    specs.field_unsigned = 0;
    asn_DEF_INTEGER.specifics = &specs;

    enc_rval = INTEGER_encode_aper(&asn_DEF_INTEGER, &srbid_constraint, &st, &po);
    assert(enc_rval.encoded >= 0);

    encoded_nbits = (size_t)(po.buffer - po.tmpspace) * 8 + (size_t)po.nboff;
    encoded_nbytes = (encoded_nbits + 7) / 8;

    printf("%d: SRBID=%ld: %zu bits (%zu bytes)\n",
           lineno, tc->value, encoded_nbits, encoded_nbytes);

    /* Verify total bit count */
    if(encoded_nbits != tc->expected_nbits) {
        fprintf(stderr,
                "%d: SRBID=%ld: expected %zu bits, got %zu bits\n",
                lineno, tc->value, tc->expected_nbits, encoded_nbits);
        assert(encoded_nbits == tc->expected_nbits);
    }

    /* Verify byte content when expected_bytes is provided */
    if(tc->expected_bytes) {
        if(memcmp(po.tmpspace, tc->expected_bytes, encoded_nbytes) != 0) {
            size_t i;
            fprintf(stderr, "%d: SRBID=%ld: byte mismatch\n  expected:", lineno, tc->value);
            for(i = 0; i < encoded_nbytes; i++)
                fprintf(stderr, " %02x", tc->expected_bytes[i]);
            fprintf(stderr, "\n  got     :");
            for(i = 0; i < encoded_nbytes; i++)
                fprintf(stderr, " %02x", po.tmpspace[i]);
            fprintf(stderr, "\n");
            assert(!"byte mismatch");
        }
    }

    /* Round-trip: decode and verify */
    pd.buffer = po.tmpspace;
    pd.nboff = 0;
    pd.nbits = (int)encoded_nbits;
    pd.moved = 0;

    dec_rval = INTEGER_decode_aper(0, &asn_DEF_INTEGER, &srbid_constraint,
                                   (void **)&dec_st, &pd);
    if(dec_rval.code != RC_OK) {
        fprintf(stderr, "%d: SRBID=%ld: decode failed (code=%d)\n",
                lineno, tc->value, dec_rval.code);
        assert(dec_rval.code == RC_OK);
    }

    {
        long decoded_value = 0;
        asn_INTEGER2long(dec_st, &decoded_value);
        if(decoded_value != tc->value) {
            fprintf(stderr,
                    "%d: SRBID=%ld: round-trip mismatch: got %ld\n",
                    lineno, tc->value, decoded_value);
            assert(decoded_value == tc->value);
        }
    }

    printf("  PASS\n");

    ASN_STRUCT_RESET(asn_DEF_INTEGER, &st);
    ASN_STRUCT_FREE(asn_DEF_INTEGER, dec_st);
}

int main(void) {
    /*
     * Root range values (0..3): encoded as extension-bit + 2-bit value = 3 bits.
     *
     * Bit layout (MSB first):
     *   bit 7: extension bit = 0
     *   bits 6-5: 2-bit value
     *   bits 4-0: trailing zeros (padding to byte boundary in printed form)
     *
     * value=0: bits=000 -> byte=0x00
     * value=1: bits=001 -> byte=0x20
     * value=2: bits=010 -> byte=0x40
     * value=3: bits=011 -> byte=0x60
     */
    static const uint8_t exp_0[] = { 0x00 };
    static const uint8_t exp_1[] = { 0x20 };
    static const uint8_t exp_2[] = { 0x40 };
    static const uint8_t exp_3[] = { 0x60 };

    /*
     * Extension values (>=4): encoded as 1 extension bit, 7 alignment bits,
     * 1-byte length determinant, then the integer bytes.
     *
     * value=4: [1][0000000][00000001][00000100]
     *        = 0x80 0x01 0x04 (24 bits)
     * value=5: [1][0000000][00000001][00000101]
     *        = 0x80 0x01 0x05
     */
    static const uint8_t exp_4[] = { 0x80, 0x01, 0x04 };
    static const uint8_t exp_5[] = { 0x80, 0x01, 0x05 };

    static const struct test_case cases[] = {
        /* Root range: 3 bits each */
        { 0, exp_0, 3 },
        { 1, exp_1, 3 },
        { 2, exp_2, 3 },
        { 3, exp_3, 3 },
        /* Extension: 24 bits (1 ext + 7 align + 8 length + 8 value) */
        { 4, exp_4, 24 },
        { 5, exp_5, 24 },
    };

    size_t i;
    printf("=== APER SRBID (INTEGER 0..3,...) encoding regression test ===\n\n");

    for(i = 0; i < sizeof(cases) / sizeof(cases[0]); i++)
        check_srbid(__LINE__, &cases[i]);

    printf("\n=== All SRBID APER tests passed! ===\n");
    return 0;
}
