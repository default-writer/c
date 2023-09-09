#!/usr/bin/env bash
if [[ "${BASHOPTS}" != *extdebug* ]]; then
    set -e
fi

function get_stack () {
   STACK=""
   local i message="${1:-""}"
   local stack_size=${#FUNCNAME[@]}
   # to avoid noise we start with 1 to skip the get_stack function
   for (( i=1; i<stack_size; i++ )); do
      local func="${FUNCNAME[$i]}"
      [[ $func = "" ]] && func=MAIN
      local linen="${BASH_LINENO[$(( i - 1 ))]}"
      local src="${BASH_SOURCE[$i]}"
      [[ "$src" = "" ]] && src=non_file_source

      STACK+=$'\n'"   at: $func $src "$linen
   done
   STACK="${message}${STACK}"
}

err_report() {
    cd ${source}
    get_stack
    echo "ERROR: on line $*: $(cat $(test -L "$0" && readlink "$0" || echo $0) | sed $1!d)" >&2
    exit 8
}

if [[ "${BASHOPTS}" != *extdebug* ]]; then
    trap 'err_report $LINENO' ERR
fi

uid=$(id -u)

if [ "${uid}" -eq 0 ]; then
    echo "Please run as user"
    exit
fi

source=$(pwd)

pwd=$(cd "$(dirname $(dirname "${BASH_SOURCE[0]}"))" &> /dev/null && pwd)

install="$1"

opts=( "${@:2}" )

. "${pwd}/bin/scripts/load.sh"

## Builds binaries and creates coverage info
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

        "--dir="*) # [optional] build directory
            dir=${opt#*=}
            ;;

        "--clean") # [optional] cleans up directories before coverage
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

        "--vm-debug") # [optional] runs using vm debug messaging
            vm_debug="--vm-debug"
            ;;

        "--help") # [optional] shows command desctiption
            help
            ;;

        *)
            help
            ;;

    esac
done

if [[ "${silent}" == "--silent" ]]; then
    exec 2>&1 >/dev/null
fi

build=( "coverage-v1" "coverage-v2" "coverage-v3" "coverage-v4" "coverage-v5" "coverage-v6" )

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

directories=${build[@]}

coverage=( "*.gcda" "*.gcno" "*.s" "*.i" "*.o" "*.info" )

for directory in ${directories[@]}; do
    for f in ${coverage[@]}; do
        if [[ -d "${directory}" ]]; then
            find "${directory}" -type f -name "callgrind.out.*" -delete
            find "${directory}" -type f -name "*.s" -delete
            find "${directory}" -type f -name "${f}" -delete
        fi
    done
done

[[ ! -d "${pwd}/coverage" ]] && mkdir "${pwd}/coverage"

find "${pwd}/coverage" -type f -name "lcov.info" -delete

targets=( $(get-source-targets ${source}) )
for target in ${targets[@]}; do
    if [[ -f "${pwd}/coverage/${target}.info" ]]; then
        rm "${pwd}/coverage/${target}.info"
    fi
done

registered=$(echo "${sanitize} ${gc} ${valgrind} ${callgrind}" | xargs)

if [[ "${registered[@]}" == "" || ("${gc}" == "" && "${sanitize}" == "" && "${valgrind}" == "") ]]; then
    "${pwd}/bin/utils/coverage.sh" --target ${source} --dir=coverage-v1 ${silent} ${opts[@]}
fi
if [[ "${registered[@]}" == "" || ("${gc}" == "--gc" && "${sanitize}" == "" && "${valgrind}" == "") ]]; then
    "${pwd}/bin/utils/coverage.sh" --target ${source} --dir=coverage-v2 --gc ${silent} ${opts[@]}
fi
if [[ "${registered[@]}" == "" || ("${gc}" == "" && "${sanitize}" == "--sanitize" && "${valgrind}" == "") ]]; then
    "${pwd}/bin/utils/coverage.sh" --target ${source} --dir=coverage-v3 --sanitize ${silent} ${opts[@]}
fi
if [[ "${registered[@]}" == "" || ("${gc}" == "--gc" && "${sanitize}" == "--sanitize" && "${valgrind}" == "") ]]; then
    "${pwd}/bin/utils/coverage.sh" --target ${source} --dir=coverage-v4 --gc --sanitize ${silent} ${opts[@]}
fi
if [[ "${registered[@]}" == "" || ("${gc}" == "" && "${sanitize}" == "" && "${valgrind}" == "--valgrind") ]]; then
    "${pwd}/bin/utils/coverage.sh" --target ${source} --dir=coverage-v5 --valgrind ${silent} ${opts[@]}
fi
if [[ "${registered[@]}" == "" || ("${gc}" == "--gc" && "${sanitize}" == "" && "${valgrind}" == "--valgrind") ]]; then
    "${pwd}/bin/utils/coverage.sh" --target ${source} --dir=coverage-v6 --gc --valgrind ${silent} ${opts[@]}
fi

for directory in ${directories[@]}; do
    files=$(find "${directory}" -type f -name "lcov.info" -exec echo {} \;)
    for file in ${files[@]}; do
        targets=( $(get-source-targets ${source}) )
        for target in ${targets[@]}; do
            cp "${file}" "${pwd}/coverage/${directory}-${source}.info"
        done
    done
done

files=$(find "${pwd}/coverage" -type f -name "*.info" -exec echo {} \;)
if [[ ! "${files[@]}" == "" ]]; then
    find "${pwd}/coverage" -type f -name "*.info" -exec echo -a {} \; | xargs lcov -o "${pwd}/coverage/lcov.info"
fi

$(find "$(pwd)/coverage" -type f -not -name "lcov.info" -exec rm {} \;)

if [[ "${silent}" == "--silent" ]]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
