/*-
 * Copyright (c) 2025 Contributors. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
/*
 * Test suite for CBOR (Concise Binary Object Representation) codec.
 * Tests round-trip encoding/decoding for key ASN.1 types.
 */
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#include <asn_application.h>
#include <INTEGER.h>
#include <NativeInteger.h>
#include <OCTET_STRING.h>
#include <BIT_STRING.h>
#include <OBJECT_IDENTIFIER.h>
#include <cbor_encoder.h>
#include <cbor_decoder.h>
#include <cbor_support.h>

#if defined(__SANITIZE_ADDRESS__)
#define TEST_ASN_STACK_CHECK_DISABLED 1
#elif defined(__has_feature)
#if __has_feature(address_sanitizer)
#define TEST_ASN_STACK_CHECK_DISABLED 1
#endif
#endif
#if defined(ASN_DISABLE_STACK_OVERFLOW_CHECK)
#define TEST_ASN_STACK_CHECK_DISABLED 1
#endif

/* ------------------------------------------------------------------ */
/* Buffer accumulator for encoding output                               */
/* ------------------------------------------------------------------ */
struct buffer_acc {
    uint8_t *data;
    size_t   len;
    size_t   cap;
};

static int
buf_append(const void *bytes, size_t sz, void *key) {
    struct buffer_acc *acc = (struct buffer_acc *)key;
    if(acc->len + sz > acc->cap) {
        size_t new_cap = (acc->cap == 0) ? 64 : acc->cap * 2;
        while(new_cap < acc->len + sz) new_cap *= 2;
        uint8_t *p = (uint8_t *)realloc(acc->data, new_cap);
        if(!p) return -1;
        acc->data = p;
        acc->cap  = new_cap;
    }
    memcpy(acc->data + acc->len, bytes, sz);
    acc->len += sz;
    return 0;
}

static void
buf_free(struct buffer_acc *acc) {
    free(acc->data);
    acc->data = NULL;
    acc->len = acc->cap = 0;
}

/* ------------------------------------------------------------------ */
/* INTEGER round-trip helper                                            */
/* ------------------------------------------------------------------ */
static void
test_integer_roundtrip(intmax_t val, const char *label) {
    INTEGER_t orig, *decoded = NULL;
    struct buffer_acc enc;
    asn_enc_rval_t er;
    asn_dec_rval_t dr;
    intmax_t result;

    memset(&orig, 0, sizeof(orig));
    memset(&enc, 0, sizeof(enc));

    if(asn_imax2INTEGER(&orig, val)) {
        fprintf(stderr, "FAIL: asn_imax2INTEGER(%s)\n", label);
        exit(1);
    }

    er = cbor_encode(&asn_DEF_INTEGER, &orig, buf_append, &enc);
    if(er.encoded < 0) {
        fprintf(stderr, "FAIL: encode %s\n", label);
        exit(1);
    }

    dr = cbor_decode(NULL, &asn_DEF_INTEGER, (void **)&decoded,
                     enc.data, enc.len);
    if(dr.code != RC_OK || !decoded) {
        fprintf(stderr, "FAIL: decode %s (code=%d)\n", label, dr.code);
        exit(1);
    }

    if(asn_INTEGER2imax(decoded, &result)) {
        fprintf(stderr, "FAIL: asn_INTEGER2imax for %s\n", label);
        exit(1);
    }

    if(result != val) {
        fprintf(stderr, "FAIL: round-trip mismatch %s: got %jd, want %jd\n",
                label, result, val);
        exit(1);
    }

    ASN_STRUCT_FREE(asn_DEF_INTEGER, decoded);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_INTEGER, &orig);
    buf_free(&enc);
    printf("  ✓ INTEGER round-trip: %s (%jd)\n", label, val);
}

static void
test_integer_cbor_edge_cases(void) {
    printf("test_integer_cbor_edge_cases\n");

    test_integer_roundtrip(0,             "zero");
    test_integer_roundtrip(1,             "one");
    test_integer_roundtrip(-1,            "minus_one");
    test_integer_roundtrip(23,            "23_1byte_boundary");
    test_integer_roundtrip(24,            "24_2byte_boundary");
    test_integer_roundtrip(127,           "127");
    test_integer_roundtrip(-127,          "minus_127");
    test_integer_roundtrip(-128,          "INT8_MIN");
    test_integer_roundtrip(255,           "UINT8_MAX");
    test_integer_roundtrip(256,           "256");
    test_integer_roundtrip(-256,          "minus_256");
    test_integer_roundtrip(32767,         "INT16_MAX");
    test_integer_roundtrip(-32768,        "INT16_MIN");
    test_integer_roundtrip(65535,         "UINT16_MAX");
    test_integer_roundtrip(65536,         "65536");
    test_integer_roundtrip(-65536,        "minus_65536");
    test_integer_roundtrip(2147483647LL,  "INT32_MAX");
    test_integer_roundtrip(-2147483648LL, "INT32_MIN");
    test_integer_roundtrip(4294967295LL,  "UINT32_MAX");
    test_integer_roundtrip(INT64_MAX,     "INT64_MAX");
    test_integer_roundtrip(INT64_MIN,     "INT64_MIN");

    /* Verify CBOR negint decoding for large values.
     * argument == 2^63 means value = -(2^63+1) which doesn't fit int64_t.
     * Construct the CBOR manually and verify decode succeeds. */
    {
        /* CBOR negint with argument = INT64_MAX + 1 = 0x8000000000000000 */
        uint8_t cbor_neg2_63[9] = {
            0x3B,  /* major=1 (negint), additional=27 (8-byte) */
            0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        };
        /* value = -(0x8000000000000000 + 1) = -0x8000000000000001 */
        INTEGER_t *decoded = NULL;
        asn_dec_rval_t dr = cbor_decode(NULL, &asn_DEF_INTEGER,
                                        (void **)&decoded,
                                        cbor_neg2_63, sizeof(cbor_neg2_63));
        if(dr.code != RC_OK || !decoded) {
            fprintf(stderr, "FAIL: decode negint 2^63 (code=%d)\n", dr.code);
            exit(1);
        }
        printf("  ✓ INTEGER decode: CBOR negint argument=2^63 (bignum path)\n");
        ASN_STRUCT_FREE(asn_DEF_INTEGER, decoded);
    }

    {
        /* CBOR negint with argument = UINT64_MAX = 0xFFFFFFFFFFFFFFFF */
        uint8_t cbor_neg_u64max[9] = {
            0x3B,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
        };
        /* value = -(UINT64_MAX + 1) = -2^64 */
        INTEGER_t *decoded = NULL;
        asn_dec_rval_t dr = cbor_decode(NULL, &asn_DEF_INTEGER,
                                        (void **)&decoded,
                                        cbor_neg_u64max, sizeof(cbor_neg_u64max));
        if(dr.code != RC_OK || !decoded) {
            fprintf(stderr, "FAIL: decode negint UINT64_MAX (code=%d)\n", dr.code);
            exit(1);
        }
        printf("  ✓ INTEGER decode: CBOR negint argument=UINT64_MAX (bignum -2^64)\n");
        ASN_STRUCT_FREE(asn_DEF_INTEGER, decoded);
    }

    printf("PASSED: test_integer_cbor_edge_cases\n\n");
}

/* ------------------------------------------------------------------ */
/* NativeInteger round-trip                                             */
/* ------------------------------------------------------------------ */
static void
test_native_integer_roundtrip(long val, const char *label) {
    long orig = val, result = 0;
    long *decoded = NULL;
    struct buffer_acc enc;
    asn_enc_rval_t er;
    asn_dec_rval_t dr;

    memset(&enc, 0, sizeof(enc));

    er = cbor_encode(&asn_DEF_NativeInteger, &orig, buf_append, &enc);
    if(er.encoded < 0) {
        fprintf(stderr, "FAIL: NativeInteger encode %s\n", label);
        exit(1);
    }

    dr = cbor_decode(NULL, &asn_DEF_NativeInteger, (void **)&decoded,
                     enc.data, enc.len);
    if(dr.code != RC_OK || !decoded) {
        fprintf(stderr, "FAIL: NativeInteger decode %s (code=%d)\n", label, dr.code);
        exit(1);
    }
    result = *decoded;
    free(decoded);
    buf_free(&enc);

    if(result != val) {
        fprintf(stderr, "FAIL: NativeInteger mismatch %s: got %ld, want %ld\n",
                label, result, val);
        exit(1);
    }
    printf("  ✓ NativeInteger round-trip: %s (%ld)\n", label, val);
}

static void
test_native_integer_cbor_edge_cases(void) {
    printf("test_native_integer_cbor_edge_cases\n");

    test_native_integer_roundtrip(0,           "zero");
    test_native_integer_roundtrip(1,           "one");
    test_native_integer_roundtrip(-1,          "minus_one");
    test_native_integer_roundtrip(23,          "23");
    test_native_integer_roundtrip(24,          "24");
    test_native_integer_roundtrip(127,         "127");
    test_native_integer_roundtrip(-127,        "minus_127");
    test_native_integer_roundtrip(-128,        "INT8_MIN");
    test_native_integer_roundtrip(255,         "UINT8_MAX");
    test_native_integer_roundtrip(256,         "256");
    test_native_integer_roundtrip(-256,        "minus_256");
    test_native_integer_roundtrip(32767,       "INT16_MAX");
    test_native_integer_roundtrip(-32768,      "INT16_MIN");
    test_native_integer_roundtrip(65535,       "UINT16_MAX");
    test_native_integer_roundtrip(65536,       "65536");
    test_native_integer_roundtrip(-65536,      "minus_65536");
    test_native_integer_roundtrip(2147483647L, "INT32_MAX");
    test_native_integer_roundtrip(-2147483648L, "INT32_MIN");
    test_native_integer_roundtrip(LONG_MAX,    "LONG_MAX");
    test_native_integer_roundtrip(LONG_MIN,    "LONG_MIN");

    printf("PASSED: test_native_integer_cbor_edge_cases\n\n");
}

/* ------------------------------------------------------------------ */
/* BIT STRING round-trip                                                */
/* ------------------------------------------------------------------ */
static void
test_bit_string_roundtrip(const uint8_t *bits, int nbytes, int bits_unused,
                          const char *label) {
    BIT_STRING_t orig, *decoded = NULL;
    struct buffer_acc enc;
    asn_enc_rval_t er;
    asn_dec_rval_t dr;

    memset(&orig, 0, sizeof(orig));
    memset(&enc, 0, sizeof(enc));

    if(nbytes > 0) {
        orig.buf = (uint8_t *)malloc(nbytes);
        assert(orig.buf);
        memcpy(orig.buf, bits, nbytes);
    }
    orig.size = nbytes;
    orig.bits_unused = bits_unused;

    er = cbor_encode(&asn_DEF_BIT_STRING, &orig, buf_append, &enc);
    if(er.encoded < 0) {
        fprintf(stderr, "FAIL: BIT_STRING encode %s\n", label);
        exit(1);
    }

    dr = cbor_decode(NULL, &asn_DEF_BIT_STRING, (void **)&decoded,
                     enc.data, enc.len);
    if(dr.code != RC_OK || !decoded) {
        fprintf(stderr, "FAIL: BIT_STRING decode %s (code=%d)\n", label, dr.code);
        exit(1);
    }

    if(decoded->size != nbytes || decoded->bits_unused != bits_unused) {
        fprintf(stderr, "FAIL: BIT_STRING mismatch %s: "
                "size=%zu (want %d), unused=%d (want %d)\n",
                label, decoded->size, nbytes,
                decoded->bits_unused, bits_unused);
        exit(1);
    }
    if(nbytes > 0 && memcmp(decoded->buf, bits, nbytes) != 0) {
        fprintf(stderr, "FAIL: BIT_STRING data mismatch %s\n", label);
        exit(1);
    }

    ASN_STRUCT_FREE(asn_DEF_BIT_STRING, decoded);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_BIT_STRING, &orig);
    buf_free(&enc);
    printf("  ✓ BIT_STRING round-trip: %s\n", label);
}

static void
test_bit_string_cbor_edge_cases(void) {
    printf("test_bit_string_cbor_edge_cases\n");

    /* 0 bits: size=0, bits_unused=0 */
    test_bit_string_roundtrip(NULL, 0, 0, "0_bits");

    /* 8 bits (1 byte): bits_unused=0 */
    {
        uint8_t data[1] = {0xA5};
        test_bit_string_roundtrip(data, 1, 0, "8_bits");
    }

    /* 1 bit: size=1, bits_unused=7 */
    {
        uint8_t data[1] = {0x80};
        test_bit_string_roundtrip(data, 1, 7, "1_bit");
    }

    /* 9 bits: size=2, bits_unused=7 */
    {
        uint8_t data[2] = {0xFF, 0x80};
        test_bit_string_roundtrip(data, 2, 7, "9_bits");
    }

    /* 16 bits: size=2, bits_unused=0 */
    {
        uint8_t data[2] = {0xDE, 0xAD};
        test_bit_string_roundtrip(data, 2, 0, "16_bits");
    }

    printf("PASSED: test_bit_string_cbor_edge_cases\n\n");
}

/* ------------------------------------------------------------------ */
/* OCTET STRING round-trip                                              */
/* ------------------------------------------------------------------ */
static void
test_octet_string_roundtrip(const uint8_t *data, size_t len, const char *label) {
    OCTET_STRING_t orig, *decoded = NULL;
    struct buffer_acc enc;
    asn_enc_rval_t er;
    asn_dec_rval_t dr;

    memset(&orig, 0, sizeof(orig));
    memset(&enc, 0, sizeof(enc));

    if(len > 0) {
        orig.buf = (uint8_t *)malloc(len);
        assert(orig.buf);
        memcpy(orig.buf, data, len);
    }
    orig.size = (int)len;

    er = cbor_encode(&asn_DEF_OCTET_STRING, &orig, buf_append, &enc);
    if(er.encoded < 0) {
        fprintf(stderr, "FAIL: OCTET_STRING encode %s\n", label);
        exit(1);
    }

    dr = cbor_decode(NULL, &asn_DEF_OCTET_STRING, (void **)&decoded,
                     enc.data, enc.len);
    if(dr.code != RC_OK || !decoded) {
        fprintf(stderr, "FAIL: OCTET_STRING decode %s (code=%d)\n", label, dr.code);
        exit(1);
    }

    if((size_t)decoded->size != len) {
        fprintf(stderr, "FAIL: OCTET_STRING size mismatch %s: "
                "got %zu, want %zu\n", label, (size_t)decoded->size, len);
        exit(1);
    }
    if(len > 0 && memcmp(decoded->buf, data, len) != 0) {
        fprintf(stderr, "FAIL: OCTET_STRING data mismatch %s\n", label);
        exit(1);
    }

    ASN_STRUCT_FREE(asn_DEF_OCTET_STRING, decoded);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_OCTET_STRING, &orig);
    buf_free(&enc);
    printf("  ✓ OCTET_STRING round-trip: %s (len=%zu)\n", label, len);
}

static void
test_octet_string_cbor_edge_cases(void) {
    printf("test_octet_string_cbor_edge_cases\n");

    /* Empty */
    test_octet_string_roundtrip(NULL, 0, "empty");

    /* Single byte */
    {
        uint8_t d[1] = {0x42};
        test_octet_string_roundtrip(d, 1, "single_byte");
    }

    /* 23 bytes (last with 1-byte CBOR header) */
    {
        uint8_t d[23];
        for(int i = 0; i < 23; i++) d[i] = (uint8_t)i;
        test_octet_string_roundtrip(d, 23, "23_bytes");
    }

    /* 24 bytes (first with 2-byte CBOR header) */
    {
        uint8_t d[24];
        for(int i = 0; i < 24; i++) d[i] = (uint8_t)(i + 1);
        test_octet_string_roundtrip(d, 24, "24_bytes");
    }

    /* 100 bytes */
    {
        uint8_t d[100];
        for(int i = 0; i < 100; i++) d[i] = (uint8_t)(i * 2);
        test_octet_string_roundtrip(d, 100, "100_bytes");
    }

    printf("PASSED: test_octet_string_cbor_edge_cases\n\n");
}

/* ------------------------------------------------------------------ */
/* Generic asn_encode / asn_decode dispatch via ATS_CBOR                */
/* ------------------------------------------------------------------ */
static void
test_asn_encode_decode_cbor_dispatch(void) {
    printf("test_asn_encode_decode_cbor_dispatch\n");

    /* Use INTEGER round-trip through the generic API */
    {
        INTEGER_t orig, *decoded = NULL;
        struct buffer_acc enc;
        asn_enc_rval_t er;
        asn_dec_rval_t dr;
        intmax_t result;

        memset(&orig, 0, sizeof(orig));
        memset(&enc, 0, sizeof(enc));

        if(asn_imax2INTEGER(&orig, 12345)) {
            fprintf(stderr, "FAIL: asn_imax2INTEGER\n");
            exit(1);
        }

        er = asn_encode(NULL, ATS_CBOR, &asn_DEF_INTEGER, &orig,
                        buf_append, &enc);
        if(er.encoded < 0) {
            fprintf(stderr, "FAIL: asn_encode(ATS_CBOR, INTEGER)\n");
            exit(1);
        }

        dr = asn_decode(NULL, ATS_CBOR, &asn_DEF_INTEGER,
                        (void **)&decoded, enc.data, enc.len);
        if(dr.code != RC_OK || !decoded) {
            fprintf(stderr, "FAIL: asn_decode(ATS_CBOR, INTEGER) code=%d\n",
                    dr.code);
            exit(1);
        }

        if(asn_INTEGER2imax(decoded, &result) || result != 12345) {
            fprintf(stderr, "FAIL: dispatch round-trip mismatch: %jd\n", result);
            exit(1);
        }

        ASN_STRUCT_FREE(asn_DEF_INTEGER, decoded);
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_INTEGER, &orig);
        buf_free(&enc);
        printf("  ✓ asn_encode/asn_decode dispatch: ATS_CBOR INTEGER\n");
    }

    /* Use OCTET STRING round-trip through the generic API */
    {
        OCTET_STRING_t orig, *decoded = NULL;
        struct buffer_acc enc;
        asn_enc_rval_t er;
        asn_dec_rval_t dr;
        static const uint8_t payload[] = {0xDE, 0xAD, 0xBE, 0xEF};

        memset(&orig, 0, sizeof(orig));
        memset(&enc, 0, sizeof(enc));

        orig.buf = (uint8_t *)malloc(sizeof(payload));
        assert(orig.buf);
        memcpy(orig.buf, payload, sizeof(payload));
        orig.size = sizeof(payload);

        er = asn_encode(NULL, ATS_CBOR, &asn_DEF_OCTET_STRING, &orig,
                        buf_append, &enc);
        if(er.encoded < 0) {
            fprintf(stderr, "FAIL: asn_encode(ATS_CBOR, OCTET_STRING)\n");
            exit(1);
        }

        dr = asn_decode(NULL, ATS_CBOR, &asn_DEF_OCTET_STRING,
                        (void **)&decoded, enc.data, enc.len);
        if(dr.code != RC_OK || !decoded) {
            fprintf(stderr, "FAIL: asn_decode(ATS_CBOR, OCTET_STRING) code=%d\n",
                    dr.code);
            exit(1);
        }

        if((size_t)decoded->size != sizeof(payload)
           || memcmp(decoded->buf, payload, sizeof(payload)) != 0) {
            fprintf(stderr, "FAIL: dispatch OCTET_STRING data mismatch\n");
            exit(1);
        }

        ASN_STRUCT_FREE(asn_DEF_OCTET_STRING, decoded);
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_OCTET_STRING, &orig);
        buf_free(&enc);
        printf("  ✓ asn_encode/asn_decode dispatch: ATS_CBOR OCTET_STRING\n");
    }

    printf("PASSED: test_asn_encode_decode_cbor_dispatch\n\n");
}

/* ------------------------------------------------------------------ */
/* OBJECT IDENTIFIER round-trip                                         */
/* ------------------------------------------------------------------ */
static void
test_oid_roundtrip(const uint8_t *der_bytes, size_t der_len, const char *label) {
    OBJECT_IDENTIFIER_t orig, *decoded = NULL;
    struct buffer_acc enc;
    asn_enc_rval_t er;
    asn_dec_rval_t dr;

    memset(&orig, 0, sizeof(orig));
    memset(&enc, 0, sizeof(enc));

    if(der_len > 0) {
        orig.buf = (uint8_t *)malloc(der_len);
        assert(orig.buf);
        memcpy(orig.buf, der_bytes, der_len);
    }
    orig.size = (int)der_len;

    er = cbor_encode(&asn_DEF_OBJECT_IDENTIFIER, &orig, buf_append, &enc);
    if(er.encoded < 0) {
        fprintf(stderr, "FAIL: OBJECT_IDENTIFIER encode %s\n", label);
        exit(1);
    }

    dr = cbor_decode(NULL, &asn_DEF_OBJECT_IDENTIFIER, (void **)&decoded,
                     enc.data, enc.len);
    if(dr.code != RC_OK || !decoded) {
        fprintf(stderr, "FAIL: OBJECT_IDENTIFIER decode %s (code=%d)\n",
                label, dr.code);
        exit(1);
    }

    if((size_t)decoded->size != der_len) {
        fprintf(stderr, "FAIL: OBJECT_IDENTIFIER size mismatch %s: "
                "got %zu, want %zu\n", label, (size_t)decoded->size, der_len);
        exit(1);
    }
    if(der_len > 0 && memcmp(decoded->buf, der_bytes, der_len) != 0) {
        fprintf(stderr, "FAIL: OBJECT_IDENTIFIER data mismatch %s\n", label);
        exit(1);
    }

    ASN_STRUCT_FREE(asn_DEF_OBJECT_IDENTIFIER, decoded);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_OBJECT_IDENTIFIER, &orig);
    buf_free(&enc);
    printf("  ✓ OBJECT_IDENTIFIER round-trip: %s (len=%zu)\n", label, der_len);
}

static void
test_oid_cbor_roundtrip(void) {
    printf("test_oid_cbor_roundtrip\n");

    /* OID 1.2.840.10045.4.3.2 (ecdsaWithSHA256): DER value bytes */
    {
        /* 1.2.840.10045.4.3.2 -> 2a 86 48 ce 3d 04 03 02 */
        static const uint8_t oid_sha256[] = {
            0x2a, 0x86, 0x48, 0xce, 0x3d, 0x04, 0x03, 0x02
        };
        test_oid_roundtrip(oid_sha256, sizeof(oid_sha256), "ecdsaWithSHA256");
    }

    /* OID 2.5.4.3 (commonName): DER value bytes */
    {
        /* 2.5.4.3 -> 55 04 03 */
        static const uint8_t oid_cn[] = {0x55, 0x04, 0x03};
        test_oid_roundtrip(oid_cn, sizeof(oid_cn), "commonName");
    }

    /* OID 1.2.840.10045.2.1 (ecPublicKey): DER value bytes */
    {
        /* 1.2.840.10045.2.1 -> 2a 86 48 ce 3d 02 01 */
        static const uint8_t oid_ec[] = {
            0x2a, 0x86, 0x48, 0xce, 0x3d, 0x02, 0x01
        };
        test_oid_roundtrip(oid_ec, sizeof(oid_ec), "ecPublicKey");
    }

    /* OID 2.5.29.19 (basicConstraints): DER value bytes */
    {
        /* 2.5.29.19 -> 55 1d 13 */
        static const uint8_t oid_bc[] = {0x55, 0x1d, 0x13};
        test_oid_roundtrip(oid_bc, sizeof(oid_bc), "basicConstraints");
    }

    printf("PASSED: test_oid_cbor_roundtrip\n\n");
}

/* ------------------------------------------------------------------ */
/* CBOR tag encoding and tag-transparent decoding tests (RFC 8949 §3.4) */
/* ------------------------------------------------------------------ */

/*
 * Build a CBOR-tagged item in a local buffer: tag_header + inner_buf.
 * Returns total length written into out_buf (must be at least
 * 9 + inner_len bytes).
 */
static size_t
prepend_tag(uint64_t tag, const uint8_t *inner, size_t inner_len,
            uint8_t *out, size_t out_cap) {
    struct buffer_acc acc;
    asn_enc_rval_t er;

    /* Reuse the cbor_encode_tag primitive to write the tag header */
    memset(&acc, 0, sizeof(acc));
    er.encoded = (ssize_t)cbor_encode_tag(tag, buf_append, &acc);
    if(er.encoded < 0 || acc.len + inner_len > out_cap) {
        buf_free(&acc);
        return 0;
    }
    memcpy(out, acc.data, acc.len);
    memcpy(out + acc.len, inner, inner_len);
    size_t total = acc.len + inner_len;
    buf_free(&acc);
    return total;
}

static void
test_cbor_tag_encoding(void) {
    printf("test_cbor_tag_encoding\n");

    /* Verify cbor_encode_tag produces the correct byte sequence */

    /* Tag 0 (datetime): 0xC0 */
    {
        struct buffer_acc acc;
        memset(&acc, 0, sizeof(acc));
        ssize_t ret = cbor_encode_tag(CBOR_TAG_DATETIME_STRING, buf_append, &acc);
        assert(ret == 1 && acc.len == 1 && acc.data[0] == 0xC0);
        buf_free(&acc);
        printf("  ✓ cbor_encode_tag(CBOR_TAG_DATETIME_STRING=0) -> 0xC0\n");
    }

    /* Tag 1 (epoch datetime): 0xC1 */
    {
        struct buffer_acc acc;
        memset(&acc, 0, sizeof(acc));
        ssize_t ret = cbor_encode_tag(CBOR_TAG_EPOCH_DATETIME, buf_append, &acc);
        assert(ret == 1 && acc.len == 1 && acc.data[0] == 0xC1);
        buf_free(&acc);
        printf("  ✓ cbor_encode_tag(CBOR_TAG_EPOCH_DATETIME=1) -> 0xC1\n");
    }

    /* Tag 2 (positive bignum): 0xC2 */
    {
        struct buffer_acc acc;
        memset(&acc, 0, sizeof(acc));
        ssize_t ret = cbor_encode_tag(CBOR_TAG_POSINT_BIGNUM, buf_append, &acc);
        assert(ret == 1 && acc.len == 1 && acc.data[0] == 0xC2);
        buf_free(&acc);
        printf("  ✓ cbor_encode_tag(CBOR_TAG_POSINT_BIGNUM=2) -> 0xC2\n");
    }

    /* Tag 32 (URI): 0xD8 0x20 */
    {
        struct buffer_acc acc;
        memset(&acc, 0, sizeof(acc));
        ssize_t ret = cbor_encode_tag(CBOR_TAG_URI, buf_append, &acc);
        assert(ret == 2 && acc.len == 2);
        assert(acc.data[0] == 0xD8 && acc.data[1] == 0x20);
        buf_free(&acc);
        printf("  ✓ cbor_encode_tag(CBOR_TAG_URI=32) -> 0xD8 0x20\n");
    }

    /* Tag 55799 (self-described CBOR): 0xD9 0xD9 0xF7 */
    {
        struct buffer_acc acc;
        memset(&acc, 0, sizeof(acc));
        ssize_t ret = cbor_encode_tag(CBOR_TAG_SELF_DESCRIBED, buf_append, &acc);
        assert(ret == 3 && acc.len == 3);
        assert(acc.data[0] == 0xD9 && acc.data[1] == 0xD9 && acc.data[2] == 0xF7);
        buf_free(&acc);
        printf("  ✓ cbor_encode_tag(CBOR_TAG_SELF_DESCRIBED=55799) -> 0xD9 0xD9 0xF7\n");
    }

    printf("PASSED: test_cbor_tag_encoding\n\n");
}

static void
test_cbor_skip_tags(void) {
    printf("test_cbor_skip_tags\n");

    /* No tag: should return 0 */
    {
        uint8_t buf[] = {0x42};  /* major 2, len 2 */
        ssize_t n = cbor_skip_tags(buf, sizeof(buf));
        assert(n == 0);
        printf("  ✓ cbor_skip_tags: no tag -> 0\n");
    }

    /* Single tag 0 (0xC0): returns 1 */
    {
        uint8_t buf[] = {0xC0, 0x42};
        ssize_t n = cbor_skip_tags(buf, sizeof(buf));
        assert(n == 1);
        printf("  ✓ cbor_skip_tags: single tag 0 -> 1\n");
    }

    /* Tag 32 (0xD8 0x20): returns 2 */
    {
        uint8_t buf[] = {0xD8, 0x20, 0x60};
        ssize_t n = cbor_skip_tags(buf, sizeof(buf));
        assert(n == 2);
        printf("  ✓ cbor_skip_tags: tag 32 -> 2\n");
    }

    /* Two nested tags: tag1(0xC1) + tag0(0xC0) + value */
    {
        uint8_t buf[] = {0xC1, 0xC0, 0x60};
        ssize_t n = cbor_skip_tags(buf, sizeof(buf));
        assert(n == 2);
        printf("  ✓ cbor_skip_tags: two nested tags -> 2\n");
    }

    /* Truncated tag: returns -1 */
    {
        uint8_t buf[] = {0xD8};  /* tag major, needs 1 more byte */
        ssize_t n = cbor_skip_tags(buf, sizeof(buf));
        assert(n == -1);
        printf("  ✓ cbor_skip_tags: truncated tag -> -1\n");
    }

    /* Complete tag header with no following item (EOF after tag): returns -1 */
    {
        uint8_t buf[] = {0xC0};  /* tag 0, complete header but no tagged item */
        ssize_t n = cbor_skip_tags(buf, sizeof(buf));
        assert(n == -1);
        printf("  ✓ cbor_skip_tags: tag header with no item -> -1\n");
    }

    /* Self-described CBOR tag 55799 (0xD9 0xD9 0xF7): returns 3 */
    {
        uint8_t buf[] = {0xD9, 0xD9, 0xF7, 0x01};
        ssize_t n = cbor_skip_tags(buf, sizeof(buf));
        assert(n == 3);
        printf("  ✓ cbor_skip_tags: tag 55799 (3-byte header) -> 3\n");
    }

    printf("PASSED: test_cbor_skip_tags\n\n");
}

static void
test_cbor_skip_item_stack_limit(void) {
    enum { tag_depth = 512 };
    uint8_t nested_tags[tag_depth + 1];
    ssize_t n;
    int i;

    printf("test_cbor_skip_item_stack_limit\n");

    /*
     * Build tag(0) wrappers around integer 0:
     *   C0 C0 ... C0 00
     * cbor_skip_item() recurses through every tag wrapper.
     */
    for(i = 0; i < tag_depth; i++) {
        nested_tags[i] = 0xC0;
    }
    nested_tags[tag_depth] = 0x00;

    n = cbor_skip_item(nested_tags, sizeof(nested_tags));
    assert(n == (ssize_t)sizeof(nested_tags));
    printf("  ✓ compatibility cbor_skip_item skips nested tags\n");

#if !defined(TEST_ASN_STACK_CHECK_DISABLED)
    {
        asn_codec_ctx_t ctx;

        memset(&ctx, 0, sizeof(ctx));
        ctx.max_stack_size = 4096;

        n = cbor_skip_item_with_ctx(&ctx, nested_tags, sizeof(nested_tags));
        assert(n == -1);
        printf("  ✓ cbor_skip_item_with_ctx rejects deep nesting at stack limit\n");
    }
#else
    printf("  - stack-limit assertion skipped for sanitizer/no-stack-check build\n");
#endif

    printf("PASSED: test_cbor_skip_item_stack_limit\n\n");
}

/* Helper: prepend a tag header to an existing encoded buffer and decode */
static void
test_tag_transparent_integer(uint64_t tag, intmax_t val, const char *label) {
    INTEGER_t orig, *decoded = NULL;
    struct buffer_acc inner;
    asn_enc_rval_t er;
    asn_dec_rval_t dr;
    intmax_t result;
    uint8_t tagged[32];
    size_t tagged_len;

    memset(&orig, 0, sizeof(orig));
    memset(&inner, 0, sizeof(inner));

    if(asn_imax2INTEGER(&orig, val)) {
        fprintf(stderr, "FAIL: asn_imax2INTEGER(%s)\n", label);
        exit(1);
    }

    er = cbor_encode(&asn_DEF_INTEGER, &orig, buf_append, &inner);
    if(er.encoded < 0) {
        fprintf(stderr, "FAIL: encode %s\n", label);
        exit(1);
    }

    /* Prepend tag header */
    tagged_len = prepend_tag(tag, inner.data, inner.len, tagged, sizeof(tagged));
    assert(tagged_len > 0);

    dr = cbor_decode(NULL, &asn_DEF_INTEGER, (void **)&decoded,
                     tagged, tagged_len);
    if(dr.code != RC_OK || !decoded) {
        fprintf(stderr, "FAIL: tag-transparent INTEGER decode %s (code=%d)\n",
                label, dr.code);
        exit(1);
    }

    if(asn_INTEGER2imax(decoded, &result) || result != val) {
        fprintf(stderr, "FAIL: tag-transparent mismatch %s: got %jd want %jd\n",
                label, result, val);
        exit(1);
    }

    /* Verify all bytes were consumed */
    if(dr.consumed != tagged_len) {
        fprintf(stderr, "FAIL: consumed=%zu but tagged_len=%zu for %s\n",
                dr.consumed, tagged_len, label);
        exit(1);
    }

    ASN_STRUCT_FREE(asn_DEF_INTEGER, decoded);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_INTEGER, &orig);
    buf_free(&inner);
    printf("  ✓ tag %llu transparent decode: INTEGER %s\n",
           (unsigned long long)tag, label);
}

static void
test_cbor_tag_transparent_decode(void) {
    printf("test_cbor_tag_transparent_decode\n");

    /* ---- INTEGER: decode through various metadata tags ---- */
    /* Tag 1 = epoch-based datetime wrapping an integer */
    test_tag_transparent_integer(CBOR_TAG_EPOCH_DATETIME, 0, "zero");
    test_tag_transparent_integer(CBOR_TAG_EPOCH_DATETIME, 1700000000LL,
                                 "epoch_timestamp");
    test_tag_transparent_integer(CBOR_TAG_EPOCH_DATETIME, -1, "minus_one");
    /* Tag 100: another calendar tag */
    test_tag_transparent_integer(100, INT32_MAX, "INT32_MAX");
    /* Self-described CBOR (tag 55799) wrapping an integer */
    test_tag_transparent_integer(CBOR_TAG_SELF_DESCRIBED, 42, "self_described_42");

    /* ---- OCTET STRING: decode through Base64 hint tag (tag 22 is "just a hint") ---- */
    {
        static const uint8_t payload[] = {0x68, 0x65, 0x6C, 0x6C, 0x6F}; /* "hello" */
        uint8_t tagged[32];
        size_t tagged_len;
        OCTET_STRING_t *decoded = NULL;
        asn_dec_rval_t dr;
        struct buffer_acc inner_acc;

        memset(&inner_acc, 0, sizeof(inner_acc));
        {
            OCTET_STRING_t orig;
            asn_enc_rval_t er;
            memset(&orig, 0, sizeof(orig));
            orig.buf = (uint8_t *)(uintptr_t)payload;
            orig.size = sizeof(payload);
            er = cbor_encode(&asn_DEF_OCTET_STRING, &orig, buf_append, &inner_acc);
            assert(er.encoded > 0);
        }

        tagged_len = prepend_tag(CBOR_TAG_BASE64, inner_acc.data, inner_acc.len,
                                 tagged, sizeof(tagged));
        assert(tagged_len > 0);

        dr = cbor_decode(NULL, &asn_DEF_OCTET_STRING, (void **)&decoded,
                         tagged, tagged_len);
        if(dr.code != RC_OK || !decoded) {
            fprintf(stderr, "FAIL: tag-transparent OCTET_STRING decode (code=%d)\n",
                    dr.code);
            exit(1);
        }
        if((size_t)decoded->size != sizeof(payload)
           || memcmp(decoded->buf, payload, sizeof(payload)) != 0) {
            fprintf(stderr, "FAIL: tag-transparent OCTET_STRING data mismatch\n");
            exit(1);
        }
        assert(dr.consumed == tagged_len);
        ASN_STRUCT_FREE(asn_DEF_OCTET_STRING, decoded);
        buf_free(&inner_acc);
        printf("  ✓ tag %d (base64 hint) transparent decode: OCTET_STRING\n",
               CBOR_TAG_BASE64);
    }

    /* ---- NativeInteger: decode through tag 1 ---- */
    {
        long orig_val = 9999, result = 0;
        long *decoded = NULL;
        struct buffer_acc inner_acc;
        uint8_t tagged[32];
        size_t tagged_len;
        asn_enc_rval_t er;
        asn_dec_rval_t dr;

        memset(&inner_acc, 0, sizeof(inner_acc));
        er = cbor_encode(&asn_DEF_NativeInteger, &orig_val, buf_append, &inner_acc);
        assert(er.encoded > 0);

        tagged_len = prepend_tag(CBOR_TAG_EPOCH_DATETIME,
                                 inner_acc.data, inner_acc.len,
                                 tagged, sizeof(tagged));
        assert(tagged_len > 0);

        dr = cbor_decode(NULL, &asn_DEF_NativeInteger, (void **)&decoded,
                         tagged, tagged_len);
        if(dr.code != RC_OK || !decoded) {
            fprintf(stderr,
                    "FAIL: tag-transparent NativeInteger decode (code=%d)\n",
                    dr.code);
            exit(1);
        }
        result = *decoded;
        free(decoded);
        buf_free(&inner_acc);
        if(result != orig_val) {
            fprintf(stderr, "FAIL: tag-transparent NativeInteger mismatch: "
                    "got %ld want %ld\n", result, orig_val);
            exit(1);
        }
        printf("  ✓ tag %d transparent decode: NativeInteger\n",
               CBOR_TAG_EPOCH_DATETIME);
    }

    /* ---- BIT STRING: decode through self-described tag ---- */
    {
        uint8_t bits[] = {0xAB, 0xCD};
        BIT_STRING_t orig, *decoded = NULL;
        struct buffer_acc inner_acc;
        uint8_t tagged[32];
        size_t tagged_len;
        asn_enc_rval_t er;
        asn_dec_rval_t dr;

        memset(&orig, 0, sizeof(orig));
        memset(&inner_acc, 0, sizeof(inner_acc));
        orig.buf = bits;
        orig.size = 2;
        orig.bits_unused = 0;

        er = cbor_encode(&asn_DEF_BIT_STRING, &orig, buf_append, &inner_acc);
        assert(er.encoded > 0);

        tagged_len = prepend_tag(CBOR_TAG_SELF_DESCRIBED,
                                 inner_acc.data, inner_acc.len,
                                 tagged, sizeof(tagged));
        assert(tagged_len > 0);

        dr = cbor_decode(NULL, &asn_DEF_BIT_STRING, (void **)&decoded,
                         tagged, tagged_len);
        if(dr.code != RC_OK || !decoded) {
            fprintf(stderr,
                    "FAIL: tag-transparent BIT_STRING decode (code=%d)\n",
                    dr.code);
            exit(1);
        }
        if(decoded->size != 2 || decoded->bits_unused != 0
           || memcmp(decoded->buf, bits, 2) != 0) {
            fprintf(stderr, "FAIL: tag-transparent BIT_STRING data mismatch\n");
            exit(1);
        }
        assert(dr.consumed == tagged_len);
        ASN_STRUCT_FREE(asn_DEF_BIT_STRING, decoded);
        buf_free(&inner_acc);
        printf("  ✓ tag %d (self-described) transparent decode: BIT_STRING\n",
               CBOR_TAG_SELF_DESCRIBED);
    }

    printf("PASSED: test_cbor_tag_transparent_decode\n\n");
}

/*
 * Test nested (multiple) CBOR tags on a single value.
 * RFC 8949 §3.4 allows any number of consecutive tag wrappers.
 */
static void
test_cbor_nested_tags(void) {
    printf("test_cbor_nested_tags\n");

    /* Manually construct: tag(55799) + tag(1) + integer(42)
     * 0xD9 0xD9 0xF7  (tag 55799)
     * 0xC1             (tag 1)
     * 0x18 0x2A        (uint 42)
     */
    {
        uint8_t buf[] = {0xD9, 0xD9, 0xF7, 0xC1, 0x18, 0x2A};
        INTEGER_t *decoded = NULL;
        asn_dec_rval_t dr = cbor_decode(NULL, &asn_DEF_INTEGER,
                                        (void **)&decoded,
                                        buf, sizeof(buf));
        if(dr.code != RC_OK || !decoded) {
            fprintf(stderr, "FAIL: nested tags INTEGER decode (code=%d)\n",
                    dr.code);
            exit(1);
        }
        intmax_t result;
        if(asn_INTEGER2imax(decoded, &result) || result != 42) {
            fprintf(stderr, "FAIL: nested tags INTEGER value: got %jd want 42\n",
                    result);
            exit(1);
        }
        assert(dr.consumed == sizeof(buf));
        ASN_STRUCT_FREE(asn_DEF_INTEGER, decoded);
        printf("  ✓ nested tags (55799 + 1) decode: INTEGER 42\n");
    }

    /* Manually construct: tag(22) + tag(32) + bytes("hi")
     * Tag numbers 0-23 use a single-byte header (major 6, arg inline):
     *   0xD6 = 0xC0 | 22   (tag 22, base64 hint – compact form)
     * Tag numbers 24-255 use a two-byte header:
     *   0xD8 0x20 = tag 32  (URI hint – extended 1-byte form)
     * 0x42 0x68 0x69        (bytes "hi", length 2)
     */
    {
        uint8_t buf[] = {0xD6, 0xD8, 0x20, 0x42, 0x68, 0x69};
        OCTET_STRING_t *decoded = NULL;
        asn_dec_rval_t dr = cbor_decode(NULL, &asn_DEF_OCTET_STRING,
                                        (void **)&decoded,
                                        buf, sizeof(buf));
        if(dr.code != RC_OK || !decoded) {
            fprintf(stderr,
                    "FAIL: nested tags OCTET_STRING decode (code=%d)\n",
                    dr.code);
            exit(1);
        }
        if(decoded->size != 2 || decoded->buf[0] != 0x68
           || decoded->buf[1] != 0x69) {
            fprintf(stderr, "FAIL: nested tags OCTET_STRING data mismatch\n");
            exit(1);
        }
        assert(dr.consumed == sizeof(buf));
        ASN_STRUCT_FREE(asn_DEF_OCTET_STRING, decoded);
        printf("  ✓ nested tags (22 + 32) decode: OCTET_STRING \"hi\"\n");
    }

    printf("PASSED: test_cbor_nested_tags\n\n");
}

/* ------------------------------------------------------------------ */
/* C509 draft CBOR sequence fixture tests                               */
/* ------------------------------------------------------------------ */

struct c509_draft_vector {
    const char *label;
    const char *section;
    const char *hex;
    size_t expected_len;
    size_t expected_items;
    uint8_t expected_first_major;
    uint64_t expected_first_arg;
};

static int
is_hex_space(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

static int
hex_nibble(char c) {
    if(c >= '0' && c <= '9') return c - '0';
    if(c >= 'a' && c <= 'f') return c - 'a' + 10;
    if(c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

static int
decode_hex_fixture(const char *hex, uint8_t *out, size_t out_cap,
                   size_t *out_len) {
    size_t len = 0;
    int high = -1;

    if(!hex || !out || !out_len)
        return -1;

    for(; *hex; hex++) {
        int n;

        if(is_hex_space(*hex))
            continue;

        n = hex_nibble(*hex);
        if(n < 0)
            return -1;

        if(high < 0) {
            high = n;
        } else {
            if(len >= out_cap)
                return -1;
            out[len++] = (uint8_t)((high << 4) | n);
            high = -1;
        }
    }

    if(high >= 0)
        return -1;

    *out_len = len;
    return 0;
}

static int
count_cbor_sequence_items(const uint8_t *buf, size_t len, size_t *item_count) {
    size_t offset = 0;
    size_t count = 0;

    if(!buf || !item_count)
        return -1;

    while(offset < len) {
        ssize_t n = cbor_skip_item(buf + offset, len - offset);
        if(n <= 0 || (size_t)n > len - offset)
            return -1;
        offset += (size_t)n;
        count++;
    }

    *item_count = count;
    return 0;
}

static void
check_c509_draft_vector(const struct c509_draft_vector *vector) {
    uint8_t buf[512];
    size_t len = 0;
    size_t item_count = 0;
    uint8_t major = 0;
    uint64_t arg = 0;
    ssize_t hlen;

    assert(vector);
    assert(vector->expected_len <= sizeof(buf));

    if(decode_hex_fixture(vector->hex, buf, sizeof(buf), &len) != 0) {
        fprintf(stderr, "FAIL: parse %s (%s)\n",
                vector->label, vector->section);
        exit(1);
    }

    if(len != vector->expected_len) {
        fprintf(stderr, "FAIL: %s length=%zu want=%zu\n",
                vector->label, len, vector->expected_len);
        exit(1);
    }

    hlen = cbor_decode_head(buf, len, &major, &arg);
    if(hlen <= 0 || major != vector->expected_first_major
       || arg != vector->expected_first_arg) {
        fprintf(stderr,
                "FAIL: %s first item major=%u arg=%llu want major=%u arg=%llu\n",
                vector->label,
                (unsigned)major, (unsigned long long)arg,
                (unsigned)vector->expected_first_major,
                (unsigned long long)vector->expected_first_arg);
        exit(1);
    }

    if(count_cbor_sequence_items(buf, len, &item_count) != 0) {
        fprintf(stderr, "FAIL: %s is not a complete CBOR sequence\n",
                vector->label);
        exit(1);
    }

    if(item_count != vector->expected_items) {
        fprintf(stderr, "FAIL: %s item_count=%zu want=%zu\n",
                vector->label, item_count, vector->expected_items);
        exit(1);
    }

    printf("  ✓ %s (%s): %zu bytes, %zu CBOR sequence items\n",
           vector->label, vector->section, len, item_count);
}

static void
test_c509_draft_hex_fixture_validation(void) {
    uint8_t buf[2];
    size_t len = 0;

    assert(decode_hex_fixture(NULL, buf, sizeof(buf), &len) < 0);
    assert(decode_hex_fixture("0G", buf, sizeof(buf), &len) < 0);
    assert(decode_hex_fixture("0", buf, sizeof(buf), &len) < 0);
    assert(decode_hex_fixture("0001", buf, 1, &len) < 0);
    assert(decode_hex_fixture("00 01\n", buf, sizeof(buf), &len) == 0);
    assert(len == 2 && buf[0] == 0x00 && buf[1] == 0x01);

    printf("  ✓ C509 draft hex fixture parser rejects malformed input\n");
}

static void
test_c509_draft_negative_vectors(void) {
    uint8_t buf[128];
    size_t len = 0;
    size_t item_count = 0;

    /*
     * The positive C509 CA certificate fixture ends with an empty byte
     * string.  Requiring one payload byte turns that final item into a
     * truncated byte string, which cbor_skip_item() must reject.
     */
    static const char *ca_type2_hex =
        "02410105F61A677485801A6B36EC7F67746573742063610C58205A9414AC56D1B6AF"
        "0C966FC53B9476B5C95D0EEAAEF764D9EFE86DB7320C36E18801540369D71F96FE12"
        "58A746AC2B208E756E6D1D3ED921186003676162632E636F6D232040";

    assert(decode_hex_fixture(ca_type2_hex, buf, sizeof(buf), &len) == 0);
    assert(len > 0);
    buf[len - 1] = 0x41;
    assert(count_cbor_sequence_items(buf, len, &item_count) < 0);

    /*
     * The one-element template fixture has its extensions field as the
     * final CBOR array.  Inflating that final array count makes the
     * sequence structurally incomplete.
     */
    assert(decode_hex_fixture("008102810084010101F78101F78303F4F7",
                              buf, sizeof(buf), &len) == 0);
    assert(len == 17);
    buf[13] = 0x84;
    assert(count_cbor_sequence_items(buf, len, &item_count) < 0);

    printf("  ✓ C509 draft malformed/truncated vectors are rejected\n");
}

static void
test_c509_draft_vectors(void) {
    static const struct c509_draft_vector vectors[] = {
        {
            "C509 CA certificate type 3",
            "draft-ietf-cose-c509-test-vectors-01 section 2.3",
            "03410105F61A677485801A6B36EC7F67746573742063610C58205A9414AC56D1B6AF"
            "0C966FC53B9476B5C95D0EEAAEF764D9EFE86DB7320C36E18801547FCDB82D04952E"
            "1A36B90AF37A3CF166D15EF92121186003676162632E636F6D232040",
            96, 11, CBOR_MAJOR_UINT, 3
        },
        {
            "C509 CA certificate type 2",
            "draft-ietf-cose-c509-test-vectors-01 section 2.4",
            "02410105F61A677485801A6B36EC7F67746573742063610C58205A9414AC56D1B6AF"
            "0C966FC53B9476B5C95D0EEAAEF764D9EFE86DB7320C36E18801540369D71F96FE12"
            "58A746AC2B208E756E6D1D3ED921186003676162632E636F6D232040",
            96, 11, CBOR_MAJOR_UINT, 2
        },
        {
            "C509 unsigned X25519 certification request type 3",
            "draft-ietf-cose-c509-test-vectors-01 section 8.6.3",
            "0305667832353531390858208AFF516FAC71244150E70F9277F4ADF7FB29F41A7A4A"
            "8828BD476722FC1B7F088202836B64656D6F206973737565724102F640",
            63, 7, CBOR_MAJOR_UINT, 3
        },
        {
            "C509 unsigned X25519 certification request type 2",
            "draft-ietf-cose-c509-test-vectors-01 section 8.6.4",
            "0205667832353531390858208AFF516FAC71244150E70F9277F4ADF7FB29F41A7A4A"
            "8828BD476722FC1B7F088202836B64656D6F206973737565724102F640",
            63, 7, CBOR_MAJOR_UINT, 2
        },
        {
            "C509 request template undefined fields",
            "draft-ietf-cose-c509-test-vectors-01 section 10.1",
            "00F7F7F7F7F7F7",
            7, 7, CBOR_MAJOR_UINT, 0
        },
        {
            "C509 request template one element per field",
            "draft-ietf-cose-c509-test-vectors-01 section 10.2",
            "008102810084010101F78101F78303F4F7",
            17, 7, CBOR_MAJOR_UINT, 0
        },
        {
            "C509 request template complex choices",
            "draft-ietf-cose-c509-test-vectors-01 section 10.3",
            "008202038301492B0601040181FD590982492B0601040181FD590A42050090010101"
            "F7040101624445492B0601040181FD590B0101F7492B0601040181FD590C01014D0C"
            "0B636F6E73742D76616C75658301492B0601040181FD590982492B0601040181FD59"
            "0A420500F78C08F4F702F51860492B0601040181FD590DF4F7492B0601040181FD59"
            "0EF44D0C0B636F6E73742D76616C7565",
            152, 7, CBOR_MAJOR_UINT, 0
        }
    };
    size_t i;

    printf("test_c509_draft_vectors\n");
    test_c509_draft_hex_fixture_validation();

    for(i = 0; i < sizeof(vectors) / sizeof(vectors[0]); i++) {
        check_c509_draft_vector(&vectors[i]);
    }

    test_c509_draft_negative_vectors();
    printf("PASSED: test_c509_draft_vectors\n\n");
}

/* ------------------------------------------------------------------ */
/* main                                                                 */
/* ------------------------------------------------------------------ */
int
main(void) {
    printf("=== CBOR Codec Tests ===\n\n");

    test_integer_cbor_edge_cases();
    test_native_integer_cbor_edge_cases();
    test_bit_string_cbor_edge_cases();
    test_octet_string_cbor_edge_cases();
    test_asn_encode_decode_cbor_dispatch();
    test_oid_cbor_roundtrip();
    test_cbor_tag_encoding();
    test_cbor_skip_tags();
    test_cbor_skip_item_stack_limit();
    test_cbor_tag_transparent_decode();
    test_cbor_nested_tags();
    test_c509_draft_vectors();

    printf("=== ALL CBOR TESTS PASSED ===\n");
    return 0;
}
