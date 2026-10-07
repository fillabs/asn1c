/*
 * Regression test for APER coding of an extensible NativeEnumerated
 * (skeletons/NativeEnumerated_aper.c), parallel to check-PER-NativeEnumerated.c
 * which tests the UPER variant.
 *
 * An unknown APER extension value must:
 *   - decode successfully (X.680 §6 forbids failing) so an enclosing type
 *     keeps parsing its subsequent fields (forward compatibility);
 *   - be stored as LONG_MAX - wire_index, derived from the raw wire ordinal
 *     (before the root-count offset is applied), distinct per index and
 *     never aliasing any value known to this older decoder (even sparse maps);
 *   - re-encode byte-for-byte identically (relay).
 *
 * APER-specific trap: the decoder historically applied the root-count offset
 * *before* the unknown-extension check, so the marker was derived from the
 * post-offset value.  A non-zero root count with wire ordinal 0 pins this
 * down: base (root=2, extension=3) receives nsnnwn=0; the old code computed
 * 0+(3-1)=2 then stored value2enum[0].nat_value, the new code stores LONG_MAX.
 *
 * Golden bytes for the small-index cases are identical to the UPER golden
 * bytes (extension bit = 1, nsnnwn short-form = 0, 6-bit ordinal):
 *   index  0: 0x80
 *   index  1: 0x81
 *   index  3: 0x83
 *   index 63: 0xBF
 * For index 64 and 65535 APER inserts an alignment pad after the long-form
 * indicator bit, producing multi-byte wire sequences:
 *   index  64: 0xC0 0x01 0x40
 *   index 65535: 0xC0 0x02 0xFF 0xFF
 */
#undef NDEBUG
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#include <asn_application.h>
#include <NativeEnumerated.h>
#include <per_support.h>
#include <aper_support.h>

/*
 * base:   E ::= ENUMERATED { ee1(0), ee2(1), ... }
 * (root count 2, so specs->extension = 3, map_count = 2)
 */
static const asn_INTEGER_enum_map_t base_value2enum[] = {
    { 0, 3, "ee1" },
    { 1, 3, "ee2" }
};
static const unsigned int base_enum2value[] = { 0, 1 };
static const asn_INTEGER_specifics_t base_specs = {
    base_value2enum, base_enum2value, 2, 3, 1, 0, 0
};

/*
 * full:   E ::= ENUMERATED { ee1(0), ee2(1), ..., ee3(3), ee4(4), ee5(5), ee6(6) }
 * A newer version that knows the extension additions the base version does not.
 */
static const asn_INTEGER_enum_map_t full_value2enum[] = {
    { 0, 3, "ee1" }, { 1, 3, "ee2" }, { 3, 3, "ee3" },
    { 4, 3, "ee4" }, { 5, 3, "ee5" }, { 6, 3, "ee6" }
};
static const unsigned int full_enum2value[] = { 0, 1, 2, 3, 4, 5 };
static const asn_INTEGER_specifics_t full_specs = {
    full_value2enum, full_enum2value, 6, 3, 1, 0, 0
};

/*
 * sparse: S ::= ENUMERATED { a(0), b(2), ... }
 * The unknown-extension placeholder must not alias the known b(2).
 */
static const asn_INTEGER_enum_map_t sparse_value2enum[] = {
    { 0, 1, "a" },
    { 2, 1, "b" }
};
static const unsigned int sparse_enum2value[] = { 0, 1 };
static const asn_INTEGER_specifics_t sparse_specs = {
    sparse_value2enum, sparse_enum2value, 2, 3, 1, 0, 0
};

/*
 * noext:  N ::= ENUMERATED { x(0), y(1) }  -- NOT extensible.
 */
static const asn_INTEGER_enum_map_t noext_value2enum[] = {
    { 0, 1, "x" },
    { 1, 1, "y" }
};
static const unsigned int noext_enum2value[] = { 0, 1 };
static const asn_INTEGER_specifics_t noext_specs = {
    noext_value2enum, noext_enum2value, 2, 0, 1, 0, 0
};

static const asn_per_constraints_t ext_constraints = {
    { APC_CONSTRAINED | APC_EXTENSIBLE, 1, 1, 0, 1 }, /* value (root index) */
    { APC_UNCONSTRAINED, -1, -1, 0, 0 },
    0, 0
};
static const asn_per_constraints_t noext_constraints = {
    { APC_CONSTRAINED, 1, 1, 0, 1 },
    { APC_UNCONSTRAINED, -1, -1, 0, 0 },
    0, 0
};

static long
decode_aper(int lineno, const asn_INTEGER_specifics_t *specs,
            const asn_per_constraints_t *ct,
            enum asn_dec_rval_code_e expected_code,
            const void *bytes, size_t nbytes) {
    asn_TYPE_descriptor_t td = asn_DEF_NativeEnumerated;
    asn_per_data_t pd;
    asn_dec_rval_t rv;
    long value = -1;
    long *value_ptr = &value;

    td.specifics = specs;
    memset(&pd, 0, sizeof(pd));
    pd.buffer = (const uint8_t *)bytes;
    pd.nboff = 0;
    pd.nbits = 8 * nbytes;

    rv = NativeEnumerated_decode_aper(NULL, &td, ct, (void **)&value_ptr, &pd);
    fprintf(stderr, "%d: aper decode [%02x] => code %d, value %ld\n",
            lineno, *(const uint8_t *)bytes, (int)rv.code, value);
    assert(rv.code == expected_code);
    return value;
}

/*
 * Encode "value" and, on success, return the number of whole bytes produced,
 * copying them into out[].  Returns -1 on ENCODE_FAILED.
 */
static ssize_t
encode_aper(const asn_INTEGER_specifics_t *specs,
            const asn_per_constraints_t *ct, long value, uint8_t *out) {
    asn_TYPE_descriptor_t td = asn_DEF_NativeEnumerated;
    asn_per_outp_t po;
    asn_enc_rval_t er;
    size_t nbytes;

    td.specifics = specs;
    memset(&po, 0, sizeof(po));
    po.buffer = po.tmpspace;
    po.nbits = 8 * sizeof(po.tmpspace);

    er = NativeEnumerated_encode_aper(&td, ct, &value, &po);
    if(er.encoded < 0) return -1;
    /* Normalize: flush complete bytes that are still in nboff. */
    if(po.nboff >= 8) {
        po.buffer += (po.nboff >> 3);
        po.nboff   &= 0x07;
    }
    nbytes = (po.buffer - po.tmpspace) + (po.nboff ? 1 : 0);
    if(out) memcpy(out, po.tmpspace, nbytes);
    return (ssize_t)nbytes;
}

/* Decode "wire" (nbytes bytes), then re-encode; assert byte-identical round trip. */
static long
relay_ok(int lineno, const asn_INTEGER_specifics_t *specs,
         const uint8_t *wire, size_t nbytes, long expect_value) {
    uint8_t out[8];
    ssize_t n;
    long v = decode_aper(lineno, specs, &ext_constraints, RC_OK, wire, nbytes);
    assert(v == expect_value);
    n = encode_aper(specs, &ext_constraints, v, out);
    fprintf(stderr, "%d: relay [%02x...] => value %ld => re-encode %zd bytes [%02x...]\n",
            lineno, wire[0], v, n, n > 0 ? out[0] : 0);
    assert(n == (ssize_t)nbytes);
    assert(memcmp(out, wire, (size_t)n) == 0);
    return v;
}

int
main(void) {
    long value;
    uint8_t out[8];
    ssize_t n;

    /* ------- Known root values: decode and re-encode unchanged. ------- */
    value = decode_aper(__LINE__, &base_specs, &ext_constraints, RC_OK, "\x00", 1);
    assert(value == 0);                                 /* ee1 */
    value = decode_aper(__LINE__, &base_specs, &ext_constraints, RC_OK, "\x40", 1);
    assert(value == 1);                                 /* ee2 */
    n = encode_aper(&base_specs, &ext_constraints, 0, out);
    assert(n == 1 && out[0] == 0x00);
    n = encode_aper(&base_specs, &ext_constraints, 1, out);
    assert(n == 1 && out[0] == 0x40);

    /*
     * ------- Unknown extension values: lossless relay. -------
     * A newer peer's ee3/ee4/ee6 arrive as extension indices 0/1/3/63.
     * They must decode (RC_OK) into LONG_MAX-index (distinct per index,
     * no aliasing) and re-encode to the identical bytes.
     *
     * Small-index APER wire format (one byte each):
     *   ext_bit=1 | nsnnwn_short=0 | 6-bit ordinal
     *   index  0: 1_0_000000 = 0x80
     *   index  1: 1_0_000001 = 0x81
     *   index  3: 1_0_000011 = 0x83
     *   index 63: 1_0_111111 = 0xBF
     */
    {
        static const uint8_t w0[] = { 0x80 };
        static const uint8_t w1[] = { 0x81 };
        static const uint8_t w3[] = { 0x83 };
        static const uint8_t w63[] = { 0xBF };
        relay_ok(__LINE__, &base_specs, w0, 1, LONG_MAX);       /* index 0 */
        relay_ok(__LINE__, &base_specs, w1, 1, LONG_MAX - 1);   /* index 1 */
        relay_ok(__LINE__, &base_specs, w3, 1, LONG_MAX - 3);   /* index 3 */
        relay_ok(__LINE__, &base_specs, w63, 1, LONG_MAX - 63); /* index 63 */
    }

    /*
     * APER long-form nsnnwn: for ordinals >= 64 the encoder emits an alignment
     * pad after the long-form indicator bit.
     *
     * index 64 (3 bytes): ext=1 | long=1 | align(6 bits=0) | 0|0000001 | 01000000
     *   = 11000000 | 00000001 | 01000000 = 0xC0 0x01 0x40
     * index 65535 (4 bytes): same preamble, len=2, value 0xFFFF
     *   = 0xC0 0x02 0xFF 0xFF
     */
    {
        static const uint8_t w64[]    = { 0xC0, 0x01, 0x40 };
        static const uint8_t w65535[] = { 0xC0, 0x02, 0xFF, 0xFF };
        relay_ok(__LINE__, &base_specs, w64,    3, LONG_MAX - 64);
        relay_ok(__LINE__, &base_specs, w65535, 4, LONG_MAX - 65535);
    }

    /* The stored value is recognised as an unknown-extension placeholder. */
    assert(ASN_NATIVE_ENUMERATED_IS_UNKNOWN_EXT(LONG_MAX));
    assert(ASN_NATIVE_ENUMERATED_IS_UNKNOWN_EXT(LONG_MAX - 63));
    assert(ASN_NATIVE_ENUMERATED_IS_UNKNOWN_EXT(LONG_MAX - 65535));
    assert(!ASN_NATIVE_ENUMERATED_IS_UNKNOWN_EXT(0));
    assert(!ASN_NATIVE_ENUMERATED_IS_UNKNOWN_EXT(LONG_MAX - 70000));

    /*
     * ------- APER-specific trap: raw wire ordinal, not post-offset. -------
     * base (root=2, extension=3) receives nsnnwn=0 (wire 0x80).  The old
     * code computed 0+(3-1)=2 (post-offset), checked 2>=map_count(2) and
     * stored value2enum[0].nat_value = 0, aliasing ee1.  The fixed code uses
     * the raw wire ordinal 0 and stores LONG_MAX-0 = LONG_MAX.
     */
    {
        static const uint8_t w[] = { 0x80 };
        long v = decode_aper(__LINE__, &base_specs, &ext_constraints, RC_OK, w, 1);
        assert(v == LONG_MAX);       /* must not alias ee1(0) */
        assert(v != 0);              /* the aliasing value the old code produced */
    }

    /*
     * ------- Sparse enumeration { a(0), b(2), ... }: no aliasing. -------
     * The known b(2) still decodes to 2; an unknown extension (ordinal 0
     * that would collide with b under an ordinal scheme) must become
     * LONG_MAX and relay back to 0x80.
     */
    {
        static const uint8_t w_b[]  = { 0x40 };
        static const uint8_t w_u0[] = { 0x80 };
        value = decode_aper(__LINE__, &sparse_specs, &ext_constraints, RC_OK, w_b, 1);
        assert(value == 2);                                 /* known b(2) */
        value = relay_ok(__LINE__, &sparse_specs, w_u0, 1, LONG_MAX);
        assert(value != 2);                                 /* must not alias b(2) */
    }

    /*
     * ------- Newer version knows the additions: byte-exact encoding. -------
     * full encodes ee1..ee6 to 00/40/80/81/82/83 and decodes 0x81 to the
     * real value 4 (a known extension now, not unknown).
     */
    {
        static const long vals[6]          = { 0, 1, 3, 4, 5, 6 };
        static const unsigned char gold[6] = { 0x00, 0x40, 0x80, 0x81, 0x82, 0x83 };
        int i;
        for(i = 0; i < 6; i++) {
            n = encode_aper(&full_specs, &ext_constraints, vals[i], out);
            fprintf(stderr, "full encode %ld => %zd byte 0x%02x (want 0x%02x)\n",
                    vals[i], n, n > 0 ? out[0] : 0, gold[i]);
            assert(n == 1 && out[0] == gold[i]);
        }
        static const uint8_t w81[] = { 0x81 };
        value = decode_aper(__LINE__, &full_specs, &ext_constraints, RC_OK, w81, 1);
        assert(value == 4);                             /* known ee4 */
    }

    /*
     * ------- Cross-version relay. -------
     * A value the base version could not understand (0x83) is stored by
     * base, then handed to the full version's encoder.  Even though the
     * full version knows extension index 3 as ee6, the relay path replays
     * the wire ordinal, producing the identical 0x83.
     */
    {
        static const uint8_t w[] = { 0x83 };
        value = decode_aper(__LINE__, &base_specs, &ext_constraints, RC_OK, w, 1);
        assert(value == LONG_MAX - 3);
        n = encode_aper(&full_specs, &ext_constraints, value, out);
        fprintf(stderr, "cross-version relay: base stored %ld => full encodes %zd byte 0x%02x\n",
                value, n, n > 0 ? out[0] : 0);
        assert(n == 1 && out[0] == 0x83);
    }

    /*
     * ------- Negative cases. -------
     * A value below the reserved region (ordinal would exceed 65535) is not
     * a valid relay placeholder and must fail to encode; a reserved-region
     * value handed to a NON-extensible enumeration must also fail.
     */
    assert(encode_aper(&base_specs, &ext_constraints, LONG_MAX - 70000, out) == -1);
    assert(encode_aper(&noext_specs, &noext_constraints, LONG_MAX, out) == -1);
    /* A plain unknown integer (not a placeholder, not a known value) fails. */
    assert(encode_aper(&base_specs, &ext_constraints, 42, out) == -1);

    printf("Finished OK\n");
    return 0;
}
