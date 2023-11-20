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

uid=$(id -u)

if [ "${uid}" -eq 0 ]; then
    echo "Please run as user"
    exit
fi

install="$1"

opts=( "${@:2}" )

. "${pwd}/bin/scripts/load.sh"

## Builds binaries and creates coverage info
## Usage: ${script} <option> [optional]
## ${commands}


while (($#)); do
    case "$1" in

        "")
            source="all"
            ;;

        "--target") # builds and runs specified target
            shift
            source="$1"
            opts=( "${@:2}" )
            ;;

        "--all") # builds and runs specified target
            source="all"
            ;;

        "--silent") # [optional] suppress verbose output
            silent="--silent"
            ;;

        "--help") # [optional] shows command description
            help
            ;;

        "--clean") # [optional] cleans up directories before coverage
            clean="--clean"
            ;;

        *)
            help
            ;;
    esac
    shift
done

build="${pwd}/coverage"

if [[ "${clean}" == "--clean" ]]; then
    if [[ -d "${pwd}/coverage" ]]; then
        find "${pwd}/coverage" -type f -name "lcov.info" -delete
    fi

    coverage=( "*.gcda" "*.gcno" "*.s" "*.i" "*.o" "*.info" )

    for f in ${coverage[@]}; do
        if [[ -d "${build}" ]]; then
            find "${build}" -type f -name "callgrind.out.*" -delete
            find "${build}" -type f -name "*.s" -delete
            find "${build}" -type f -name "${f}" -delete
        fi
    done

    targets=( $(get-source-targets ${source}) )

    if [[ "${targets[@]}" == "" ]]; then
        if [[ "${help}" == "--help" ]]; then
            help
        fi
        echo no source targets: ${source} >&2
        exit 8
    fi    

    for target in ${targets[@]}; do
        if [[ -f "${pwd}/coverage/${target}.info" ]]; then
            rm "${pwd}/coverage/${target}.info"
        fi
    done
fi

"${pwd}/bin/coverage.sh" --target ${source} --index 1 ${silent} ${opts[@]}
"${pwd}/bin/coverage.sh" --target ${source} --index 2 ${silent} ${opts[@]}
"${pwd}/bin/coverage.sh" --target ${source} --index 3 ${silent} ${opts[@]}
"${pwd}/bin/coverage.sh" --target ${source} --index 4 ${silent} ${opts[@]}
"${pwd}/bin/coverage.sh" --target ${source} --index 5 ${silent} ${opts[@]}
"${pwd}/bin/coverage.sh" --target ${source} --index 6 ${silent} ${opts[@]}
"${pwd}/bin/coverage.sh" --target ${source} --coverage ${silent} ${opts[@]}

files=$(find "${pwd}/coverage" -type f -name "*.info" -exec echo {} \;)
if [[ ! "${files[@]}" == "" ]]; then
    find "${pwd}/coverage" -type f -name "*.info" -exec echo -a {} \; | xargs lcov -o "${pwd}/coverage/lcov.info"
fi

find "${build}" -type f -not -name "lcov.info" -exec rm {} \;

if [[ "${silent}" == "--silent" ]]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
