#!/usr/bin/env bash
set -e


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
    get_stack
    echo "ERROR: on line $*: $(cat $0 | sed $1!d)" >&2
    exit 8
}

trap 'get_stack' ERR

uid=$(id -u)

if [ "${uid}" -eq 0 ]; then
    echo "Please run as user"
    exit
fi

install="$1"

opts=( "${@:2}" )

. "$(pwd)/bin/scripts/load.sh"

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

[[ ! -d "$(pwd)/coverage" ]] && mkdir "$(pwd)/coverage"

targets=( $(get-source-targets ${source}) )
for target in ${targets[@]}; do
    if [[ -f "$(pwd)/coverage/${target}.info" ]]; then
        rm "$(pwd)/coverage/${target}.info"
    fi
done

if [[ "${opts[@]}" == "" || ("${gc}" == "" && "${sanitize}" == "" && "${valgrind}" == "") ]]; then
    "$(pwd)/bin/utils/coverage.sh" --target ${source} --dir=coverage-v1 ${silent} ${opts[@]}
fi
if [[ "${opts[@]}" == "" || ("${gc}" == "--gc" && "${sanitize}" == "" && "${valgrind}" == "") ]]; then
    "$(pwd)/bin/utils/coverage.sh" --target ${source} --dir=coverage-v2 --gc ${silent} ${opts[@]}
fi
if [[ "${opts[@]}" == "" || ("${gc}" == "" && "${sanitize}" == "--sanitize" && "${valgrind}" == "") ]]; then
    "$(pwd)/bin/utils/coverage.sh" --target ${source} --dir=coverage-v3 --sanitize ${silent} ${opts[@]}
fi
if [[ "${opts[@]}" == "" || ("${gc}" == "--gc" && "${sanitize}" == "--sanitize" && "${valgrind}" == "") ]]; then
    "$(pwd)/bin/utils/coverage.sh" --target ${source} --dir=coverage-v4 --gc --sanitize ${silent} ${opts[@]}
fi
if [[ "${opts[@]}" == "" || ("${gc}" == "" && "${sanitize}" == "" && "${valgrind}" == "--valgrind") ]]; then
    "$(pwd)/bin/utils/coverage.sh" --target ${source} --dir=coverage-v6 --valgrind ${silent} ${opts[@]}
fi
if [[ "${opts[@]}" == "" || ("${gc}" == "--gc" && "${sanitize}" == "" && "${valgrind}" == "--valgrind") ]]; then
    "$(pwd)/bin/utils/coverage.sh" --target ${source} --dir=coverage-v5 --gc --valgrind ${silent} ${opts[@]}
fi

for directory in ${directories[@]}; do
    files=()
    if [[ -d "${directory}" ]]; then
        files=$(find "${directory}" -type f -name "lcov.info" -exec echo {} \;)
    fi
    for file in ${files[@]}; do
        link=$(basename $(dirname "${file}"))
        targets=( $(get-source-targets ${source}) )
        for target in ${targets[@]}; do
            cp "${file}" "$(pwd)/coverage/${target}-${link}-$(basename ${file})"
        done
    done
done

targets=( $(get-source-targets ${source}) )
for target in ${targets[@]}; do
    files=$(find "$(pwd)/coverage" -type f -name "${target}-*.info" -exec echo {} \;)
    for file in ${files[@]}; do
        find "$(pwd)/coverage" -type f -name "${target}-*.info" -exec echo -a {} \; | xargs lcov -o "$(pwd)/coverage/${target}.info"
        lcov --remove "$(pwd)/coverage/${target}.info" "$(pwd)/.deps/*" -o "$(pwd)/coverage/${target}.info"
    done
done

directories=( "coverage-v1" "coverage-v2" "coverage-v3" "coverage-v4" "coverage-v5" "coverage-v6" )
for directory in ${directories[@]}; do
    files=()
    if [[ -d "${directory}" ]]; then
        files=$(find "${directory}" -type f -name "lcov.info" -exec echo {} \;)
    fi
    for file in ${files[@]}; do
        link=$(basename $(dirname "${file}"))
        targets=( $(get-source-targets ${source}) )
        for target in ${targets[@]}; do
            if [[ -f "$(pwd)/coverage/${target}-${link}-$(basename ${file})" ]]; then
                rm "$(pwd)/coverage/${target}-${link}-$(basename ${file})"
            fi
        done
    done
done

if [[ "${source}" == "all" ]]; then
    files=$(find "$(pwd)/coverage" -type f -name "*.info" -exec echo {} \;)
    if [[ ! "${files}" == "" ]]; then
        lcov --remove "$(pwd)/coverage/all.info" -o "$(pwd)/coverage/lcov.info"
        rm "$(pwd)/coverage/all.info"
    fi
fi

if [[ "${silent}" == "--silent" ]]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "$(pwd)"
