/*-
 * Copyright (c) 2025 Contributors. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#ifndef _CBOR_ENCODER_H_
#define _CBOR_ENCODER_H_

#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

struct asn_TYPE_descriptor_s;  /* Forward declaration */

/*
 * The CBOR encoder of any ASN.1 type. May be invoked by the application.
 * Produces canonical CBOR output per RFC 7049.
 */
asn_enc_rval_t cbor_encode(
    const struct asn_TYPE_descriptor_s *type_descriptor,
    const void *struct_ptr,
    asn_app_consume_bytes_f *consume_bytes_cb,
    void *app_key);

/*
 * Type of the generic CBOR encoder.
 */
typedef asn_enc_rval_t(cbor_type_encoder_f)(
    const struct asn_TYPE_descriptor_s *type_descriptor,
    const void *struct_ptr,
    asn_app_consume_bytes_f *consume_bytes_cb,
    void *app_key);

#ifdef __cplusplus
}
#endif

#endif  /* _CBOR_ENCODER_H_ */
