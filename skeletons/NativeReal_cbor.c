/*-
 * Copyright (c) 2025 Contributors. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <NativeReal.h>
#include <cbor_encoder.h>
#include <cbor_decoder.h>
#include <cbor_support.h>

asn_enc_rval_t
NativeReal_encode_cbor(const asn_TYPE_descriptor_t *td, const void *sptr,
                       asn_app_consume_bytes_f *cb, void *app_key) {
    asn_enc_rval_t er = {0, 0, 0};
    double native;
    ssize_t ret;

    native = NativeReal__get_double(td, sptr);
    ret = cbor_encode_float64(native, cb, app_key);
    if(ret < 0) ASN__ENCODE_FAILED;
    er.encoded = ret;
    ASN__ENCODED_OK(er);
}

asn_dec_rval_t
NativeReal_decode_cbor(const asn_codec_ctx_t *opt_codec_ctx,
                       const asn_TYPE_descriptor_t *td,
                       void **sptr, const void *buf_ptr, size_t size) {
    const uint8_t *buf = (const uint8_t *)buf_ptr;
    ssize_t tag_skip;
    double d;
    ssize_t consumed;
    asn_dec_rval_t rval = {RC_FAIL, 0};

    (void)opt_codec_ctx;

    /* Skip any leading CBOR tags (RFC 8949 §3.4) */
    tag_skip = cbor_skip_tags(buf, size);
    if(tag_skip < 0) ASN__DECODE_FAILED;

    consumed = cbor_decode_float64(buf + tag_skip, size - (size_t)tag_skip, &d);
    if(consumed < 0) ASN__DECODE_FAILED;

    if(NativeReal__set(td, sptr, d) < 0) ASN__DECODE_FAILED;

    rval.consumed = (size_t)tag_skip + (size_t)consumed;
    rval.code = RC_OK;
    return rval;
}
