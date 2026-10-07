/*-
 * Copyright (c) 2025 Contributors. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
/*
 * Primitive string type CBOR codecs.
 * String types (VisibleString, IA5String, UTF8String, etc.) encode as
 * CBOR text strings (major type 3).
 * Other primitive types (OBJECT IDENTIFIER, etc.) encode as CBOR byte strings.
 */
#include <asn_internal.h>
#include <asn_codecs_prim.h>
#include <OCTET_STRING.h>
#include <cbor_encoder.h>
#include <cbor_decoder.h>
#include <cbor_support.h>

/*
 * Encode a primitive type as a CBOR text string (for visible string types).
 */
asn_enc_rval_t
OCTET_STRING_encode_cbor_utf8(const asn_TYPE_descriptor_t *td,
                              const void *sptr,
                              asn_app_consume_bytes_f *cb, void *app_key) {
    const OCTET_STRING_t *st = (const OCTET_STRING_t *)sptr;
    asn_enc_rval_t er = {0, 0, 0};
    ssize_t ret;

    (void)td;

    if(!st || (!st->buf && st->size)) ASN__ENCODE_FAILED;

    ret = cbor_encode_text((const char *)st->buf, st->size, cb, app_key);
    if(ret < 0) ASN__ENCODE_FAILED;
    er.encoded = ret;
    ASN__ENCODED_OK(er);
}

/*
 * Decode a CBOR text string into a primitive type.
 */
asn_dec_rval_t
OCTET_STRING_decode_cbor_utf8(const asn_codec_ctx_t *opt_codec_ctx,
                              const asn_TYPE_descriptor_t *td,
                              void **sptr, const void *buf_ptr, size_t size) {
    OCTET_STRING_t *st = (OCTET_STRING_t *)*sptr;
    const uint8_t *buf = (const uint8_t *)buf_ptr;
    ssize_t tag_skip;
    uint8_t major;
    uint64_t str_len;
    ssize_t hlen;
    asn_dec_rval_t rval = {RC_FAIL, 0};

    (void)opt_codec_ctx;
    (void)td;

    if(!st) {
        st = (OCTET_STRING_t *)CALLOC(1, sizeof(*st));
        if(!st) ASN__DECODE_FAILED;
        *sptr = st;
    }

    if(size < 1) ASN__DECODE_FAILED;

    /* Skip any leading CBOR tags (RFC 8949 §3.4) */
    tag_skip = cbor_skip_tags(buf, size);
    if(tag_skip < 0) ASN__DECODE_FAILED;

    hlen = cbor_decode_head(buf + tag_skip, size - (size_t)tag_skip,
                            &major, &str_len);
    if(hlen < 0) ASN__DECODE_FAILED;

    /* Accept both text (3) and bytes (2) for interoperability */
    if(major != CBOR_MAJOR_TEXT && major != CBOR_MAJOR_BYTES) ASN__DECODE_FAILED;
    if(size - (size_t)tag_skip - (size_t)hlen < str_len) ASN__DECODE_FAILED;

    uint8_t *p = (uint8_t *)MALLOC(str_len + 1);
    if(!p) ASN__DECODE_FAILED;

    if(str_len > 0)
        memcpy(p, buf + tag_skip + hlen, str_len);
    p[str_len] = '\0';

    FREEMEM(st->buf);
    st->buf = p;
    st->size = (int)str_len;

    rval.consumed = (size_t)tag_skip + (size_t)hlen + (size_t)str_len;
    rval.code = RC_OK;
    return rval;
}
