#undef	NDEBUG
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <assert.h>

#include <Choice.h>

static int validate(Choice_t *source, const char *desc) {
    uint8_t tmpbuf[128];
    Choice_t *decoded = NULL;
    int rc = 0;

    asn_enc_rval_t er =
        asn_encode_to_buffer(NULL, ATS_ALIGNED_BASIC_PER, &asn_DEF_Choice, source, tmpbuf, sizeof(tmpbuf));
    assert(er.encoded != -1);

    asn_dec_rval_t dr =
        asn_decode(NULL, ATS_ALIGNED_BASIC_PER, &asn_DEF_Choice, (void **)&decoded, tmpbuf, er.encoded);

    assert(dr.code == RC_OK);
    if((ssize_t)dr.consumed != er.encoded) {
        ASN_DEBUG("Consumed %zd, expected %zu", dr.consumed, er.encoded);
        assert((ssize_t)dr.consumed == er.encoded);
    }

    if(XEQ_SUCCESS != xer_equivalent(&asn_DEF_Choice, source, decoded, stderr)) {
        ASN_DEBUG("Equivalency failed for: %s\n", desc);
        rc = 1;
    }
    ASN_STRUCT_RESET(asn_DEF_Choice, source);
    ASN_STRUCT_FREE(asn_DEF_Choice, decoded);

    return rc;
}

int
main(int ac, char **av) {
    int result = 0;
    Choice_t source;
    Choice_t *child;

	(void)ac;	/* Unused argument */
	(void)av;	/* Unused argument */

	memset(&source, 0, sizeof(source));
    source.present = Choice_PR_a;
    source.choice.a = 1;
    result += validate(&source, "Choice A");

	memset(&source, 0, sizeof(source));
    source.present = Choice_PR_b;
    source.choice.b = 1;
    result += validate(&source, "Choice B");

    child = calloc(1, sizeof(*child));
    child->present = Choice_PR_a;
    child->choice.a = 1;
	memset(&source, 0, sizeof(source));
    source.present = Choice_PR_c;
    source.choice.c = child;
    result += validate(&source, "Choice C, Child A");

    child = calloc(1, sizeof(*child));
    child->present = Choice_PR_b;
    child->choice.b = 1;
	memset(&source, 0, sizeof(source));
    source.present = Choice_PR_c;
    source.choice.c = child;
    result += validate(&source, "Choice C, Child B");

	return result;
}
