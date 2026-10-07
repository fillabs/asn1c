#!/bin/sh

set -e

top_srcdir=$(cd "${top_srcdir:-../..}" && pwd)
top_builddir=$(cd "${top_builddir:-../..}" && pwd)

ASN1C="${top_builddir}/asn1c/asn1c"
SCHEMA="${top_srcdir}/tests/tests-asn1c-compiler/18-class-OK.asn1"

TMPDIR_TEST=$(mktemp -d)
trap 'rm -rf "$TMPDIR_TEST"' EXIT

run_expect_fail() {
    name="$1"
    shift

    set +e
    (cd "$TMPDIR_TEST" && "$ASN1C" -S "${top_srcdir}/skeletons" "$@" "$SCHEMA") \
        >"$TMPDIR_TEST/$name.out" 2>"$TMPDIR_TEST/$name.err"
    status=$?
    set -e

    if [ "$status" -eq 0 ]; then
        echo "FAIL: $name unexpectedly succeeded"
        cat "$TMPDIR_TEST/$name.out"
        cat "$TMPDIR_TEST/$name.err" >&2
        exit 1
    fi

    if ! grep "fno-constraints is incompatible" "$TMPDIR_TEST/$name.err" >/dev/null; then
        echo "FAIL: $name did not report the expected diagnostic"
        cat "$TMPDIR_TEST/$name.err" >&2
        exit 1
    fi

    echo "PASS: $name rejected"
}

run_expect_ok() {
    name="$1"
    shift

    (cd "$TMPDIR_TEST" && "$ASN1C" -S "${top_srcdir}/skeletons" "$@" "$SCHEMA") \
        >"$TMPDIR_TEST/$name.out" 2>"$TMPDIR_TEST/$name.err"

    echo "PASS: $name accepted"
}

run_expect_fail default-per-oer -fno-constraints
run_expect_fail explicit-oer -fno-constraints -no-gen-UPER -no-gen-APER
run_expect_fail explicit-uper -fno-constraints -no-gen-OER -no-gen-APER
run_expect_fail explicit-aper -fno-constraints -no-gen-OER -no-gen-UPER

run_expect_ok ber-xer-only -fno-constraints -no-gen-OER -no-gen-UPER -no-gen-APER
