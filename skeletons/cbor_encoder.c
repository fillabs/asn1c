/*-
 * Copyright (c) 2025 Contributors. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <cbor_encoder.h>

asn_enc_rval_t
cbor_encode(const asn_TYPE_descriptor_t *td, const void *sptr,
            asn_app_consume_bytes_f *cb, void *app_key) {
    asn_enc_rval_t er = {0, 0, 0};

    if(!td || !sptr || !td->op->cbor_encoder) {
        ASN__ENCODE_FAILED;
    }

    er = td->op->cbor_encoder(td, sptr, cb, app_key);
    return er;
}
