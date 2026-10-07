/*
 * Purpose: Exercise APER open types through the complete data-126 XER corpus.
 * Original source: the round-trip structure of check-126.-gen-UPER.c, adapted
 *                  to APER-native encoding and decoding.
 * Version: 2026-07-18.
 * Inputs: XER values under data-126 and malformed APER vectors defined below.
 * Returns: process status zero when every APER round trip succeeds.
 * Exceptions: assertions abort the test; malformed input must not crash.
 * Responsible party: asn1c maintainers.
 * History: added with the APER open-type null-pointer-arithmetic fix.
 * Example: this file is run by check-assembly.sh with -gen-APER.
 *
 * APER does not reuse the data-126 output files: those are UPER encodings and are
 * not valid APER goldens.  Instead, each XER value is encoded and decoded by
 * APER, then compared after a BASIC-XER round trip.
 */
#undef NDEBUG

#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <aper_decoder.h>
#include <aper_encoder.h>
#include <xer_decoder.h>
#include <xer_encoder.h>

#include <PDU.h>

#ifndef SRCDIR
#define SRCDIR_S ".."
#else
#define STRINGIFY_MACRO2(x) #x
#define STRINGIFY_MACRO(x) STRINGIFY_MACRO2(x)
#define SRCDIR_S STRINGIFY_MACRO(SRCDIR)
#endif

#define TEST_BUFFER_SIZE 4096

static uint8_t aper_buffer[TEST_BUFFER_SIZE];
static size_t aper_size;
static uint8_t xer_buffer[TEST_BUFFER_SIZE];
static size_t xer_size;

/*
 * Purpose: Collect XER output into its bounded comparison buffer.
 * Original source: the writer used by check-126.-gen-UPER.c.
 * Version: 2026-07-18.
 * Inputs: buffer and size identify emitted XER bytes; app_key is unused.
 * Returns: zero on success, non-zero on overflow or a null source pointer.
 * Exceptions: none; invalid writes are reported to the encoder.
 * Responsible party: asn1c maintainers.
 * History: added for APER-to-XER round-trip comparisons.
 * Example: xer_encode(..., write_xer_bytes, NULL).
 */
static int
write_xer_bytes(const void *buffer, size_t size, void *app_key) {
    (void)app_key;
    if(buffer == NULL || size > TEST_BUFFER_SIZE - xer_size) {
        return -1;
    }
    memcpy(xer_buffer + xer_size, buffer, size);
    xer_size += size;
    return 0;
}

/*
 * Purpose: Read one bounded XER corpus member from the data-126 directory.
 * Original source: the file-loading path in check-126.-gen-UPER.c.
 * Version: 2026-07-18.
 * Inputs: fname is the corpus basename; destination and capacity bound input.
 * Returns: the number of bytes read, excluding the terminating sentinel.
 * Exceptions: assertions abort on path, I/O, or oversized corpus failures.
 * Responsible party: asn1c maintainers.
 * History: adapted for APER-native corpus processing.
 * Example: read_xer_file("data-126-19.in", input, sizeof(input)).
 */
static size_t
read_xer_file(const char *fname, uint8_t *destination, size_t capacity) {
    char path[sizeof(SRCDIR_S) + 256];
    FILE *file;
    size_t size;

    assert(fname != NULL && destination != NULL && capacity > 1);
    snprintf(path, sizeof(path), SRCDIR_S "/data-126/%s", fname);
    file = fopen(path, "rb");
    assert(file != NULL);
    size = fread(destination, 1, capacity - 1, file);
    assert(!ferror(file));
    assert(fclose(file) == 0);
    assert(size < capacity - 1);
    destination[size] = '\0';
    return size;
}

/*
 * Purpose: Compare two XER buffers while ignoring formatting whitespace.
 * Original source: xer_encoding_equal() in check-126.-gen-UPER.c.
 * Version: 2026-07-18.
 * Inputs: left/right and their sizes identify the BASIC-XER buffers.
 * Returns: non-zero when the significant XML bytes are equal.
 * Exceptions: none; null buffers are accepted only when their sizes are zero.
 * Responsible party: asn1c maintainers.
 * History: retained for APER round-trip comparisons.
 * Example: assert(xer_equal(input, input_size, xer_buffer, xer_size)).
 */
static int
xer_equal(const uint8_t *left, size_t left_size,
          const uint8_t *right, size_t right_size) {
    size_t left_pos = 0;
    size_t right_pos = 0;

    while(left_pos < left_size || right_pos < right_size) {
        while(left_pos < left_size
              && isspace((unsigned char)left[left_pos])) {
            left_pos++;
        }
        while(right_pos < right_size
              && isspace((unsigned char)right[right_pos])) {
            right_pos++;
        }
        if(left_pos == left_size || right_pos == right_size) {
            return left_pos == left_size && right_pos == right_size;
        }
        if(left[left_pos++] != right[right_pos++]) {
            return 0;
        }
    }
    return 1;
}

/*
 * Purpose: Exercise the APER decoder's zero-length first open-type chunk.
 * Original source: the malformed aligned-PER analogue of data-126-06-P.out.
 * Version: 2026-07-18.
 * Inputs: no parameters; malformed contains an extension bitmap, mandatory
 *         str-m extension, and an invalid zero-byte open-type determinant.
 * Returns: no value; the malformed encoding must be rejected.
 * Exceptions: assertion failure indicates accidental acceptance or a crash.
 * Responsible party: asn1c maintainers.
 * History: added with the APER open-type null-pointer-arithmetic fix.
 * Example: check_zero_length_open_type();
 */
static void
check_zero_length_open_type(void) {
    static const uint8_t malformed[] = { 0x83, 0x40, 0x00 };
    PDU_t *decoded = NULL;
    asn_dec_rval_t rval;

    /* The zero determinant is invalid, but must not form NULL + zero. */
    rval = aper_decode_complete(NULL, &asn_DEF_PDU, (void **)&decoded,
                                malformed, sizeof(malformed));
    assert(rval.code != RC_OK);
    if(decoded != NULL) {
        ASN_STRUCT_FREE(asn_DEF_PDU, decoded);
    }
}

/*
 * Purpose: Verify that truncating a valid APER open-type encoding is rejected.
 * Original source: the malformed-input expectations in check-126.-gen-UPER.c.
 * Version: 2026-07-18.
 * Inputs: no parameters; aper_buffer contains the current valid encoding.
 * Returns: no value; a truncated complete encoding must not decode successfully.
 * Exceptions: assertion failure indicates unsafe or incomplete rejection.
 * Responsible party: asn1c maintainers.
 * History: added for APER malformed-input coverage.
 * Example: check_truncated_encoding();
 */
static void
check_truncated_encoding(void) {
    PDU_t *decoded = NULL;
    asn_dec_rval_t rval;

    if(aper_size < 2) {
        return;
    }
    rval = aper_decode_complete(NULL, &asn_DEF_PDU, (void **)&decoded,
                                aper_buffer, aper_size - 1);
    assert(rval.code != RC_OK);
    if(decoded != NULL) {
        ASN_STRUCT_FREE(asn_DEF_PDU, decoded);
    }
}

/*
 * Purpose: Encode, decode, and XER-compare one data-126 PDU through APER.
 * Original source: process_XER_data() in check-126.-gen-UPER.c.
 * Version: 2026-07-18.
 * Inputs: fname names a corpus XER file; input and input_size hold its bytes.
 * Returns: no value; assertions enforce successful APER and XER round trips.
 * Exceptions: assertion failures report codec or corpus regressions.
 * Responsible party: asn1c maintainers.
 * History: added to broaden APER extension/default/choice coverage.
 * Example: check_xer_case("data-126-19.in", input, input_size).
 */
static void
check_xer_case(const char *fname, const uint8_t *input, size_t input_size) {
    PDU_t *original = NULL;
    PDU_t *decoded = NULL;
    asn_dec_rval_t dr;
    asn_enc_rval_t er;
    size_t encoded_size;
    uint8_t expected_xer[TEST_BUFFER_SIZE];
    size_t expected_xer_size;

    dr = xer_decode(NULL, &asn_DEF_PDU, (void **)&original,
                    input, input_size);
    assert(dr.code == RC_OK && original != NULL);

    /* Establish the canonical XER form before crossing the APER boundary. */
    xer_size = 0;
    er = xer_encode(&asn_DEF_PDU, original, XER_F_BASIC,
                    write_xer_bytes, NULL);
    assert(er.encoded >= 0 && xer_size <= sizeof(expected_xer));
    expected_xer_size = xer_size;
    memcpy(expected_xer, xer_buffer, expected_xer_size);

    memset(aper_buffer, 0, sizeof(aper_buffer));
    er = aper_encode_to_buffer(&asn_DEF_PDU, NULL, original,
                               aper_buffer, sizeof(aper_buffer));
    assert(er.encoded > 0);
    encoded_size = ((size_t)er.encoded + 7) / 8;
    assert(encoded_size <= sizeof(aper_buffer));
    aper_size = encoded_size;

    check_truncated_encoding();
    dr = aper_decode_complete(NULL, &asn_DEF_PDU, (void **)&decoded,
                              aper_buffer, aper_size);
    assert(dr.code == RC_OK && decoded != NULL);

    xer_size = 0;
    er = xer_encode(&asn_DEF_PDU, decoded, XER_F_BASIC,
                    write_xer_bytes, NULL);
    assert(er.encoded >= 0);
    assert(xer_equal(expected_xer, expected_xer_size,
                     xer_buffer, xer_size));

    ASN_STRUCT_FREE(asn_DEF_PDU, decoded);
    ASN_STRUCT_FREE(asn_DEF_PDU, original);
}

/*
 * Purpose: Process one data-126 XER corpus entry through APER.
 * Original source: process() in check-126.-gen-UPER.c.
 * Version: 2026-07-18.
 * Inputs: fname is a directory entry name.
 * Returns: one for an input file processed, zero for other directory entries.
 * Exceptions: assertions abort on an invalid corpus entry or codec result.
 * Responsible party: asn1c maintainers.
 * History: added for APER-native data-126 coverage.
 * Example: process_case("data-126-19.in");
 */
static int
process_case(const char *fname) {
    uint8_t input[TEST_BUFFER_SIZE];
    const char *suffix;
    size_t input_size;

    suffix = strrchr(fname, '.');
    if(suffix == NULL || strcmp(suffix, ".in") != 0) {
        return 0;
    }
    input_size = read_xer_file(fname, input, sizeof(input));
    check_xer_case(fname, input, input_size);
    return 1;
}

/*
 * Purpose: Run the focused malformed vector and the complete APER corpus.
 * Original source: main() in check-126.-gen-UPER.c.
 * Version: 2026-07-18.
 * Inputs: DATA_126_FILE optionally selects one corpus entry.
 * Returns: zero after all selected tests pass.
 * Exceptions: assertions abort on missing corpus data or codec regressions.
 * Responsible party: asn1c maintainers.
 * History: added with the APER-native check-126 companion test.
 * Example: DATA_126_FILE=data-126-19.in check-program.
 */
int
main(void) {
    DIR *directory;
    struct dirent *entry;
    char *selected;
    int processed = 0;

    check_zero_length_open_type();
    selected = getenv("DATA_126_FILE");
    if(selected != NULL && strncmp(selected, "data-126-", 9) == 0) {
        assert(process_case(selected));
        return 0;
    }

    directory = opendir(SRCDIR_S "/data-126");
    assert(directory != NULL);
    while((entry = readdir(directory)) != NULL) {
        if(strncmp(entry->d_name, "data-126-", 9) == 0) {
            processed += process_case(entry->d_name);
        }
    }
    assert(closedir(directory) == 0);
    assert(processed > 0);
    return 0;
}
