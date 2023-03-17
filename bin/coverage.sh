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

## Builds binaries and creates coverage info
## Usage: ${script} <option> [optional]
## ${commands}

case "${install}" in

    "--zen") # builds and runs '-zen' target
        array=("zen")
        ;;

    "--brain") # builds and runs '-brain' target
        array=("main-brain")
        ;;

    "--pointer") # builds and runs '-pointer' target
        array=("main-pointer")
        ;;

    "--hashtable") # builds and runs '-hashtable' target
        array=("main-hashtable")
        ;;

    "--memory1") # builds and runs '-memory1' target
        array=("main-memory1")
        ;;

    "--memory2") # builds and runs '-memory2' target
        array=("main-memory2")
        ;;

    "--memory3") # builds and runs '-memory3' target
        array=("main-memory3")
        ;;

    "--memory4") # builds and runs '-memory4' target
        array=("main-memory4")
        ;;

    "--playground1") # builds and runs '-playground1' target
        array=("main-playground1")
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
        array=("zen" "main" "main-pointer" "main-hashtable" "main-brain" "main-light" "main-micro" "main-experimental" "main-alloc" "main-playground1" "main-playground2" "main-memory1" "main-memory2" "main-memory3" "main-memory4")
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

[ ! -d "${pwd}/coverage" ] && mkdir "${pwd}/coverage"

if [ "${clean}" == "--clean" ]; then
    rm -rf "${pwd}/coverage"
    mkdir "${pwd}/coverage"
fi

for m in "${array[@]}"; do
    rm -f "${pwd}/coverage/${m}.lcov"
done

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

export LCOV_PATH=$(which lcov)
export GENHTML_PATH==$(which genhtml)
export MAKEFLAGS=-j8

find "${pwd}/coverage" -type f -name "*.gcda" -delete
find "${pwd}/coverage" -type f -name "*.gcno" -delete
find "${pwd}" -type f -name "*.s" -delete

[ -d "${pwd}/cmake-3.25/bin" ] && cmake=${pwd}/cmake-3.25/bin/cmake || cmake=cmake

${cmake} \
    -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
    -DCMAKE_BUILD_TYPE:STRING=Debug \
    -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/gcc \
    -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++ \
    -DLCOV_PATH=${LCOV_PATH} \
    -DGENHTML_PATH=${GENHTML_PATH} \
    -DCODE_COVERAGE:BOOL=TRUE \
    ${OPTIONS} \
    -S"${pwd}" \
    -B"${pwd}/coverage" \
    -G "Ninja"

for m in "${array[@]}"; do
    ${cmake} --build "${pwd}/coverage" --target "${m}" || (echo ERROR: "${m}" && exit 1)
    timeout --foreground 15 ${VALGRIND_OPTIONS} "${pwd}/coverage/${m}" 2>&1 >"${pwd}/coverage/log-${m}.txt" || (echo ERROR: "${m}" && exit 1)
    lcov --capture --directory "${pwd}/coverage/" --output-file "${pwd}/coverage/${m}.lcov" &>/dev/null
    lcov --remove "${pwd}/coverage/${m}.lcov" "${pwd}/src/rexo/*" -o "${pwd}/coverage/${m}.lcov"
done

find "${pwd}/coverage" -type f -type f -empty -delete
find "${pwd}/coverage" -type f -name "*.lcov" -exec echo -a {} \; | xargs lcov -o "${pwd}/coverage/lcov.info"
for m in "${array[@]}"; do
    rm -f "${pwd}/coverage/${m}.lcov"
done

if [ "${silent}" == "--silent" ]; then
    exec 1>&2 2>&-
fi

"${pwd}/bin/build.sh" ${install} "${opts[@]}"

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
