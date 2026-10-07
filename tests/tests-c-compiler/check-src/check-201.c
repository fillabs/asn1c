/*
 * Runtime test for nested CHOICE/SET members with identical identifiers.
 * Tests the fix for issue where criticalExtensions was used as both
 * a CHOICE name and an empty SEQUENCE name in nested structures.
 *
 * This test verifies that UPER encoding/decoding works correctly
 * when the compiler generates code with only one weak alias for the
 * outermost type descriptor.
 */
#undef	NDEBUG
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <assert.h>

#include <HandoverCommand.h>

/* Buffer for encoding/decoding */
static unsigned char buf[1024];

static int
encode_decode_test(void) {
	HandoverCommand_t *hc1 = NULL;
	HandoverCommand_t *hc2 = NULL;
	asn_enc_rval_t enc_rval;
	asn_dec_rval_t dec_rval;
	
	/* Allocate and initialize structure */
	hc1 = calloc(1, sizeof(HandoverCommand_t));
	assert(hc1);
	
	/* Set rrcTransId */
	hc1->rrcTransId = 123;
	
	/* Set criticalExtensions to r8 choice */
	hc1->criticalExtensions.present = criticalExtensions_PR_r8;
	
	/* Allocate and set data in r8 */
	OCTET_STRING_fromBuf(&hc1->criticalExtensions.choice.r8.data,
	                     "test", 4);
	
	/* Encode to UPER */
	enc_rval = uper_encode_to_buffer(&asn_DEF_HandoverCommand,
	                                 NULL, hc1, buf, sizeof(buf));
	assert(enc_rval.encoded > 0);
	
	printf("Encoded %zd bits\n", enc_rval.encoded);
	
	/* Decode from UPER */
	dec_rval = uper_decode_complete(NULL, &asn_DEF_HandoverCommand,
	                                (void **)&hc2, buf,
	                                (enc_rval.encoded + 7) / 8);
	assert(dec_rval.code == RC_OK);
	
	printf("Decoded successfully\n");
	
	/* Verify structure */
	assert(hc2);
	assert(hc2->rrcTransId == 123);
	assert(hc2->criticalExtensions.present == criticalExtensions_PR_r8);
	assert(hc2->criticalExtensions.choice.r8.data.size == 4);
	assert(memcmp(hc2->criticalExtensions.choice.r8.data.buf, "test", 4) == 0);
	
	printf("Verification passed for r8 choice\n");
	
	/* Clean up */
	ASN_STRUCT_FREE(asn_DEF_HandoverCommand, hc1);
	ASN_STRUCT_FREE(asn_DEF_HandoverCommand, hc2);
	
	return 0;
}

static int
encode_decode_nested_test(void) {
	HandoverCommand_t *hc1 = NULL;
	HandoverCommand_t *hc2 = NULL;
	asn_enc_rval_t enc_rval;
	asn_dec_rval_t dec_rval;
	
	/* Allocate and initialize structure */
	hc1 = calloc(1, sizeof(HandoverCommand_t));
	assert(hc1);
	
	/* Set rrcTransId */
	hc1->rrcTransId = 456;
	
	/* Set criticalExtensions to nested criticalExtensions choice */
	hc1->criticalExtensions.present = criticalExtensions_PR_criticalExtensions;
	
	/* Set nested criticalExtensions to r11 choice */
	hc1->criticalExtensions.choice.criticalExtensions.present =
	    HandoverCommand__criticalExtensions__criticalExtensions_PR_r11;
	
	/* Allocate and set data in r11 */
	OCTET_STRING_fromBuf(&hc1->criticalExtensions.choice.criticalExtensions.choice.r11.data2,
	                     "nested", 6);
	
	/* Encode to UPER */
	enc_rval = uper_encode_to_buffer(&asn_DEF_HandoverCommand,
	                                 NULL, hc1, buf, sizeof(buf));
	assert(enc_rval.encoded > 0);
	
	printf("Encoded %zd bits for nested structure\n", enc_rval.encoded);
	
	/* Decode from UPER */
	dec_rval = uper_decode_complete(NULL, &asn_DEF_HandoverCommand,
	                                (void **)&hc2, buf,
	                                (enc_rval.encoded + 7) / 8);
	assert(dec_rval.code == RC_OK);
	
	printf("Decoded nested structure successfully\n");
	
	/* Verify structure */
	assert(hc2);
	assert(hc2->rrcTransId == 456);
	assert(hc2->criticalExtensions.present == criticalExtensions_PR_criticalExtensions);
	assert(hc2->criticalExtensions.choice.criticalExtensions.present ==
	       HandoverCommand__criticalExtensions__criticalExtensions_PR_r11);
	assert(hc2->criticalExtensions.choice.criticalExtensions.choice.r11.data2.size == 6);
	assert(memcmp(hc2->criticalExtensions.choice.criticalExtensions.choice.r11.data2.buf,
	              "nested", 6) == 0);
	
	printf("Verification passed for nested r11 choice\n");
	
	/* Clean up */
	ASN_STRUCT_FREE(asn_DEF_HandoverCommand, hc1);
	ASN_STRUCT_FREE(asn_DEF_HandoverCommand, hc2);
	
	return 0;
}

int
main(int ac, char **av) {
	(void)ac;	/* Unused argument */
	(void)av;	/* Unused argument */
	
	printf("Testing UPER encode/decode for nested CHOICE with identical identifiers...\n");
	
	/* Test outer choice (r8) */
	if(encode_decode_test() != 0) {
		fprintf(stderr, "Test failed for r8 choice\n");
		return 1;
	}
	
	/* Test nested choice (r11) */
	if(encode_decode_nested_test() != 0) {
		fprintf(stderr, "Test failed for nested r11 choice\n");
		return 1;
	}
	
	printf("All tests passed!\n");
	return 0;
}
