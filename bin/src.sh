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

    "--help") # shows help message
        help
        ;;

    *)
        help
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

        "--help") # shows command desctiption
            help
            ;;

        *)
            help
            ;;

    esac
done

if [ "${silent}" == "--silent" ]; then
    exec 2>&1 >/dev/null
fi

[ ! -d "${pwd}/config" ] && mkdir "${pwd}/config"

if [ "${clean}" == "--clean" ]; then
    rm -rf "${pwd}/config"
    mkdir "${pwd}/config"
fi

cmake=$(get-cmake)
targets=( $(get-targets) )

found_target_base=false
sources=$(find "${pwd}/config" -type f -name "sources.txt")
for source in ${sources[@]}; do
    # Find target directory for the given source file
    while IFS= read -r line; do
        if [[ "${target}" == "--target ${line}" ]]; then
            found_target_base=true
            target_base=$(basename $(dirname "${source}"))
            break
        fi
    done <  $source
done

if [[ "$found_target_base" = true ]]; then
    linked_targets=$(get-linked-targets ${target_base})
    echo "${linked_targets[@]}"
    for linked_target in ${linked_targets[@]}; do
        case ${linked_target} in
            main-*) ;& tests-*)
                if [[ " ${targets[*]} " == *" ${linked_target} "* ]]; then
                    ${pwd}/bin/coverageall.sh --target ${linked_target} --keep ${opts[@]}
                fi
                ;;
            *)
                ${pwd}/bin/buildall.sh --target ${linked_target} --keep
                ;;
        esac
    done
    find "${pwd}/coverage" -type f -name "*.lcov" -exec echo -a {} \; | xargs lcov -o "${pwd}/coverage/lcov.info"
else
    help
    exit 8
fi

if [ "${silent}" == "--silent" ]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
