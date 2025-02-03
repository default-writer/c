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

## Builds binaries and creates coverage info
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

        "--silent") # [optional] suppress verbose output
            silent="--silent"
            ;;

        "--clean") # [optional] cleans up directories before coverage
            clean="--clean"
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

build="${pwd}/build"

if [ ! -d "${build}" ]; then
    "${pwd}/bin/build.sh" --all
fi

if [[ "${clean}" == "--clean" ]]; then
    if [[ -d "${build}" ]]; then
        find "${build}" -type f -name "lcov.info" -delete
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
        if [[ -f "${build}/${target}.info" ]]; then
            rm "${build}/${target}.info"
        fi
    done
fi

 if [[ ! -d "${pwd}/build" ]]; then
    "${pwd}/bin/init.sh" --init
 fi 

"${pwd}/bin/coverage.sh" --target=${source} --index=1 ${opts[@]}
"${pwd}/bin/coverage.sh" --target=${source} --index=2 ${opts[@]}
"${pwd}/bin/coverage.sh" --target=${source} --index=3 ${opts[@]}
"${pwd}/bin/coverage.sh" --target=${source} --index=4 ${opts[@]}
"${pwd}/bin/coverage.sh" --target=${source} --index=5 ${opts[@]}
"${pwd}/bin/coverage.sh" --target=${source} --index=6 ${opts[@]}
"${pwd}/bin/coverage.sh" --target=${source} --coverage ${opts[@]}

[[ ! -d "${pwd}/coverage" ]] && mkdir "${pwd}/coverage"

files=$(find "${build}" -type f -name "*.info" -exec echo {} \;)
if [[ ! "${files[@]}" == "" ]]; then
    find "${build}" -type f -name "*.info" -exec echo -a {} \; | xargs lcov -o "${pwd}/coverage/lcov.info"
fi

if [[ "${silent}" == "--silent" ]]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
