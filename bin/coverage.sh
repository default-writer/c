#!/usr/bin/env bash
set -e

err_report() {
    echo "ERROR: on line $*: $(cat $0 | sed $1!d)" >&2
}

trap 'err_report $LINENO' ERR

uid=$(id -u)

if [ "${uid}" -eq 0 ]; then
    echo "Please run as user"
    exit
fi

pwd=$(pwd)

install="$1"

opts=( "${@:2}" )

. "${pwd}/bin/scripts/load.sh"

## Builds binaries and creates coverage info
## Usage: ${script} <option> [optional]
## ${commands}

case "${install}" in

    "")
        ;;

    "--target") # builds and runs specified target
        opts=( "${@:3}" )
        target="--target"
        ;;

    "--all") # builds and runs all targets
        target="--all"
        ;;

    *)
        help
        ;;

esac

for opt in ${opts[@]}; do
    case ${opt} in

        "")
            ;;

        "--clean") # [optional] cleans up directories before build
            clean="--clean"
            ;;

        "--sanitize") # [optional] builds using sanitizer
            sanitize="--sanitize"
            ;;

        "--mocks") # [optional] builds with mocks
            mocks="--mocks"
            ;;

        "--gc") # [optional] builds with garbage collector
            gc="--gc"
            ;;

        "--silent") # [optional] suppress verbose output
            silent="--silent"
            ;;

        "--valgrind") # [optional] runs using valgrind (disables --sanitize on build)
            valgrind="--valgrind"
            ;;

        "--callgrind") # [optional] runs using valgrind with tool callgrind (disables --sanitize on build)
            callgrind="--callgrind"
            ;;

        "--help") # shows command desctiption
            help
            ;;

        *)
            help
            ;;

    esac
done

if [ "${silent}" == "--silent" ]; then
    exec 2>&1 >/dev/null
fi

[ ! -d "${pwd}/coverage" ] && mkdir "${pwd}/coverage"

if [ "${clean}" == "--clean" ]; then
    rm -rf "${pwd}/coverage"
    mkdir "${pwd}/coverage"
fi

for m in ${array[@]}; do
    rm -f "${pwd}/coverage/${m}.lcov"
done

cmake=$(get-cmake)
if [ "${target}" == "--all" ]; then
    array=( $(get-targets) )
else
    array=( $2 )
fi

coverage=( "*.gcda" "*.gcno" "*.s" "*.i" "*.o" )
for f in "${coverage}"; do
    find "${pwd}/coverage" -type f -name "${f}" -delete
done

export LCOV_PATH=$(which lcov)
export GENHTML_PATH==$(which genhtml)
export MAKEFLAGS=-j8

${cmake} \
    -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
    -DCMAKE_BUILD_TYPE:STRING=Debug \
    -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/gcc \
    -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++ \
    -DLCOV_PATH=${LCOV_PATH} \
    -DGENHTML_PATH=${GENHTML_PATH} \
    -DCODE_COVERAGE:BOOL=TRUE \
    $(cmake-options) \
    -S"${pwd}" \
    -B"${pwd}/coverage" \
    -G "Ninja" 2>&1 >/dev/null

for m in ${array[@]}; do
    ${cmake} --build "${pwd}/coverage" --target "${m}" 2>&1 >/dev/null || (echo ERROR: "${m}" && exit 1)
    timeout --foreground 15 $(cmake-valgrind-options) "${pwd}/coverage/${m}" 2>&1 >"${pwd}/coverage/log-${m}.txt" || (echo ERROR: "${m}" && exit 1)
    lcov --capture --directory "${pwd}/coverage/" --output-file "${pwd}/coverage/${m}.lcov" &>/dev/null
    lcov --remove "${pwd}/coverage/${m}.lcov" "${pwd}/src/rexo/*" -o "${pwd}/coverage/${m}.lcov"
done

find "${pwd}/coverage" -type f -name "*.lcov" -exec echo -a {} \; | xargs lcov -o "${pwd}/coverage/lcov.info"

for m in ${array[@]}; do
    rm -f "${pwd}/coverage/${m}.lcov"
done

if [ "${silent}" == "--silent" ]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
