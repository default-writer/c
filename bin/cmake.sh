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

        "--keep") # [optional] keeps coverage files and merges them
            keep="--keep"
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

        "--verbose") # [optional] shows verbose messages
            verbose="--verbose"
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

[ ! -d "${pwd}/cmake" ] && mkdir "${pwd}/cmake"
[ ! -d "${pwd}/logs" ] && mkdir "${pwd}/logs"

if [ "${keep}" == "" ]; then
if [ "${clean}" == "--clean" ]; then
    rm -rf "${pwd}/cmake"
    mkdir "${pwd}/cmake"
fi
fi

cmake=$(get-cmake)
targets=( $(get-cmake-targets) )

found_target=false
for target in ${targets[@]}; do
    if [ "${target}" == "$2" ]; then 
        array=( ${target} )
        found_target=true
    fi
done

if [[ "${found_target}" == false ]]; then
    array=()
    target=$2
    found_target_base=false
    sources=$(find "${pwd}/config" -type f -name "sources.txt")
    for source in ${sources[@]}; do
        # Find target directory for the given source file
        while IFS= read -r line; do
            if [[ "${target}" == "${line}" ]]; then
                found_target_base=true
                target_base=$(basename $(dirname "${source}"))
                array+=( ${target_base} )
                break
            fi
        done <  $source
        if [[ "${found_target_base}" == true ]]; then
            linked_targets=$(get-linked-targets ${target_base})
            if [ "${target}" == "--target ${line}" ]; then
                for linked_target in ${linked_targets[@]}; do
                    if [ "${linked_target}" == "$2" ]; then 
                        array+=( ${linked_target} )
                    fi
                done
            fi
        fi
    done
fi

targets=( ${array[@]} )

if [[ "${target_base}" == "" ]]; then
    target_base="${targets[@]}"
fi

targets=$(echo "${targets[@]} ${linked_targets[@]}" | xargs -n1 | sort -u | xargs)

if [[ "${targets[@]}" == "" ]]; then
    if [[ "${help}" == "--help" ]]; then
        help
    fi
    echo ERROR
    exit 8
fi

export MAKEFLAGS=-j8

${cmake} \
    -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
    -DCMAKE_BUILD_TYPE:STRING=Debug \
    -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/gcc \
    -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++ \
    $(cmake-options) \
    -S"${pwd}" \
    -B"${pwd}/cmake" \
    -G "Ninja" 2>&1 >/dev/null

for target in ${targets[@]}; do
    if [[ "${verbose}" == "--verbose" ]]; then
        echo Building target ${target}
        echo Building with options $(cmake-options)
    fi
    if [ "${silent}" == "--silent" ]; then
        ${cmake} --build "${pwd}/cmake" --target "${target}" 2>&1 >/dev/null || (echo ERROR: "${target}" && exit 1)
    else
        ${cmake} --build "${pwd}/cmake" --target "${target}" || (echo ERROR: "${target}" && exit 1)
    fi
    case "${target}" in main-*)
        timeout --foreground 180 $(cmake-valgrind-options) "${pwd}/cmake/${target}" 2>&1 >"${pwd}/logs/log-${target}.txt" || (echo ERROR: "${target}" && exit 1)
    esac
done

find "${pwd}/logs" -type f -not -name "log-*" -delete

if [ "${silent}" == "--silent" ]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
