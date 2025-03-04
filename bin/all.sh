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

## Builds binaries
## Usage: ${script} <option> [optional]
## ${commands}

while (($#)); do
    case "$1" in

        "--all") # builds and runs all targets
            source="all"
            ;;

        "--target="*) # builds and runs specified target
            source=${1#*=}
            opts=( "${@:2}" )
            ;;

        "--silent") # [optional] suppress verbose output
            silent="--silent"
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
