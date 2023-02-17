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

    "--test") # builds and runs '-test' target
        array=("main-test")
        ;;

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
        array=("zen" "main" "main-test" "main-brain" "main-light" "main-micro" "main-experimental" "main-alloc" "main-playground" "main-playground2" "main-memory")
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

        "--gc") # [optional] builds with garbage collector
            gc="--gc"
            ;;

        "--silent") # [optional] suppress verbose output
            silent="--silent"
            ;;

        "--valgrind") # [optional] runs using valgrind (disables --sanitize on build)
            valgrind="--valgrind"
            ;;

        *)
            help
            ;;

    esac
done

if [ "${silent}" == "--silent" ]; then
    exec 2>&1 >/dev/null
fi

[ ! -d "${pwd}/build" ] && mkdir "${pwd}/build"

if [ "${clean}" == "--clean" ]; then
    rm -rf "${pwd}/build"
    mkdir "${pwd}/build"
fi

if [ "${sanitize}" == "--sanitize" ] && [ "${valgrind}" != "--valgrind" ]; then
    SANITIZER_OPTIONS=-DCODE_SANITIZER:BOOL=TRUE
else
    SANITIZER_OPTIONS=
fi

if [ "${mocks}" == "--mocks" ]; then
    MOCKS_OPTIONS=-DMOCKS:BOOL=TRUE
else
    MOCKS_OPTIONS=
fi

if [ "${gc}" == "--gc" ]; then
    GC_OPTIONS=-DGC:BOOL=TRUE
else
    GC_OPTIONS=
fi

if [ "${valgrind}" == "--valgrind" ]; then
    VALGRIND_OPTIONS=valgrind
else
    VALGRIND_OPTIONS=
fi

OPTIONS=$(echo "${MOCKS_OPTIONS} ${GC_OPTIONS} ${SANITIZER_OPTIONS}")

export MAKEFLAGS=-j8

find "${pwd}/src" -type f -name "*.s" -delete
find "${pwd}/tests" -type f -name "*.s" -delete

cmake \
    -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
    -DCMAKE_BUILD_TYPE:STRING=Debug \
    -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/gcc \
    -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++ \
    ${OPTIONS} \
    -S"${pwd}" \
    -B"${pwd}/build" \
    -G "Ninja"

for m in "${array[@]}"; do
    cmake --build "${pwd}/build" --target "${m}" || (echo ERROR: "${m}" && exit 1)
    timeout --foreground 5 ${VALGRIND_OPTIONS} "${pwd}/build/${m}" 2>&1 >"${pwd}/build/log-${m}.txt" || (echo ERROR: "${m}" && exit 1)
done

main=$(find "${pwd}/build" -type f -name "*.s" -exec echo {} \;)
for i in $main; do
    path="${pwd}/$(echo $i | sed -n -e 's/^.*.dir\/\(.*\)$/\1/p')"
    cp "${i}" "${path}"
done

if [ "${silent}" == "--silent" ]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"