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

## Finds targets by file relative path/name and executes them
## Usage: ${script} <option> [optional]
## ${commands}

case "${install}" in

    "")
        source="all"
        ;;

    "--target") # builds and runs specified target
        source="$2"
        opts=( "${@:3}" )
        ;;

    "--all") # builds and runs specified target
        source="all"
        opts=( "${@:2}" )
        ;;

    "--help") # [optional] shows command desctiption
        help
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

        "--help") # [optional] shows command desctiption
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

if [ "${clean}" == "--clean" ]; then
    rm -rf "${pwd}/coverage"
    mkdir "${pwd}/coverage"
fi

cmake=$(get-cmake)

if [[ "${cmake}" == "" ]]; then
    echo cmake not found. please run "$(pwd)/bin/utils/install.sh" --cmake
    exit 8
fi

targets=( $(get-source-targets ${source}) )

if [[ "${targets[@]}" == "" ]]; then
    if [[ "${help}" == "--help" ]]; then
        help
    fi
    echo ERROR
    exit 8
fi

find "${pwd}" -type f -name *.info -delete

[ ! -d "${pwd}/coverage" ] && mkdir "${pwd}/coverage"

if [[ -f "${pwd}/coverage/lcov.info" ]]; then 
    rm "${pwd}/coverage/lcov.info"
fi

for linked_target in ${targets[@]}; do
    case ${linked_target} in
        c-*) ;& main-*) ;& test-*)
            if [[ " ${targets[*]} " == *" ${linked_target} "* ]]; then
                ${pwd}/bin/utils/coverage.sh --target ${linked_target} ${opts[@]}
            fi
            ;;
        *)
            ${pwd}/bin/buildall.sh --target ${linked_target} ${opts[@]}
            ;;
    esac
done

find "${pwd}/coverage" -type f -name "*.info" -exec echo -a {} \; | xargs lcov -o "${pwd}/coverage/lcov.info"

for linked_target in ${targets[@]}; do
    case ${linked_target} in
        c-*) ;& main-*) ;& test-*)
            if [[ -f "${pwd}/coverage/${linked_target}.info" ]]; then
               rm "${pwd}/coverage/${linked_target}.info"
            fi
            ;;
        *)
            ;;
    esac
done

if [ "${silent}" == "--silent" ]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
