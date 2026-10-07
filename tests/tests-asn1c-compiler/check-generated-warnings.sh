#!/bin/sh

# Regression test for generated equality constraints and Clang's
# -Wparentheses-equality diagnostic.

set -eu

# Automake exports top_srcdir/top_builddir to tests; accept absolute variants
# as well because this script is also useful when invoked directly.
src_root="${abs_top_srcdir:-${top_srcdir:?top_srcdir must name the source tree}}"
build_root="${abs_top_builddir:-${top_builddir:?top_builddir must name the build tree}}"
compiler="${build_root}/asn1c/asn1c"
schema="${src_root}/tests/tests-asn1c-compiler/test_simple_constraint.asn1"
skeletons="${src_root}/skeletons"
tmpdir=$(mktemp -d "${TMPDIR:-/tmp}/asn1c-generated-warning.XXXXXX")

cleanup() {
    rm -rf "$tmpdir"
}

trap cleanup EXIT HUP INT TERM

# Generate a constrained OCTET STRING so the test exercises the compiler's
# generic constraint emitter, rather than a hand-written runtime skeleton.
if ! "$compiler" -S "$skeletons" -D "$tmpdir" \
    -no-gen-OER -no-gen-UPER -no-gen-APER -no-gen-JER "$schema" \
    >"$tmpdir/asn1c.log" 2>&1; then
    cat "$tmpdir/asn1c.log" >&2
    exit 1
fi

if grep -Fq 'if((size == 3UL))' "$tmpdir/TestMessage.c"; then
    echo "FAIL: generated equality constraint retains redundant parentheses" >&2
    exit 1
fi

# Compile the generated module with the diagnostic promoted to an error when
# Clang is in use; the textual assertion above keeps this test portable.
compiler_version=$(${CC:-cc} --version 2>/dev/null || true)
case "$compiler_version" in
    *[Cc]lang*)
        ${CC:-cc} -std=gnu18 -Werror=parentheses-equality \
            -I"$tmpdir" -c "$tmpdir/TestMessage.c" \
            -o "$tmpdir/TestMessage.o"
        ;;
esac

echo "PASS: generated equality constraints compile without redundant-parentheses warnings"
