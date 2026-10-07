#!/usr/bin/env bash
set -ex

basepath=$(dirname "$0")
#type=$(basename "$0" | rev | cut -c4- | rev | cut -c7- | tr _ " ")
type="INTEGER(0..1)"

"${basepath}/check-asn1c-smoke.sh" "${type}"
