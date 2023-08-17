#!/usr/bin/env bash
set -e

export LCOV_PATH=$(which lcov)
export GENHTML_PATH==$(which genhtml)
export MAKEFLAGS=-j8

pwd=$(pwd)
logs="${pwd}/logs"
build="${pwd}/build"
target=c-substrings

[ ! -d "${logs}" ] && mkdir "${logs}"

export C_INCLUDE_PATH=$(readlink -m "${pwd}/../src")
export C_LIBRARY_PATH=$(readlink -m "${pwd}/../lib")

cmake \
    -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
    -DCMAKE_BUILD_TYPE:STRING=Debug \
    -DCONFIG_MEMORY_DEBUG_INFO:BOOL=TRUE \
    -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/gcc \
    -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++ \
    -S"${pwd}" \
    -B"${build}" \
    -G "Ninja" 2>&1 >/dev/null

cmake --build "${build}" --target "${target}" || (echo ERROR: "${target}" && exit 1)

timeout --foreground 180 "${build}/${target}" 2>&1 >"${logs}/log-${target}.txt" || (echo ERROR: "${target}" && exit 1)
