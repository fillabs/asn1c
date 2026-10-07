# Issue #552: ENUMERATED typedefs and PER constraints

Issue [#552](https://github.com/vlm/asn1c/issues/552) was reported by
[@zhouvlia](https://github.com/zhouvlia). The report identified a generated
descriptor for an `ENUMERATED` typedef whose UPER/APER constraint pointer was
`NULL`. `NativeEnumerated_decode_uper()` then had no `range_bits` information
when decoding an alias field.

## Resolution

`emit_type_DEF()` now resolves the terminal ASN.1 type once and uses that type
when deciding whether to attach generated OER, UPER, and APER constraint
tables. This applies the same inheritance rule to aliases of `ENUMERATED`,
`CHOICE`, and known-multiplier character-string types.

The exact `ENUMERATED` failure was not reproducible at this checkout's
pre-change `HEAD`: an earlier enum-specific condition already wired the PER
slot for that alias. The issue nevertheless identified the invariant that
the descriptor must satisfy, and this change replaces the special cases with
one terminal-type rule while adding a regression that prevents the reported
failure from returning.

The regression schema
`tests/tests-asn1c-compiler/552-enum-typedef-per-OK.asn1` models the minimal
report. Its C test verifies that the alias descriptor points at its generated
PER table, decodes the `Container { value e2 }` vector through both UPER and
APER, and rejects a zero-length truncated input without retaining a partial
object.

The report labels its one-byte reproduction vector `0x40`, but that byte has a
zero optional-field presence bit for this schema. The regression uses `0xc0`,
where the presence bit is `1`, the two-bit `ENUMERATED` ordinal for `e2` is
`10`, and the remaining bits are padding.

Run the focused regression from `tests/tests-c-compiler` with:

```sh
srcdir=. abs_top_srcdir=/Users/ur20980/src/asn1c \
abs_top_builddir=/Users/ur20980/src/asn1c \
sh check-assembly.sh check-src/check-552.-gen-UPER.-gen-APER.c
```
