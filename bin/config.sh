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

pwd=$(cd "$(dirname "$(dirname "${BASH_SOURCE[0]}")")" &> /dev/null && pwd)

cd "${pwd}"

install="$1"

. "${pwd}/bin/scripts/load.sh"

## Builds binaries
## Usage: ${script} <option> [optional]
## ${commands}

while (($#)); do
    case "$1" in

        "--release") # builds RELEASE version
            release="--release"
            ;;

        "--all") # builds and runs all targets
            source="all"
            ;;

        "--target="*) # builds and runs specified target
            source=${1#*=}
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

        "--no-memory-leak-detection") # [optional] skip runs using valgrind / sanitizer
            skip="--no-memory-leak-detection"
            ;;

        "--verbose") # [optional] runs using debug output
            verbose="--verbose"
            ;;

        "--tty") # [optional] runs using TTY ASCII ESC output
            tty="--tty"
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
    exit
fi

config_memory_debug_info="FALSE"
if [[ "${verbose}" == "--verbose" ]]; then
    config_memory_debug_info="TRUE"
fi

if [[ "${silent}" == "--silent" ]]; then
    exec >/dev/null 2>&1
fi

build="${pwd}/config"

if [[ "${source}" == "all" ]]; then
    build="${pwd}/config"
fi

if [[ ! "${dir}" == "" ]]; then
    build="${pwd}/${dir}"
fi

if [[ "${clean}" == "--clean" ]]; then
    if [[ -d "${dir}" ]]; then
        rm -rf "${dir}"
        mkdir -p "${dir}"
    fi
    if [[ -d "${build}" ]]; then
        rm -rf "${build}"
        mkdir -p "${build}"
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

[[ ! -d "${build}" ]] && mkdir -p "${build}"

output="${pwd}/output"
[[ ! -d "${output}" ]] && mkdir -p "${output}"

for target in ${targets[@]}; do
    if [[ -f "${output}/log-${target}.txt" ]]; then
        rm "${output}/log-${target}.txt"
    fi
done

cleanup-dirs

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

[[ ! -d "${pwd}/build" ]] && mkdir -p "${pwd}/build"

if [[ ! "${pwd}/config" == "${pwd}/build" ]]; then
    [[ -f "${pwd}/config/compile_commands.json" ]] && cp -f "${pwd}/config/compile_commands.json" "${pwd}/build/compile_commands.json"
fi

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
            else
                timeout --foreground 180 "${build}/${target}" 2>&1 >"${output}/log-${target}.txt" || (echo ERROR: "${target}" && exit 1)
            fi
            ;;
        *)
            ;;
    esac
done

main=$(find "${build}" -type f -name "*.s" -exec echo {} \;)
for i in ${main[@]}; do
    path="${pwd}/$(echo $i | sed -n -e 's/^.*.dir\/\(.*\)$/\1/p')"
    cp "${i}" "${path}"
done

if [[ "${silent}" == "--silent" ]]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -eq 2 ]] && echo OK

cd "${pwd}"
