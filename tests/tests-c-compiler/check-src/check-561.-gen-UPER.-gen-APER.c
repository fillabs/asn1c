#undef NDEBUG
#include <assert.h>
#include <stdint.h>
#include <string.h>

#include "Alt4.h"

static const uint8_t expected[] = { 0x72, 0x00 };

static void
check_uper(void) {
	Alt4_t value = { .present = Alt4_PR_num, .choice.num = 200 };
	Alt4_t *decoded = 0;
	uint8_t buf[sizeof(expected)] = { 0 };
	asn_enc_rval_t er;
	asn_dec_rval_t dr;

	er = uper_encode_to_buffer(&asn_DEF_Alt4, 0, &value, buf, sizeof(buf));
	assert(er.encoded == 10);
	assert(memcmp(buf, expected, sizeof(expected)) == 0);

	dr = uper_decode_complete(0, &asn_DEF_Alt4, (void **)&decoded,
		expected, sizeof(expected));
	assert(dr.code == RC_OK);
	assert(decoded && decoded->present == Alt4_PR_num);
	assert(decoded->choice.num == 200);
	ASN_STRUCT_FREE(asn_DEF_Alt4, decoded);
}

static void
check_aper(void) {
	Alt4_t value = { .present = Alt4_PR_num, .choice.num = 200 };
	Alt4_t *decoded = 0;
	uint8_t buf[sizeof(expected)] = { 0 };
	asn_enc_rval_t er;
	asn_dec_rval_t dr;

	er = aper_encode_to_buffer(&asn_DEF_Alt4, 0, &value, buf, sizeof(buf));
	assert(er.encoded == 10);
	assert(memcmp(buf, expected, sizeof(expected)) == 0);

	dr = aper_decode_complete(0, &asn_DEF_Alt4, (void **)&decoded,
		expected, sizeof(expected));
	assert(dr.code == RC_OK);
	assert(decoded && decoded->present == Alt4_PR_num);
	assert(decoded->choice.num == 200);
	ASN_STRUCT_FREE(asn_DEF_Alt4, decoded);
}

int
main(void) {
	check_uper();
	check_aper();
	return 0;
}
