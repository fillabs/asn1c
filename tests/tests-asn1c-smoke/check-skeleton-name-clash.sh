#!/usr/bin/env bash

# Regression test: ASN.1 types whose names match a skeleton file name
# case-insensitively (e.g. "Null" vs skeletons/NULL.[ch]) must not generate
# files that collide with the skeleton on case-insensitive filesystems
# (macOS, Windows). Previously "Null ::= NULL" generated Null.[ch], the
# NULL.[ch] skeleton was then "retained local", and compilation failed with
# "unknown type name 'NULL_t'".

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

testdir=test-skeleton-name-clash
cleanup() {
    rm -rf "$testdir"
}
trap cleanup EXIT

rm -rf "$testdir"
mkdir "$testdir"
cd "$testdir"

cat > test.asn <<'EOF'
Module DEFINITIONS ::= BEGIN
Null ::= NULL
Integer ::= INTEGER
Boolean ::= BOOLEAN
Real ::= REAL
Any ::= SEQUENCE { n Null, i Integer, b Boolean, r Real }
END
EOF

"${ASN1C}" -flink-skeletons -S "${SKELETONS_DIR}" test.asn 2>&1 | tee asn1c.log

# The skeletons must be linked in, never shadowed by generated code.
! grep -F "Retaining local" asn1c.log

for t in Null Integer Boolean Real Any; do
    test -f "asn1c_${t}.h"
    test -f "asn1c_${t}.c"
    grep -F "#include \"asn1c_${t}.h\"" "./asn1c_${t}.c"
done

# Skeleton headers must contain the skeleton, not generated code.
grep -F "NULL_t" NULL.h
grep -F "asn_OP_INTEGER" INTEGER.h
grep -F "BOOLEAN_t" BOOLEAN.h
grep -F "asn_OP_ANY" ANY.h

# References from other generated code use the disambiguated header.
grep -F '#include "asn1c_Null.h"' asn1c_Any.h

# The generated sources must compile.
CC=${CC:-cc}
for t in Null Integer Boolean Real Any; do
    ${CC} -I. -c "asn1c_${t}.c" -o "asn1c_${t}.o"
done
