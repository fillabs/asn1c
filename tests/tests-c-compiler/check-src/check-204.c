#undef NDEBUG
#include <assert.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <asn_application.h>
#include <SeqOfNull.h>
#include <SeqOfString.h>
#include <SetOfString.h>

/*
 * Regression test for malformed XER/JER collection decoders.
 *
 * Fuzzer-discovered examples:
 *   XER: 1<SEQUENCE-OF>
 *     The SET OF XER keyword skipper advanced to EOF, then accidentally
 *     continued the inner keyword loop and compared the next keyword using
 *     the stale token length.
 *
 *   JER: [[
 *     A nested array delimiter could make the element decoder return RC_OK
 *     with zero bytes consumed, causing SET OF / SEQUENCE OF to add an empty
 *     element forever.
 */

static void
decode_once(asn_TYPE_descriptor_t *td, enum asn_transfer_syntax syntax,
            const uint8_t *buf, size_t len) {
    void *ptr = NULL;
    asn_dec_rval_t r = asn_decode(0, syntax, td, &ptr, buf, len);

    assert(r.consumed <= len);
    ASN_STRUCT_FREE(*td, ptr);
}

#ifdef ENABLE_LIBFUZZER

int
LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    decode_once(&asn_DEF_SeqOfNull, ATS_BASIC_XER, Data, Size);
    decode_once(&asn_DEF_SeqOfString, ATS_BASIC_XER, Data, Size);
    decode_once(&asn_DEF_SetOfString, ATS_BASIC_XER, Data, Size);
    decode_once(&asn_DEF_SeqOfString, ATS_JER, Data, Size);
    decode_once(&asn_DEF_SetOfString, ATS_JER, Data, Size);
    return 0;
}

#else  /* !ENABLE_LIBFUZZER */

static void
alarm_handler(int sig) {
    (void)sig;
    fprintf(stderr, "FAIL: malformed SET OF / SEQUENCE OF decode did not terminate\n");
    _exit(1);
}

static void
check_not_ok(asn_TYPE_descriptor_t *td, enum asn_transfer_syntax syntax,
             const char *label, const uint8_t *buf, size_t len) {
    void *ptr = NULL;
    asn_dec_rval_t r;

    signal(SIGALRM, alarm_handler);
    alarm(5);
    r = asn_decode(0, syntax, td, &ptr, buf, len);
    alarm(0);

    fprintf(stderr, "%s: %s code=%d consumed=%zu\n",
            td->name, label, (int)r.code, r.consumed);
    assert(r.code != RC_OK);
    assert(r.consumed <= len);
    ASN_STRUCT_FREE(*td, ptr);
}

static void
check_ok(asn_TYPE_descriptor_t *td, enum asn_transfer_syntax syntax,
         const char *label, const uint8_t *buf, size_t len) {
    void *ptr = NULL;
    asn_dec_rval_t r = asn_decode(0, syntax, td, &ptr, buf, len);

    fprintf(stderr, "%s: %s code=%d consumed=%zu\n",
            td->name, label, (int)r.code, r.consumed);
    assert(r.code == RC_OK);
    assert(r.consumed <= len);
    ASN_STRUCT_FREE(*td, ptr);
}

int
main(void) {
    static const uint8_t jer_ok[] = "[\"A\"]";
    static const uint8_t jer_nested_array[] = "[[";
    static const uint8_t xer_keyword_after_text[] = "1<SEQUENCE-OF>";
    static const uint8_t xer_keyword[] = "<SEQUENCE-OF>";

    check_ok(&asn_DEF_SeqOfString, ATS_JER,
             "valid JER array", jer_ok, sizeof(jer_ok) - 1);
    check_ok(&asn_DEF_SetOfString, ATS_JER,
             "valid JER array", jer_ok, sizeof(jer_ok) - 1);

    check_not_ok(&asn_DEF_SeqOfString, ATS_JER,
                 "malformed nested array", jer_nested_array,
                 sizeof(jer_nested_array) - 1);
    check_not_ok(&asn_DEF_SetOfString, ATS_JER,
                 "malformed nested array", jer_nested_array,
                 sizeof(jer_nested_array) - 1);

    check_not_ok(&asn_DEF_SeqOfNull, ATS_BASIC_XER,
                 "truncated keyword after text", xer_keyword_after_text,
                 sizeof(xer_keyword_after_text) - 1);
    check_not_ok(&asn_DEF_SeqOfString, ATS_BASIC_XER,
                 "truncated keyword after text", xer_keyword_after_text,
                 sizeof(xer_keyword_after_text) - 1);
    check_not_ok(&asn_DEF_SetOfString, ATS_BASIC_XER,
                 "truncated keyword", xer_keyword, sizeof(xer_keyword) - 1);

    printf("Malformed SET OF / SEQUENCE OF decoder regression tests passed.\n");
    return 0;
}

#endif /* ENABLE_LIBFUZZER */
