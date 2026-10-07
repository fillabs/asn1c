/*
 * Regression test for issue #552, reported by @zhouvlia.
 *
 * Purpose: verify that an ENUMERATED typedef keeps a non-NULL PER constraint
 * slot in its generated descriptor and decodes through both PER variants.
 * Original source: issue #552's minimal alias_enum.asn1 reproduction.
 * Version: 2026-07-18.
 * Inputs: generated MyEnumAlias and Container descriptors plus one-byte PER
 *         encodings for Container { value e2 }.
 * Returns: process status zero on success; assert failure otherwise.
 * Exceptions: none are thrown; malformed input must return a non-success
 *             decoder result and must not crash or leak a decoded object.
 * Responsible party: asn1c maintainers; discovery credited to @zhouvlia.
 * History: added with the issue #552 fix.
 * Example: this file is run by check-assembly.sh with -gen-UPER -gen-APER.
 */
#undef NDEBUG

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#include <aper_decoder.h>
#include <uper_decoder.h>

#include "Container.h"
#include "MyEnumAlias.h"

/*
 * A complete PER decoder has one common callable signature for UPER and APER.
 * The test uses this type to apply the same valid and truncated-input checks
 * to both generated codec variants.
 */
typedef asn_dec_rval_t (*decode_complete_f)(
    const asn_codec_ctx_t *opt_codec_ctx,
    const asn_TYPE_descriptor_t *type_descriptor,
    void **struct_ptr,
    const void *buffer,
    size_t size);

/*
 * Purpose: verify that the alias descriptor points to its generated PER table.
 * Original source: issue #552's generated MyEnumAlias.c inspection.
 * Version: 2026-07-18.
 * Inputs: no function parameters.
 * Returns: no value.
 * Exceptions: assert aborts if the generator leaves the slot NULL or points
 *             it at a different constraint table.
 * Responsible party: asn1c maintainers; discovery credited to @zhouvlia.
 * History: added with the issue #552 regression test.
 * Example: check_alias_descriptor();
 */
static void
check_alias_descriptor(void) {
    assert(asn_DEF_MyEnumAlias.encoding_constraints.per_constraints != NULL);
    assert(asn_DEF_MyEnumAlias.encoding_constraints.per_constraints
           == &asn_PER_type_MyEnumAlias_constr_1);
}

/*
 * Purpose: decode the valid e2 vector and reject an incomplete vector.
 * Original source: issue #552's 0x40 Container reproduction.
 * Version: 2026-07-18.
 * Inputs: codec_name identifies the variant; decode_complete is either
 *         uper_decode_complete or aper_decode_complete.
 * Returns: no value.
 * Exceptions: assert aborts on incorrect decoding, wrong enum value, or a
 *             malformed-input success; decoded objects are always released.
 * Responsible party: asn1c maintainers; discovery credited to @zhouvlia.
 * History: added with the issue #552 regression test.
 * Example: check_codec("UPER", uper_decode_complete);
 */
static void
check_codec(const char *codec_name, decode_complete_f decode_complete) {
    static const uint8_t valid[] = { 0xc0 };
    static const uint8_t incomplete[] = { 0x00 };
    Container_t *decoded = NULL;
    asn_dec_rval_t rval;

    assert(codec_name != NULL && codec_name[0] != '\0');
    rval = decode_complete(NULL, &asn_DEF_Container, (void **)&decoded,
                           valid, sizeof(valid));
    assert(rval.code == RC_OK);
    assert(decoded != NULL);
    assert(decoded->value != NULL);
    assert(*decoded->value == 2);
    ASN_STRUCT_FREE(asn_DEF_Container, decoded);

    decoded = NULL;
    rval = decode_complete(NULL, &asn_DEF_Container, (void **)&decoded,
                           incomplete, 0);
    assert(rval.code != RC_OK);
    if(decoded != NULL) {
        ASN_STRUCT_FREE(asn_DEF_Container, decoded);
    }
}

/*
 * Purpose: execute descriptor, UPER, and APER regression checks.
 * Original source: issue #552's minimal reproduction program.
 * Version: 2026-07-18.
 * Inputs: no function parameters.
 * Returns: zero when all assertions pass.
 * Exceptions: assert aborts on a regression; the codec must not crash on the
 *             truncated input exercised by check_codec().
 * Responsible party: asn1c maintainers; discovery credited to @zhouvlia.
 * History: added with the issue #552 regression test.
 * Example: main();
 */
int
main(void) {
    check_alias_descriptor();
    check_codec("UPER", uper_decode_complete);
    check_codec("APER", aper_decode_complete);
    return 0;
}
