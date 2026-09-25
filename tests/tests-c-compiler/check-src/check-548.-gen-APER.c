/*
 * Regression test for the APER path of issue #18: extensible ENUMERATED
 * with an extension addition value below the root maximum must use the
 * two-segment value2enum lookup (root first, then extension additions),
 * not a whole-array bsearch() which is undefined behaviour on a
 * non-monotonic array and fails in practice for known enumerators.
 *
 * The golden bytes below are produced by the same commercial ASN.1
 * toolchain used for the UPER test and cross-verified by hand.  For
 * these small enumerated types (range < 256) APER and UPER produce
 * identical wire bytes, so the golden values match those in the
 * companion check-548.-gen-UPER.c test.
 */
#undef	NDEBUG
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

#include "E.h"
#include "F.h"
#include "G.h"
#include "H.h"

/*
 * Verify one enumeration value against the reference toolchain's golden
 * APER byte: encode, compare, decode the golden byte back, re-encode.
 */
static void
check_value(const asn_TYPE_descriptor_t *td, long value,
		uint8_t expected_byte) {
	uint8_t buf[16];
	long *decoded = 0;
	asn_enc_rval_t er;
	asn_dec_rval_t rv;

	/* Encode and compare against the golden byte. */
	er = aper_encode_to_buffer(td, 0, &value, buf, sizeof(buf));
	assert(er.encoded >= 1);
	assert(er.encoded <= 8);
	printf("%s %ld => %02x (expected %02x)\n",
		td->name, value, buf[0], expected_byte);
	assert(buf[0] == expected_byte);

	/* Decode the golden byte; expect the original value back. */
	rv = aper_decode_complete(0, td, (void **)&decoded,
		&expected_byte, 1);
	assert(rv.code == RC_OK);
	assert(decoded);
	printf("%s %02x => %ld (expected %ld)\n",
		td->name, expected_byte, *decoded, value);
	assert(*decoded == value);

	/* Re-encode the decoded value: must be idempotent. */
	memset(buf, 0xAA, sizeof(buf));
	er = aper_encode_to_buffer(td, 0, decoded, buf, sizeof(buf));
	assert(er.encoded >= 1);
	assert(buf[0] == expected_byte);

	ASN_STRUCT_FREE(*td, decoded);
}

/*
 * Purpose: Verify that generated APER ENUMERATED descriptors preserve an
 *          unknown extension ordinal without replacing it with a known value.
 * Original source: New regression coverage for NativeEnumerated APER dispatch.
 * Version/date: 1.5.1, 2026-09-24.
 * Parameters:
 *   td             Generated ENUMERATED descriptor under test.
 *   wire           APER encoding containing an unknown extension ordinal.
 *   wire_size     Number of bytes in wire.
 *   expected_value Reserved LONG_MAX - ordinal marker expected after decode.
 * Returns: Nothing. Assertions terminate the test on failure.
 * Exceptions: None; the test uses assertions for unexpected codec results.
 * Responsible party: asn1c maintainers.
 * History: Added to cover generated APER ENUMERATED use of NativeEnumerated.
 * Example: check_unknown_extension(&asn_DEF_E, bytes, sizeof(bytes), LONG_MAX - 63).
 */
static void
check_unknown_extension(const asn_TYPE_descriptor_t *td,
			const uint8_t *wire, size_t wire_size, long expected_value) {
	uint8_t encoded[8] = { 0 };
	long *decoded = 0;
	asn_dec_rval_t rv;
	asn_enc_rval_t er;

	/* Decode the unknown extension and require the reserved marker, not an alias. */
	rv = aper_decode_complete(0, td, (void **)&decoded, wire, wire_size);
	assert(rv.code == RC_OK);
	assert(decoded);
	assert(*decoded == expected_value);

	/* Re-encode the marker and require byte-for-byte APER relay. */
	er = aper_encode_to_buffer(td, 0, decoded, encoded, sizeof(encoded));
	assert(er.encoded == (ssize_t)(wire_size * 8));
	assert(memcmp(encoded, wire, wire_size) == 0);
	ASN_STRUCT_FREE(*td, decoded);
}

int
main() {

	/* E ::= ENUMERATED { a, b(3), ..., c(1) } */
	check_value(&asn_DEF_E, 0, 0x00);	/* root #0 */
	check_value(&asn_DEF_E, 3, 0x40);	/* root #1 */
	check_value(&asn_DEF_E, 1, 0x80);	/* extension #0 */

	/* F ::= ENUMERATED { a, z(25), ..., d } -- d(1), X.680 20.6 */
	check_value(&asn_DEF_F, 0, 0x00);	/* root #0 */
	check_value(&asn_DEF_F, 25, 0x40);	/* root #1 */
	check_value(&asn_DEF_F, 1, 0x80);	/* extension #0 */

	/* H ::= ENUMERATED { a, b(3), ..., c(1), d(50) } -- two additions */
	check_value(&asn_DEF_H, 0, 0x00);	/* root #0 */
	check_value(&asn_DEF_H, 3, 0x40);	/* root #1 */
	check_value(&asn_DEF_H, 1, 0x80);	/* extension #0 */
	check_value(&asn_DEF_H, 50, 0x81);	/* extension #1 */

	/* G ::= ENUMERATED { x(5), y(2), w } -- w(0), not extensible */
	check_value(&asn_DEF_G, 0, 0x00);	/* index 0 */
	check_value(&asn_DEF_G, 2, 0x40);	/* index 1 */
	check_value(&asn_DEF_G, 5, 0x80);	/* index 2 */

	/*
	 * E and H know their declared additions, but not extension ordinal 63.
	 * These vectors reproduce the advisory's unknown APER input (0xBF) and
	 * also exercise the aligned long-form normally-small number path (index 64).
	 */
	{
		static const uint8_t unknown_63[] = { 0xBF };
		static const uint8_t unknown_64[] = { 0xC0, 0x01, 0x40 };
		check_unknown_extension(&asn_DEF_E, unknown_63, sizeof(unknown_63),
					       LONG_MAX - 63);
		check_unknown_extension(&asn_DEF_H, unknown_64, sizeof(unknown_64),
					       LONG_MAX - 64);
	}

	printf("Finished APER checks for issue #18\n");
	return 0;
}
