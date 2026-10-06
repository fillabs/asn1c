#undef NDEBUG
#include <assert.h>
#include <stdint.h>
#include <string.h>

#include "OctetRext.h"
#include "Rext.h"
#include <aper_decoder.h>
#include <aper_encoder.h>
#include <uper_decoder.h>
#include <uper_encoder.h>

static const uint8_t expected[] = { 0x7f, 0x01, 0x00 };

static void
check_octet_identifier_size(void) {
	const asn_per_constraints_t *constraints =
		asn_DEF_OctetRext.elements[0].encoding_constraints.per_constraints;

	assert(constraints);
	const asn_per_constraint_t *size_ct = &constraints->size;
	assert(size_ct->flags & APC_EXTENSIBLE);
	assert(size_ct->lower_bound == 1);
	assert(size_ct->upper_bound == 2);
}

static void
check_uper(void) {
	Rext_t value = { .id = 128, .cnt.present = cnt_PR_Null };
	Rext_t *decoded = 0;
	uint8_t buf[sizeof(expected)] = { 0 };
	asn_enc_rval_t er;
	asn_dec_rval_t dr;

	er = uper_encode_to_buffer(&asn_DEF_Rext, 0, &value, buf, sizeof(buf));
	assert(er.encoded == 24);
	assert(memcmp(buf, expected, sizeof(expected)) == 0);

	dr = uper_decode_complete(0, &asn_DEF_Rext, (void **)&decoded, expected,
		sizeof(expected));
	assert(dr.code == RC_OK);
	assert(decoded && decoded->id == 128);
	assert(decoded->cnt.present == cnt_PR_Null);
	ASN_STRUCT_FREE(asn_DEF_Rext, decoded);
}

static void
check_aper(void) {
	Rext_t value = { .id = 128, .cnt.present = cnt_PR_Null };
	Rext_t *decoded = 0;
	uint8_t buf[sizeof(expected)] = { 0 };
	asn_enc_rval_t er;
	asn_dec_rval_t dr;

	er = aper_encode_to_buffer(&asn_DEF_Rext, 0, &value, buf, sizeof(buf));
	assert(er.encoded == 24);
	assert(memcmp(buf, expected, sizeof(expected)) == 0);

	dr = aper_decode_complete(0, &asn_DEF_Rext, (void **)&decoded, expected,
		sizeof(expected));
	assert(dr.code == RC_OK);
	assert(decoded && decoded->id == 128);
	assert(decoded->cnt.present == cnt_PR_Null);
	ASN_STRUCT_FREE(asn_DEF_Rext, decoded);
}

int
main(void) {
	check_octet_identifier_size();
	check_uper();
	check_aper();
	return 0;
}
