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

COMMAND_LINE_OPTIONS=$(get-options ${opts[@]})
if [ $? -eq 0 ]; then
    "${pwd}/bin/build.sh" ${COMMAND_LINE_OPTIONS}
fi
if [ $? -eq 0 ]; then
    "${pwd}/bin/coverage.sh" ${COMMAND_LINE_OPTIONS}
fi
if [ $? -eq 0 ]; then
    "${pwd}/bin/logs.sh" ${COMMAND_LINE_OPTIONS}
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
