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
Builds and logs binaries
Usage: ${script} <option> [optional]
${commands}
EOF
)
        echo "${help}"
        exit
}

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

        "--clean") # [optional] cleans up directories before run
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

OPTIONS=${SANITIZER_OPTIONS}

export MAKEFLAGS=-j8

cmake \
    -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
    -DCMAKE_BUILD_TYPE:STRING=Debug \
    -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/gcc \
    -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++ \
    ${SANITIZER_OPTIONS} \
    ${MOCKS_OPTIONS} \
    -S"${pwd}" \
    -B"${pwd}/logs" \
    -G "Ninja"

for m in "${array[@]}"; do
    cmake --build "${pwd}/logs" --target "${m}"
    timeout --foreground 5 "${pwd}/logs/${m}" 2>&1 >"${pwd}/logs/log-${m}.txt" || echo ERROR: "${m}"
done

find "${pwd}/logs" -type f -not -name "log-*" -delete
find "${pwd}/logs" -type d -empty -delete

if [ "${silent}" == "--silent" ]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"