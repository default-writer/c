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

## Builds binaries
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
            opts=( "${@:2}" )
            ;;

        "--clean") # [optional] cleans up directories before build
            clean="--clean"
            ;;

        "--sanitize") # [optional] builds using sanitizer
            sanitize="--sanitize"
            opts=( "${@:2}" )
            ;;

        "--mocks") # [optional] builds with mocks
            mocks="--mocks"
            ;;

        "--gc") # [optional] builds with garbage collector
            gc="--gc"
            opts=( "${@:2}" )
            ;;

        "--silent") # [optional] suppress verbose output
            silent="--silent"
            ;;

        "--valgrind") # [optional] runs using valgrind
            valgrind="--valgrind"
            opts=( "${@:2}" )
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

build=( "build/build-v1" "build/build-v2" "build/build-v3" "build/build-v4" "build/build-v5" "build/build-v6" )

if [[ ! "${dir}" == "" ]]; then
    build="${dir}"
fi

if [[ "${clean}" == "--clean" ]]; then
    if [[ -d "${dir}" ]]; then
        rm -rf "${dir}"
    fi
fi

directories=${build[@]}

targets=( $(get-source-targets ${source}) )

if [[ "${targets[@]}" == "" ]]; then
    if [[ "${help}" == "--help" ]]; then
        help
    fi
    echo no source targets: ${source} >&2
    exit 8
fi

coverage=( "*.gcda" "*.gcno" "*.s" "*.i" "*.o" )

for directory in ${directories[@]}; do
    if [[ "${clean}" == "--clean" ]]; then
        if [[ -d "${directory}" ]]; then
            rm -rf "${directory}"
        fi
        rm -rf "${directory}"
    fi
    if [[ "${clean}" == "" ]]; then
        for f in ${coverage[@]}; do
            if [[ -d "${directory}" ]]; then
                find "${directory}" -type f -name "callgrind.out.*" -delete
                find "${directory}" -type f -name "*.s" -delete
                find "${directory}" -type f -name "${f}" -delete
            fi
        done
    fi
done

registered=$(echo "${sanitize} ${gc} ${valgrind} ${callgrind}" | xargs)

[[ ! -d "${pwd}/build" ]] && mkdir "${pwd}/build"

if [[ "${registered[@]}" == "" || ("${gc}" == "" && "${sanitize}" == "" && "${valgrind}" == "") ]]; then
    "${pwd}/bin/utils/build.sh" --target=${source} --dir=build/build-v1 ${opts[@]}
fi
if [[ "${registered[@]}" == "" || ("${gc}" == "--gc" && "${sanitize}" == "" && "${valgrind}" == "") ]]; then
    "${pwd}/bin/utils/build.sh" --target=${source} --dir=build/build-v2 --gc ${opts[@]}
fi
if [[ "${registered[@]}" == "" || ("${gc}" == "" && "${sanitize}" == "--sanitize" && "${valgrind}" == "") ]]; then
    "${pwd}/bin/utils/build.sh" --target=${source} --dir=build/build-v3 --sanitize ${opts[@]}
fi
if [[ "${registered[@]}" == "" || ("${gc}" == "--gc" && "${sanitize}" == "--sanitize" && "${valgrind}" == "") ]]; then
    "${pwd}/bin/utils/build.sh" --target=${source} --dir=build/build-v4 --gc --sanitize ${opts[@]}
fi
if [[ "${registered[@]}" == "" || ("${gc}" == "" && "${sanitize}" == "" && "${valgrind}" == "--valgrind") ]]; then
    "${pwd}/bin/utils/build.sh" --target=${source} --dir=build/build-v5 --valgrind ${opts[@]}
fi
if [[ "${registered[@]}" == "" || ("${gc}" == "--gc" && "${sanitize}" == "" && "${valgrind}" == "--valgrind") ]]; then
    "${pwd}/bin/utils/build.sh" --target=${source} --dir=build/build-v6 --gc --valgrind ${opts[@]}
fi

for directory in ${directories[@]}; do
    files=()
    if [[ -d "${directory}" ]]; then
        files=$(find "${directory}" -type f -name "log-*.txt" -exec echo {} \;)
    fi
    for file in ${files[@]}; do
        link=$(basename $(dirname "${file}"))
        cp "${file}" "${pwd}/build/${link}-$(basename ${file})"
    done
done

if [[ "${silent}" == "--silent" ]]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
