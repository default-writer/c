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

        "--c-sys") # [optional] extracts sys library info
            echo "--------libcsys.so--------"
            nm -D -l "${pwd}/lib/libc-sys.so" | sort
            echo "--------libcsys.so--------"
            objdump -T "${pwd}/lib/libc-sys.so" | grep "DF \*UND\*" | sort
            echo "--------libcsys.so--------"
            objdump -T "${pwd}/lib/libc-sys.so" | grep ".data.rel.ro" | sort
            echo "--------libcsys.so--------"
            objdump -p "${pwd}/lib/libc-sys.so" | grep "NEEDED" | sort
            echo "--------libcsys.so--------"
            file -b "${pwd}/lib/libc-sys.so"
            ;;

        "--c-vm") # [optional] extracts vm library info
            echo "--------libc-vm.so--------"
            nm -D -l "${pwd}/lib/libc-vm.so" | sort
            echo "--------libc-vm.so--------"
            objdump -T "${pwd}/lib/libc-vm.so" | grep "DF \*UND\*" | sort
            echo "--------libc-vm.so--------"
            objdump -T "${pwd}/lib/libc-vm.so" | grep ".data.rel.ro" | sort
            echo "--------libc-vm.so--------"
            objdump -p "${pwd}/lib/libc-vm.so" | grep "NEEDED" | sort
            echo "--------libc-vm.so--------"
            file -b "${pwd}/lib/libc-vm.so"
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

[[ $SHLVL -eq 2 ]] && echo OK

cd "${pwd}"
