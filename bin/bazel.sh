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

uid=$(id -u)

if [ "${uid}" -eq 0 ]; then
    echo "Please run as user"
    exit
fi

install="$1"

opts=( "${@:2}" )

. "${pwd}/bin/scripts/load.sh"

## Builds main binaries using bazel
## Usage: ${script} <option> [optional]
## ${commands}

case "${install}" in

    "")
        source="all"
        ;;

    "--target") # builds and runs specified target
        source="$2"
        opts=( "${@:3}" )
        ;;

    "--help") # [optional] shows command description
        help
        ;;

    *)
        help
        ;;

esac

for opt in ${opts[@]}; do
    case ${opt} in

        "")
            ;;

        "--silent") # [optional] suppress verbose output
            silent="--silent"
            ;;

        *)
            help
            ;;

    esac
done

if [[ "${silent}" == "--silent" ]]; then
    exec 2>&1 >/dev/null
fi

bazel build --config=clang_config //main:${source}

if [[ "${silent}" == "--silent" ]]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
