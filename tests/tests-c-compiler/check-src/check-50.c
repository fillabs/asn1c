#undef	NDEBUG
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <assert.h>

#include <Int5.h>
#include <PER-Visible.h>
#include <Str4.h>
#include <Utf8-4.h>
#include <VisibleIdentifier.h>

/**
 * Purpose: Run one generated ASN.1 constraint check and report diagnostic text.
 * Original source: vlm/asn1c PR #550, with defensive test-helper validation added here.
 * Version: 2026-07-17, permitted-alphabet empty-value regression coverage.
 * Parameters:
 *   td - Non-NULL descriptor for the ASN.1 value being checked.
 *   sptr - Non-NULL pointer to the value being checked.
 * Returns: Zero when the value satisfies its constraints; non-zero otherwise.
 * Exceptions: None; invalid helper inputs are rejected without dereferencing them.
 * Author: libo <shakespark@gmail.com>; port maintained by the asn1c maintainers.
 * History: Added on 2026-07-17 while porting vlm/asn1c PR #550.
 * Example: check_constraints(&asn_DEF_PER_Visible, &value).
 */
static int
check_constraints(const asn_TYPE_descriptor_t *td, const void *sptr) {
	/* Keep generated diagnostics in bounded local storage for failed checks. */
	char errbuf[128];
	size_t errlen = sizeof(errbuf);
	int ret;

	/* Reject malformed calls before the diagnostic path dereferences the descriptor. */
	if(!td || !sptr) {
		fprintf(stderr, "check_constraints: descriptor and value are required\n");
		return -1;
	}

	/* Preserve the generated constraint diagnostic when a valid call fails. */
	ret = asn_check_constraints(td, sptr, errbuf, &errlen);
	if(ret) {
		fprintf(stderr, "%s: %s\n", td->name, errbuf);
	}
	return ret;
}

/**
 * Purpose: Exercise permitted-alphabet checks for empty, malformed, valid, and invalid values.
 * Original source: Runtime regression test from vlm/asn1c PR #550.
 * Version: 2026-07-17, local port with test-helper input-validation cases.
 * Parameters:
 *   ac - Process argument count; accepted for the Automake test interface and otherwise unused.
 *   av - Process argument vector; accepted for the Automake test interface and otherwise unused.
 * Returns: Zero after every assertion passes; assertion failure terminates the test otherwise.
 * Exceptions: None in the C sense; failed assertions abort the regression executable.
 * Author: libo <shakespark@gmail.com>; port maintained by the asn1c maintainers.
 * History: Replaced the compile-only check on 2026-07-17 while porting vlm/asn1c PR #550.
 * Example: Automake invokes this program through check-assembly.sh.
 */
int
main(int ac, char **av) {
	/* PER-Visible ::= IA5String (FROM("A".."F")), with no SIZE constraint. */
	PER_Visible_t st = {0}; /* Test state variable for each representation. */
	uint8_t empty[1] = {0};
	uint8_t good[] = {'A', 'B', 'F'};
	uint8_t bad[] = {'G'};

	(void)ac;	/* Unused argument */
	(void)av;	/* Unused argument */

	/*
	 * A buffer-less empty string is valid because the type has no lower SIZE
	 * bound. The generated permitted-alphabet check must accept it without
	 * arithmetic or an ordered comparison on a NULL pointer.
	 */
	assert(st.buf == NULL);
	assert(st.size == 0);
	assert(check_constraints(&asn_DEF_PER_Visible, &st) == 0);

	/* Reject invalid helper calls without crashing or dereferencing NULL. */
	assert(check_constraints(NULL, &st) != 0);
	assert(check_constraints(&asn_DEF_PER_Visible, NULL) != 0);

	/* Reject a malformed value that claims bytes while its buffer is NULL. */
	st.buf = NULL;
	st.size = 1;
	assert(check_constraints(&asn_DEF_PER_Visible, &st) != 0);

	/* Accept an empty value represented by a non-NULL zero-size backing buffer. */
	st.buf = empty;
	st.size = 0;
	assert(check_constraints(&asn_DEF_PER_Visible, &st) == 0);

	/* Accept every member of the permitted alphabet used by this test. */
	st.buf = good;
	st.size = sizeof(good);
	assert(check_constraints(&asn_DEF_PER_Visible, &st) == 0);

	/* Reject a non-empty value containing a character outside the alphabet. */
	st.buf = bad;
	st.size = sizeof(bad);
	assert(check_constraints(&asn_DEF_PER_Visible, &st) != 0);

	return 0;
}
