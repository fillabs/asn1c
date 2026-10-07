# ILP32 build and cross-build plan

## Summary

The reported `90-cond-int-type-OK.asn1` failure should be treated as two
separate problems:

1. A real undefined-behavior bug in the compiler's integer-bound emission.
2. A missing target ABI model for generated-code tests and output templates.

The immediate fix should be the undefined-behavior bug. It is small, low risk,
and should make the `ACV_UINT` output stable across a 32-bit asn1c build and a
64-bit asn1c build.

The target ABI model should be added separately as a code-generation option,
for example `-flong-size=32|64`, and then used to make intentional ILP32 versus
LP64 storage decisions explicit in tests.

The plan must support two valid workflows:

- cross-generating ASN.1 output for an ILP32 target while the `asn1c` compiler
  itself runs on a different host, usually LP64;
- building and running the `asn1c` compiler itself on an ILP32 platform.

## Severity

Severity: high for portability and release confidence, medium for shipped
runtime behavior, low as a security issue.

Reasons:

- The compiler executes undefined behavior when `asn1c_integer_t` is a signed
  64-bit type and code computes `((asn1c_integer_t)INT64_MAX << 1) + 1`.
- The undefined behavior is host-build dependent. A 32-bit compiler build
  without `HAVE_128_BIT_INT` can emit different generated C than a normal
  64-bit compiler build.
- The failure blocks `--enable-test-32bit` style confidence checks and makes
  generated-output templates sensitive to how the compiler binary itself was
  built.
- The specific `ACV_INTEGER_BYTES` fallback for `3000000000` may still be
  semantically usable by the generated constraint checker, but it is not the
  intended representation because the value fits in `uintmax_t`.
- There is no evidence that this is a memory-safety or exploitable runtime
  vulnerability. The main damage is incorrect or unstable code generation.

## Current evidence in this tree

- `libasn1parser/asn1p_integer.h` defines `asn1c_integer_t` as `__int128` when
  `HAVE_128_BIT_INT` is available, otherwise as `intmax_t`.
- `libasn1compiler/asn1c_constraint.c` computes `U64MAX` with a signed left
  shift in `emit_cval_bound()`.
- `libasn1compiler/asn1c_misc.c` has the same signed-shift pattern in
  `asn1c_select_integer_storage()`.
- `tests/tests-asn1c-compiler/90-cond-int-type-OK.asn1.+-P` already expects
  bounds such as `3000000000` to be emitted as `ACV_UINT` with `UINTMAX_C(...)`.
- `configure.ac` already has `--enable-test-32bit`, but that checks and uses
  `-m32` for tests. That is useful for generated-code/runtime checks, but it is
  not the same coverage as building the `asn1c` compiler binary itself as an
  ILP32 executable.

## Decision

Proceed with both changes, but do them in order:

1. Fix the undefined behavior first.
2. Add the explicit target-long-size model second.

The first change is required for both workflows: an ILP32-built compiler must
not execute signed-overflow UB, and an LP64-built compiler must be able to
generate the same portable constraint metadata for an ILP32 target.

Do not split the `90-cond-int-type-OK.asn1.+-P` template just to hide the
`ACV_UINT` versus `ACV_INTEGER_BYTES` difference. After the undefined-behavior
fix, `3000000000` should produce the same `ACV_UINT` representation on both
32-bit and 64-bit compiler builds.

Template variants make sense only for intentional target ABI differences,
especially choices like `long`, `unsigned long`, fixed-width native integer
storage, or `INTEGER_t` under `-flong-size=32` versus `-flong-size=64`.

## Phase 1: fix the undefined behavior

### Goal

Make integer-bound classification independent of whether `asn1c_integer_t` is
`__int128` or `intmax_t`.

### Implementation steps

1. In `libasn1compiler/asn1c_constraint.c`, replace the signed-shift `U64MAX`
   calculation in `emit_cval_bound()`.

2. Express the actual decision being made:

   - negative values require `ACV_INTEGER_BYTES` only when they do not fit in
     `intmax_t`;
   - non-negative values require `ACV_INTEGER_BYTES` only when they do not fit
     in `uintmax_t`;
   - when `asn1c_integer_t == intmax_t`, every representable negative value
     already fits in `intmax_t`, and every representable non-negative value
     fits in `uintmax_t`.

3. Use preprocessor branching rather than a runtime expression that still
   constructs an unrepresentable signed value:

   ```c
   #ifdef HAVE_128_BIT_INT
       const asn1c_integer_t UMAX = (asn1c_integer_t)UINTMAX_MAX;
       const asn1c_integer_t IMIN = -(asn1c_integer_t)INTMAX_MAX - 1;
       use_bytes = (v < 0) ? (v < IMIN) : (v > UMAX);
   #else
       use_bytes = 0;
   #endif
   ```

   The exact patch can keep local style, but it must not shift a signed value
   into or past the sign bit. If the local code keeps the ternary structure,
   the no-`__int128` branch can spell the same conclusion as
   `(v < 0) ? 0 : 0`; the important point is that every representable
   `intmax_t` value already fits either `intmax_t` or `uintmax_t`.

4. In `libasn1compiler/asn1c_misc.c`, replace the `U64MAX` calculation in
   `asn1c_select_integer_storage()`.

5. For `AINT_NATIVE_UINT64`, use equivalent logic:

   - with `HAVE_128_BIT_INT`, compare `hi` against `(asn1c_integer_t)UINT64_MAX`;
   - without `HAVE_128_BIT_INT`, any non-negative representable `hi` fits in
     `uint64_t`, so the upper-bound check is true by construction.

6. Search the compiler code for the same signed-shift idiom and fix any direct
   duplicate in the touched code path. Keep a broader parser integer-limit audit
   as a follow-up unless UBSan or tests show it is part of this failure.

### Tests for phase 1

Run normal generated-output checks first:

```sh
make -j12
make -C tests/tests-asn1c-compiler check
```

Run the integer-native focused tests:

```sh
tests/tests-integer-native/run.sh
```

Run a host compiler build with 32-bit generated-code/runtime tests enabled:

```sh
./configure --enable-test-32bit
make -j12
make check -j12
```

Run a compiler-on-ILP32 build, either on a native ILP32 machine or with a local
`-m32` compiler setup that can build and run 32-bit executables:

```sh
CC="gcc -m32" ./configure --enable-test-32bit
make -j12
make -C tests/tests-asn1c-compiler check
```

This validates that the `asn1c` executable itself can be built ILP32 and no
longer changes the `ACV_UINT` lines for `3000000000`.

### Acceptance criteria for phase 1

- `90-cond-int-type-OK.asn1.+-P` remains unchanged for the `ACV_UINT` bounds.
- An ILP32-built asn1c no longer emits `ACV_INTEGER_BYTES` for `3000000000`.
- An LP64-built asn1c and an ILP32-built asn1c emit the same `ACV_UINT`
  constraint metadata for values that fit `uintmax_t`.
- UBSan does not report signed overflow from the fixed expressions.
- No template split is needed for the reported `ACV_UINT` issue.

## Phase 2: add explicit target long-size modelling

### Goal

Make target C storage decisions explicit while preserving native ILP32 builds.
The compiler should be able to generate for an ILP32 or LP64 target while
running as a different host executable, and it should also build and run
correctly when the host executable itself is ILP32.

### Recommended user-facing option

Add:

```text
-flong-size=32
-flong-size=64
```

Default should preserve today's behavior for normal users.  The safest default is
an auto/native policy that resolves to the build host's `sizeof(long)` when that
is known, but never relies on UB. For reproducible generated-output tests and
cross-generation, use explicit `-flong-size=32` or `-flong-size=64`.

### Implementation steps

1. Add a new global target-long-size policy near the existing
   `asn1c_integer_native_type` policy in `libasn1compiler/asn1compiler.h` and
   `libasn1compiler/asn1compiler.c`.

   Suggested shape:

   ```c
   typedef enum asn_target_long_size_e {
       ASN_TARGET_LONG_AUTO = 0,
       ASN_TARGET_LONG_32 = 32,
       ASN_TARGET_LONG_64 = 64
   } asn_target_long_size_e;

   extern asn_target_long_size_e asn1c_target_long_size;
   ```

2. Parse `-flong-size=32` and `-flong-size=64` in `asn1c/asn1c.c`.

3. Reject other values with a clear error:

   ```text
   -flong-size expects one of: 32, 64
   ```

4. Document the option in `usage()` next to `-fwide-types` and
   `-finteger-native-type`.

5. Resolve `ASN_TARGET_LONG_AUTO` once, using configure-time knowledge of
   `sizeof(long)` or a compile-time fallback. A native ILP32 compiler build
   should therefore default to 32-bit `long`; a native LP64 compiler build
   should default to 64-bit `long` only if that matches existing compatibility
   expectations. If preserving old generated output is more important, keep auto
   conservative by default and require `-flong-size=64` for LP64-specific
   output.

6. Refactor `asn1c_type_fits_long()` in `libasn1compiler/asn1c_misc.c` so the
   target limits are not hard-coded as `RIGHTMAX` and `LEFTMIN`.

7. Use target limits derived from the policy:

   - `-flong-size=32`: signed range `INT32_MIN..INT32_MAX`, unsigned range
     `0..UINT32_MAX`;
   - `-flong-size=64`: signed range `INT64_MIN..INT64_MAX`, unsigned range
     `0..UINT64_MAX`.

8. Keep all comparisons UB-free:

   - with `HAVE_128_BIT_INT`, represent all 64-bit signed and unsigned limits
     as `asn1c_integer_t`;
   - without `HAVE_128_BIT_INT`, do not construct `UINT64_MAX` as a signed
     `asn1c_integer_t`; treat all non-negative representable values as within
     unsigned 64-bit range.

9. Decide and document signedness priority for LP64. The natural rule is:

   - if a range fits signed target `long`, return `FL_FITS_SIGNED`;
   - otherwise, if it is non-negative and fits target `unsigned long`, return
     `FL_FITS_UNSIGN`;
   - otherwise return `FL_NOTFIT` or `FL_PRESUMED` according to existing
     `-fwide-types` behavior.

10. Make sure `-fwide-types` still means "prefer `INTEGER_t` when the target
   native type cannot be proven appropriate", not "pretend target long is
   wider".

### Tests for phase 2

1. Add option parsing coverage, preferably near
   `tests/tests-integer-native/run.sh` or the compiler smoke tests:

   ```sh
   asn1c -flong-size=32 -E tests/tests-asn1c-compiler/90-cond-int-type-OK.asn1
   asn1c -flong-size=64 -E tests/tests-asn1c-compiler/90-cond-int-type-OK.asn1
   asn1c -flong-size=bogus -E tests/tests-asn1c-compiler/90-cond-int-type-OK.asn1
   ```

2. Add generated-output templates only where storage choices intentionally
   differ. Candidate templates:

   - `90-cond-int-type-OK.asn1.+-P_-flong-size=32`
   - `90-cond-int-type-OK.asn1.+-P_-flong-size=64`

3. Keep `ACV_UINT` expectations identical between the templates for
   `3000000000`; only native storage choices should differ.

4. Add a small dedicated ASN.1 fixture if `90-cond-int-type-OK.asn1` is too
   noisy. It should contain ranges that distinguish these cases:

   - `-2147483648..2147483647`
   - `0..4294967295`
   - `3000000000..3000000001`
   - `-3000000000..3000000000`

5. Run cross-generation and native-host tests:

   ```sh
   # LP64 or normal host, generating ILP32-targeted output explicitly.
   make -j12
   ./asn1c/asn1c -S skeletons -P -flong-size=32 \
       tests/tests-asn1c-compiler/90-cond-int-type-OK.asn1

   # Normal host with generated-code/runtime ILP32 tests.
   ./configure --enable-test-32bit
   make -j12
   make check -j12

   # Compiler-on-ILP32 validation, on native ILP32 or an -m32-capable host.
   CC="gcc -m32" ./configure --enable-test-32bit
   make -j12
   make check -j12

   tests/tests-integer-native/run.sh
   ```

### Acceptance criteria for phase 2

- A 64-bit host asn1c can intentionally generate ILP32-style output with
  `-flong-size=32`.
- A 64-bit host asn1c can intentionally generate LP64-style output with
  `-flong-size=64`.
- An ILP32-built asn1c compiler builds, runs, and passes generated-output tests
  without needing `__int128`.
- Generated-output templates no longer depend on whether the asn1c executable
  itself was compiled with `-m32`.
- `--enable-test-32bit` tests generated code and skeleton/runtime behavior in
  32-bit mode, while the separate compiler-on-ILP32 build validates the compiler
  binary itself.

## Build guidance after the fix

Preferred cross-generation workflow from an LP64 or normal host:

```sh
./configure --enable-test-32bit
make -j12
make check -j12
./asn1c/asn1c -flong-size=32 -S skeletons ...
```

Preferred native/compiler-on-ILP32 workflow:

```sh
CC="gcc -m32" ./configure --enable-test-32bit
CFLAGS="-m32" make -j12
CFLAGS="-m32" make check -j12
```

The workflows answer different questions. The first proves that an arbitrary
host can generate ILP32-targeted code. The second proves that `asn1c` itself is
portable enough to build and run on ILP32.

## Risks and mitigations

- Risk: changing integer-bound classification could alter generated output for
  oversized values.
  Mitigation: the change should only affect values that fit `intmax_t` or
  `uintmax_t`; truly oversized values still use `ACV_INTEGER_BYTES`.

- Risk: `-flong-size=64` may reveal currently hidden LP64 expectations in old
  templates.
  Mitigation: add template variants only for explicit `-flong-size` tests and
  keep default output backward-compatible.

- Risk: no-`__int128` builds cannot represent ASN.1 constants above
  `INTMAX_MAX` in `asn1c_integer_t`.
  Mitigation: do not promise more than the current parser can represent in
  phase 1. Phase 2 should avoid creating unrepresentable signed constants, but
  broader arbitrary-precision parser support is a separate project.

## Final recommendation

Fix the undefined behavior immediately. Then add `-flong-size=32|64` as an
explicit target model for cross-generation and keep ILP32 compiler builds in the
validation matrix. Do not split templates for the `ACV_UINT` issue itself.
