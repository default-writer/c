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

## Builds and tests binaries
## Usage: ${script} <option> [optional]
## ${commands}

case "${install}" in

    "--args") # builds and runs with args from .args file
        args = $(cat "${pwd}/.coverage")
        "${pwd}/bin/covarage.sh" $args
        ;;

    *)
        help
        exit
        ;;

esac

