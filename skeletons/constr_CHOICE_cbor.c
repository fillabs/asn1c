/*-
 * Copyright (c) 2025 Contributors. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <constr_CHOICE.h>
#include <cbor_encoder.h>
#include <cbor_decoder.h>
#include <cbor_support.h>
#include <string.h>

asn_enc_rval_t
CHOICE_encode_cbor(const asn_TYPE_descriptor_t *td, const void *sptr,
                   asn_app_consume_bytes_f *cb, void *app_key) {
    const asn_CHOICE_specifics_t *specs =
        (const asn_CHOICE_specifics_t *)td->specifics;
    asn_enc_rval_t er = {0, 0, 0};
    unsigned present;
    const asn_TYPE_member_t *elm;
    const void *memb_ptr;
    ssize_t ret;

    if(!sptr) ASN__ENCODE_FAILED;

    present = _fetch_present_idx(sptr, specs->pres_offset, specs->pres_size);
    if(present == 0 || present > td->elements_count) ASN__ENCODE_FAILED;

    elm = &td->elements[present - 1];

    if(elm->flags & ATF_POINTER) {
        memb_ptr =
            *(const void *const *)((const char *)sptr + elm->memb_offset);
        if(!memb_ptr) ASN__ENCODE_FAILED;
    } else {
        memb_ptr = (const char *)sptr + elm->memb_offset;
    }

    /* CBOR map with one entry: key=name, value=encoded value */
    ret = cbor_encode_head(CBOR_MAJOR_MAP, 1, cb, app_key);
    if(ret < 0) ASN__ENCODE_FAILED;
    er.encoded += ret;

    ret = cbor_encode_text(elm->name, strlen(elm->name), cb, app_key);
    if(ret < 0) ASN__ENCODE_FAILED;
    er.encoded += ret;

    if(!elm->type->op->cbor_encoder) ASN__ENCODE_FAILED;
    {
        asn_enc_rval_t tmper =
            elm->type->op->cbor_encoder(elm->type, memb_ptr, cb, app_key);
        if(tmper.encoded < 0) return tmper;
        er.encoded += tmper.encoded;
    }

    ASN__ENCODED_OK(er);
}

asn_dec_rval_t
CHOICE_decode_cbor(const asn_codec_ctx_t *opt_codec_ctx,
                   const asn_TYPE_descriptor_t *td,
                   void **sptr, const void *buf_ptr, size_t size) {
    const asn_CHOICE_specifics_t *specs =
        (const asn_CHOICE_specifics_t *)td->specifics;
    void *st = *sptr;
    const uint8_t *buf = (const uint8_t *)buf_ptr;
    uint8_t major;
    uint64_t map_count;
    ssize_t hlen;
    size_t consumed = 0;
    asn_dec_rval_t rval = {RC_FAIL, 0};

    if(!st) {
        st = *sptr = CALLOC(1, specs->struct_size);
        if(!st) ASN__DECODE_FAILED;
    }

    if(ASN__STACK_OVERFLOW_CHECK(opt_codec_ctx)) ASN__DECODE_FAILED;

    /* Skip any leading CBOR tags (RFC 8949 §3.4) */
    {
        ssize_t tag_skip = cbor_skip_tags(buf, size);
        if(tag_skip < 0) ASN__DECODE_FAILED;
        consumed += (size_t)tag_skip;
    }

    hlen = cbor_decode_head(buf + consumed, size - consumed, &major, &map_count);
    if(hlen < 0 || major != CBOR_MAJOR_MAP || map_count != 1) ASN__DECODE_FAILED;
    consumed += (size_t)hlen;

    /* Read the single key */
    {
        uint8_t key_major;
        uint64_t key_len;
        ssize_t key_hlen;
        const char *key_str;
        size_t edx;

        if(consumed >= size) ASN__DECODE_FAILED;
        key_hlen = cbor_decode_head(buf + consumed, size - consumed,
                                    &key_major, &key_len);
        if(key_hlen < 0 || key_major != CBOR_MAJOR_TEXT) ASN__DECODE_FAILED;
        consumed += (size_t)key_hlen;
        if(size - consumed < key_len) ASN__DECODE_FAILED;
        key_str = (const char *)(buf + consumed);
        consumed += (size_t)key_len;

        for(edx = 0; edx < td->elements_count; edx++) {
            const asn_TYPE_member_t *elm = &td->elements[edx];
            if(strlen(elm->name) == key_len
               && memcmp(elm->name, key_str, key_len) == 0) {
                void *memb_ptr;
                void **memb_ptr2;
                asn_dec_rval_t tmprval;

                if(elm->flags & ATF_POINTER) {
                    memb_ptr2 = (void **)((char *)st + elm->memb_offset);
                } else {
                    memb_ptr = (char *)st + elm->memb_offset;
                    memb_ptr2 = &memb_ptr;
                }

                if(!elm->type->op->cbor_decoder) ASN__DECODE_FAILED;
                tmprval = elm->type->op->cbor_decoder(
                    opt_codec_ctx, elm->type, memb_ptr2,
                    buf + consumed, size - consumed);
                if(tmprval.code != RC_OK) ASN__DECODE_FAILED;
                consumed += tmprval.consumed;

                _set_present_idx(st, specs->pres_offset,
                                 specs->pres_size, edx + 1);
                break;
            }
        }
        if(edx == td->elements_count) ASN__DECODE_FAILED;
    }

    rval.consumed = consumed;
    rval.code = RC_OK;
    return rval;
}
