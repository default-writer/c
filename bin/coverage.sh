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

function help() {
        commands=$(cat $0 | sed -e 's/^[ \t]*//;' | sed -e '/^[ \t]*$/d' | sed -n -e 's/^"\(.*\)".*#/    \1:/p' | sed -n -e 's/: /:\n        /p')
        script="$(basename "$(test -L "$0" && readlink "$0" || echo "$0")")"
        help=$(\
cat << EOF
Builds binaries
Usage: ${script} <option> [optional]
${commands}
EOF
)
        echo "${help}"
        exit
}

case "${install}" in

    "--memory") # builds and runs '-memory' target
        array=("main-memory")
        ;;

    "--playground") # builds and runs '-playground' target
        array=("main-playground")
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
        array=("zen" "main" "main-light" "main-micro" "main-experimental" "main-alloc" "main-playground" "main-memory")
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

[ ! -d "${pwd}/coverage" ] && mkdir "${pwd}/coverage"

if [ "${clean}" == "--clean" ]; then
    rm -rf "${pwd}/cmake"
    mkdir "${pwd}/cmake"
fi

for m in "${array[@]}"; do
    rm -f "${pwd}/coverage/${m}.lcov"
done

export LCOV_PATH=$(which lcov)
export GENHTML_PATH==$(which genhtml)

OPTIONS=${SANITIZER_OPTIONS}

export MAKEFLAGS=-j8

find "${pwd}/coverage" -type f -name "*.gcda" -delete
find "${pwd}/coverage" -type f -name "*.gcno" -delete
find "${pwd}" -type f -name "*.s" -delete

cmake \
    -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
    -DCMAKE_BUILD_TYPE:STRING=Debug \
    -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/gcc \
    -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++ \
    -DCODE_SANITIZER:BOOL=TRUE \
    -DCODE_COVERAGE:BOOL=TRUE \
    -DLCOV_PATH=${LCOV_PATH} \
    -DGENHTML_PATH=${GENHTML_PATH} \
    -S"${pwd}" \
    -B"${pwd}/cmake" \
    -G "Ninja"

for m in "${array[@]}"; do
    cmake --build "${pwd}/cmake" --target "${m}"
    timeout --foreground 5 "${pwd}/cmake/${m}"
    lcov --capture --directory "${pwd}/cmake/" --output-file "${pwd}/coverage/${m}.lcov" &>/dev/null
    lcov --remove "${pwd}/coverage/${m}.lcov" "${pwd}/src/rexo/*" -o "${pwd}/coverage/${m}.lcov"
done

find "${pwd}/coverage" -type f -name "main*.lcov" -exec echo -a {} \; | xargs lcov -o "${pwd}/coverage/lcov.info"
find "${pwd}/coverage" -type f -name "main*.lcov" -delete

if [ "${silent}" == "--silent" ]; then
    exec 1>&2 2>&-
fi

"${pwd}/bin/build.sh" ${install} "${opts[@]}"

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
