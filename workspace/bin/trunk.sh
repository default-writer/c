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

. "${pwd}/bin/scripts/load.sh"

## Formats sources based on provided style guide
## Usage: ${script} [<option>]
## ${commands}

case "${install}" in

	"--help") # [optional] shows command description
		help
		;;

	*)
		;;

esac

cd "${pwd}"

trunk fmt --all

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
