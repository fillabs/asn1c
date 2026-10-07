#!/usr/bin/env bash
set -Eeuo pipefail

# Delete generated dependency files under tests/.
#
# Usage:
#   scripts/clean-tests-deps.sh
#   scripts/clean-tests-deps.sh --dry-run

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="${ROOT:-$(git -C "${SCRIPT_DIR}" rev-parse --show-toplevel)}"
TESTS_DIR="${ROOT}/tests"

case "${1:-}" in
    "")
        dry_run=0
        ;;
    --dry-run|-n)
        dry_run=1
        ;;
    *)
        echo "Usage: $0 [--dry-run|-n]" >&2
        exit 2
        ;;
esac

if [[ ! -d "${TESTS_DIR}" ]]; then
    echo "Missing tests directory: ${TESTS_DIR}" >&2
    exit 2
fi

if [[ "${dry_run}" -eq 1 ]]; then
    find "${TESTS_DIR}" -type f -name '*.d' -print
    exit 0
fi

count="$(find "${TESTS_DIR}" -type f -name '*.d' -print | wc -l | tr -d '[:space:]')"
find "${TESTS_DIR}" -type f -name '*.d' -delete
echo "Deleted ${count} generated dependency file(s) under ${TESTS_DIR}."
