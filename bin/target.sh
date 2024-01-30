#!/usr/bin/env bash
if [[ "${BASHOPTS}" != *extdebug* ]]; then
    set -e
fi

err_report() {
    cd ${source}
    echo "ERROR: $0:$*"
    exit 8
}

if [[ "${BASHOPTS}" != *extdebug* ]]; then
    trap 'err_report $LINENO' ERR
fi

source=$(pwd)

pwd=$(cd "$(dirname $(dirname "${BASH_SOURCE[0]}"))" &> /dev/null && pwd)

cd "${pwd}"

export LCOV_PATH=$(which lcov)
export GENHTML_PATH==$(which genhtml)
export MAKEFLAGS=-j8

logs="${pwd}/logs"
build="${pwd}/build"
target="$1"

if [[ "${target}" == "" ]]; then
    echo script needs a target name to build
    exit
fi

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
