#!/bin/sh

# Regression test for the asn1c exit status (see asn1c(1), EXIT STATUS).
# A FATAL diagnostic shall always give a non-zero exit status.

set -eu

top_srcdir=$(cd "${top_srcdir:-../..}" && pwd)
top_builddir=$(cd "${top_builddir:-../..}" && pwd)

ASN1C="${top_builddir}/asn1c/asn1c"
SKELETONS="${top_srcdir}/skeletons"
T="${top_srcdir}/tests/tests-asn1c-compiler"

TMPDIR_TEST=$(mktemp -d)
trap 'rm -rf "$TMPDIR_TEST"' EXIT

failures=0

# expect_status <name> <expected-status> [--fatal|--no-fatal] <asn1c arguments...>
# With --fatal, stderr shall contain a FATAL diagnostic.
# With --no-fatal, stderr shall contain no FATAL diagnostic.
expect_status() {
    name="$1"
    expected="$2"
    shift 2
    want_fatal=any
    case "${1:-}" in
    --fatal) want_fatal=yes; shift ;;
    --no-fatal) want_fatal=no; shift ;;
    esac

    mkdir -p "$TMPDIR_TEST/$name"
    set +e
    (cd "$TMPDIR_TEST/$name" && "$ASN1C" -S "$SKELETONS" "$@") \
        >"$TMPDIR_TEST/$name.out" 2>"$TMPDIR_TEST/$name.err"
    status=$?
    set -e

    if [ "$status" -ne "$expected" ]; then
        echo "FAIL: $name: exit status $status, expected $expected"
        cat "$TMPDIR_TEST/$name.err" >&2
        failures=$((failures + 1))
        return 0
    fi
    if [ "$want_fatal" = yes ] \
        && ! grep "^FATAL: " "$TMPDIR_TEST/$name.err" >/dev/null; then
        echo "FAIL: $name: no FATAL diagnostic on stderr"
        failures=$((failures + 1))
        return 0
    fi
    if [ "$want_fatal" = no ] \
        && grep "^FATAL: " "$TMPDIR_TEST/$name.err" >/dev/null; then
        echo "FAIL: $name: unexpected FATAL diagnostic on stderr"
        cat "$TMPDIR_TEST/$name.err" >&2
        failures=$((failures + 1))
        return 0
    fi
    echo "PASS: $name (exit status $status)"
}

OK="$T/03-enum-OK.asn1"
NP="$T/02-garbage-NP.asn1"
SE="$T/04-enum-SE.asn1"
SE_UNRETURNED="$T/102-class-ref-SE.asn1"  # fixer reports FATAL, returns success
EXPORTS_OK="$T/16-constraint-OK.asn1"     # own non-exported symbol in a constraint
CLASH="$T/72-same-names-OK.asn1"          # C name clash without -fcompound-names
PARAM="$T/165-param-class-governed-objectset-OK.asn1"

expect_status ok-parse            0  --no-fatal -E "$OK"
expect_status ok-fix              0  --no-fatal -E -F "$OK"
expect_status ok-compile          0  --no-fatal -no-gen-example "$OK"
expect_status no-input-files      64
expect_status missing-file        66 -E "$TMPDIR_TEST/does-not-exist.asn1"
expect_status syntax-error        65 -E "$NP"
expect_status semantic-error      65 --fatal -E -F "$SE"
expect_status unreturned-fatal    65 --fatal -E -F "$SE_UNRETURNED"
expect_status unexported-own-sym  0  --no-fatal -E -F "$EXPORTS_OK"
expect_status param-fix           0  --no-fatal -E -F "$PARAM"
expect_status param-compile       0  --no-fatal -no-gen-example "$PARAM"
expect_status clash-print-fatal   70 --fatal -P "$CLASH"

if [ "$failures" -ne 0 ]; then
    echo "$failures exit status check(s) failed"
    exit 1
fi
