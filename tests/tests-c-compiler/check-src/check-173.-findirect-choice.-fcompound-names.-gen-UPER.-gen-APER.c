/*
 * Regression test for decode-failure cleanup of IOC OPEN TYPE values
 * represented as indirect CHOICE alternatives.
 */
#undef NDEBUG
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <aper_decoder.h>
#include <aper_encoder.h>
#include <uper_decoder.h>
#include <uper_encoder.h>
#include <OBJECT_IDENTIFIER.h>
#include <OCTET_STRING.h>

#include "ContentWrapper.h"
#include "PersonInfo.h"

static void
fill_wrapper(ContentWrapper_t *wrapper) {
    asn_oid_arc_t arcs[] = {1, 2, 3, 4, 1};
    PersonInfo_t *person;

    memset(wrapper, 0, sizeof(*wrapper));

    assert(OBJECT_IDENTIFIER_set_arcs(&wrapper->contentType, arcs,
                                      sizeof(arcs) / sizeof(arcs[0])) == 0);

    person = calloc(1, sizeof(*person));
    assert(person);

    assert(OCTET_STRING_fromString(&person->name,
                                   "Alice OpenType Regression") == 0);
    person->age = 42;

    person->email = calloc(1, sizeof(*person->email));
    assert(person->email);
    assert(OCTET_STRING_fromString(person->email,
                                   "alice.opentype@example.test") == 0);

    wrapper->content.present = ContentWrapper__content_PR_PersonInfo;
    wrapper->content.choice.PersonInfo = person;
}

static void
check_decoded(const ContentWrapper_t *wrapper) {
    assert(wrapper);
    assert(wrapper->content.present == ContentWrapper__content_PR_PersonInfo);
    assert(wrapper->content.choice.PersonInfo);
    assert(wrapper->content.choice.PersonInfo->age == 42);
}

static void
check_aper_decode(const void *buffer, size_t size) {
    ContentWrapper_t *decoded = NULL;
    asn_dec_rval_t rval;

    rval = aper_decode(0, &asn_DEF_ContentWrapper, (void **)&decoded,
                       buffer, size, 0, 0);
    assert(rval.code == RC_OK);
    check_decoded(decoded);
    ASN_STRUCT_FREE(asn_DEF_ContentWrapper, decoded);
}

static void
check_uper_decode(const void *buffer, size_t size) {
    ContentWrapper_t *decoded = NULL;
    asn_dec_rval_t rval;

    rval = uper_decode(0, &asn_DEF_ContentWrapper, (void **)&decoded,
                       buffer, size, 0, 0);
    assert(rval.code == RC_OK);
    check_decoded(decoded);
    ASN_STRUCT_FREE(asn_DEF_ContentWrapper, decoded);
}

static void
check_truncated_aper_decode_cleanup(const void *buffer, size_t size) {
    size_t cut;
    int saw_failure = 0;

    assert(size > 1);

    for(cut = 1; cut < size; cut++) {
        ContentWrapper_t *decoded = NULL;
        asn_dec_rval_t rval;

        rval = aper_decode(0, &asn_DEF_ContentWrapper, (void **)&decoded,
                           buffer, cut, 0, 0);
        if(rval.code == RC_OK) {
            ASN_STRUCT_FREE(asn_DEF_ContentWrapper, decoded);
            continue;
        }

        saw_failure = 1;
        ASN_STRUCT_FREE(asn_DEF_ContentWrapper, decoded);
    }

    assert(saw_failure);
}

static void
check_truncated_uper_decode_cleanup(const void *buffer, size_t size) {
    size_t cut;
    int saw_failure = 0;

    assert(size > 1);

    for(cut = 1; cut < size; cut++) {
        ContentWrapper_t *decoded = NULL;
        asn_dec_rval_t rval;

        rval = uper_decode(0, &asn_DEF_ContentWrapper, (void **)&decoded,
                           buffer, cut, 0, 0);
        if(rval.code == RC_OK) {
            ASN_STRUCT_FREE(asn_DEF_ContentWrapper, decoded);
            continue;
        }

        saw_failure = 1;
        ASN_STRUCT_FREE(asn_DEF_ContentWrapper, decoded);
    }

    assert(saw_failure);
}

int
main(int ac, char **av) {
    ContentWrapper_t wrapper;
    void *aper_buffer = NULL;
    void *uper_buffer = NULL;
    ssize_t aper_size;
    ssize_t uper_size;

    (void)ac;
    (void)av;

    fill_wrapper(&wrapper);

    aper_size = aper_encode_to_new_buffer(&asn_DEF_ContentWrapper, 0,
                                          &wrapper, &aper_buffer);
    assert(aper_size > 0);
    assert(aper_buffer);

    uper_size = uper_encode_to_new_buffer(&asn_DEF_ContentWrapper, 0,
                                          &wrapper, &uper_buffer);
    assert(uper_size > 0);
    assert(uper_buffer);

    check_aper_decode(aper_buffer, (size_t)aper_size);
    check_uper_decode(uper_buffer, (size_t)uper_size);

    /*
     * Exercise decode-failure cleanup after OPEN TYPE starts decoding the
     * selected indirect CHOICE alternative.  LeakSanitizer/ASan will catch a
     * regression where the temporary inner pointer becomes unreachable before
     * it is copied back into ContentWrapper.content.choice.PersonInfo.
     */
    check_truncated_aper_decode_cleanup(aper_buffer, (size_t)aper_size);
    check_truncated_uper_decode_cleanup(uper_buffer, (size_t)uper_size);

    free(aper_buffer);
    free(uper_buffer);
    ASN_STRUCT_RESET(asn_DEF_ContentWrapper, &wrapper);

    return 0;
}
