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

uid=$(id -u)

source=$(pwd)

pwd=$(cd "$(dirname $(dirname $(dirname "${BASH_SOURCE[0]}")))" &> /dev/null && pwd)

install="$1"

. "${pwd}/bin/scripts/load.sh"

if [[ "${install}" == "--help" ]]; then
    help
    exit
fi

if [ ! "${uid}" -eq 0 ]; then
    echo "Please run as root"
    exit
fi

## Build optional dependencies
## Usage: ${script} <option>
## ${commands}

case "${install}" in

    "--musl") # builds musl as git submodule
        cd "${pwd}/.deps/musl"
        make
        make install
        ;;

    *)
        help
        exit
        ;;

esac

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
