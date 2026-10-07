#!/usr/bin/env bash

set -x
set -e
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

cleanup() {
    rm -rf *.[acho] Makefile.am.* *.mk *.txt *.asn
    rm -f converter-example
}

print_state() {
    local err=$?
    set +x
    set +e
    trap "" EXIT ERR
    echo "Error $err while processing:"
    cat test.asn
    cat status.txt
    echo "FAILED"
    exit $err
}

verify() {
    local type="$1"
    local flags="$2"

    cleanup

    rm -rf "test-${type}"
    mkdir "test-${type}"
    cd "test-${type}"
    asncmd="${ASN1C} -fall-defs-global -Wdebug-compiler -flink-skeletons -S ${SKELETONS_DIR} $flags test.asn"

    {
    echo "$asncmd"
    echo "${MAKE:-make} -f converter-example.mk"
    } > status.txt

    echo "Module DEFINITIONS::=BEGIN T::=$type END" > test.asn
    $asncmd
    CFLAGS=-O0 ${MAKE:-make} -f converter-example.mk | tail -10
    cd ..
}

verify_type_with_variants() {
    local type="$1"
    for flags in "-no-gen-UPER" "-no-gen-APER" "-no-gen-OER" "-no-gen-XER" "-no-gen-JER" "-no-gen-BER" "-no-gen-print" "-no-gen-random-fill" "-no-gen-UPER -no-gen-APER -no-gen-OER -no-gen-XER -no-gen-JER -no-gen-BER -no-gen-print -no-gen-random-fill" ""; do
#    for flags in "-no-gen-UPER" "-no-gen-APER" "-no-gen-OER" "-no-gen-UPER -no-gen-APER -no-gen-OER" ""; do
        for native in "" "-fwide-types"; do
            verify "$type" "$flags $native"
        done
    done
}

verify_compile_and_link_variants() {
    for type in INTEGER "INTEGER(0..1)" "ENUMERATED{foo}" NULL BOOLEAN \
                "BIT STRING" \
                "OBJECT IDENTIFIER" "RELATIVE-OID" \
                "SEQUENCE{f INTEGER}" \
                "CHOICE{f INTEGER}" \
                "OCTET STRING" IA5String "IA5String(SIZE(1))" UTF8String \
                REAL "SET OF INTEGER" "SEQUENCE OF INTEGER"; do
        verify_type_with_variants "$type"
    done
}

trap print_state EXIT ERR
if [ "x$*" = "x" ]; then
    verify_compile_and_link_variants
else
    for type in "$@"; do
        verify_type_with_variants "$type"
    done
fi
set +x
trap '' EXIT ERR

cleanup
echo "OK"
