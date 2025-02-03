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

pwd=$(cd "$(dirname $(dirname $(dirname "${BASH_SOURCE[0]}")))" &> /dev/null && pwd)

install="$1"

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
            ;;

        "--dir="*) # [optional] build directory
            dir=${1#*=}
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

build="${pwd}/build/coverage"

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

[[ ! -d "${build}" ]] && mkdir "${build}"

output="${pwd}/output"
[[ ! -d "${output}" ]] && mkdir "${output}"

for target in ${targets[@]}; do
    if [[ -f "${build}/${target}" ]]; then
        rm "${build}/${target}"
    fi
    if [[ -f "${output}/log-${target}.txt" ]]; then
        rm "${output}/log-${target}.txt"
    fi

    if [[ "${clean}" == "--clean" ]]; then
        if [[ -f "${build}/${target}.info" ]]; then
            rm "${build}/${target}.info"
        fi
    fi
done

export LCOV_PATH=$(which lcov)
export GENHTML_PATH==$(which genhtml)
export MAKEFLAGS=-j8
export LD_LIBRARY_PATH="${pwd}/lib"

${cmake} \
    -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
    -DCMAKE_BUILD_TYPE:STRING=Debug \
    -DCMAKE_C_COMPILER:FILEPATH=$(get-cmake-c-compiler-path) \
    -DCMAKE_CXX_COMPILER:FILEPATH=$(get-cmake-cxx-compiler-path) \
    -DLCOV_PATH=${LCOV_PATH} \
    -DGENHTML_PATH=${GENHTML_PATH} \
    -DCODE_COVERAGE:BOOL=TRUE \
    $(cmake-options) \
    -S"${pwd}" \
    -B"${build}" \
    -G "Ninja" 2>&1 >/dev/null

ignore=$(get-ignore)

for config in ${targets[@]}; do
    target="${config}"
    echo building ${target}
    echo options "$(cmake-options)"
    ${cmake} --build "${build}" --target "${target}" 2>&1 || (echo ERROR: "${target}" && exit 1)
    case "${target}" in 
        c-*) ;& main-*) ;& test-*)
            if [ ! "$(cat /proc/version | grep -c MSYS)" == "1" ] && [ ! "$(cat /proc/version | grep -c MINGW64)" == "1" ]; then
                timeout --foreground 180 $(cmake-valgrind-options) "${build}/${target}" 2>&1 >"${output}/log-${target}.txt" || (echo ERROR: "${target}" && exit 1)
            else
                timeout --foreground 180 "${build}/${target}" 2>&1 >"${output}/log-${target}.txt" || (echo ERROR: "${target}" && exit 1)
            fi
            lcov --capture --directory "${build}/" --output-file "${build}/${target}-all.info" &>/dev/null
            lcov --remove "${build}/${target}-all.info" -o "${build}/${target}.info" ${ignore}
            rm "${build}/${target}-all.info"
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
