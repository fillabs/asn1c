/*-
 * Copyright (c) 2025 Contributors. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <cbor_decoder.h>

asn_dec_rval_t
cbor_decode(const asn_codec_ctx_t *opt_codec_ctx,
            const asn_TYPE_descriptor_t *td, void **struct_ptr,
            const void *buffer, size_t size) {
    asn_codec_ctx_t s_codec_ctx;

    if(opt_codec_ctx) {
        if(opt_codec_ctx->max_stack_size) {
            s_codec_ctx = *opt_codec_ctx;
            opt_codec_ctx = &s_codec_ctx;
        }
    } else {
        memset(&s_codec_ctx, 0, sizeof(s_codec_ctx));
        s_codec_ctx.max_stack_size = ASN__DEFAULT_STACK_MAX;
        opt_codec_ctx = &s_codec_ctx;
    }

    if(!td || !td->op->cbor_decoder) {
        ASN__DECODE_FAILED;
    }

    return td->op->cbor_decoder(opt_codec_ctx, td, struct_ptr, buffer, size);
}
