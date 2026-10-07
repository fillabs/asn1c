#!/usr/bin/env bash

set -ex
set -o pipefail

top_builddir=${top_builddir:-../..}
top_srcdir=${top_srcdir:-../..}

path_from_testdir() {
    case "$1" in
        /*) printf '%s\n' "$1" ;;
        *) printf '../%s\n' "$1" ;;
    esac
}

ASN1C="$(path_from_testdir "${top_builddir}")/asn1c/asn1c"
SKELETONS_DIR="$(path_from_testdir "${top_srcdir}")/skeletons"

testdir=test-Time
cleanup() {
    rm -rf "$testdir"
}
trap cleanup EXIT

rm -rf "$testdir"
mkdir "$testdir"
cd "$testdir"

cat > test.asn <<'EOF'
Module DEFINITIONS ::= BEGIN
Validity ::= SEQUENCE {
  notBefore Time,
  notAfter Time
}
Time ::= CHOICE {
  utcTime UTCTime,
  generalTime GeneralizedTime
}
END
EOF

"${ASN1C}" -flink-skeletons \
    -S "${SKELETONS_DIR}" test.asn

test -f asn1c_time.h
test -f asn1c_time.c
test ! -f Time.h
test ! -f Time.c
grep -F '#include "asn1c_time.h"' ./asn1c_time.c
grep -R "\"asn1c_time.h\"" ./*.h
