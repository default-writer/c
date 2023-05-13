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

. "${pwd}/bin/scripts/load.sh"

## Formats sources based on provided style guide
## Usage: ${script} [<option>]
## ${commands}

case "${install}" in

"--help") # Use .trunk formatter
	help
	;;

*) ;;

esac

cd "${pwd}"

trunk fmt --all

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
