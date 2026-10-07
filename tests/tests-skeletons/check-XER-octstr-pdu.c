/*
 * Full-path tests for XER OCTET STRING Base64/hex handling.
 *
 * Tests the complete encode→decode round-trip and verifies that schema-level
 * ENCODING-CONTROL instructions take priority over runtime XER_F_BASE64,
 * which in turn is overridden by XER_F_CANONICAL.
 *
 * Priority table (highest to lowest):
 *   1. Schema ::= hexadecimal — always hex; masks XER_F_BASE64
 *   2. Schema ::= base64      — always Base64; ignores XER_F_CANONICAL
 *   3. XER_F_CANONICAL        — forces hex for un-annotated types
 *   4. XER_F_BASE64           — requests Base64 for un-annotated types
 *
 * Tests:
 *   RT1. Base64 encode→decode round-trip ({0xD3,0x1E,0x81} = "0x6B").
 *   RT2. All-hex-alphabet Base64 round-trip: "ABCD" pinned to Base64,
 *        not misclassified as hex (the key #538 disambiguation case).
 *   RT3. Hex encode→decode round-trip.
 *   RT4. Large value (1 KB) Base64 round-trip.
 *   RT5. Hex annotation masks XER_F_BASE64: flags & ~XER_F_BASE64 → hex.
 *   RT6. Base64 annotation ignores XER_F_CANONICAL: schema wins.
 *   RT7. Auto-decode after Base64-encoded PDU: pinned decoder handles
 *        all-hex-alphabet content ("ABCD" → {0x00,0x10,0x83}).
 *   RT8. XER_F_CANONICAL + XER_F_BASE64 → hex (canonical overrides flag).
 */
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include <OCTET_STRING.h>
#include <xer_encoder.h>

/* ------------------------------------------------------------------ */
/* Encode helpers                                                       */
/* ------------------------------------------------------------------ */

static char enc_buf[16384];
static size_t enc_off;

static int collect(const void *buf, size_t size, void *key) {
    (void)key;
    assert(enc_off + size < sizeof(enc_buf));
    memcpy(enc_buf + enc_off, buf, size);
    enc_off += size;
    return 0;
}

/* Build a mutable OCTET_STRING_t from a const buffer for encoding.
 * Encoders only READ buf, but the field is uint8_t* (non-const). */
static OCTET_STRING_t
make_os(const uint8_t *data, size_t len) {
    OCTET_STRING_t os;
    memset(&os, 0, sizeof(os));
    os.buf = malloc(len ? len : 1);
    assert(os.buf);
    memcpy(os.buf, data, len);
    os.size = len;
    return os;
}

/* Encode via OCTET_STRING_encode_xer, return the result string. */
static const char *
hex_encode(const uint8_t *data, size_t len, enum xer_encoder_flags_e flags) {
    OCTET_STRING_t os = make_os(data, len);
    enc_off = 0;
    asn_enc_rval_t er = OCTET_STRING_encode_xer(
        &asn_DEF_OCTET_STRING, &os, 0, flags, collect, NULL);
    free(os.buf);
    assert(er.encoded >= 0);
    enc_buf[enc_off] = '\0';
    return enc_buf;
}

/* Encode via OCTET_STRING_encode_xer_base64. */
static const char *
b64_encode(const uint8_t *data, size_t len, enum xer_encoder_flags_e flags) {
    OCTET_STRING_t os = make_os(data, len);
    enc_off = 0;
    asn_enc_rval_t er = OCTET_STRING_encode_xer_base64(
        &asn_DEF_OCTET_STRING, &os, 0, flags, collect, NULL);
    free(os.buf);
    assert(er.encoded >= 0);
    enc_buf[enc_off] = '\0';
    return enc_buf;
}

/* Decode an "<tag>VALUE</tag>" fragment via the specified decoder. */
typedef asn_dec_rval_t(*xer_dec_f)(const asn_codec_ctx_t *,
    const asn_TYPE_descriptor_t *, void **,
    const char *, const void *, size_t);

static int
decode_xml(xer_dec_f decoder, const char *value_b64,
           uint8_t **out_buf, size_t *out_size) {
    char xml[16384];
    snprintf(xml, sizeof(xml), "<tag>%s</tag>", value_b64);
    OCTET_STRING_t *st = NULL;
    asn_dec_rval_t dr = decoder(NULL, &asn_DEF_OCTET_STRING,
                                (void **)&st, "tag",
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
/* RT1: Basic Base64 encode→decode round-trip                         */
/* ------------------------------------------------------------------ */
static void test_rt1_base64_roundtrip(void) {
    static const uint8_t data[] = {0xD3, 0x1E, 0x81};
    printf("RT1: Base64 encode→decode round-trip\n");

    const char *encoded = b64_encode(data, sizeof(data), XER_F_CANONICAL);
    printf("     {0xD3,0x1E,0x81} -> Base64 \"%s\"\n", encoded);
    assert(strcmp(encoded, "0x6B") == 0);  /* verified by Python */

    uint8_t *out; size_t out_sz;
    assert(decode_xml(OCTET_STRING_decode_xer_base64,
                      encoded, &out, &out_sz) == 0);
    assert(out_sz == sizeof(data));
    assert(memcmp(out, data, sizeof(data)) == 0);
    free(out);
    printf("     Round-trip: OK\n");
}

/* ------------------------------------------------------------------ */
/* RT2: All-hex-alphabet Base64 round-trip with PINNED decoder        */
/* ------------------------------------------------------------------ */
static void test_rt2_allhex_base64_pinned(void) {
    /*
     * {0x00,0x10,0x83} Base64-encodes to "ABCD" — a value that is also
     * valid hex (but would decode to {0xAB,0xCD} if misclassified).
     * The pinned Base64 decoder (used when schema says ::= base64) must
     * return {0x00,0x10,0x83}, not {0xAB,0xCD}.
     */
    static const uint8_t data[]     = {0x00, 0x10, 0x83};
    static const uint8_t hex_wrong[]= {0xAB, 0xCD};
    printf("RT2: All-hex-alphabet Base64 round-trip (pinned decoder)\n");

    const char *encoded = b64_encode(data, sizeof(data), XER_F_CANONICAL);
    printf("     {0x00,0x10,0x83} -> Base64 \"%s\"\n", encoded);
    assert(strcmp(encoded, "ABCD") == 0);

    /* Pinned Base64 decoder: must give original bytes */
    uint8_t *out; size_t out_sz;
    assert(decode_xml(OCTET_STRING_decode_xer_base64,
                      encoded, &out, &out_sz) == 0);
    printf("     Pinned-Base64 decode: %zu bytes:", out_sz);
    for(size_t i = 0; i < out_sz; i++) printf(" %02X", out[i]);
    printf("\n");
    assert(out_sz == sizeof(data));
    assert(memcmp(out, data, sizeof(data)) == 0);
    /* Verify it would have been wrong with hex */
    assert(!(out_sz == sizeof(hex_wrong) &&
             memcmp(out, hex_wrong, sizeof(hex_wrong)) == 0));
    free(out);
    printf("     Not misclassified as hex: OK\n");
}

/* ------------------------------------------------------------------ */
/* RT3: Hex encode→decode round-trip                                  */
/* ------------------------------------------------------------------ */
static void test_rt3_hex_roundtrip(void) {
    static const uint8_t data[] = {0xAA, 0xBB, 0xCC, 0xDD};
    printf("RT3: Hex encode→decode round-trip\n");

    const char *encoded = hex_encode(data, sizeof(data), XER_F_CANONICAL);
    printf("     {0xAA,0xBB,0xCC,0xDD} -> hex \"%s\"\n", encoded);
    assert(strcmp(encoded, "AABBCCDD") == 0);

    uint8_t *out; size_t out_sz;
    assert(decode_xml(OCTET_STRING_decode_xer_hex,
                      encoded, &out, &out_sz) == 0);
    assert(out_sz == sizeof(data));
    assert(memcmp(out, data, sizeof(data)) == 0);
    free(out);
    printf("     Round-trip: OK\n");
}

/* ------------------------------------------------------------------ */
/* RT4: Large value (1 KB) Base64 round-trip                          */
/* ------------------------------------------------------------------ */
static void test_rt4_large_roundtrip(void) {
    printf("RT4: Large value (1 KB) Base64 round-trip\n");

    uint8_t large[1024];
    for(size_t i = 0; i < sizeof(large); i++) large[i] = (uint8_t)(i & 0xFF);

    const char *encoded = b64_encode(large, sizeof(large), XER_F_CANONICAL);
    size_t enc_len = strlen(encoded);
    printf("     1024 bytes -> %zu Base64 chars\n", enc_len);
    /* Base64 of 1024 bytes = ceil(1024/3)*4 = 1368 chars */
    assert(enc_len == 1368);

    uint8_t *out; size_t out_sz;
    assert(decode_xml(OCTET_STRING_decode_xer_base64,
                      encoded, &out, &out_sz) == 0);
    assert(out_sz == sizeof(large));
    assert(memcmp(out, large, sizeof(large)) == 0);
    free(out);
    printf("     Round-trip: OK\n");
}

/* ------------------------------------------------------------------ */
/* RT5: Schema hex annotation masks XER_F_BASE64                      */
/* ------------------------------------------------------------------ */
static void test_rt5_hex_annotation_masks_flag(void) {
    /*
     * Simulates what the compiler-generated HexData_encode_xer does:
     *   return OCTET_STRING_encode_xer(td, sptr, ilevel,
     *       flags & ~XER_F_BASE64, cb, app_key);
     *
     * Even if the caller passes XER_F_BASE64, the generated encoder
     * strips it out so the schema instruction wins.
     */
    static const uint8_t data[] = {0xD3, 0x1E, 0x81};
    printf("RT5: Schema ::= hexadecimal masks runtime XER_F_BASE64\n");

    enum xer_encoder_flags_e caller_flags =
        (enum xer_encoder_flags_e)(XER_F_BASIC | XER_F_BASE64);

    /* Simulated generated encoder: strip XER_F_BASE64 */
    const char *encoded = hex_encode(data, sizeof(data),
        (enum xer_encoder_flags_e)(caller_flags & ~XER_F_BASE64));
    printf("     With flags=XER_F_BASIC|XER_F_BASE64 (masked) -> \"%s\"\n",
           encoded);
    assert(strcmp(encoded, "D31E81") == 0);  /* hex, not Base64 */

    /* Compare: without masking, XER_F_BASE64 would win */
    const char *unmasked = hex_encode(data, sizeof(data), caller_flags);
    printf("     Without masking -> \"%s\" (would be Base64)\n", unmasked);
    assert(strcmp(unmasked, "0x6B") == 0);   /* Base64 */

    printf("     Schema hex wins over runtime flag: OK\n");
}

/* ------------------------------------------------------------------ */
/* RT6: Schema Base64 annotation ignores XER_F_CANONICAL              */
/* ------------------------------------------------------------------ */
static void test_rt6_base64_annotation_ignores_canonical(void) {
    /*
     * When the schema says ::= base64, the encoder should produce Base64
     * regardless of XER_F_CANONICAL (schema defines the wire format).
     * This is what OCTET_STRING_encode_xer_base64() already does — it
     * doesn't check XER_F_CANONICAL to switch to hex.
     */
    static const uint8_t data[] = {0xD3, 0x1E, 0x81};
    printf("RT6: Schema ::= base64 ignores XER_F_CANONICAL\n");

    const char *canonical = b64_encode(data, sizeof(data), XER_F_CANONICAL);
    const char *basic     = b64_encode(data, sizeof(data), XER_F_BASIC);
    printf("     XER_F_CANONICAL -> \"%s\"\n", canonical);
    printf("     XER_F_BASIC     -> \"%s\"\n", basic);
    assert(strcmp(canonical, "0x6B") == 0);
    assert(strcmp(basic,     "0x6B") == 0);

    printf("     Schema Base64 annotation ignores canonical flag: OK\n");
}

/* ------------------------------------------------------------------ */
/* RT7: Auto-decode correctly handles ambiguous content after B64 enc */
/* ------------------------------------------------------------------ */
static void test_rt7_auto_decode_after_b64(void) {
    /*
     * Scenario: a non-schema-annotated OCTET STRING whose value happens to
     * Base64-encode to a pure-hex-alphabet string.
     * With XER_F_BASE64 at runtime → encodes to "ABCD".
     * Auto-decoder sees "ABCD" → AMBIGUOUS_HEX → decodes as hex → wrong!
     *
     * This is the documented limitation: the runtime flag is unsafe for
     * all-hex-alphabet content. The test confirms the behavior and
     * documents why schema annotations ([BASE64] / ::= base64) are
     * necessary for reliable round-trips with such values.
     */
    static const uint8_t data[] = {0x00, 0x10, 0x83};
    printf("RT7: Auto-decode limitation with all-hex-alphabet Base64\n");

    const char *encoded = b64_encode(data, sizeof(data), XER_F_CANONICAL);
    printf("     {0x00,0x10,0x83} Base64 -> \"%s\"\n", encoded);
    assert(strcmp(encoded, "ABCD") == 0);

    /* Auto-decoder classifies "ABCD" as hex (ambiguous → hex preference) */
    uint8_t *out; size_t out_sz;
    assert(decode_xml(OCTET_STRING_decode_xer_auto,
                      encoded, &out, &out_sz) == 0);
    printf("     Auto-decode result: %zu bytes:", out_sz);
    for(size_t i = 0; i < out_sz; i++) printf(" %02X", out[i]);
    printf("\n");
    /* It decoded as hex {0xAB, 0xCD} — known limitation documented */
    static const uint8_t hex_result[] = {0xAB, 0xCD};
    assert(out_sz == 2 && memcmp(out, hex_result, 2) == 0);
    free(out);
    printf("     Confirmed: auto-decode misclassifies to hex as documented.\n");
    printf("     Use [BASE64] / ENCODING-CONTROL ::= base64 for safe round-trips.\n");
}

/* ------------------------------------------------------------------ */
/* RT8: XER_F_CANONICAL + XER_F_BASE64 → canonical wins (hex output) */
/* ------------------------------------------------------------------ */
static void test_rt8_canonical_overrides_base64_flag(void) {
    static const uint8_t data[] = {0xD3, 0x1E, 0x81};
    printf("RT8: XER_F_CANONICAL overrides XER_F_BASE64 flag\n");

    enum xer_encoder_flags_e both =
        (enum xer_encoder_flags_e)(XER_F_CANONICAL | XER_F_BASE64);
    const char *encoded = hex_encode(data, sizeof(data), both);
    printf("     XER_F_CANONICAL|XER_F_BASE64 -> \"%s\"\n", encoded);
    assert(strcmp(encoded, "D31E81") == 0);  /* hex, not Base64 */
    printf("     Canonical wins: OK\n");
}

/* ------------------------------------------------------------------ */
/* RT9: numeric entity references handle zero and reject invalid refs  */
/* ------------------------------------------------------------------ */
static void test_rt9_rejects_invalid_numeric_entrefs(void) {
    uint8_t *out; size_t out_sz;
    printf("RT9: Handle zero and reject invalid numeric entity references\n");

    assert(decode_xml(OCTET_STRING_decode_xer_utf8,
                      "something&#0;here", &out, &out_sz) == 0);
    assert(out_sz == 14);
    assert(memcmp(out, "something\0here", out_sz) == 0);
    free(out);

    assert(decode_xml(OCTET_STRING_decode_xer_utf8,
                      "something&#x00;here", &out, &out_sz) == 0);
    assert(out_sz == 14);
    assert(memcmp(out, "something\0here", out_sz) == 0);
    free(out);

    assert(decode_xml(OCTET_STRING_decode_xer_utf8,
                      "something&#;PDU>", &out, &out_sz) == -1);
    assert(decode_xml(OCTET_STRING_decode_xer_utf8,
                      "something&#xD800;here", &out, &out_sz) == -1);
    assert(decode_xml(OCTET_STRING_decode_xer_utf8,
                      "something&#A;here", &out, &out_sz) == -1);

    printf("     Numeric character references handled as expected: OK\n");
}

/* ------------------------------------------------------------------ */
/* main                                                                 */
/* ------------------------------------------------------------------ */
int main(void) {
    printf("=== XER OCTET STRING Base64 full-path and PDU round-trip tests ===\n\n");

    test_rt1_base64_roundtrip();
    test_rt2_allhex_base64_pinned();
    test_rt3_hex_roundtrip();
    test_rt4_large_roundtrip();
    test_rt5_hex_annotation_masks_flag();
    test_rt6_base64_annotation_ignores_canonical();
    test_rt7_auto_decode_after_b64();
    test_rt8_canonical_overrides_base64_flag();
    test_rt9_rejects_invalid_numeric_entrefs();

    printf("\n=== All round-trip tests passed ===\n");
    return 0;
}
