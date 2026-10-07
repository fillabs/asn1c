#undef NDEBUG
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include <Message.h>

/*
 * Behavioral regression test for CVE-2025-32893.
 *
 * Spec 212 defines two information object sets that bind the same &id (1) to
 * different types:
 *
 *     SetA: id 1 -> IntPayload (INTEGER)
 *     SetB: id 1 -> StrPayload (IA5String)
 *
 * Message.second is parameterized by SetB, so its open type at id 1 must decode
 * as an IA5String and reject an INTEGER. If parameterization ignores the
 * VALUESET constraint, second can reuse SetA's table and these assertions flip.
 */

/* Message { first {id 1, INTEGER 42}, second {id 1, IA5String "hi"} } */
static const uint8_t msg_setB_ia5[] = {
    0x30, 0x15, 0xa0, 0x08, 0x80, 0x01, 0x01, 0xa1, 0x03, 0x02, 0x01, 0x2a,
    0xa1, 0x09, 0x80, 0x01, 0x01, 0xa1, 0x04, 0x16, 0x02, 0x68, 0x69
};

/* Same, but second's open type carries INTEGER 42 (valid only under SetA). */
static const uint8_t msg_setB_int[] = {
    0x30, 0x14, 0xa0, 0x08, 0x80, 0x01, 0x01, 0xa1, 0x03, 0x02, 0x01, 0x2a,
    0xa1, 0x08, 0x80, 0x01, 0x01, 0xa1, 0x03, 0x02, 0x01, 0x2a
};

static int
decode_code(const uint8_t *buf, size_t size) {
    Message_t *m = 0;
    asn_dec_rval_t rv = ber_decode(0, &asn_DEF_Message, (void **)&m, buf, size);
    ASN_STRUCT_FREE(asn_DEF_Message, m);
    return rv.code;
}

int
main(void) {
    int code;

    code = decode_code(msg_setB_ia5, sizeof(msg_setB_ia5));
    printf("second = IA5String at id 1: ber_decode -> %d\n", code);
    assert(code == RC_OK);

    code = decode_code(msg_setB_int, sizeof(msg_setB_int));
    printf("second = INTEGER  at id 1: ber_decode -> %d\n", code);
    assert(code != RC_OK);

    printf("OK: open type at id 1 is constrained by SetB, not SetA.\n");
    return 0;
}
