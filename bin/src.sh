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


install="$1"

opts=( "${@:2}" )

. "${pwd}/bin/scripts/load.sh"

## Finds targets by file relative path/name and executes them
## Usage: ${script} <option> [optional]
## ${commands}

while (($#)); do
    case "$1" in

        "--all") # builds and runs all targets
            source="all"
            ;;

        "--target="*) # builds and runs specified target
            source=${1#*=}
            opts=( "${@:2}" )
            ;;

        "--dir="*) # [optional] build directory
            dir=${1#*=}
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

        "--valgrind") # [optional] runs using valgrind
            valgrind="--valgrind"
            ;;

        "--callgrind") # [optional] runs using valgrind with tool callgrind
            callgrind="--callgrind"
            ;;

        "--debug") # [optional] runs using debug messaging
            debug="--debug"
            ;;

        "--help") # [optional] shows command description
            help
            ;;

        *)
            help
            ;;

    esac
    shift
done

if [[ "${install}" == "" ]]; then
    help
    exit;
fi

if [[ "${silent}" == "--silent" ]]; then
    exec 2>&1 >/dev/null
fi

build=( "build/coverage-v1" "build/coverage-v2" "build/coverage-v3" "build/coverage-v4" "build/coverage-v5" "build/coverage-v6" )

if [[ ! "${dir}" == "" ]]; then
    build="${dir}"
fi

if [[ "${clean}" == "--clean" ]]; then
    if [[ -d "${dir}" ]]; then
        rm -rf "${dir}"
    fi
fi

targets=( $(get-source-targets ${source}) )

if [[ "${targets[@]}" == "" ]]; then
    if [[ "${help}" == "--help" ]]; then
        help
    fi
    echo no source targets: ${source} >&2
    exit 8
fi

for target in ${targets[@]}; do
    find "${pwd}" -type f -name "${target}-*.info" -delete
done

[[ ! -d "${pwd}/coverage" ]] && mkdir "${pwd}/coverage"

if [[ -f "${pwd}/coverage/lcov.info" ]]; then 
    rm "${pwd}/coverage/lcov.info"
fi

for linked_target in ${targets[@]}; do
    case ${linked_target} in
        c-*) ;& main-*) ;& test-*)
            ;;
        *)
            ${pwd}/bin/build.sh --target=${linked_target} ${opts[@]}
            ;;
    esac
done

for linked_target in ${targets[@]}; do
    case ${linked_target} in
        c-*) ;& main-*) ;& test-*)
            if [[ " ${targets[*]} " == *" ${linked_target} "* ]]; then
                ${pwd}/bin/coverage.sh --target=${linked_target} ${opts[@]}
            fi
            ;;
        *)
            ;;
    esac
done

files=$(find "${pwd}/coverage" -type f -name "*.info" -exec echo {} \;)
if [[ ! "${files[@]}" == "" ]]; then
    find "${pwd}/coverage" -type f -name "*.info" -exec echo -a {} \; | xargs lcov -o "${pwd}/coverage/lcov.info"
fi

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

if [[ "${silent}" == "--silent" ]]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
