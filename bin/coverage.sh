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
        exit 8
        ;;

esac

for opt in ${opts[@]}; do
    case ${opt} in

        "")
            ;;

        "--keep") # [optional] keeps coverage files and merges them
            keep="--keep"
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

        "--debug") # [optional] runs using debug memory debug info
            debug="--debug"
            ;;

        "--verbose") # [optional] shows verbose messages
            verbose="--verbose"
            ;;

        "--help") # [optional] shows command desctiption
            help
            ;;

        *)
            echo "Error: unknown argyment ${opt}"
            help
            exit 8
            ;;

    esac
done

if [ "${silent}" == "--silent" ]; then
    exec 2>&1 >/dev/null
fi


build="${pwd}/build-coverage"

if [ "${gc}" == "--gc" ]; then
    build="${build}-gc"
fi

if [ "${sanitize}" == "--sanitize" ]; then
    build="${build}-sanitize"
fi

if [ "${valgrind}" == "--valgrind" ]; then
    build="${build}-valgrind"
fi

[ ! -d "${build}" ] && mkdir "${build}"

if [ "${keep}" == "" ]; then
if [ "${clean}" == "--clean" ]; then
    rm -rf "${build}"
    mkdir "${build}"
fi
fi

cmake=$(get-cmake)
targets=( $(get-targets) )

default=${target}

target=${default}
if [ "${target}" == "--target" ]; then
    for target in ${targets[@]}; do
        if [ "${target}" == "$2" ]; then 
            array=( ${target} )
            break
        fi
    done
    if [ "$(echo ${array[@]})" == "" ]; then
        if [[ "${help}" == "--help" ]]; then
            help
        fi
        echo ERROR
        exit 8
    fi
    targets=( ${array[@]} )
fi

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
    -B"${build}" \
    -G "Ninja" 2>&1 >/dev/null

for target in ${targets[@]}; do
    if [[ "${verbose}" == "--verbose" ]]; then
        echo Building target ${target}
        echo Building with options $(cmake-options)
    fi
    if [ "${silent}" == "--silent" ]; then
        ${cmake} --build "${build}" --target "${target}" 2>&1 >/dev/null || (echo ERROR: "${target}" && exit 1)
    else
        ${cmake} --build "${build}" --target "${target}" || (echo ERROR: "${target}" && exit 1)
    fi
    case "${target}" in main-*)
        timeout --foreground 180 $(cmake-valgrind-options) "${build}/${target}" 2>&1 >"${pwd}/out/log-${target}.txt" || (echo ERROR: "${target}" && exit 1)
        lcov --capture --directory "${build}/" --output-file "${build}/${target}.lcov" &>/dev/null
        lcov --remove "${build}/${target}.lcov" "${pwd}/.deps/*" -o "${build}/${target}.lcov"
    esac
done

find "${build}" -type f -name "*.lcov" -exec echo -a {} \; | xargs lcov -o "${pwd}/coverage/lcov.info"

find "${build}" -type f -name "*.lcov" ! -name "lcov.info" -delete

if [ "${silent}" == "--silent" ]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
