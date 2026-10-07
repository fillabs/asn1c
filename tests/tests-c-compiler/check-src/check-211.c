#undef NDEBUG
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <Message.h>
#include <T.h>

static void
decode_ber_once(const uint8_t *buf, size_t size, int expect_ok) {
    T_t *decoded = NULL;
    asn_dec_rval_t rv;

    rv = ber_decode(0, &asn_DEF_T, (void **)&decoded, buf, size);
    if(expect_ok) {
        assert(rv.code == RC_OK);
        assert(decoded);
    } else {
        assert(rv.code != RC_OK);
    }
    ASN_STRUCT_FREE(asn_DEF_T, decoded);
}

static void
fill_defined(T_t *t) {
    memset(t, 0, sizeof(*t));
    t->id = 1;
    t->value.present = value_PR_Payload;
    t->value.choice.Payload = 1;
}

static void
decode_oer_valid_and_truncated(void) {
    uint8_t buf[32];
    T_t source;
    T_t *decoded = NULL;
    asn_enc_rval_t er;
    asn_dec_rval_t rv;
    ssize_t encoded;
    size_t cut;
    int saw_failure = 0;

    fill_defined(&source);

    er = oer_encode_to_buffer(&asn_DEF_T, 0, &source, buf, sizeof(buf));
    assert(er.encoded > 0);
    encoded = er.encoded;

    rv = oer_decode(0, &asn_DEF_T, (void **)&decoded, buf, encoded);
    assert(rv.code == RC_OK);
    assert(decoded);
    assert(decoded->id == 1);
    assert(decoded->value.present == value_PR_Payload);
    assert(decoded->value.choice.Payload != 0);
    ASN_STRUCT_FREE(asn_DEF_T, decoded);

    /*
     * Truncated OER input exercises OPEN TYPE cleanup after the selected
     * BOOLEAN decoder has started, where the wrapper must be reset as a
     * CHOICE rather than as the selected primitive type.
     */
    for(cut = 1; cut < (size_t)encoded; cut++) {
        decoded = NULL;
        rv = oer_decode(0, &asn_DEF_T, (void **)&decoded, buf, cut);
        if(rv.code == RC_OK) {
            ASN_STRUCT_FREE(asn_DEF_T, decoded);
            continue;
        }
        saw_failure = 1;
        ASN_STRUCT_FREE(asn_DEF_T, decoded);
    }
    assert(saw_failure);

    ASN_STRUCT_RESET(asn_DEF_T, &source);
}

static void
decode_message_with_undefined_key_row(void) {
    /*
     * The first PROC-SET row has VALUE but no CODE, so its constraining
     * procedureCode cell is undefined while its value type cell is defined.
     * The generated selector must skip that row without dereferencing the
     * missing constraining cell descriptor, then match CODE 1.
     */
    static const uint8_t packet[] = {
        0x30, 0x0a, 0x80, 0x01, 0x01, 0xa1, 0x05, 0x30, 0x03, 0x80, 0x01, 0x07
    };
    Message_t *decoded = NULL;
    asn_dec_rval_t rv;

    rv = ber_decode(0, &asn_DEF_Message, (void **)&decoded,
                    packet, sizeof(packet));
    assert(rv.code == RC_OK);
    assert(decoded);
    assert(decoded->procedureCode == 1);
    assert(decoded->msgValue.present == msgValue_PR_PayloadSeq_1);
    assert(decoded->msgValue.choice.PayloadSeq_1.n == 7);
    ASN_STRUCT_FREE(asn_DEF_Message, decoded);
}

static void
fill_message(Message_t *msg) {
    memset(msg, 0, sizeof(*msg));
    msg->procedureCode = 1;
    msg->msgValue.present = msgValue_PR_PayloadSeq_1;
    msg->msgValue.choice.PayloadSeq_1.n = 7;
}

static void
check_decoded_message(const Message_t *decoded) {
    assert(decoded);
    assert(decoded->procedureCode == 1);
    assert(decoded->msgValue.present == msgValue_PR_PayloadSeq_1);
    assert(decoded->msgValue.choice.PayloadSeq_1.n == 7);
}

static void
roundtrip_message_syntax(enum asn_transfer_syntax syntax, const char *name) {
    Message_t source;
    Message_t *decoded = NULL;
    asn_encode_to_new_buffer_result_t encoded;
    asn_dec_rval_t rv;

    fill_message(&source);

    encoded = asn_encode_to_new_buffer(NULL, syntax, &asn_DEF_Message, &source);
    assert(encoded.result.encoded > 0);
    assert(encoded.buffer);

    rv = asn_decode(NULL, syntax, &asn_DEF_Message, (void **)&decoded,
                    encoded.buffer, (size_t)encoded.result.encoded);
    assert(rv.code == RC_OK);
    check_decoded_message(decoded);

    ASN_STRUCT_FREE(asn_DEF_Message, decoded);
    free(encoded.buffer);
    ASN_STRUCT_RESET(asn_DEF_Message, &source);
    printf("  OK: Message OPEN TYPE round-trip via %s\n", name);
}

static void
roundtrip_message_all_syntaxes(void) {
    roundtrip_message_syntax(ATS_BER, "BER");
    roundtrip_message_syntax(ATS_DER, "DER");
    roundtrip_message_syntax(ATS_BASIC_OER, "BASIC-OER");
    roundtrip_message_syntax(ATS_CANONICAL_OER, "CANONICAL-OER");
    roundtrip_message_syntax(ATS_UNALIGNED_BASIC_PER, "BASIC-UPER");
    roundtrip_message_syntax(ATS_UNALIGNED_CANONICAL_PER, "CANONICAL-UPER");
    roundtrip_message_syntax(ATS_ALIGNED_BASIC_PER, "BASIC-APER");
    roundtrip_message_syntax(ATS_ALIGNED_CANONICAL_PER, "CANONICAL-APER");
    roundtrip_message_syntax(ATS_BASIC_XER, "BASIC-XER");
    roundtrip_message_syntax(ATS_CANONICAL_XER, "CANONICAL-XER");
    roundtrip_message_syntax(ATS_JER, "JER");
    roundtrip_message_syntax(ATS_JER_MINIFIED, "JER-MINIFIED");
    roundtrip_message_syntax(ATS_CBOR, "CBOR");
}

static void
print_message_plaintext(void) {
    Message_t source;
    asn_encode_to_new_buffer_result_t encoded;

    fill_message(&source);

    encoded = asn_encode_to_new_buffer(NULL, ATS_NONSTANDARD_PLAINTEXT,
                                       &asn_DEF_Message, &source);
    assert(encoded.result.encoded > 0);
    assert(encoded.buffer);

    free(encoded.buffer);
    ASN_STRUCT_RESET(asn_DEF_Message, &source);
    printf("  OK: Message OPEN TYPE plaintext print\n");
}

int
main(void) {
    /*
     * id 0 selects an information object set row with no &Type.  The decoder
     * must fail cleanly instead of dereferencing a NULL type descriptor.
     */
    static const uint8_t typeless_ber[] = {
        0x30, 0x07, 0x80, 0x01, 0x00, 0xa1, 0x02, 0x30, 0x00
    };

    /*
     * id 1 selects the later row that does define Payload.  The generated
     * selector must use the compact OPEN TYPE CHOICE presence index.
     */
    static const uint8_t defined_ber[] = {
        0x30, 0x08, 0x80, 0x01, 0x01, 0xa1, 0x03, 0x01, 0x01, 0xff
    };

    /*
     * The inner BOOLEAN is truncated.  Failure cleanup must reset the OPEN
     * TYPE wrapper, not size that reset from the selected primitive type.
     */
    static const uint8_t truncated_ber[] = {
        0x30, 0x80, 0x80, 0x01, 0x01, 0x81
    };

    decode_ber_once(typeless_ber, sizeof(typeless_ber), 0);
    decode_ber_once(defined_ber, sizeof(defined_ber), 1);
    decode_ber_once(truncated_ber, sizeof(truncated_ber), 0);
    decode_oer_valid_and_truncated();
    decode_message_with_undefined_key_row();
    roundtrip_message_all_syntaxes();
    print_message_plaintext();

    printf("OK: OPEN TYPE typeless row regressions decoded without crashing.\n");
    return 0;
}
