/*-
 * Copyright (c) 2025 Contributors. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <BIT_STRING.h>
#include <cbor_encoder.h>
#include <cbor_decoder.h>
#include <cbor_support.h>

/*
 * Encode BIT STRING as CBOR byte string.
 * Format: [unused_bits_count] + [bit data bytes]
 * For 0-bit BIT STRING: encode as 1-byte byte string containing 0x00.
 */
asn_enc_rval_t
BIT_STRING_encode_cbor(const asn_TYPE_descriptor_t *td, const void *sptr,
                       asn_app_consume_bytes_f *cb, void *app_key) {
    const BIT_STRING_t *st = (const BIT_STRING_t *)sptr;
    asn_enc_rval_t er = {0, 0, 0};
    ssize_t ret;
    uint8_t unused_byte;

    (void)td;

    if(!st) ASN__ENCODE_FAILED;
    if(st->size > 0 && !st->buf) ASN__ENCODE_FAILED;

    /* The encoded byte string has length = 1 + st->size */
    /* First byte = unused bits count (0-7), rest = bit data */
    unused_byte = (uint8_t)st->bits_unused;

    /* Emit the byte string header */
    ret = cbor_encode_head(CBOR_MAJOR_BYTES, (uint64_t)(st->size + 1), cb, app_key);
    if(ret < 0) ASN__ENCODE_FAILED;
    er.encoded += ret;

    /* Emit unused bits byte */
    if(cb(&unused_byte, 1, app_key) < 0) ASN__ENCODE_FAILED;
    er.encoded += 1;

    /* Emit bit data */
    if(st->size > 0) {
        if(cb(st->buf, st->size, app_key) < 0) ASN__ENCODE_FAILED;
        er.encoded += st->size;
    }

    ASN__ENCODED_OK(er);
}

/*
 * Decode CBOR byte string into BIT STRING.
 * Format: first byte = unused bits, remaining = bit data.
 */
asn_dec_rval_t
BIT_STRING_decode_cbor(const asn_codec_ctx_t *opt_codec_ctx,
                       const asn_TYPE_descriptor_t *td,
                       void **sptr, const void *buf_ptr, size_t size) {
    BIT_STRING_t *st = (BIT_STRING_t *)*sptr;
    const uint8_t *buf = (const uint8_t *)buf_ptr;
    ssize_t tag_skip;
    uint8_t major;
    uint64_t bstr_len;
    ssize_t hlen;
    asn_dec_rval_t rval = {RC_FAIL, 0};

    (void)opt_codec_ctx;
    (void)td;

    if(!st) {
        st = (BIT_STRING_t *)CALLOC(1, sizeof(*st));
        if(!st) ASN__DECODE_FAILED;
        *sptr = st;
    }

    if(size < 1) ASN__DECODE_FAILED;

    /* Skip any leading CBOR tags (RFC 8949 §3.4) */
    tag_skip = cbor_skip_tags(buf, size);
    if(tag_skip < 0) ASN__DECODE_FAILED;

    hlen = cbor_decode_head(buf + tag_skip, size - (size_t)tag_skip,
                            &major, &bstr_len);
    if(hlen < 0 || major != CBOR_MAJOR_BYTES) ASN__DECODE_FAILED;
    if(size - (size_t)tag_skip - (size_t)hlen < bstr_len) ASN__DECODE_FAILED;

    /* Minimum 1 byte for unused bits indicator */
    if(bstr_len < 1) ASN__DECODE_FAILED;

    const uint8_t *data = buf + tag_skip + hlen;
    uint8_t bits_unused = data[0];
    size_t data_len = (size_t)bstr_len - 1;

    if(bits_unused > 7) ASN__DECODE_FAILED;
    if(data_len == 0 && bits_unused != 0) ASN__DECODE_FAILED;

    /* Allocate bit data */
    uint8_t *p = NULL;
    if(data_len > 0) {
        p = (uint8_t *)MALLOC(data_len + 1);
        if(!p) ASN__DECODE_FAILED;
        memcpy(p, data + 1, data_len);
        p[data_len] = '\0';
    }

    FREEMEM(st->buf);
    st->buf = p;
    st->size = (int)data_len;
    st->bits_unused = bits_unused;

    rval.consumed = (size_t)tag_skip + (size_t)hlen + (size_t)bstr_len;
    rval.code = RC_OK;
    return rval;
}
