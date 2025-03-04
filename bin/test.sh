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

## Builds and tests binaries
## Usage: ${script} <option>
## ${commands}
## 
## Example .args file:
## --all
## --clean
## --silent
## --sanitize
## --gc

while (($#)); do
    case "$1" in

        "--args") # builds and runs with args from .args file
            source=".args"
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

args=$(get-args)

if [[ -f "${source}" ]]; then
    "${pwd}/bin/build.sh" ${args}
else
    echo "ERROR: ${source} not found"
fi

if [[ "${silent}" == "--silent" ]]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -eq 2 ]] && echo OK

cd "${pwd}"
