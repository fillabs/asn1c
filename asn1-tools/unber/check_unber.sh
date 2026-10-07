#!/bin/sh

FUZZ_TIME=${FUZZ_TIME:-10}
builddir=${builddir:-.}

if [ "x${FUZZ_ASAN_OPTIONS:-}" != "x" ]; then
    FUZZ_ASAN_ENV="ASAN_OPTIONS=${FUZZ_ASAN_OPTIONS}"
else
    FUZZ_ASAN_ENV=""
fi

env ${ASAN_ENV_FLAGS:-} ${FUZZ_ASAN_ENV} ${builddir}/check_unber \
    -timeout=3                      \
    -max_total_time=${FUZZ_TIME}    \
    -max_len=500
