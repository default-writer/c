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

trap 'err_report $LINENO' ERR

uid=$(id -u)

if [ "${uid}" -eq 0 ]; then
    echo "Please run as user"
    exit
fi

pwd=$(cd "$(dirname $(dirname $(dirname "${BASH_SOURCE[0]}")))" &> /dev/null && pwd)

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

build="${pwd}/coverage"

if [[ ! "${dir}" == "" ]]; then
    build="${pwd}/${dir}"
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

cmake=$(get-cmake)

if [[ "${cmake}" == "" ]]; then
    echo cmake not found. please run "${pwd}/bin/utils/install.sh" --cmake
    exit 8
fi

output="${pwd}/output"
[[ ! -d "${output}" ]] && mkdir "${output}"

for target in ${targets[@]}; do
    if [[ -f "${build}/${target}" ]]; then
        rm "${build}/${target}"
    fi
    if [[ -f "${output}/log-${target}.txt" ]]; then
        rm "${output}/log-${target}.txt"
    fi

    if [[ -f "${build}/${target}.info" ]]; then
        rm "${build}/${target}.info"
    fi
done

coverage=( "*.gcda" "*.gcno" "*.s" "*.i" "*.o" "*.info" )

for f in ${coverage[@]}; do
    if [[ -d "${build}" ]]; then
        find "${build}" -type f -name "callgrind.out.*" -delete
        find "${build}" -type f -name "*.s" -delete
        find "${build}" -type f -name "${f}" -delete
    fi
done

export LCOV_PATH=$(which lcov)
export GENHTML_PATH==$(which genhtml)
export MAKEFLAGS=-j8
export LD_LIBRARY_PATH="${pwd}/lib"

${cmake} \
    -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
    -DCMAKE_BUILD_TYPE:STRING=Debug \
    -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/gcc \
    -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++ \
    -DLCOV_PATH=${LCOV_PATH} \
    -DGENHTML_PATH=${GENHTML_PATH} \
    -DCODE_COVERAGE:BOOL=TRUE \
    $(cmake-options) \
    -S"${pwd}" \
    -B"${build}" \
    -G "Ninja" 2>&1 >/dev/null

for target in ${targets[@]}; do
    echo building ${target}
    echo options "$(cmake-options | xargs -n1 | sort -u | xargs)"
    if [[ "${silent}" == "--silent" ]]; then
        ${cmake} --build "${build}" --target "${target}" 2>&1 >/dev/null || (echo ERROR: "${target}" && exit 1)
    else
        ${cmake} --build "${build}" --target "${target}" || (echo ERROR: "${target}" && exit 1)
    fi
    case "${target}" in 
        c-*) ;& main-*) ;& test-*)
            timeout --foreground 180 $(cmake-valgrind-options) "${build}/${target}" 2>&1 >"${output}/log-${target}.txt" || (echo ERROR: "${target}" && exit 1)
            lcov --capture --directory "${build}/" --output-file "${build}/${target}.info" &>/dev/null
            lcov --remove "${build}/${target}.info" "${pwd}/.deps/*" -o "${build}/${target}.info"
            ;;
        *)
            ;;
    esac
done

files=$(find "${build}" -type f -name "*.info" -exec echo {} \;)
if [[ ! "${files[@]}" == "" ]]; then
    find "${build}" -type f -name "*.info" -exec echo -a {} \; | xargs lcov -o "${build}/lcov.info"
fi

main=$(find "${build}" -type f -name "*.s" -exec echo {} \;)
for i in ${main[@]}; do
    path="${pwd}/$(echo $i | sed -n -e 's/^.*.dir\/\(.*\)$/\1/p')"
    cp "${i}" "${path}"
done

if [[ "${silent}" == "--silent" ]]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
