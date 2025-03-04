#!/usr/bin/env bash
if [[ "${BASHOPTS}" != *extdebug* ]]; then
    set -e -x
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

## Clean up root directory
## Usage: ${script} <option> [optional]
## ${commands}

while (($#)); do
    case "$1" in

        "--clean") # cleans up build/cmake/config foders
            install="--clean"
            ;;

        "--help") # [optional] shows command description
            help
            ;;

        *)
            help
            ;;

    esac
    shift
done

if [[ "${install}" == "" ]]; then
    help
    exit;
fi

if [[ "${install}" == "--clean" ]]; then
    rm -rf "${pwd}/build"
    rm -rf "${pwd}/cmake"
    rm -rf "${pwd}/config"
    rm -rf "${pwd}/lib"
    rm -f "${pwd}/src/std/version.h"
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
