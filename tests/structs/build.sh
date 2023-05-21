#!/usr/bin/env bash
set -e

err_report() {
    echo "ERROR: on line $*: $(cat $0 | sed $1!d)" >&2
}

trap 'err_report $LINENO' ERR

uid=$(id -u)

if [[ "${uid}" -eq 0 ]]; then
    echo "Please run as user"
    exit
fi

pwd=$(pwd)

cmake="${pwd}/../../.tools/cmake-3.25/bin/cmake"

${cmake} \
    -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
    -DCMAKE_BUILD_TYPE:STRING=Debug \
    -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/gcc \
    -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++ \
    -S"${pwd}/config" \
    -B"${pwd}/build" \
    -G "Ninja" 2>&1 >/dev/null

target="main-structs"

${cmake} --build "${pwd}/build" --target "${target}" || (echo ERROR: "${target}" && exit 1)
