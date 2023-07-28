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
        target="--all"
        ;;

    "--target") # builds and runs specified target
        target="--target $2"
        source=$2
        opts=( "${@:3}" )
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

        "--silent") # [optional] suppress verbose output
            silent="--silent"
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

if [[ "${silent}" == "--silent" ]]; then
    exec 2>&1 >/dev/null
fi

cmake=$(get-cmake)
targets=( $(get-source-targets ${source}) )

if [[ "${targets[@]}" == "" ]]; then
    if [[ "${help}" == "--help" ]]; then
        help
    fi
    echo ERROR
    exit 8
fi

for target in ${targets[@]}; do
    "${pwd}/bin/build.sh" --target ${target} --gc ${opts[@]}
    "${pwd}/bin/build.sh" --target ${target} --gc --sanitize ${opts[@]}
    "${pwd}/bin/build.sh" --target ${target} --gc --valgrind ${opts[@]}
    "${pwd}/bin/build.sh" --target ${target} ${silent} ${opts[@]}
    "${pwd}/bin/build.sh" --target ${target} --sanitize ${opts[@]}
    "${pwd}/bin/build.sh" --target ${target} --valgrind ${opts[@]}
done

if [ "${silent}" == "--silent" ]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
