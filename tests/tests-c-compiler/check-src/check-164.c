#undef	NDEBUG
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <assert.h>

#include <A.h>
#include <B.h>

int
main(int ac, char **av) {
	A_t a;
	B_t b;

	(void)ac;	/* Unused argument */
	(void)av;	/* Unused argument */

	memset(&a, 0, sizeof(a));
	memset(&b, 0, sizeof(b));

    /* Check existence of the following enum values */
    assert(a_Ordered_val_PR_UTF8String);
    assert(a_Ordered_val_PR_OCTET_STRING);
    assert(b_Unordered_val_PR_UTF8String);
    assert(b_Unordered_val_PR_OCTET_STRING);

	/*
	 * No plans to fill it up: just checking whether it compiles or not.
	 */

	return 0;
}
