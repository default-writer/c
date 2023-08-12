#!/usr/bin/env bash
set -e

export LCOV_PATH=$(which lcov)
export GENHTML_PATH==$(which genhtml)
export MAKEFLAGS=-j8
export LD_LIBRARY_PATH=/usr/local/lib

pwd=$(pwd)
logs="${pwd}/logs"
raylib="${pwd}/raylib"
build="${pwd}/build"
target=raylib-experiment1

[ ! -d "${raylib}" ] && git clone https://github.com/raysan5/raylib.git
[ ! -d "${logs}" ] && mkdir "${logs}"

cmake \
    -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
    -DCMAKE_BUILD_TYPE:STRING=Debug \
    -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/gcc \
    -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++ \
    -S"${pwd}" \
    -B"${build}" \
    -G "Ninja" 2>&1 >/dev/null

cmake --build "${build}" --target "${target}" || (echo ERROR: "${target}" && exit 1)

timeout --foreground 180 "${build}/${target}" 2>&1 >"${logs}/log-${target}.txt" || (echo ERROR: "${target}" && exit 1)
