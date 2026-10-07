/*
 * Tests for the updated XER OCTET STRING hex/Base64 handling (issue #538).
 *
 * Covers:
 *  17a. Default OCTET_STRING XER encoder (OCTET_STRING_encode_xer) emits
 *       contiguous upper-case hex for XER_F_BASIC and XER_F_CANONICAL.
 *  17b. XER_F_BASIC|XER_F_BASE64 emits Base64; XER_F_CANONICAL|XER_F_BASE64
 *       emits hex (canonical overrides XER_F_BASE64).
 *  17c. Regression #538: "0x6B" treated as Base64, not hex (no 0x heuristic).
 *  17d. Liberal hex: lower-case digits, internal whitespace, H'aAbB'.
 *  17e. Odd-length pure-hex-alphabet string decoded as liberal hex.
 *  17f. Garbage input → RC_FAIL.
 *  17g. Chunked auto-decode: format pinned by first non-ws chunk.
 */
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include <OCTET_STRING.h>
#include <xer_decoder.h>
#include <xer_encoder.h>

/* ------------------------------------------------------------------ */
/* Encode helpers                                                       */
/* ------------------------------------------------------------------ */

static char enc_buf[8192];
static size_t enc_off;

static int
collect(const void *buf, size_t size, void *key) {
    (void)key;
    assert(enc_off + size < sizeof(enc_buf));
    memcpy(enc_buf + enc_off, buf, size);
    enc_off += size;
    return 0;
}

static const char *
encode_os(const uint8_t *data, size_t len, enum xer_encoder_flags_e flags) {
    OCTET_STRING_t os;
    asn_enc_rval_t er;
    /* Copy to a mutable buffer: OCTET_STRING_t.buf is uint8_t* (non-const)
     * and the encoder only reads it, but the cast would drop const. */
    uint8_t *buf_copy = malloc(len ? len : 1);
    assert(buf_copy);
    memcpy(buf_copy, data, len);

    memset(&os, 0, sizeof(os));
    os.buf = buf_copy;
    os.size = len;

    enc_off = 0;
    memset(enc_buf, 0, sizeof(enc_buf));

    er = OCTET_STRING_encode_xer(&asn_DEF_OCTET_STRING, &os,
                                 0, flags, collect, NULL);
    free(buf_copy);
    assert(er.encoded >= 0);
    enc_buf[enc_off] = '\0';
    return enc_buf;
}

/* Decode a complete "<tag>VALUE</tag>" XML fragment via _decode_xer_auto. */
static int
decode_auto(const char *xml, uint8_t **out_buf, size_t *out_size) {
    OCTET_STRING_t *st = NULL;
    asn_dec_rval_t dr;

    dr = OCTET_STRING_decode_xer_auto(NULL, &asn_DEF_OCTET_STRING,
                                      (void **)&st, "tag",
                                      xml, strlen(xml));
    if(dr.code != RC_OK) {
        if(st) ASN_STRUCT_FREE(asn_DEF_OCTET_STRING, st);
        return -1;
    }
    *out_buf  = st->buf;
    *out_size = st->size;
    /* Transfer ownership of buf; free the shell */
    st->buf = NULL;
    ASN_STRUCT_FREE(asn_DEF_OCTET_STRING, st);
    return 0;
}

static int
decode_default(const char *xml, uint8_t **out_buf, size_t *out_size) {
    OCTET_STRING_t *st = NULL;
    asn_dec_rval_t dr;

    dr = xer_decode(NULL, &asn_DEF_OCTET_STRING, (void **)&st,
                    xml, strlen(xml));
    if(dr.code != RC_OK) {
        if(st) ASN_STRUCT_FREE(asn_DEF_OCTET_STRING, st);
        return -1;
    }
    *out_buf  = st->buf;
    *out_size = st->size;
    st->buf = NULL;
    ASN_STRUCT_FREE(asn_DEF_OCTET_STRING, st);
    return 0;
}

/* ------------------------------------------------------------------ */
/* 17a: Default encoder emits contiguous upper-case hex                 */
/* ------------------------------------------------------------------ */
static void
test_17a_default_encoder_hex(void) {
    static const uint8_t data[] = {0xD3, 0x1E, 0x81};
    static const uint8_t extn_value[] = {0x03, 0x02, 0x01, 0xC6};
    const char *got;

    printf("17a: Default encoder emits upper-case hex\n");

    /* BASIC-XER */
    got = encode_os(data, sizeof(data), XER_F_BASIC);
    printf("     XER_F_BASIC   -> \"%s\"\n", got);
    assert(strcmp(got, "D31E81") == 0);

    /* CANONICAL-XER */
    got = encode_os(data, sizeof(data), XER_F_CANONICAL);
    printf("     XER_F_CANONICAL -> \"%s\"\n", got);
    assert(strcmp(got, "D31E81") == 0);

    /* Auto-decode of the hex output must give back the original bytes */
    {
        uint8_t *out; size_t out_sz;
        assert(decode_auto("<tag>D31E81</tag>", &out, &out_sz) == 0);
        assert(out_sz == sizeof(data));
        assert(memcmp(out, data, sizeof(data)) == 0);
        free(out);
    }
    {
        uint8_t *out; size_t out_sz;
        assert(decode_default("<OCTET_STRING>030201C6</OCTET_STRING>",
                              &out, &out_sz) == 0);
        assert(out_sz == sizeof(extn_value));
        assert(memcmp(out, extn_value, sizeof(extn_value)) == 0);
        free(out);
    }
    printf("     Round-trip: OK\n");
}

/* ------------------------------------------------------------------ */
/* 17b: XER_F_BASE64 flag; canonical overrides                          */
/* ------------------------------------------------------------------ */
static void
test_17b_base64_flag(void) {
    static const uint8_t data[] = {0xD3, 0x1E, 0x81};
    const char *got;

    printf("17b: XER_F_BASE64 flag and canonical override\n");

    /* BASIC + BASE64 should give Base64 output */
    got = encode_os(data, sizeof(data), XER_F_BASIC | XER_F_BASE64);
    printf("     XER_F_BASIC|XER_F_BASE64   -> \"%s\"\n", got);
    assert(strcmp(got, "0x6B") == 0);  /* D3 1E 81 → Base64 "0x6B" */

    /* CANONICAL + BASE64 → canonical wins, so hex output */
    got = encode_os(data, sizeof(data), XER_F_CANONICAL | XER_F_BASE64);
    printf("     XER_F_CANONICAL|XER_F_BASE64 -> \"%s\"\n", got);
    assert(strcmp(got, "D31E81") == 0);
}

/* ------------------------------------------------------------------ */
/* 17c: Regression #538 — "0x6B" is Base64, not hex-with-0x-prefix    */
/* ------------------------------------------------------------------ */
static void
test_17c_regression_538(void) {
    /* {0xD3, 0x1E, 0x81} → Base64 "0x6B"
     * The old code had a "0x"/"0X" heuristic that would have treated this as
     * hex 0x6B = {0x6B}, which is wrong.  Correct: parse as Base64. */
    static const uint8_t expected[] = {0xD3, 0x1E, 0x81};
    uint8_t *out; size_t out_sz;

    printf("17c: \"0x6B\" decoded as Base64 (regression #538)\n");
    assert(decode_auto("<tag>0x6B</tag>", &out, &out_sz) == 0);
    printf("     got %zu byte(s): ", out_sz);
    for(size_t i = 0; i < out_sz; i++) printf("%02X ", out[i]);
    printf("\n");
    assert(out_sz == sizeof(expected));
    assert(memcmp(out, expected, sizeof(expected)) == 0);
    free(out);

    /* Likewise "0X12" */
    printf("     \"0X12\" also treated as Base64 (not hex)\n");
    assert(decode_auto("<tag>0X12</tag>", &out, &out_sz) == 0);
    /* Base64 "0X12" → {0xD1, 0x7D, 0xA6} — just verify it is NOT {0x12} */
    assert(out_sz != 1 || out[0] != 0x12);
    free(out);
}

/* ------------------------------------------------------------------ */
/* 17d: Liberal hex — lower-case, internal whitespace, H' prefix       */
/* ------------------------------------------------------------------ */
static void
test_17d_liberal_hex(void) {
    static const uint8_t expected_aabb[] = {0xAA, 0xBB};
    uint8_t *out; size_t out_sz;

    printf("17d: Liberal hex decoding\n");

    /* Lower-case digits */
    assert(decode_auto("<tag>aabb</tag>", &out, &out_sz) == 0);
    assert(out_sz == 2 && memcmp(out, expected_aabb, 2) == 0);
    free(out);
    printf("     lower-case \"aabb\": OK\n");

    /* Internal whitespace */
    assert(decode_auto("<tag>AA BB</tag>", &out, &out_sz) == 0);
    assert(out_sz == 2 && memcmp(out, expected_aabb, 2) == 0);
    free(out);
    printf("     whitespace \"AA BB\": OK\n");

    /* H' prefix (nonstandard) */
    assert(decode_auto("<tag>H'aAbB'</tag>", &out, &out_sz) == 0);
    assert(out_sz == 2 && memcmp(out, expected_aabb, 2) == 0);
    free(out);
    printf("     H'aAbB': OK\n");
}

/* ------------------------------------------------------------------ */
/* 17e: Odd-length pure-hex-alphabet string decoded as liberal hex     */
/* ------------------------------------------------------------------ */
static void
test_17e_odd_length_as_base64(void) {
    /* "ABC" has 3 characters in hex alphabet; liberal hex decoding accepts
     * odd nibble counts by treating the trailing nibble as high 4 bits. */
    static const uint8_t expected[] = {0xAB, 0xC0};
    uint8_t *out; size_t out_sz;

    printf("17e: Odd-length hex-alphabet string decoded as liberal hex\n");
    assert(decode_auto("<tag>ABC</tag>", &out, &out_sz) == 0);
    printf("     \"ABC\" → %zu byte(s)\n", out_sz);
    assert(out_sz == sizeof(expected));
    assert(memcmp(out, expected, sizeof(expected)) == 0);
    free(out);
}

/* ------------------------------------------------------------------ */
/* 17f: Garbage input → RC_FAIL                                         */
/* ------------------------------------------------------------------ */
static void
test_17f_garbage(void) {
    OCTET_STRING_t *st = NULL;
    asn_dec_rval_t dr;

    printf("17f: Garbage input rejected\n");

    dr = OCTET_STRING_decode_xer_auto(NULL, &asn_DEF_OCTET_STRING,
                                      (void **)&st, "tag",
                                      "<tag>P!Q</tag>", 14);
    if(st) ASN_STRUCT_FREE(asn_DEF_OCTET_STRING, st);
    assert(dr.code != RC_OK);
    printf("     \"P!Q\" correctly rejected (code=%d)\n", dr.code);
}

/* ------------------------------------------------------------------ */
/* 17g: Chunked auto-decode — format pinned by first chunk             */
/* ------------------------------------------------------------------ */
static void
test_17g_chunked(void) {
    /*
     * "AABB" is an ambiguous body (pure hex alphabet, even count).
     * We split it at the body level: first feed "<tag>AA" so that
     * body_receiver gets "AA", classifies it as AMBIGUOUS_HEX and pins
     * format_decided=1 (hex).  Then feed "BB</tag>" so body_receiver
     * gets "BB" using the pinned hex converter.
     * Expected result: {0xAA, 0xBB}.
     *
     * NOTE: The XER tokeniser (pxml) needs at least a complete XML token
     * to make progress.  Feeding 1 byte at a time produces consumed=0
     * (PXER_WMORE) for partial tokens, which would stall.  We split at
     * real token boundaries instead, using the advancing-pointer API that
     * the XER decoder is designed for.
     */
    static const uint8_t expected[] = {0xAA, 0xBB};
    /* chunk1 contains the opening tag plus the first 2 body bytes.
     * chunk2 contains the remaining 2 body bytes plus the closing tag. */
    static const char chunk1[] = "<tag>AA";     /* 7 bytes */
    static const char chunk2[] = "BB</tag>";    /* 8 bytes */
    OCTET_STRING_t *st = NULL;
    asn_dec_rval_t dr;

    printf("17g: Chunked decode — format pinned by first body chunk\n");

    /* First chunk: opening tag + first 2 body bytes */
    dr = OCTET_STRING_decode_xer_auto(NULL, &asn_DEF_OCTET_STRING,
                                      (void **)&st, "tag",
                                      chunk1, sizeof(chunk1) - 1);
    if(dr.code != RC_WMORE) {
        printf("  ERROR: Expected RC_WMORE after chunk1, got %d\n", dr.code);
        if(st) ASN_STRUCT_FREE(asn_DEF_OCTET_STRING, st);
        assert(0);
    }
    printf("     After chunk1 (%s): RC_WMORE, consumed=%zu\n",
           chunk1, dr.consumed);

    /* Second chunk: remaining body bytes + closing tag */
    dr = OCTET_STRING_decode_xer_auto(NULL, &asn_DEF_OCTET_STRING,
                                      (void **)&st, "tag",
                                      chunk2, sizeof(chunk2) - 1);
    if(dr.code != RC_OK) {
        printf("  ERROR: Expected RC_OK after chunk2, got %d\n", dr.code);
        if(st) ASN_STRUCT_FREE(asn_DEF_OCTET_STRING, st);
        assert(0);
    }

    assert(st != NULL);
    printf("     After chunk2 (%s): RC_OK, result %zu byte(s):", chunk2, st->size);
    for(size_t i = 0; i < st->size; i++) printf(" %02X", st->buf[i]);
    printf("\n");
    assert(st->size == sizeof(expected));
    assert(memcmp(st->buf, expected, sizeof(expected)) == 0);
    ASN_STRUCT_FREE(asn_DEF_OCTET_STRING, st);
    printf("     Format pinned to hex across chunks: OK\n");
}

/* ------------------------------------------------------------------ */
/* main                                                                 */
/* ------------------------------------------------------------------ */
int
main(void) {
    printf("=== XER OCTET STRING auto-detect tests (issue #538) ===\n\n");

    test_17a_default_encoder_hex();
    test_17b_base64_flag();
    test_17c_regression_538();
    test_17d_liberal_hex();
    test_17e_odd_length_as_base64();
    test_17f_garbage();
    test_17g_chunked();

    printf("\n=== All tests passed ===\n");
    return 0;
}
