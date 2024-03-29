#!/usr/bin/env bash
if [[ "${BASHOPTS}" != *extdebug* ]]; then
    set -e
fi

export LCOV_PATH=$(which lcov)
export GENHTML_PATH==$(which genhtml)
export MAKEFLAGS=-j8
export LD_LIBRARY_PATH="$(pwd)/lib"

logs="$(pwd)/logs"
build="$(pwd)/build"
target=gtk-experiment1

[[ ! -d "${logs}" ]] && mkdir "${logs}"

cmake \
    -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
    -DCMAKE_BUILD_TYPE:STRING=Debug \
    -DCMAKE_C_COMPILER:FILEPATH=$(get-cmake-c-compiler-path) \
    -DCMAKE_CXX_COMPILER:FILEPATH=$(get-cmake-cxx-compiler-path) \
    -S"$(pwd)" \
    -B"${build}" \
    -G "Ninja" 2>&1 >/dev/null

cmake --build "${build}" --target "${target}" || (echo ERROR: "${target}" && exit 1)

timeout --foreground 180 "${build}/${target}" 2>&1 >"${logs}/log-${target}.txt" || (echo ERROR: "${target}" && exit 1)
