/*-
 * Copyright (c) 2025 Contributors. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
/*
 * ENUMERATED CBOR codec delegates to INTEGER CBOR codec since
 * ENUMERATED is stored as an INTEGER_t (BER-encoded integer).
 */
#include <asn_internal.h>
#include <ENUMERATED.h>
#include <INTEGER.h>
#include <cbor_encoder.h>
#include <cbor_decoder.h>

asn_enc_rval_t
ENUMERATED_encode_cbor(const asn_TYPE_descriptor_t *td, const void *sptr,
                       asn_app_consume_bytes_f *cb, void *app_key) {
    return INTEGER_encode_cbor(td, sptr, cb, app_key);
}

asn_dec_rval_t
ENUMERATED_decode_cbor(const asn_codec_ctx_t *opt_codec_ctx,
                       const asn_TYPE_descriptor_t *td,
                       void **sptr, const void *buf_ptr, size_t size) {
    return INTEGER_decode_cbor(opt_codec_ctx, td, sptr, buf_ptr, size);
}
