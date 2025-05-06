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

## Builds targets
## Usage: ${script} <option> [optional]
## ${commands}

while (($#)); do
    case "$1" in

        "--release") # builds RELEASE version
            release="--release"
            ;;

        "--target="*) # builds and runs specified target
            target=${1#*=}
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

if [[ "${target}" == "" ]]; then
    help
    exit
fi

cmake=$(get-cmake)

if [[ "${cmake}" == "" ]]; then
    echo cmake not found. please run "${pwd}/bin/utils/install.sh" --cmake
    exit 8
fi

"${pwd}/bin/cleanup.sh" --clean

export LCOV_PATH=$(which lcov)
export GENHTML_PATH=$(which genhtml)
export MAKEFLAGS=-j8

logs="${pwd}/logs"
build="${pwd}/build"

[[ ! -d "${logs}" ]] && mkdir -p "${logs}"

cat << EOF | sed 's/[[:space:]]\+/ /g' | sed 's/[[:space:]]\+/ /g'
${cmake} \
    -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
    -DCMAKE_C_COMPILER:FILEPATH=$(get-cmake-c-compiler-path) \
    -DCMAKE_CXX_COMPILER:FILEPATH=$(get-cmake-cxx-compiler-path) \
    $(cmake-options) \
    -S"$(pwd)" \
    -B"${build}" \
    -G "Ninja" >/dev/null 2>&1
EOF

${cmake} \
    -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
    -DCMAKE_C_COMPILER:FILEPATH=$(get-cmake-c-compiler-path) \
    -DCMAKE_CXX_COMPILER:FILEPATH=$(get-cmake-cxx-compiler-path) \
    $(cmake-options) \
    -S"$(pwd)" \
    -B"${build}" \
    -G "Ninja" >/dev/null 2>&1

${cmake} --build "${build}" --target "${target}" || (echo ERROR: "${target}" && exit 1)
