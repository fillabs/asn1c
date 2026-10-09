# Generated filenames colliding with skeleton files (case-insensitive FS)

## Symptom

On macOS, `tests/tests-c-compiler` failed in
`check-src/check-563.-gen-UPER.-gen-APER.c`:

```text
Retaining local NULL.h (.../skeletons/NULL.h suggested)
Retaining local NULL.c (.../skeletons/NULL.c suggested)
...
./Null.h:15:10: warning: non-portable path to file '<Null.h>'; specified path
      differs in case from file name on disk [-Wnonportable-include-path]
   15 | #include <NULL.h>
./Null.h:22:9: error: unknown type name 'NULL_t'
Null.c:20:3: error: use of undeclared identifier 'asn_OP_NULL'
Null.c:37:3: error: use of undeclared identifier 'NULL_constraint'
```

## Root cause

The test schema `563-ios-extensible-identifier-OK.asn1` defines the type
`Null ::= NULL`. asn1c named the generated files after the type: `Null.c` and
`Null.h`. The type itself needs the `NULL.c` and `NULL.h` skeleton files.

On a case-insensitive filesystem (the macOS default, and Windows),
`Null.h` and `NULL.h` are the same file. When asn1c went to symlink the
`NULL.[ch]` skeletons, it found an "existing" file and kept it ("Retaining
local"), so the skeleton was never installed. Then `#include <NULL.h>` inside
the generated `Null.h` included itself. As a result `NULL_t`, `asn_OP_NULL` and
`NULL_constraint` were never declared.

asn1c already avoided this kind of clash for libc headers (`Time.h` becomes
`asn1c_time.h`) in `asn1c_disambiguate_generated_filename()`, but that check
did not cover skeleton files.

## Fix

`asn1c_disambiguate_generated_filename()` in `libasn1compiler/asn1c_misc.c`
now also compares the generated filename, ignoring case, against every
skeleton basename that a legal ASN.1 type name could produce. ASN.1 names
cannot contain `_`, so only skeleton names without an underscore can collide:
`ANY`, `BOOLEAN`, `INTEGER`, `NULL`, `REAL`, `ENUMERATED`, `UTCTime`,
`IA5String`, and so on. A match gets the prefix `asn1c_` and keeps its
original case (`Null` becomes `asn1c_Null.[ch]`). The same function is used
for:

- the generated file names,
- `#include` directives in other generated code, and
- `Makefile.am.libasncodec`.

So all of them stay consistent. C symbol names do not change (`Null_t`,
`asn_DEF_Null`, ...). When `-fprefix` is given, the function returns early as
before, because the prefix already prevents the clash.

## Test

`tests/tests-asn1c-smoke/check-skeleton-name-clash.sh` compiles a schema with
the types `Null`, `Integer`, `Boolean`, `Real` and `Any`. It checks that:

- no "Retaining local" message appears,
- the `asn1c_*.{c,h}` files are created,
- the skeleton headers contain the real skeleton content,
- other generated headers include the disambiguated names, and
- every generated source compiles.

On case-insensitive filesystems the old code fails this test; on all systems
it also checks the new file names. `check-563` in `tests-c-compiler` also
covers the original failure.
