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

## Installs codacity binaries and sets environment variables
## Usage: ${script} <option> [optional]
## ${commands}

while (($#)); do
    case "$1" in

        "--coverage") # runs and generates code coverage for all targets
            coverage="--coverage"
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

if [[ "${coverage}" == "--coverage" ]]; then

    "${pwd}/bin/coverageall.sh" --all --clean

    gcovr -r . --txt-metric branch --sonarqube coverage/sonarqube.xml --gcov-exclude="rexo.h"

    if [[ -f "${pwd}/coverage/lcov.info" ]]; then
        bash <(curl -Ls https://coverage.codacy.com/get.sh) report -r "${pwd}/coverage/lcov.info"
    fi
fi

if [[ "${install}" == "" ]]; then
    help
    exit
fi

[[ $SHLVL -eq 2 ]] && echo OK

cd "${pwd}"
