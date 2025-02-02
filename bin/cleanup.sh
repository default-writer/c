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

## TODO: template for bash script
## Usage: ${script} <option> [optional]
## ${commands}

while (($#)); do
    case "$1" in

        "--cleanup") # cleans up build/cmake/config foders
            install="--cleanup"
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

if [[ "${install}" == "--cleanup" ]]; then
    rm -rf "${pwd}/build"
    rm -rf "${pwd}/cmake"
    rm -rf "${pwd}/config"
    rm -rf "${pwd}/lib"
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
