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

pwd=$(cd "$(dirname "$(dirname "${BASH_SOURCE[0]}")")" &> /dev/null && pwd)

cd "${pwd}"

install="$1"

. "${pwd}/bin/scripts/load.sh"

## Builds main binaries using bazel
## Usage: ${script} <option> [optional]
## ${commands}

while (($#)); do
    case "$1" in

        "--all") # builds and runs all targets
            source="all"
            ;;

        "--target="*) # builds and runs specified target
            source=${1#*=}
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
    exit
fi

if [[ "${silent}" == "--silent" ]]; then
    exec >/dev/null 2>&1
fi

bazel build --config=clang_config //main:${source}

if [[ "${silent}" == "--silent" ]]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -eq 2 ]] && echo OK

cd "${pwd}"
