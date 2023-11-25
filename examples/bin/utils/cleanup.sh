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

id=$(id -u)

source=$(pwd)

pwd=$(cd "$(dirname $(dirname $(dirname "${BASH_SOURCE[0]}")))" &> /dev/null && pwd)

install="$1"

. "${pwd}/bin/scripts/load.sh"

## Cleans up directories
## Usage: ${script} <option>
## ${commands}

case "${install}" in
    
    "--all") # cleans up all folders
        ;;
 
    "--help") # shows command description
        help
        ;;

    *)
        help
        ;;

esac

git clean -f -q -d -x

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
