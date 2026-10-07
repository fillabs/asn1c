#!/usr/bin/env bash

set -ex
set -o pipefail

top_builddir=${top_builddir:-../..}
top_srcdir=${top_srcdir:-../..}
srcdir=$(cd "$(dirname "$0")" && pwd)

path_from_testdir() {
    case "$1" in
        /*) printf '%s\n' "$1" ;;
        *) printf '../%s\n' "$1" ;;
    esac
}

ASN1C="$(path_from_testdir "${top_builddir}")/asn1c/asn1c"
SKELETONS_DIR="$(path_from_testdir "${top_srcdir}")/skeletons"

testdir=test-fprefix-link
cleanup() {
    rm -rf "$testdir"
}
trap cleanup EXIT

rm -rf "$testdir"
mkdir "$testdir"
cd "$testdir"

cp "${srcdir}/data/fprefix-link.asn" test.asn

for prefix in A_ B_; do
    mkdir "$prefix"
    "${ASN1C}" -S "${SKELETONS_DIR}" -flink-skeletons \
        -fprefix="$prefix" -D "$prefix" test.asn

    grep -F "asn_MBR_${prefix}Cause_" "${prefix}/${prefix}Cause.c"
    grep -F "asn_MBR_Cause_" "${prefix}/${prefix}Cause.c" && exit 1

    "${CC:-cc}" -c -I"${prefix}" -I"${SKELETONS_DIR}" \
        "${prefix}/${prefix}Cause.c" -o "${prefix}.o"
done

"${CC:-cc}" -r A_.o B_.o -o combined.o

test -f combined.o
