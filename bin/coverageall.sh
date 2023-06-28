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

        "--debug") # [optional] runs using debug memory debug info
            debug="--debug"
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

cmake=$(get-cmake)
targets=( $(get-targets) )

default=${target}

find "${pwd}/coverage" -type f -name "*.lcov" -delete

target=${default}
if [ "${target}" == "--target" ]; then
    for target in ${targets[@]}; do
        if [ "${target}" == "$2" ]; then 
            array=( ${target} )
            break
        fi
    done
    if [ "$(echo ${array[@]})" == "" ]; then
        help
        exit 8
    fi
    targets=( ${array[@]} )
fi

export LCOV_PATH=$(which lcov)
export GENHTML_PATH==$(which genhtml)
export MAKEFLAGS=-j8

coverage=( "*.gcda" "*.gcno" "*.s" "*.i" "*.o" )
for f in ${coverage}; do
    find "${pwd}/coverage" -type f -name "${f}" -delete
done

if [ "${gc}" == "--gc" ]; then
    ${cmake} \
        -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
        -DCMAKE_BUILD_TYPE:STRING=Debug \
        -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/gcc \
        -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++ \
        -DLCOV_PATH=${LCOV_PATH} \
        -DGENHTML_PATH=${GENHTML_PATH} \
        -DCODE_COVERAGE:BOOL=TRUE \
        -DGC:BOOL=TRUE \
        $(cmake-coverage-options) \
        -S"${pwd}" \
        -B"${pwd}/coverage" \
        -G "Ninja" 2>&1 >/dev/null

    for target in ${targets[@]}; do
        echo Building target ${target}
        echo Building with options $(cmake-coverage-options) -DGC:BOOL=TRUE
        if [ "${silent}" == "--silent" ]; then
            ${cmake} --build "${pwd}/coverage" --target "${target}" 2>&1 >/dev/null || (echo ERROR: "${target}" && exit 1)
        else
            ${cmake} --build "${pwd}/coverage" --target "${target}" || (echo ERROR: "${target}" && exit 1)
        fi
        case "${target}" in main-*)    
            timeout --foreground 180 $(cmake-valgrind-options) "${pwd}/coverage/${target}" 2>&1 >"${pwd}/coverage/log-${target}.txt" || (echo ERROR: "${target}" && exit 1)
            lcov --capture --directory "${pwd}/coverage/" --output-file "${pwd}/coverage/${target}-gc.lcov" &>/dev/null
            lcov --remove "${pwd}/coverage/${target}-gc.lcov" "${pwd}/.deps/*" -o "${pwd}/coverage/${target}-gc.lcov"
        esac
    done
fi

if [ "${gc}" == "" ]; then
    ${cmake} \
        -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
        -DCMAKE_BUILD_TYPE:STRING=Debug \
        -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/gcc \
        -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++ \
        -DLCOV_PATH=${LCOV_PATH} \
        -DGENHTML_PATH=${GENHTML_PATH} \
        -DCODE_COVERAGE:BOOL=TRUE \
        -DGC:BOOL=FALSE \
        $(cmake-coverage-options) \
        -S"${pwd}" \
        -B"${pwd}/coverage" \
        -G "Ninja" 2>&1 >/dev/null

    for target in ${targets[@]}; do
        echo Building target ${target}
        echo Building with options $(cmake-coverage-options) -DGC:BOOL=FALSE
        if [ "${silent}" == "--silent" ]; then
            ${cmake} --build "${pwd}/coverage" --target "${target}" 2>&1 >/dev/null || (echo ERROR: "${target}" && exit 1)
        else
            ${cmake} --build "${pwd}/coverage" --target "${target}" || (echo ERROR: "${target}" && exit 1)
        fi
        case "${target}" in main-*)    
            timeout --foreground 180 $(cmake-valgrind-options) "${pwd}/coverage/${target}" 2>&1 >"${pwd}/coverage/log-${target}.txt" || (echo ERROR: "${target}" && exit 1)
            lcov --capture --directory "${pwd}/coverage/" --output-file "${pwd}/coverage/${target}-main.lcov" &>/dev/null
            lcov --remove "${pwd}/coverage/${target}-main.lcov" "${pwd}/.deps/*" -o "${pwd}/coverage/${target}-main.lcov"
        esac
    done
fi

find "${pwd}/coverage" -type f -name "*.lcov" -exec echo -a {} \; | xargs lcov -o "${pwd}/coverage/lcov.info"

find "${pwd}/coverage" -type f -name "*.lcov" ! -name "lcov.info" -delete

coverage=( "*.gcda" "*.gcno" "*.s" "*.i" "*.o" )
for f in ${coverage}; do
    find "${pwd}/coverage" -type f -name "${f}" -delete
done

if [ "${silent}" == "--silent" ]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
