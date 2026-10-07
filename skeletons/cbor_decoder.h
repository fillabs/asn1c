/*-
 * Copyright (c) 2025 Contributors. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#ifndef _CBOR_DECODER_H_
#define _CBOR_DECODER_H_

#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

struct asn_TYPE_descriptor_s;  /* Forward declaration */

/*
 * The CBOR decoder of any ASN.1 type. May be invoked by the application.
 * Decodes canonical CBOR per RFC 7049.
 */
asn_dec_rval_t cbor_decode(
    const struct asn_codec_ctx_s *opt_codec_ctx,
    const struct asn_TYPE_descriptor_s *type_descriptor,
    void **struct_ptr,
    const void *buffer,
    size_t size);

/*
 * Type of the type-specific CBOR decoder function.
 */
typedef asn_dec_rval_t(cbor_type_decoder_f)(
    const asn_codec_ctx_t *opt_codec_ctx,
    const struct asn_TYPE_descriptor_s *type_descriptor,
    void **struct_ptr,
    const void *buf_ptr,
    size_t size);

#ifdef __cplusplus
}
#endif

#endif  /* _CBOR_DECODER_H_ */
