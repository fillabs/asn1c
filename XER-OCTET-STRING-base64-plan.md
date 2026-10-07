# Plan: Standards-compliant XER hex/Base64 handling for OCTET STRING (#538)

Goal: default XER encoding of `OCTET STRING` reverts to **hex** (`xmlhstring`) per
X.680 §22.3 / X.693; Base64 remains available **opt-in** (per-type via
`ENCODING-CONTROL XER`, globally via a new `XER_F_BASE64` encoder flag).
Decoder is liberal: accepts hex (any case, with whitespace), Base64, and
nonstandard `H'...'` prefix; logs deviations via `ASN_DEBUG`; the `0x` prefix
heuristic is removed (collides with valid Base64, see #538).

## Standards basis (verify before coding — Phase 0)

Verified directly:
- X.693 (2001) §7.3: decoders claiming XER conformance **shall support all
  alternative encodings** → liberal decoding is itself conformant.
- X.693 (2001) §9.4 (CXER): octetstring uses `xmlhstring` "with all white-space
  removed, and all letters in upper-case".
- RFC 4648 §3.3: non-alphabet characters MUST be rejected unless the referring
  spec says otherwise; §12: ignoring them is a covert-channel risk. We accept
  only XML white-space inside Base64 (and log it), reject everything else.

Cited in #538 but **not yet verified against full text — verify in Phase 0**
(free ITU download: itu.int/rec/T-REC-X.680-X.693-202102-I):
- [ ] X.680 (2021) §22.3: `XMLOctetStringValue ::= XMLTypedValue | xmlhstring`
      (no Base64 alternative without encoding instruction).
- [ ] X.680 §12.x `xmlhstring` lexical item: confirm whether lower-case letters
      and internal white-space are permitted in BASIC-XER. (Plan assumes yes
      for decoding-liberality; encoder emits upper-case contiguous regardless,
      which is safe under either reading.)
- [ ] X.693 (2021) §21.1.3–21.1.4: `[BASE64]` is EXTENDED-XER only, opt-in.
- [ ] X.693 (2021) BASE64 instruction specifics (needed for Phase 6): exact
      assignment syntax in a type prefix (`[BASE64]`, `[XER:BASE64]`) per
      X.680 (2021) §31 `EncodingPrefixedType`, and in an `ENCODING-CONTROL
      XER` section (X.680 §54); which types it may target (OCTET STRING only,
      or also open types); whether `GLOBAL-DEFAULTS MODIFIED-ENCODINGS` is a
      prerequisite; whether canonical EXTENDED-XER keeps Base64 for
      `[BASE64]` types (plan assumes yes — instruction defines *the* encoding
      of that type, unlike our nonstandard global flag).

If any check contradicts the plan, stop and revisit.

## Capability check (done)

asn1c already has everything required; no new infrastructure beyond one flag
bit and one state field:
- Hex encoder `OCTET_STRING_encode_xer`, Base64 encoder/decoder, auto-detecting
  decoder, `H'...'` parsing — all in `skeletons/OCTET_STRING_xer.c`.
- Per-value streaming decode state `_xer_decode_state` in `OCTET_STRING_t`.
- `ENCODING-CONTROL XER ... ::= hexadecimal|base64|utf8` parsed
  (libasn1parser) and acted on (libasn1compiler/asn1c_C.c
  `emit_custom_xer_{encoder,decoder}`); compiler test
  `tests/tests-asn1c-compiler/175-encoding-control-body-OK.asn1`.
- Test harness: `tests/tests-skeletons/check-XER-base64.c`,
  `check-OCTET_STRING.c`, `check-XER.c`, tests-c-compiler fixtures,
  tests-randomized round-trips.
- Limitation (accepted): no runtime warning channel besides `ASN_DEBUG`.

## Phase 1 — Encoder (skeletons)

1. `xer_encoder.h`: add `XER_F_BASE64 = 0x04` to `enum xer_encoder_flags_e`.
   Document: applies to OCTET STRING only; **ignored when `XER_F_CANONICAL`
   is set** (CXER must stay hex); flags propagate to members automatically.
2. `OCTET_STRING_xer.c` / `OCTET_STRING_encode_xer`:
   - At entry: `if((flags & XER_F_BASE64) && !(flags & XER_F_CANONICAL))
     return OCTET_STRING_encode_xer_base64(...);`
   - Make the non-canonical path emit **contiguous upper-case hex** (delete the
     per-byte `0x20` separator and 16-byte line-break logic; canonical path is
     already correct). Keep `ASN__TEXT_INDENT` only around the value, not
     inside it.
3. `OCTET_STRING.c` op table: restore `OCTET_STRING_encode_xer` as the XER
   encoder (replacing `OCTET_STRING_encode_xer_base64`). Keep
   `OCTET_STRING_decode_xer_auto` as decoder. Fix the incorrect comment
   "Per X.693, Base64 is the default" (it is not).
4. `OCTET_STRING_encode_xer_base64`: keep, but in non-canonical mode do NOT
   insert 76-char line breaks *with indentation* inside the value unless
   verified harmless to other decoders; simplest conformant-liberal choice:
   emit contiguous Base64 in both modes (white-space inside the value is what
   X.693 conformance of *other* decoders is least likely to accept).

## Phase 2 — Decoder (skeletons/OCTET_STRING_xer.c)

5. Rewrite `OCTET_STRING__is_hexadecimal` → `OCTET_STRING__classify(buf,len)`
   returning HEX / BASE64 / AMBIGUOUS_PREFER_HEX:
   - **Delete the `0x`/`0X` prefix heuristic** (core #538 fix; `0x6B` is valid
     Base64 for `D3 1E 81`).
   - HEX if content is only `[0-9A-Fa-f]` + XML white-space **and** digit count
     is even. Lower-case or internal white-space → accept, `ASN_DEBUG` note.
   - BASE64 if it contains any of `[G-Zg-z+/=]` (minus a–f) — chars impossible
     in hex.
   - Ambiguous (pure hex alphabet, even count) → **hex** (the standard default
     encoding; reverses the current Base64 preference).
   - Odd hex-digit count but valid Base64 alphabet → Base64, `ASN_DEBUG` note.
   - Neither valid → return error (RC_FAIL); unambiguous decode impossible.
6. Chunked-input safety: detection currently runs per chunk. Add
   `int format_decided;` (or reuse a spare field) inside `_xer_decode_state`;
   classify on the first non-white-space chunk and pin the converter for all
   subsequent chunks of the same value. Audit `have_more` semantics in
   `xer_decode_general` first and document them at the classifier.
7. Keep `H'...'`/`h'...'` explicit-prefix handling in
   `OCTET_STRING__convert_auto` (liberal, unambiguous: `'` is invalid in both
   formats); add `ASN_DEBUG("nonstandard H'' prefix accepted")`.
8. Base64 converter: keep RFC 4648 strictness (reject non-alphabet, reject
   data after `=`). Additionally at value end: if `bits_collected` leaves
   non-zero residual bits or padding is missing, accept but `ASN_DEBUG` the
   deviation (liberal). Reject residual bits that are non-zero *and* would
   change the value ambiguously? No — residual <8 bits are simply dropped
   today; keep, log.
9. BIT STRING `binary_or_hex` path: out of scope except adding `ASN_DEBUG`
   notes; do not change behavior.

## Phase 3 — Compiler (libasn1compiler/asn1c_C.c)

10. `type_needs_custom_xer_encoder`: flip — custom encoder now needed for
    `EC_XER_BASE64` and `EC_XER_UTF8`; `EC_XER_HEXADECIMAL`/`EC_NONE` use the
    (hex) default.
11. `emit_custom_xer_encoder`: `EC_XER_BASE64` → emit a thin wrapper calling
    `OCTET_STRING_encode_xer_base64`; delete the hand-rolled hexbuf code
    (hex is default now).
12. `emit_custom_xer_decoder`: `EC_XER_BASE64` → `OCTET_STRING_decode_xer_base64`
    (NOT auto: a Base64 value like "ABCD" is also plausible hex; pinning the
    decoder removes the ambiguity for these types). `EC_XER_UTF8` →
    `..._decode_xer_utf8`. Hex/default → `..._decode_xer_auto`.
13. Document (README/ChangeLog): `XER_F_BASE64` is a debug/size convenience;
    for ambiguous values its output round-trips only through this library's
    liberal decoder when content is non-ambiguous — use `[BASE64]` (Phase 6)
    or `ENCODING-CONTROL ... ::= base64` when machine round-trips are
    required.

## Phase 4 — Tests

Update to conformance (current expectations contradict the new — standard —
preference):
14. `tests/tests-skeletons/check-XER-base64.c`:
    - Any auto-decode case relying on "ambiguous → Base64" must flip:
      `<tag>AAEA</tag>` now decodes as hex `{0xAA,0xEA}`. Audit every
      `OCTET_STRING_decode_xer_auto` call.
    - Keep all explicit `OCTET_STRING_{en,de}code_xer_base64` tests unchanged
      (incl. whitespace, invalid-char rejection, chunked decode, H' tests).
    - Fix the wrong "Base64 is the default per X.693" comments.
15. `tests/tests-skeletons/check-OCTET_STRING.c`: verify decoder-selection
    table still matches (explicit decoders unchanged → likely no edit).
16. `tests/tests-asn1c-compiler/175-encoding-control-body-OK.asn1.+-P`:
    regenerate expected compiler output (encoder/decoder emission flipped).

New tests:
17. In `check-XER-base64.c` (or new `check-XER-octstr-auto.c`, registered in
    `tests-skeletons/Makefile.am`):
    a. Default `xer_encode(&asn_DEF_OCTET_STRING, ...)` with `XER_F_BASIC` and
       `XER_F_CANONICAL` emits contiguous upper-case hex; round-trips via
       `_decode_xer_auto`.
    b. `XER_F_BASIC|XER_F_BASE64` emits Base64; `XER_F_CANONICAL|XER_F_BASE64`
       emits hex (canonical wins).
    c. Regression for #538: auto-decode `<t>0x6B</t>` → Base64 → `D3 1E 81`
       (no 0x heuristic); auto-decode `<t>0X12</t>` → Base64, not hex.
    d. Liberal hex: lower-case (`<t>aabb</t>` → `AA BB`), internal/leading
       white-space, `H'aAbB'`.
    e. Odd-length pure-hex-alphabet (`<t>ABC</t>`) → decoded as Base64.
    f. Garbage (`<t>P!Q</t>`) → RC_FAIL.
    g. Chunked auto-decode: split an ambiguous hex body across chunks; format
       pinned by first chunk (exercises step 6).
18. tests-c-compiler: new check-src module with
    `ENCODING-CONTROL XER ... ::= base64` type: BER→XER→BER round-trip of
    `{0xD3,0x1E,0x81}` and of an all-hex-alphabet Base64 output (e.g. value
    whose Base64 is "ABCD") — must survive because the generated decoder is
    pinned to Base64 (step 12).
19. Re-run `make check` end-to-end: tests-skeletons, tests-c-compiler
    (XER fixtures such as `data-202/s1.xer` use bare upper-case hex — these
    now decode as hex, as the standard requires), tests-randomized
    (`xer_equivalent` round-trips with the hex default), tests-asn1c-compiler.

## Phase 6 — Standard X.693 (2021) `[BASE64]` encoding instruction

Adopt the standard syntax as a first-class signal, mapping onto the
**existing** `expr->encoding_control.encoding_type = EC_XER_BASE64` path so
Phases 1–3 code generation is reused unchanged. Subject to the Phase 0
verification items above.

Parser (libasn1parser):
20. Lexer (`asn1p_l.l`): recognize `BASE64` (and `GLOBAL-DEFAULTS`,
    `MODIFIED-ENCODINGS`) as tokens valid inside encoding prefixes and the
    `ENCODING-CONTROL XER` section; outside those contexts they stay ordinary
    references (no new reserved words leaking into general ASN.1).
21. Grammar (`asn1p_y.y`):
    a. Type prefix: extend the bracketed-prefix production to accept
       `'[' [encodingreference ':'] BASE64 ']'` →
       `Signature ::= [BASE64] OCTET STRING` and `[XER:BASE64]`. No
       grammar collision with tags expected: a tag is class-keyword/number,
       never a bare capitalized word — verify with `bison -Wconflicts-sr`
       (zero new conflicts is the acceptance bar).
    b. Bare `[BASE64]` (no `XER:` qualifier) is honored only if the module
       header's encoding reference allows it (`DEFINITIONS XER INSTRUCTIONS`)
       — parse the `XER INSTRUCTIONS` module header; if the header is absent,
       accept `[XER:BASE64]` and warn on bare `[BASE64]` (liberal parse,
       reported deviation — consistent with the decoding philosophy).
    c. Standard encoding-control form: inside `ENCODING-CONTROL XER ... END`
       accept `BASE64 TargetList` where TargetList is comma-separated
       `Typereference` or `Typereference.componentId` — emit one
       TM_ENCODING_INSTRUCTION expr per target (same struct the legacy
       custom syntax produces). Accept and record `GLOBAL-DEFAULTS
       MODIFIED-ENCODINGS` (no-op beyond validation) so real-world
       EXTENDED-XER modules parse.
    d. Keep the legacy `Name OCTET STRING ::= base64` body syntax for
       backward compatibility; document it as deprecated in favor of (a)/(c).
22. Semantic check (asn1fix or asn1c_C.c): `[BASE64]` applied to anything
    other than an OCTET STRING(-derived) type → hard compile error with
    file:line (per X.693 applicability rules verified in Phase 0). Conflicting
    instructions for one type (e.g. `[BASE64]` prefix + `::= hexadecimal`
    control) → error, not silent precedence.
23. Codegen: no new work — `EC_XER_BASE64` already selects
    `OCTET_STRING_encode_xer_base64` + pinned `OCTET_STRING_decode_xer_base64`
    (steps 10–12). Per Phase 0 assumption, `[BASE64]` types emit Base64 in
    canonical mode too (contiguous, no white-space) — unlike the `XER_F_BASE64`
    flag, which canonical overrides; implement the distinction in the emitted
    encoder (instruction-driven encoder ignores `XER_F_CANONICAL`'s
    format-selection effect but honors its white-space rules).

Phase 6 tests:
24. tests-asn1c-compiler (parser level):
    a. `NNN-base64-prefix-OK.asn1` + expected `.+-P` output: module with
       `XER INSTRUCTIONS` header, `[BASE64] OCTET STRING` type,
       `[XER:BASE64]` variant, and a SEQUENCE member of that type.
    b. `NNN-base64-control-OK.asn1`: standard `ENCODING-CONTROL XER`
       section with `BASE64 Type1, Type2.field` targets +
       `GLOBAL-DEFAULTS MODIFIED-ENCODINGS`.
    c. `NNN-base64-misapplied-SE.asn1`: `[BASE64] INTEGER` and a
       conflicting-instruction case — both must fail compilation with a
       diagnostic (negative tests).
    d. Regenerate `175-encoding-control-body-OK` expectations; legacy syntax
       must keep producing identical `encoding_control` state to the new
       syntax (assert by comparing `-P` dumps of equivalent modules).
25. tests-c-compiler (behavior level), new check-src module:
    a. Encode: a `[BASE64]` OCTET STRING value emits Base64 in BASIC-XER
       **and** CANONICAL-XER; a sibling un-instructed OCTET STRING in the
       same PDU emits hex in both — single PDU exercising both paths.
    b. Decode: pinned Base64 decoder accepts the all-hex-alphabet ambiguous
       value (Base64 "ABCD" → `00 10 83`), proving schema knowledge beats
       content heuristics; liberal acceptance of white-space inside the
       Base64 body with `ASN_DEBUG` note.
    c. Round-trips: BER→XER→BER and XER→BER→XER equality for `{0xD3,0x1E,
       0x81}` and 1KB+ PQ-key-sized values; `xer_equivalent()` over
       random-filled PDUs containing the `[BASE64]` member.
    d. Cross-syntax equivalence: same schema expressed via `[BASE64]` prefix,
       standard `ENCODING-CONTROL BASE64 T`, and legacy `T OCTET STRING ::=
       base64` produce byte-identical XER for the same value.
26. Interop fixture: commit a hand-written XER file (as a third-party
    implementation would emit per X.693 §21: plain Base64 content, no
    prefixes) and assert decode equality against known bytes.

## Phase 7 — Verification

27. Build both autoconf and CMake paths; run full `make check`.
28. Grep for stale comments claiming Base64 is the XER default; fix all.
29. Manual interop spot-check: feed a known third-party XER sample (bare hex
    OCTET STRING) through `converter-example`/`asn1convert` and confirm
    decode + re-encode equality; repeat with a `[BASE64]`-instructed schema.
30. Valgrind the new/changed tests (chunked decoder state, REALLOC paths,
    parser error-recovery paths from step 24c).

## Order & risk

Execute phases in order; 1–2 are independent of 3 and can be tested via
tests-skeletons alone; 6 depends on 3 (reuses EC_XER_BASE64 codegen) and on
the Phase 0 syntax verification. Biggest behavioral change is step 5's
preference flip: any consumer that relied on ambiguous pure-hex-alphabet
input being Base64 will see different decodes — this is the deliberate,
standard-mandated fix. Phase 6 grammar work carries parser-regression risk:
the zero-new-bison-conflicts bar (step 21a) and the full
tests-asn1c-compiler suite guard it.
