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

## Extracts library info
## Usage: ${script} <option> [optional]
## ${commands}

while (($#)); do
    case "$1" in

        "--help") # [optional] shows command description
            help
            ;;

        "--nm-csys") # [optional] extracts sys library info
            echo "--------libcsys.so--------"
            nm -D -l "${pwd}/lib/libcsys.so"
            ;;

        "--nm-cvm") # [optional] extracts vm library info
            echo "--------libctypes.so--------"
            nm -D -l "${pwd}/lib/libcvm.so"
            ;;

        "--objdump-csys") # [optional] extracts sys library info
            echo "--------libcsys.so--------"
            objdump -T "${pwd}/lib/libcsys.so"
            ;;

        "--objdump-cvm") # [optional] extracts vm library info
            echo "--------libctypes.so--------"
            objdump -T "${pwd}/lib/libcvm.so"
            ;;

        "--branch-change-main-to-master") # [optional] changes branching strategy to master
            branch-change-main-to-master
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

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
