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

opts=( "${@:1}" )

. "${pwd}/bin/scripts/load.sh"

## Builds binaries
## Usage: ${script} <option> [optional]
## ${commands}

for opt in "${opts[@]}"; do
    case "${opt}" in

        "")
            clean="--clean"
            sanitize="--sanitize"
            mocks="--mocks"
            gc="--gc"
            silent="--silent"
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

        *)
            help
            ;;

    esac
done

if [ "${silent}" == "--silent" ]; then
    exec 2>&1 >/dev/null
fi

[ ! -d "${pwd}/build" ] && mkdir "${pwd}/build"

if [ "${clean}" == "--clean" ]; then
    rm -rf "${pwd}/build"
    mkdir "${pwd}/build"
fi

if [ "${sanitize}" == "--sanitize" ] && [ "${valgrind}" != "--valgrind" ]; then
    SANITIZER_OPTIONS=--sanitize
else
    SANITIZER_OPTIONS=
fi

if [ "${mocks}" == "--mocks" ]; then
    MOCKS_OPTIONS=-DMOCKS:BOOL=TRUE
else
    MOCKS_OPTIONS=--mocks
fi

if [ "${gc}" == "--gc" ]; then
    GC_OPTIONS=--gc
else
    GC_OPTIONS=
fi

if [ "${valgrind}" == "--valgrind" ]; then
    VALGRIND_OPTIONS=--valgrind
else
    VALGRIND_OPTIONS=
fi

OPTIONS=$(echo "${MOCKS_OPTIONS} ${GC_OPTIONS} ${SANITIZER_OPTIONS} ${VALGRIND_OPTIONS}")

"${pwd}/bin/build.sh" --all ${OPTIONS}
"${pwd}/bin/coverage.sh" --all ${OPTIONS}
"${pwd}/bin/logs.sh" --all ${OPTIONS}

if [ "${silent}" == "--silent" ]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"