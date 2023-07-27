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

## Finds targets by file relative path/name and executes them
## Usage: ${script} <option> [optional]
## ${commands}

case "${install}" in

    "--target") # builds and runs specified target
        target="--target $2"
        opts=( "${@:3}" )
        ;;

    "--help") # [optional] shows command desctiption
        help
        ;;

    *)
        help
        exit 8
        ;;

esac

options=()
for opt in ${opts[@]}; do
    case ${opt} in

        "")
            ;;

        "--keep") # [optional] keeps coverage files and merges them
            keep="--keep"
            options+=( "${keep}" )
            ;;

        "--clean") # [optional] cleans up directories before build
            clean="--clean"
            options+=( "${clean}" )
            ;;

        "--mocks") # [optional] builds with mocks
            mocks="--mocks"
            options+=( "${mocks}" )
            ;;

        "--silent") # [optional] suppress verbose output
            silent="--silent"
            options+=( "${silent}" )
            ;;

        "--valgrind") # [optional] runs using valgrind (disables --sanitize on build)
            valgrind="--valgrind"
            options+=( "${valgrind}" )
            ;;

        "--callgrind") # [optional] runs using valgrind with tool callgrind (disables --sanitize on build)
            callgrind="--callgrind"
            options+=( "${callgrind}" )
            ;;

        "--debug") # [optional] runs using debug memory debug info
            debug="--debug"
            options+=( "${debug}" )
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

[ ! -d "${pwd}/config" ] && mkdir "${pwd}/config"

if [ "${keep}" == "" ]; then
if [ "${clean}" == "--clean" ]; then
    rm -rf "${pwd}/config"
    mkdir "${pwd}/config"
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

for linked_target in ${targets[@]}; do
    case ${linked_target} in
        main-*) ;& test-*)
            if [[ " ${targets[*]} " == *" ${linked_target} "* ]]; then
                ${pwd}/bin/coverageall.sh --target ${linked_target} --keep ${options}
            fi
            ;;
        *)
            ${pwd}/bin/buildall.sh --target ${linked_target} --keep ${options}
            ;;
    esac
done
find "${pwd}/coverage" -type f -name "*.lcov" -exec echo -a {} \; | xargs lcov -o "${pwd}/coverage/lcov.info"

if [ "${silent}" == "--silent" ]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
