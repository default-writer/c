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

    "")
        ;;

    "--target") # builds and runs specified target
        opts=( "${@:3}" )
        target="--target"
        ;;

    "--all") # builds and runs all targets
        target="--all"
        ;;

    *)
        help
        exit 8
        ;;

esac

for opt in ${opts[@]}; do
    case ${opt} in

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

        "--callgrind") # [optional] runs using valgrind with tool callgrind (disables --sanitize on build)
            callgrind="--callgrind"
            ;;

        "--debug") # [optional] runs using debug memory debug info
            debug="--debug"
            ;;

        "--help") # [optional] shows command desctiption
            help
            ;;

        *)
            echo "Error: unknown argyment ${opt}"
            help
            exit 8
            ;;

    esac
done

if [ "${silent}" == "--silent" ]; then
    exec 2>&1 >/dev/null
fi

[ ! -d "${pwd}/gtk" ] && mkdir "${pwd}/gtk"

if [ "${clean}" == "--clean" ]; then
    rm -rf "${pwd}/gtk"
    mkdir "${pwd}/gtk"
fi

find "${pwd}" -type f -name "callgrind.out.*" -delete
find "${pwd}/src" -type f -name "*.s" -delete
find "${pwd}/tests" -type f -name "*.s" -delete

cmake=$(get-cmake)
gtktargets=( $(get-gtktargets) )
if [ "${target}" == "--target" ]; then
    for target in ${gtktargets[@]}; do
        if [ "${target}" == "$2" ]; then 
            array=( ${target} )
            break
        fi
    done
    if [ "$(echo ${array[@]})" == "" ]; then
        if [[ "${help}" == "--help" ]]; then
            help
        fi
        echo ERROR
        exit 8
    fi
    targets=( ${array[@]} )
fi

[ ! -d "${pwd}/coverage" ] && mkdir "${pwd}/coverage"

coverage=( "*.gcda" "*.gcno" "*.s" "*.i" "*.o" )
for f in ${coverage}; do
    find "${pwd}/coverage" -type f -name "${f}" -delete
done

export MAKEFLAGS=-j8

${cmake} \
    -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
    -DCMAKE_BUILD_TYPE:STRING=Debug \
    -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/gcc \
    -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++ \
    $(cmake-options) \
    -S"${pwd}" \
    -B"${pwd}/gtk" \
    -G "Ninja" 2>&1 >/dev/null


for target in ${gtktargets[@]}; do
    if [ "${silent}" == "--silent" ]; then
        ${cmake} --build "${pwd}/gtk" --target "${target}" 2>&1 >/dev/null || (echo ERROR: "${target}" && exit 1)
    else
        ${cmake} --build "${pwd}/gtk" --target "${target}" || (echo ERROR: "${target}" && exit 1)
    fi
    case "${target}" in gtk-*)
        timeout --foreground 180 $(cmake-valgrind-options) "${pwd}/gtk/${target}" 2>&1 >"${pwd}/gtk/log-${target}.txt" || (echo ERROR: "${target}" && exit 1)
    esac
done

main=$(find "${pwd}/gtk" -type f -name "*.s" -exec echo {} \;)
for i in $main; do
    path="${pwd}/$(echo $i | sed -n -e 's/^.*.dir\/\(.*\)$/\1/p')"
    cp "${i}" "${path}"
done

if [ "${silent}" == "--silent" ]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
