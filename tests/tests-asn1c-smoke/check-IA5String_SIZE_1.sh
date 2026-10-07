#!/usr/bin/env sh
set -ex

basepath=$(dirname "$0")
#type=$(basename "$0" | rev | cut -c4- | rev | cut -c7- | tr _ " ")
type="IA5String(SIZE(1))"

"${basepath}/check-asn1c-smoke.sh" "${type}"
