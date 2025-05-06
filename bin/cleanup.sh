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

unset LD_PRELOAD

export LD_PRELOAD=

source=$(pwd)

pwd=$(cd "$(dirname "$(dirname "${BASH_SOURCE[0]}")")" &> /dev/null && pwd)

cd "${pwd}"

install="$1"

opts=( "${@:2}" )

build=""

. "${pwd}/bin/scripts/load.sh"

## Cleans up build directories
## Usage: ${script} <option> [optional]
## ${commands}

while (($#)); do
    case "$1" in

        "--clean") # cleans up build/cmake/config foders
            install="--clean"
            ;;

        "--python") # cleans up python cache folders
            python="--python"
            ;;

        "--build") # build project using cmake
            build="--build"
            ;;

        "--help") # shows command description
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
    exit
fi

if [[ "${install}" == "--clean" ]]; then
    if [[ "${pwd}" != "" ]]; then
        rm -rf "${pwd}/build"
        rm -rf "${pwd}/cmake"
        rm -rf "${pwd}/config"
        rm -rf "${pwd}/logs"
        rm -rf "${pwd}/lib"
        rm -rf "${pwd}/.tmp"
        rm -f "${pwd}/src/std/version.h"
        find "${pwd}" -type f -name "callgrind.out.*" -delete
        find "${pwd}" -type f -name "*.s" -delete
    fi
fi

if [[ "${python}" == "--python" ]]; then
    find "${pwd}" -type d -name "__pycache__" -exec rm -rf {} +
    find "${pwd}" -type d -name ".pytest_cache" -exec rm -rf {} +
fi

if [[ "${build}" == "--build" ]]; then
    source="main-tests-vm1"

    targets=( $source )

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

    build="${pwd}/build"
    [[ ! -d "${build}" ]] && mkdir -p "${build}"

    output="${pwd}/output"
    [[ ! -d "${output}" ]] && mkdir -p "${output}"

    for target in ${targets[@]}; do
        if [[ -f "${output}/log-${target}.txt" ]]; then
            rm "${output}/log-${target}.txt"
        fi
    done

    export MAKEFLAGS=-j8
    export LD_LIBRARY_PATH="${pwd}/lib"

    cat << EOF | sed 's/[[:space:]]\+/ /g'
${cmake} \
    -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
    -DCMAKE_C_COMPILER:FILEPATH=$(get-cmake-c-compiler-path) \
    -DCMAKE_CXX_COMPILER:FILEPATH=$(get-cmake-cxx-compiler-path) \
    $(cmake-options) \
    -S"${pwd}" \
    -B"${build}" \
    -G "Ninja" >/dev/null 2>&1
EOF

    ${cmake} \
        -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
        -DCMAKE_C_COMPILER:FILEPATH=$(get-cmake-c-compiler-path) \
        -DCMAKE_CXX_COMPILER:FILEPATH=$(get-cmake-cxx-compiler-path) \
        $(cmake-options) \
        -S"${pwd}" \
        -B"${build}" \
        -G "Ninja" >/dev/null 2>&1

    for config in ${targets[@]}; do
        target="${config}"
        echo CONFIG_MEMORY_DEBUG_INFO: ${config_memory_debug_info}
        echo building ${target}
        echo options "$(cmake-options)"
        ${cmake} --build "${build}" --target "${target}" 2>&1 || (echo ERROR: "${target}" && exit 1)
        case "${target}" in
            main-*) ;& test-*)
                if [ ! "${skip}" == "--no-memory-leak-detection" ] && [ ! "$(cat /proc/version | grep -c MSYS)" == "1" ] && [ ! "$(cat /proc/version | grep -c MINGW64)" == "1" ]; then
                    timeout --foreground 180 $(cmake-valgrind-options) "${build}/${target}" 2>&1 >"${output}/log-${target}.txt" || (echo ERROR: "${target}" && exit 1)
                fi
                ;;
            *)
                ;;
        esac
    done
fi

[[ $SHLVL -eq 2 ]] && echo OK

cd "${pwd}"
