#!/usr/bin/env bash
set -e

err_report() {
    echo "ERROR: on line $*: $(cat $0 | sed $1!d)" >&2
}

trap 'err_report $LINENO' ERR

uid=$(id -u)

if [ "${uid}" -eq 0 ]; then
    echo "Please run as user"
    exit
fi

pwd=$(pwd)

install="$1"

opts=( "${@:2}" )

. "${pwd}/bin/scripts/load.sh"

## Builds binaries
## Usage: ${script} <option> [optional]
## ${commands}

case "${install}" in

    "--zen") # builds and runs '-zen' target
        array=("zen")
        ;;

    "--brain") # builds and runs '-brain' target
        array=("main-brain")
        ;;

    "--memory") # builds and runs '-memory' target
        array=("main-memory")
        ;;

    "--playground") # builds and runs '-playground' target
        array=("main-playground")
        ;;

    "--playground2") # builds and runs '-playground2' target
        array=("main-playground2")
        ;;

    "--alloc") # builds and runs '-alloc' target
        array=("main-alloc")
        ;;

    "--experimental") # builds and runs '-experimental' target
        array=("main-experimental")
        ;;

    "--micro") # builds and runs '-micro' target
        array=("main-micro")
        ;;

    "--light") # builds and runs '-light' target
        array=("main-light")
        ;;

    "--all") # builds and runs all targets
        array=("zen" "main" "main-brain" "main-light" "main-micro" "main-experimental" "main-alloc" "main-playground" "main-playground2" "main-memory")
        ;;

    *)
        help
        ;;

esac

for opt in "${opts[@]}"; do
    case "${opt}" in

        "")
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

        "--silent") # [optional] suppress verbose output
            silent="--silent"
            ;;

        *)
            help
            ;;

    esac
done

if [ "${silent}" == "--silent" ]; then
    exec 2>&1 >/dev/null
fi

[ ! -d "${pwd}/cmake" ] && mkdir "${pwd}/cmake"

if [ "${clean}" == "--clean" ]; then
    rm -rf "${pwd}/cmake"
    mkdir "${pwd}/cmake"
fi

if [ "${sanitize}" == "--sanitize" ]; then
    SANITIZER_OPTIONS=-DCODE_SANITIZER:BOOL=TRUE
else
    SANITIZER_OPTIONS=
fi

if [ "${mocks}" == "--mocks" ]; then
    MOCKS_OPTIONS=-DMOCKS:BOOL=TRUE
else
    MOCKS_OPTIONS=
fi

export MAKEFLAGS=-j8

find "${pwd}/src" -type f -name "*.s" -delete
find "${pwd}/tests" -type f -name "*.s" -delete

cmake \
    -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
    -DCMAKE_BUILD_TYPE:STRING=Debug \
    -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/gcc \
    -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++ \
    ${SANITIZER_OPTIONS} \
    ${MOCKS_OPTIONS} \
    -S"${pwd}" \
    -B"${pwd}/cmake" \
    -G "Ninja"

for m in "${array[@]}"; do
    cmake --build "${pwd}/cmake" --target "${m}"
done

main=$(find "${pwd}/cmake" -type f -name "*.s" -exec echo {} \;)
for i in $main; do
    path="${pwd}/$(echo $i | sed -n -e 's/^.*.dir\/\(.*\)$/\1/p')"
    cp "${i}" "${path}"
done

if [ "${silent}" == "--silent" ]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"