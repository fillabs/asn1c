#!/usr/bin/env bash
set -euo pipefail

TEST_NAME="v2x_anon_of_and_oid"
WORKDIR="$(mktemp -d "${TMPDIR:-/tmp}/asn1c.${TEST_NAME}.XXXXXXXX")"
cleanup() { [[ -d "${WORKDIR}" ]] && rm -rf "${WORKDIR}"; }
trap cleanup EXIT

cp "$(dirname "$0")/data/v2x.asn" "${WORKDIR}/v2x.asn"
pushd "${WORKDIR}" >/dev/null

asn1c -fcompound-names -fincludes-quoted -fline-refs -fwide-types \
      -gen-autotools -pdu=EndApplicationMessage v2x.asn

test -f EndApplicationMessage.c || { echo "EndApplicationMessage.c missing"; exit 1; }

# Verify OID placeholder is typed (uint8_t[])
grep -Eq 'static const uint8_t .*__not_supported\\[\\] = "not supported";' EndApplicationMessage.c

autoreconf -ivf >/dev/null
./configure >/dev/null
make -j >/tmp/asn1c.${TEST_NAME}.build.log 2>&1 || {
  echo "Build failed"; tail -n 100 /tmp/asn1c.${TEST_NAME}.build.log; exit 1;
}

! grep -q 'pointer-sign' /tmp/asn1c.${TEST_NAME}.build.log || {
  echo "Unexpected -Wpointer-sign warning remains"; exit 1;
}

# Encode XER -> DER to exercise OPEN TYPE decode
cat > s1.xer <<'XML'
<?xml version="1.0" encoding="UTF-8"?>
<EndApplicationMessage xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
  <endApplication-Message-id>1.0.15784.1.0.1</endApplication-Message-id>
  <endApplication-Message-msg>
    <SEQUENCE_OF_CommTxPDU>
      <CommTxPDU><index>1</index><payload>SGVsbG8sIFYxWCBUcmFuc21pdCAx</payload></CommTxPDU>
      <CommTxPDU><index>2</index><payload>VGhpcyBpcyB0aGUgc2Vjb25kIFRYIFBEVQ==</payload></CommTxPDU>
    </SEQUENCE_OF_CommTxPDU>
  </endApplication-Message-msg>
</EndApplicationMessage>
XML

./converter-example -P -ixer -oder s1.xer > s1.der
./converter-example -r s1.der -oxer >/tmp/asn1c.${TEST_NAME}.roundtrip.xer
grep -q 'CommTxPDU' /tmp/asn1c.${TEST_NAME}.roundtrip.xer

echo "[${TEST_NAME}] PASS"
popd >/dev/null
