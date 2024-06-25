#!/usr/bin/env bash
if [[ "${BASHOPTS}" != *extdebug* ]]; then
    set -e
fi

err_report() {
    cd ${source}
    echo "ERROR: $0:$*"
    exit 8
}

if [[ "${BASHOPTS}" != *extdebug* ]]; then
    trap 'err_report $LINENO' ERR
fi

source=$(pwd)

pwd=$(cd "$(dirname $(dirname "${BASH_SOURCE[0]}"))" &> /dev/null && pwd)

cd "${pwd}"


install="$1"

opts=( "${@:2}" )

. "${pwd}/bin/scripts/load.sh"

## Builds and tests binaries
## Usage: ${script} <option>
## ${commands}
## Example .args file:
## --all
## --clean
## --silent
## --sanitize
## --gc

case "${install}" in

    "--args") # builds and runs with args from .args file
        if [ -f "${pwd}/.args" ]; then args=$(cat "${pwd}/.args"); fi
        "${pwd}/bin/coverage.sh" $args ${opts[@]}
        ;;

esac

if [[ "${install}" == "" ]]; then
    help
    exit;
fi
