#!/usr/bin/env bash
set -e

err_report() {
    echo "ERROR: on line $*: $(cat $0 | sed $1!d)" >&2
}

trap 'err_report $LINENO' ERR

uid=$(id -u)

pwd=$(pwd)

install="$1"

. "${pwd}/bin/scripts/load.sh"

## Cleans up directories
## Usage: ${script} <option>
## ${commands}

case "${install}" in

    "--all") # cleans up uncommited changes and non-gited files and folders
        ;;

    *)
        help
        ;;

esac

git clean -f -q -d -x

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"