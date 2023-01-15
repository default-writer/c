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

    "--playground") # builds and runs '-playground' target
        array=("-playground")
        ;;

    "--alloc") # builds and runs '-alloc' target
        array=("-alloc")
        ;;

    "--experimental") # builds and runs '-experimental' target
        array=("-experimental")
        ;;

    "--micro") # builds and runs '-micro' target
        array=("-micro")
        ;;

    "--light") # builds and runs '-light' target
        array=("-light")
        ;;

    "--all") # builds and runs all targets
        array=("" "-light" "-micro" "-experimental" "-alloc" "-playground")
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

OPTIONS=${SANITIZER_OPTIONS}

export MAKEFLAGS=-j8

cmake \
    -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
    -DCMAKE_BUILD_TYPE:STRING=Debug \
    -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/gcc \
    -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++ \
    ${OPTIONS} \
    -S"${pwd}" \
    -B"${pwd}/cmake" \
    -G "Ninja"

for m in "${array[@]}"; do
    cmake --build "${pwd}/cmake" --target "main${m}"
done

main=$(find "${pwd}/cmake" -type f -name "*.s" -exec echo {} \; | grep -s "main")
for i in $main; do
    path="${pwd}/$(echo $i | sed -n -e 's/^.*.dir\/\(.*\)$/\1/p')"
    cp "${i}" "${path}"
done

if [ "${silent}" == "--silent" ]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"