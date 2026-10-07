/*
 * Copyright (c) 2025 Contributors. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <OPEN_TYPE.h>
#include <constr_CHOICE.h>
#include <cbor_encoder.h>
#include <cbor_decoder.h>

asn_dec_rval_t
OPEN_TYPE_cbor_get(const asn_codec_ctx_t *opt_codec_ctx,
                   const asn_TYPE_descriptor_t *td,
                   void *sptr, const asn_TYPE_member_t *elm,
                   const void *ptr, size_t size) {
    asn_type_selector_result_t selected;
    void *memb_ptr;   /* Pointer to the member */
    void **memb_ptr2; /* Pointer to that pointer */
    void *inner_value;
    asn_dec_rval_t rv;

    if(!(elm->flags & ATF_OPEN_TYPE)) {
        ASN__DECODE_FAILED;
    }

    if(!elm->type) {
        ASN_DEBUG("Open Type %s->%s: type descriptor is NULL",
                  td->name, elm->name);
        ASN__DECODE_FAILED;
    }

    if(!elm->type_selector) {
        ASN_DEBUG("Type selector is not defined for Open Type %s->%s->%s",
                  td->name, elm->name, elm->type->name);
        ASN__DECODE_FAILED;
    }

    selected = elm->type_selector(td, sptr);
    if(!selected.presence_index || !selected.type_descriptor) {
        ASN__DECODE_FAILED;
    }

    /* Fetch the pointer to this member */
    assert(elm->flags & ATF_OPEN_TYPE);
    if(elm->flags & ATF_POINTER) {
        memb_ptr2 = (void **)((char *)sptr + elm->memb_offset);
    } else {
        memb_ptr = (char *)sptr + elm->memb_offset;
        memb_ptr2 = &memb_ptr;
    }

    if(elm->type->elements_count > 0) {
        /* CHOICE wrapper mode */
        const asn_CHOICE_specifics_t *specs =
            (const asn_CHOICE_specifics_t *)elm->type->specifics;
        const asn_TYPE_member_t *variant_elm;
        unsigned int memb_offset;

        if(!specs) {
            ASN_DEBUG("Open Type %s->%s: type specifics is NULL",
                      td->name, elm->name);
            ASN__DECODE_FAILED;
        }

        if(selected.presence_index > elm->type->elements_count) {
            ASN_DEBUG("Open Type %s->%s: presence index %u out of bounds",
                      td->name, elm->name, selected.presence_index);
            ASN__DECODE_FAILED;
        }

        if(!elm->type->elements) {
            ASN_DEBUG("Open Type %s->%s: elements array is NULL",
                      td->name, elm->name);
            ASN__DECODE_FAILED;
        }

        if(*memb_ptr2 == NULL) {
            *memb_ptr2 = CALLOC(1, specs->struct_size);
            if(*memb_ptr2 == NULL) {
                ASN__DECODE_FAILED;
            }
        } else {
            if(CHOICE_variant_set_presence(elm->type, *memb_ptr2, 0) != 0) {
                ASN__DECODE_FAILED;
            }
        }

        variant_elm = &elm->type->elements[selected.presence_index - 1];
        memb_offset = variant_elm->memb_offset;

        if(variant_elm->flags & ATF_POINTER) {
            inner_value = *(void **)((char *)*memb_ptr2 + memb_offset);
        } else {
            inner_value = (char *)*memb_ptr2 + memb_offset;
        }
    } else {
        /* Direct type mode */
        inner_value = *memb_ptr2;
    }

    if(!selected.type_descriptor->op->cbor_decoder) {
        ASN_DEBUG("Open Type %s->%s: selected type %s has no CBOR decoder",
                  td->name, elm->name, selected.type_descriptor->name);
        ASN__DECODE_FAILED;
    }

    rv = selected.type_descriptor->op->cbor_decoder(
        opt_codec_ctx, selected.type_descriptor, &inner_value, ptr, size);

    switch(rv.code) {
    case RC_OK:
        if(elm->type->elements_count > 0) {
            const asn_TYPE_member_t *variant_elm =
                &elm->type->elements[selected.presence_index - 1];
            if(CHOICE_variant_set_presence(elm->type, *memb_ptr2,
                                           selected.presence_index)
               == 0) {
                if(variant_elm->flags & ATF_POINTER) {
                    void **variant_ptr =
                        (void **)((char *)*memb_ptr2 + variant_elm->memb_offset);
                    *variant_ptr = inner_value;
                }
                break;
            } else {
                rv.code = RC_FAIL;
            }
        } else {
            if(elm->flags & ATF_POINTER) {
                *memb_ptr2 = inner_value;
            }
            break;
        }
        /* Fall through */
    case RC_FAIL:
    case RC_WMORE:
        if(elm->type->elements_count > 0) {
            /*
             * CHOICE-wrapper mode: *memb_ptr2 points to the CHOICE wrapper
             * (type elm->type). Do not free inner_value directly — it may
             * refer to embedded storage inside the wrapper. Free or reset
             * the wrapper as a whole instead.
             */
            if(*memb_ptr2) {
                if(elm->flags & ATF_POINTER) {
                    ASN_STRUCT_FREE(*elm->type, *memb_ptr2);
                    *memb_ptr2 = NULL;
                } else {
                    ASN_STRUCT_RESET(*elm->type, *memb_ptr2);
                }
            }
        } else {
            /*
             * Direct type mode: inner_value is the instance of the selected
             * type. Clean it up using selected.type_descriptor.
             */
            if(inner_value) {
                if(elm->flags & ATF_POINTER) {
                    ASN_STRUCT_FREE(*selected.type_descriptor, inner_value);
                    *memb_ptr2 = NULL;
                } else {
                    ASN_STRUCT_RESET(*selected.type_descriptor, inner_value);
                }
            }
        }
        return rv;
    }

    return rv;
}

asn_enc_rval_t
OPEN_TYPE_cbor_put(const asn_TYPE_descriptor_t *td, const void *sptr,
                   const asn_TYPE_member_t *elm,
                   asn_app_consume_bytes_f *cb, void *app_key) {
    asn_type_selector_result_t selected;
    const void *memb_ptr;

    if(!(elm->flags & ATF_OPEN_TYPE)) {
        ASN__ENCODE_FAILED;
    }

    if(!elm->type) {
        ASN_DEBUG("Open Type %s->%s: type descriptor is NULL",
                  td->name, elm->name);
        ASN__ENCODE_FAILED;
    }

    if(!elm->type_selector) {
        ASN_DEBUG("Type selector is not defined for Open Type %s->%s->%s",
                  td->name, elm->name, elm->type->name);
        ASN__ENCODE_FAILED;
    }

    selected = elm->type_selector(td, sptr);
    if(!selected.presence_index) {
        ASN_DEBUG("Open Type %s->%s: type_selector returned presence_index=0",
                  td->name, elm->name);
        ASN__ENCODE_FAILED;
    }

    if(!selected.type_descriptor) {
        ASN_DEBUG("Open Type %s->%s: type_selector returned NULL type descriptor",
                  td->name, elm->name);
        ASN__ENCODE_FAILED;
    }

    if(!selected.type_descriptor->op || !selected.type_descriptor->op->cbor_encoder) {
        ASN_DEBUG("Open Type %s->%s: selected type %s has no CBOR encoder",
                  td->name, elm->name, selected.type_descriptor->name);
        ASN__ENCODE_FAILED;
    }

    /* Fetch the pointer to this member */
    assert(elm->flags & ATF_OPEN_TYPE);
    if(elm->flags & ATF_POINTER) {
        memb_ptr = *(const void *const *)((const char *)sptr + elm->memb_offset);
        if(!memb_ptr) ASN__ENCODE_FAILED;
    } else {
        memb_ptr = (const void *)((const char *)sptr + elm->memb_offset);
    }

    if(elm->type->elements_count > 0) {
        /* CHOICE wrapper mode */
        const asn_TYPE_member_t *variant_elm;
        const void *variant_memb_ptr;

        if(selected.presence_index == 0
           || selected.presence_index > elm->type->elements_count) {
            ASN_DEBUG("Open Type %s->%s: presence index %u out of bounds",
                      td->name, elm->name, selected.presence_index);
            ASN__ENCODE_FAILED;
        }

        variant_elm = &elm->type->elements[selected.presence_index - 1];

        if(variant_elm->flags & ATF_POINTER) {
            variant_memb_ptr =
                *(const void *const *)((const char *)memb_ptr
                                       + variant_elm->memb_offset);
            if(!variant_memb_ptr) {
                ASN_DEBUG("Open Type %s->%s: variant data pointer is NULL",
                          td->name, elm->name);
                ASN__ENCODE_FAILED;
            }
        } else {
            variant_memb_ptr =
                (const void *)((const char *)memb_ptr + variant_elm->memb_offset);
        }

        return selected.type_descriptor->op->cbor_encoder(
            selected.type_descriptor, variant_memb_ptr, cb, app_key);
    } else {
        /* Direct type mode */
        return selected.type_descriptor->op->cbor_encoder(
            selected.type_descriptor, memb_ptr, cb, app_key);
    }
}
