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

## Builds and logs binaries
## Usage: ${script} <option> [optional]
## ${commands}

case "${install}" in

    "--zen") # builds and runs '-zen' target
        array+=("zen")
        ;;

    "--brain") # builds and runs '-brain' target
        array+=("main-brain")
        ;;

    "--pointer") # builds and runs '-pointer' target
        array+=("main-pointer")
        ;;

    "--hashtable") # builds and runs '-hashtable' target
        array+=("main-hashtable")
        ;;

    "--memory1") # builds and runs '-memory1' target
        array+=("main-memory1")
        ;;

    "--memory2") # builds and runs '-memory2' target
        array+=("main-memory2")
        ;;

    "--memory3") # builds and runs '-memory3' target
        array+=("main-memory3")
        ;;

    "--memory4") # builds and runs '-memory4' target
        array+=("main-memory4")
        ;;

    "--playground1") # builds and runs '-playground1' target
        array+=("main-playground1")
        ;;

    "--playground2") # builds and runs '-playground2' target
        array+=("main-playground2")
        ;;

    "--alloc") # builds and runs '-alloc' target
        array+=("main-alloc")
        ;;

    "--experimental") # builds and runs '-experimental' target
        array+=("main-experimental")
        ;;

    "--micro") # builds and runs '-micro' target
        array+=("main-micro")
        ;;

    "--light") # builds and runs '-light' target
        array+=("main-light")
        ;;

    "--all") # builds and runs all targets
        all="--all"
        ;;

    *)
        help
        ;;

esac

for opt in "${opts[@]}"; do
    case "${opt}" in

        "")
            ;;

        "--clean") # [optional] cleans up directories before run
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

[ ! -d "${pwd}/logs" ] && mkdir "${pwd}/logs"

if [ "${clean}" == "--clean" ]; then
    rm -rf "${pwd}/logs"
    mkdir "${pwd}/logs"
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

[ -d "${pwd}/cmake-3.25/bin" ] && cmake=${pwd}/cmake-3.25/bin/cmake || cmake=cmake

if [ "${all}" == "--all" ]; then

[ ! -d "${pwd}/config" ] && mkdir "${pwd}/config"

${cmake} \
    -DTARGETS:BOOL=ON \
    ${OPTIONS} \
    -S"${pwd}" \
    -B"${pwd}/config" \
    -G "Ninja"

targets=$(cat "${pwd}/config/targets.txt")
for target in ${targets[@]}; do
    array+=("${target}")
done

rm -rf "${pwd}/config"

fi

${cmake} \
    -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
    -DCMAKE_BUILD_TYPE:STRING=Debug \
    -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/gcc \
    -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++ \
    ${OPTIONS} \
    -S"${pwd}" \
    -B"${pwd}/logs" \
    -G "Ninja"

for m in "${array[@]}"; do
    ${cmake} --build "${pwd}/logs" --target "${m}" || (echo ERROR: "${m}" && exit 1)
    timeout --foreground 15 ${VALGRIND_OPTIONS} "${pwd}/logs/${m}" 2>&1 >"${pwd}/logs/log-${m}.txt" || (echo ERROR: "${m}" && exit 1)
done

find "${pwd}/logs" -type f -not -name "log-*" -delete
find "${pwd}/logs" -type d -empty -delete

if [ "${silent}" == "--silent" ]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"