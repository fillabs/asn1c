/*-
 * Copyright (c) 2025 Contributors. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <constr_SEQUENCE_OF.h>
#include <asn_SEQUENCE_OF.h>
#include <cbor_encoder.h>
#include <cbor_decoder.h>
#include <cbor_support.h>

asn_enc_rval_t
SEQUENCE_OF_encode_cbor(const asn_TYPE_descriptor_t *td, const void *sptr,
                        asn_app_consume_bytes_f *cb, void *app_key) {
    asn_enc_rval_t er = {0, 0, 0};
    const asn_TYPE_member_t *elm = td->elements;
    const asn_anonymous_sequence_ *list = _A_CSEQUENCE_FROM_VOID(sptr);
    ssize_t ret;
    int i;

    if(!sptr) ASN__ENCODE_FAILED;

    /* Emit CBOR array header */
    ret = cbor_encode_head(CBOR_MAJOR_ARRAY, (uint64_t)list->count, cb, app_key);
    if(ret < 0) ASN__ENCODE_FAILED;
    er.encoded += ret;

    for(i = 0; i < list->count; i++) {
        asn_enc_rval_t tmper;
        void *memb_ptr = list->array[i];
        if(!memb_ptr) continue;

        if(!elm->type->op->cbor_encoder) ASN__ENCODE_FAILED;
        tmper = elm->type->op->cbor_encoder(elm->type, memb_ptr, cb, app_key);
        if(tmper.encoded < 0) return tmper;
        er.encoded += tmper.encoded;
    }

    ASN__ENCODED_OK(er);
}

asn_dec_rval_t
SEQUENCE_OF_decode_cbor(const asn_codec_ctx_t *opt_codec_ctx,
                        const asn_TYPE_descriptor_t *td,
                        void **sptr, const void *buf_ptr, size_t size) {
    const asn_SET_OF_specifics_t *specs =
        (const asn_SET_OF_specifics_t *)td->specifics;
    const asn_TYPE_member_t *elm = td->elements;
    const uint8_t *buf = (const uint8_t *)buf_ptr;
    uint8_t major;
    uint64_t array_count;
    ssize_t hlen;
    size_t consumed = 0;
    uint64_t i;
    asn_anonymous_sequence_ *list;
    asn_dec_rval_t rval = {RC_FAIL, 0};

    if(!*sptr) {
        *sptr = CALLOC(1, specs->struct_size);
        if(!*sptr) ASN__DECODE_FAILED;
    }
    list = _A_SEQUENCE_FROM_VOID(*sptr);

    if(ASN__STACK_OVERFLOW_CHECK(opt_codec_ctx)) ASN__DECODE_FAILED;

    /* Skip any leading CBOR tags (RFC 8949 §3.4) */
    {
        ssize_t tag_skip = cbor_skip_tags(buf, size);
        if(tag_skip < 0) ASN__DECODE_FAILED;
        consumed += (size_t)tag_skip;
    }

    hlen = cbor_decode_head(buf + consumed, size - consumed, &major, &array_count);
    if(hlen < 0 || major != CBOR_MAJOR_ARRAY) ASN__DECODE_FAILED;
    consumed += (size_t)hlen;

    for(i = 0; i < array_count; i++) {
        void *memb_ptr = NULL;
        void **memb_ptr2 = &memb_ptr;
        asn_dec_rval_t tmprval;

        if(!elm->type->op->cbor_decoder) ASN__DECODE_FAILED;
        tmprval = elm->type->op->cbor_decoder(
            opt_codec_ctx, elm->type, memb_ptr2,
            buf + consumed, size - consumed);
        if(tmprval.code != RC_OK) ASN__DECODE_FAILED;
        consumed += tmprval.consumed;

        if(ASN_SEQUENCE_ADD(list, memb_ptr)) {
            ASN_STRUCT_FREE(*elm->type, memb_ptr);
            ASN__DECODE_FAILED;
        }
    }

    rval.consumed = consumed;
    rval.code = RC_OK;
    return rval;
}
