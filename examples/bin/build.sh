#!/usr/bin/env bash
set -e


export LCOV_PATH=$(which lcov)
export GENHTML_PATH==$(which genhtml)
export MAKEFLAGS=-j8

logs="$(pwd)/logs"
build="$(pwd)/build"
target="$1"

if [[ "${target}" == "" ]]; then
    echo script needs a target name to build
    exit
fi

[[ ! -d "${logs}" ]] && mkdir "${logs}"

cmake \
    -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
    -DCMAKE_BUILD_TYPE:STRING=Debug \
    -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/gcc \
    -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++ \
    -S"$(pwd)" \
    -B"${build}" \
    -G "Ninja" 2>&1 >/dev/null

cmake --build "${build}" --target "${target}" || (echo ERROR: "${target}" && exit 1)

# timeout --foreground 180 "${build}/${target}" 2>&1 >"${logs}/log-${target}.txt" || (echo ERROR: "${target}" && exit 1)