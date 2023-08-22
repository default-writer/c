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

        "--dir="*) # [optional] build directory
            dir=${opt#*=}
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

for target in ${targets[@]}; do
    find "$(pwd)" -type f -name "${target}-*.info" -delete
done

[[ ! -d "$(pwd)/coverage" ]] && mkdir "$(pwd)/coverage"

if [[ -f "$(pwd)/coverage/lcov.info" ]]; then 
    rm "$(pwd)/coverage/lcov.info"
fi

for linked_target in ${targets[@]}; do
    case ${linked_target} in
        c-*) ;& main-*) ;& test-*)
            if [[ " ${targets[*]} " == *" ${linked_target} "* ]]; then
                $(pwd)/bin/coverage.sh --target ${linked_target} ${opts[@]}
            fi
            ;;
        *)
            $(pwd)/bin/build.sh --target ${linked_target} ${opts[@]}
            ;;
    esac
done

files=$(find "$(pwd)/coverage" -type f -name "*.info" -exec echo {} \;)
if [[ ! "${files[@]}" == "" ]]; then
    find "$(pwd)/coverage" -type f -name "*.info" -exec echo -a {} \; | xargs lcov -o "$(pwd)/coverage/lcov.info"
fi

for linked_target in ${targets[@]}; do
    case ${linked_target} in
        c-*) ;& main-*) ;& test-*)
            if [[ -f "$(pwd)/coverage/${linked_target}.info" ]]; then
               rm "$(pwd)/coverage/${linked_target}.info"
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

cd "$(pwd)"
