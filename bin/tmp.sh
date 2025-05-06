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

pwd="$(cd "$(dirname "$(dirname "${BASH_SOURCE[0]}")")" &> /dev/null && pwd)"

cd "${pwd}"

. "${pwd}/bin/scripts/load.sh"

## Builds and tests binaries to .tmp folder with coverage info
## Usage: ${script} <option>
## ${commands}

while (($#)); do
    case "$1" in

        "--silent") # [optional] suppress verbose output
            silent="--silent"
            ;;

        *)
            help
            ;;

    esac
    shift
done

if [[ "${silent}" == "--silent" ]]; then
    exec >/dev/null 2>&1
fi

"${pwd}/bin/utils/coverage.sh" --target=main-tests-vm1 --clean --dir=.tmp

if [[ -f "${pwd}/.tmp/lcov.info" ]]; then
    cp "${pwd}/.tmp/lcov.info" "${pwd}/coverage/lcov.info"
fi

if [[ "${silent}" == "--silent" ]]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -eq 2 ]] && echo OK

cd "${pwd}"
