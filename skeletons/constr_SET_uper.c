/*
 * UPER codec for ASN.1 SET values.
 *
 * Kept separate from constr_SET.c to match this fork's split skeleton
 * layout. The implementation is adapted from vlm/asn1c PR #543.
 */
#include <asn_internal.h>
#include <constr_SET.h>
#include <uper_opentype.h>

#ifndef ASN_DISABLE_UPER_SUPPORT

/*
 * X.691 (#20): the UPER encoding of a SET is the same as that of a
 * SEQUENCE, except that the root components are transmitted in the
 * canonical order of their tags (X.680 #8.6), not in the declaration
 * order. The (sorted) specs->tag2el map provides that order.
 *
 * Fill corder[0 .. td->elements_count-1] with element indices in
 * canonical tag order. Returns 0, or -1 if the order cannot be
 * reconstructed from the static tag map.
 */
static int
SET__canonical_order(const asn_TYPE_descriptor_t *td,
                     const asn_SET_specifics_t *specs, unsigned *corder) {
    unsigned i;
    unsigned n = 0;

    if(specs->tag2el_count == td->elements_count) {
        /* Each element appears exactly once: use the map directly. */
        for(i = 0; i < specs->tag2el_count; i++) {
            corder[i] = specs->tag2el[i].el_no;
        }
        return 0;
    }

    /*
     * An element (e.g. an untagged CHOICE) may occur several times in
     * the tag map, once per possible tag. In canonical order such an
     * element is sorted by its smallest tag, which is its first
     * occurrence in the (sorted) map; skip the rest.
     */
    for(i = 0; i < specs->tag2el_count && n < td->elements_count; i++) {
        unsigned el_no = specs->tag2el[i].el_no;
        unsigned j;
        for(j = 0; j < n; j++) {
            if(corder[j] == el_no) break;
        }
        if(j == n) corder[n++] = el_no;
    }

    return (n == td->elements_count) ? 0 : -1;
}

asn_dec_rval_t
SET_decode_uper(const asn_codec_ctx_t *opt_codec_ctx,
                const asn_TYPE_descriptor_t *td,
                const asn_per_constraints_t *constraints, void **sptr,
                asn_per_data_t *pd) {
    const asn_SET_specifics_t *specs = (const asn_SET_specifics_t *)td->specifics;
	void *st = *sptr;	/* Target structure. */
	uint8_t *opres;		/* Presence of optional root members */
	asn_per_data_t opmd;
	asn_dec_rval_t rv;
	unsigned *corder;	/* Canonical tag order of the members */
	size_t roms_count;	/* Number of optional root members */
	size_t edx;
	size_t i;

	(void)constraints;

	if(ASN__STACK_OVERFLOW_CHECK(opt_codec_ctx))
		ASN__DECODE_FAILED;

	if(!st) {
		st = *sptr = CALLOC(1, specs->struct_size);
		if(!st) ASN__DECODE_FAILED;
	}

	ASN_DEBUG("Decoding %s as SET (UPER)", td->name);

	if(specs->extensible) {
		/*
		 * Only extension-free SETs are supported (root-only): the SET
		 * specifics do not record which members are extension
		 * additions, so an extensible SET cannot be decoded reliably.
		 * Fail cleanly rather than misinterpret the bit stream.
		 */
		ASN_DEBUG("Extensible SET %s is not PER-decodable", td->name);
		ASN__DECODE_FAILED;
	}

	/* Reconstruct the canonical tag order of the members */
	if(td->elements_count) {
		corder = (unsigned *)MALLOC(td->elements_count * sizeof(*corder));
		if(!corder) ASN__DECODE_FAILED;
		if(SET__canonical_order(td, specs, corder)) {
			FREEMEM(corder);
			ASN__DECODE_FAILED;
		}
	} else {
		corder = 0;
	}

	/* Number of optional root members */
	roms_count = 0;
	for(edx = 0; edx < td->elements_count; edx++)
		if(td->elements[edx].optional)
			roms_count++;

	/* Prepare a place and read-in the presence bitmap */
	memset(&opmd, 0, sizeof(opmd));
	if(roms_count) {
		opres = (uint8_t *)MALLOC(((roms_count + 7) >> 3) + 1);
		if(!opres) {
			FREEMEM(corder);
			ASN__DECODE_FAILED;
		}
		/* Get the presence map */
		if(per_get_many_bits(pd, opres, 0, roms_count)) {
			FREEMEM(opres);
			FREEMEM(corder);
			ASN__DECODE_STARVED;
		}
		opmd.buffer = opres;
		opmd.nbits = roms_count;
		ASN_DEBUG("Read in presence bitmap for %s of %d bits (%x..)",
			td->name, (int)roms_count, *opres);
	} else {
		opres = 0;
	}

	/*
	 * Get the SET root elements, in canonical tag order.
	 */
	for(i = 0; i < td->elements_count; i++) {
		asn_TYPE_member_t *elm;
		void *memb_ptr;		/* Pointer to the member */
		void **memb_ptr2;	/* Pointer to that pointer */

		edx = corder[i];
		elm = &td->elements[edx];

		/* Fetch the pointer to this member */
		if(elm->flags & ATF_POINTER) {
			memb_ptr2 = (void **)((char *)st + elm->memb_offset);
		} else {
			memb_ptr = (char *)st + elm->memb_offset;
			memb_ptr2 = &memb_ptr;
		}

		/* Deal with optionality */
		if(elm->optional) {
			int present = per_get_few_bits(&opmd, 1);
			ASN_DEBUG("Member %s->%s is optional, p=%d (%d->%d)",
				td->name, elm->name, present,
				(int)opmd.nboff, (int)opmd.nbits);
			if(present == 0) {
				/* This element is not present */
				if(elm->default_value_set) {
					/* Fill-in DEFAULT */
					if(elm->default_value_set(memb_ptr2)) {
						FREEMEM(opres);
						FREEMEM(corder);
						ASN__DECODE_FAILED;
					}
					ASN_DEBUG("Filled-in default");
					ASN_SET_MKPRESENT((char *)st + specs->pres_offset,
						edx);
				}
				/* The member is just not present */
				continue;
			}
			/* Fall through */
		}

		/* Fetch the member from the stream */
		ASN_DEBUG("Decoding member \"%s\" in %s", elm->name, td->name);

		if(!elm->type->op->uper_decoder) {
			ASN_DEBUG("Member %s->%s has no UPER decoder",
				td->name, elm->name);
			FREEMEM(opres);
			FREEMEM(corder);
			ASN__DECODE_FAILED;
		}

		rv = elm->type->op->uper_decoder(opt_codec_ctx, elm->type,
				elm->encoding_constraints.per_constraints, memb_ptr2, pd);
		if(rv.code != RC_OK) {
			ASN_DEBUG("Failed decode %s in %s",
				elm->name, td->name);
			FREEMEM(opres);
			FREEMEM(corder);
			return rv;
		}

		ASN_SET_MKPRESENT((char *)st + specs->pres_offset, edx);
	}

	FREEMEM(opres);
	FREEMEM(corder);

	rv.consumed = 0;
	rv.code = RC_OK;
	return rv;
}

asn_enc_rval_t
SET_encode_uper(const asn_TYPE_descriptor_t *td,
                const asn_per_constraints_t *constraints, const void *sptr,
                asn_per_outp_t *po) {
    const asn_SET_specifics_t *specs = (const asn_SET_specifics_t *)td->specifics;
	asn_enc_rval_t er;
	unsigned *corder;	/* Canonical tag order of the members */
	size_t edx;
	size_t i;

	(void)constraints;

	if(!sptr)
		ASN__ENCODE_FAILED;

	er.encoded = 0;

	ASN_DEBUG("Encoding %s as SET (UPER)", td->name);

	if(specs->extensible) {
		/*
		 * Only extension-free SETs are supported (root-only): the SET
		 * specifics do not record which members are extension
		 * additions, so the extension bit and the root/addition split
		 * cannot be encoded reliably. Fail cleanly rather than
		 * produce a wrong encoding.
		 */
		ASN_DEBUG("Extensible SET %s is not PER-encodable", td->name);
		ASN__ENCODE_FAILED;
	}

	/* Reconstruct the canonical tag order of the members */
	if(td->elements_count) {
		corder = (unsigned *)MALLOC(td->elements_count * sizeof(*corder));
		if(!corder) ASN__ENCODE_FAILED;
		if(SET__canonical_order(td, specs, corder)) {
			FREEMEM(corder);
			ASN__ENCODE_FAILED;
		}
	} else {
		corder = 0;
	}

#undef SET__UPER_ENCODE_FAILED
#define SET__UPER_ENCODE_FAILED do {	\
		FREEMEM(corder);	\
		ASN__ENCODE_FAILED;	\
	} while(0)

	/*
	 * Encode the presence bitmap of the optional members,
	 * in canonical tag order (X.691 #20.2, #18.2).
	 */
	for(i = 0; i < td->elements_count; i++) {
		asn_TYPE_member_t *elm;
		const void *memb_ptr;		/* Pointer to the member */
		const void *const *memb_ptr2;	/* Pointer to that pointer */
		int present;

		edx = corder[i];
		elm = &td->elements[edx];

		if(!elm->optional) continue;

		/* Fetch the pointer to this member */
		if(elm->flags & ATF_POINTER) {
			memb_ptr2 =
				(const void *const *)((const char *)sptr + elm->memb_offset);
			present = (*memb_ptr2 != 0);
		} else {
			memb_ptr = (const void *)((const char *)sptr + elm->memb_offset);
			memb_ptr2 = &memb_ptr;
			present = 1;
		}

		/* Eliminate default values */
		if(present && elm->default_value_cmp
		   && elm->default_value_cmp(*memb_ptr2) == 0)
			present = 0;

		ASN_DEBUG("Element %s %s %s->%s is %s",
			elm->flags & ATF_POINTER ? "ptr" : "inline",
			elm->default_value_cmp ? "def" : "wtv",
			td->name, elm->name, present ? "present" : "absent");
		if(per_put_few_bits(po, present, 1))
			SET__UPER_ENCODE_FAILED;
	}

	/*
	 * Encode the SET root elements, in canonical tag order.
	 */
	for(i = 0; i < td->elements_count; i++) {
		asn_TYPE_member_t *elm;
		const void *memb_ptr;		/* Pointer to the member */
		const void *const *memb_ptr2;	/* Pointer to that pointer */

		edx = corder[i];
		elm = &td->elements[edx];

		ASN_DEBUG("About to encode %s", elm->type->name);

		/* Fetch the pointer to this member */
		if(elm->flags & ATF_POINTER) {
			memb_ptr2 =
				(const void *const *)((const char *)sptr + elm->memb_offset);
			if(!*memb_ptr2) {
				ASN_DEBUG("Element %s %" ASN_PRI_SIZE " not present",
					elm->name, edx);
				if(elm->optional)
					continue;
				/* Mandatory element is missing */
				SET__UPER_ENCODE_FAILED;
			}
		} else {
			memb_ptr = (const void *)((const char *)sptr + elm->memb_offset);
			memb_ptr2 = &memb_ptr;
		}

		/* Eliminate default values */
		if(elm->default_value_cmp && elm->default_value_cmp(*memb_ptr2) == 0)
			continue;

		ASN_DEBUG("Encoding %s->%s:%s", td->name, elm->name, elm->type->name);
		if(!elm->type->op->uper_encoder) {
			ASN_DEBUG("Member %s->%s has no UPER encoder",
				td->name, elm->name);
			SET__UPER_ENCODE_FAILED;
		}
		er = elm->type->op->uper_encoder(
			elm->type, elm->encoding_constraints.per_constraints, *memb_ptr2,
			po);
		if(er.encoded == -1) {
			FREEMEM(corder);
			return er;
		}
	}

#undef SET__UPER_ENCODE_FAILED

	FREEMEM(corder);
	ASN__ENCODED_OK(er);
}

#endif  /* ASN_DISABLE_UPER_SUPPORT */
