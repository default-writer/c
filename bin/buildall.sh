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
        source="all"
        ;;

    "--target") # builds and runs specified target
        source="$2"
        opts=( "${@:3}" )
        ;;

    "--all") # builds and runs specified target
        source="all"
        opts=( "${@:2}" )
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

        "--silent") # [optional] suppress verbose output
            silent="--silent"
            ;;

        "--help") # [optional] shows command desctiption
            help
            ;;

        *)
            help
            ;;

    esac
done

if [[ "${silent}" == "--silent" ]]; then
    exec 2>&1 >/dev/null
fi

"${pwd}/bin/build.sh" --target ${source} --valgrind ${silent} ${opts[@]}
"${pwd}/bin/build.sh" --target ${source} --sanitize ${silent} ${opts[@]}
"${pwd}/bin/build.sh" --target ${source} ${silent}
"${pwd}/bin/build.sh" --target ${source} --gc --valgrind ${silent} ${opts[@]}
"${pwd}/bin/build.sh" --target ${source} --gc --sanitize ${silent} ${opts[@]}
"${pwd}/bin/build.sh" --target ${source} --gc ${silent}

if [ "${silent}" == "--silent" ]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
