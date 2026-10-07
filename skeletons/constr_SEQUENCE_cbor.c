/*-
 * Copyright (c) 2025 Contributors. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <constr_SEQUENCE.h>
#include <OPEN_TYPE.h>
#include <cbor_encoder.h>
#include <cbor_decoder.h>
#include <cbor_support.h>
#include <string.h>

asn_enc_rval_t
SEQUENCE_encode_cbor(const asn_TYPE_descriptor_t *td, const void *sptr,
                     asn_app_consume_bytes_f *cb, void *app_key) {
    asn_enc_rval_t er = {0, 0, 0};
    size_t edx;
    size_t count = 0;
    ssize_t ret;

    if(!sptr) ASN__ENCODE_FAILED;

    /*
     * First pass: count present members to emit definite-length map header.
     */
    for(edx = 0; edx < td->elements_count; edx++) {
        const asn_TYPE_member_t *elm = &td->elements[edx];
        if(elm->flags & ATF_POINTER) {
            const void *memb_ptr =
                *(const void *const *)((const char *)sptr + elm->memb_offset);
            if(!memb_ptr) {
                if(elm->optional || elm->default_value_set) continue;
                ASN__ENCODE_FAILED;  /* Mandatory member missing */
            }
        }
        count++;
    }

    /* Emit CBOR map header */
    ret = cbor_encode_head(CBOR_MAJOR_MAP, count, cb, app_key);
    if(ret < 0) ASN__ENCODE_FAILED;
    er.encoded += ret;

    /*
     * Second pass: encode each present member.
     * Must mirror the counting loop exactly.
     */
    for(edx = 0; edx < td->elements_count; edx++) {
        const asn_TYPE_member_t *elm = &td->elements[edx];
        const void *memb_ptr;
        asn_enc_rval_t tmper;

        if(elm->flags & ATF_POINTER) {
            memb_ptr =
                *(const void *const *)((const char *)sptr + elm->memb_offset);
            if(!memb_ptr) {
                if(elm->optional || elm->default_value_set) continue;
                ASN__ENCODE_FAILED;
            }
        } else {
            memb_ptr = (const char *)sptr + elm->memb_offset;
        }

        /* Encode key: text string of member name */
        ret = cbor_encode_text(elm->name, strlen(elm->name), cb, app_key);
        if(ret < 0) ASN__ENCODE_FAILED;
        er.encoded += ret;

        /* Encode value */
        if(elm->flags & ATF_OPEN_TYPE) {
            tmper = OPEN_TYPE_cbor_put(td, sptr, elm, cb, app_key);
        } else {
            if(!elm->type->op->cbor_encoder) ASN__ENCODE_FAILED;
            tmper = elm->type->op->cbor_encoder(elm->type, memb_ptr, cb, app_key);
        }
        if(tmper.encoded < 0) return tmper;
        er.encoded += tmper.encoded;
    }

    ASN__ENCODED_OK(er);
}

asn_dec_rval_t
SEQUENCE_decode_cbor(const asn_codec_ctx_t *opt_codec_ctx,
                     const asn_TYPE_descriptor_t *td,
                     void **sptr, const void *buf_ptr, size_t size) {
    const asn_SEQUENCE_specifics_t *specs =
        (const asn_SEQUENCE_specifics_t *)td->specifics;
    void *st = *sptr;
    const uint8_t *buf = (const uint8_t *)buf_ptr;
    uint8_t major;
    uint64_t map_count;
    ssize_t hlen;
    size_t consumed = 0;
    uint64_t i;
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
    if(hlen < 0 || major != CBOR_MAJOR_MAP) ASN__DECODE_FAILED;
    consumed += (size_t)hlen;

    for(i = 0; i < map_count; i++) {
        uint8_t key_major;
        uint64_t key_len;
        ssize_t key_hlen;
        const char *key_str;
        size_t edx;

        /* Read key (text string) */
        if(consumed >= size) ASN__DECODE_FAILED;
        key_hlen = cbor_decode_head(buf + consumed, size - consumed,
                                    &key_major, &key_len);
        if(key_hlen < 0 || key_major != CBOR_MAJOR_TEXT) ASN__DECODE_FAILED;
        consumed += (size_t)key_hlen;
        if(size - consumed < key_len) ASN__DECODE_FAILED;
        key_str = (const char *)(buf + consumed);
        consumed += (size_t)key_len;

        /* Find matching element */
        for(edx = 0; edx < td->elements_count; edx++) {
            const asn_TYPE_member_t *elm = &td->elements[edx];
            if(strlen(elm->name) == key_len
               && memcmp(elm->name, key_str, key_len) == 0) {
                void *memb_ptr;
                void **memb_ptr2;
                asn_dec_rval_t tmprval;

                if(elm->flags & ATF_POINTER) {
                    memb_ptr2 =
                        (void **)((char *)st + elm->memb_offset);
                } else {
                    memb_ptr = (char *)st + elm->memb_offset;
                    memb_ptr2 = &memb_ptr;
                }

                if(elm->flags & ATF_OPEN_TYPE) {
                    tmprval = OPEN_TYPE_cbor_get(opt_codec_ctx, td, st,
                                                 elm, buf + consumed,
                                                 size - consumed);
                } else {
                    if(!elm->type->op->cbor_decoder) ASN__DECODE_FAILED;
                    tmprval = elm->type->op->cbor_decoder(
                        opt_codec_ctx, elm->type, memb_ptr2,
                        buf + consumed, size - consumed);
                }
                if(tmprval.code != RC_OK) ASN__DECODE_FAILED;
                consumed += tmprval.consumed;
                break;
            }
        }
        if(edx == td->elements_count) {
            /* Unknown key: skip the value completely using recursive skip */
            ssize_t skip_len;
            if(consumed >= size) ASN__DECODE_FAILED;
            skip_len = cbor_skip_item_with_ctx(opt_codec_ctx, buf + consumed,
                                               size - consumed);
            if(skip_len < 0) ASN__DECODE_FAILED;
            consumed += (size_t)skip_len;
        }
    }

    rval.consumed = consumed;
    rval.code = RC_OK;
    return rval;
}
