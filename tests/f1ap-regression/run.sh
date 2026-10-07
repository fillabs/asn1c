#!/usr/bin/env bash
set -euo pipefail

# Test for F1AP compilation regression (issue #410)
# This test ensures that F1AP code generated with -findirect-choice and 
# -fcompound-names compiles successfully without circular include errors

# For distcheck: srcdir points to source directory, current dir is build directory
# For normal check: srcdir=. and we're in the source directory
srcdir="${srcdir:-.}"
abs_top_builddir="${abs_top_builddir:-$(cd ../.. && pwd)}"
abs_top_srcdir="${abs_top_srcdir:-$(cd ../.. && pwd)}"

# Copy source files to current directory if not already present
if [ ! -f F1AP-16.7.0.asn ]; then
  cp -p "${srcdir}/F1AP-16.7.0.asn" .
fi

ASN1C_EXE="${abs_top_builddir}/asn1c/asn1c"
SKELETONS_DIR="${abs_top_srcdir}/skeletons"

echo "Testing F1AP code generation and compilation..."
echo "srcdir=${srcdir} abs_top_builddir=${abs_top_builddir} abs_top_srcdir=${abs_top_srcdir} pwd=${PWD}"

# Generate code with the same flags that exposed the issue
${ASN1C_EXE} -S "${SKELETONS_DIR}" \
  -fcompound-names \
  -findirect-choice \
  F1AP-16.7.0.asn

# Test compilation of everything.
# F1AP generates a very large number of C files; build them in parallel
# (portable CPU count detection: Linux nproc, BSD/macOS sysctl, fallback 1).
NPROC=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 1)
echo "Attempt to build converter-example (with -j${NPROC})"
make -j"${NPROC}" -f converter-example.mk

echo "F1AP test PASSED: Code generated and compiled successfully"

# Test CBOR conversion with OPEN TYPE members (regression for CBOR OPEN TYPE bug)
echo "Testing CBOR conversion with OPEN TYPE members..."

# Use mktemp to avoid /tmp name collisions during concurrent test runs
TMPDIR="${TMPDIR:-/tmp}"
f1ap_xer=$(mktemp "${TMPDIR}/f1ap-test-XXXXXX.xer")
f1ap_cbor=$(mktemp "${TMPDIR}/f1ap-test-XXXXXX.cbor")
f1ap_err=$(mktemp "${TMPDIR}/f1ap-test-XXXXXX.err")
trap 'rm -f "${f1ap_xer}" "${f1ap_cbor}" "${f1ap_err}"' EXIT

cat > "${f1ap_xer}" << 'XEOF'
<F1AP-PDU>
 <initiatingMessage>
  <procedureCode>1</procedureCode>
  <criticality><reject/></criticality>
  <value>
   <F1SetupRequest>
    <protocolIEs>
     <F1SetupRequestIEs>
      <id>78</id>
      <criticality><reject/></criticality>
      <value>
       <TransactionID>1</TransactionID>
      </value>
     </F1SetupRequestIEs>
    </protocolIEs>
   </F1SetupRequest>
  </value>
 </initiatingMessage>
</F1AP-PDU>
XEOF

if ! ./converter-example -p F1AP-PDU -ixer -ocbor "${f1ap_xer}" > "${f1ap_cbor}" 2>"${f1ap_err}"; then
  echo "FAILED: CBOR conversion of F1AP message with OPEN TYPE failed"
  cat "${f1ap_err}" >&2
  exit 1
fi

if [ ! -s "${f1ap_cbor}" ]; then
  echo "FAILED: CBOR output is empty"
  exit 1
fi

echo "CBOR conversion test PASSED: F1AP-PDU with OPEN TYPE encoded to CBOR successfully"
