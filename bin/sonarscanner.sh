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

## Installs SonarCloud binaries and sets environment variables
## Usage: ${script} <option> [optional]
## ${commands}

while (($#)); do
    case "$1" in

        "--install") # installs and runs all targets
            install="--install"
            ;;

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

if [[ "${install}" == "--install" ]]; then
    ./bin/utils/install.sh --sonar-scanner
    ./bin/utils/env.sh --sonar-scanner
fi

if [[ "${coverage}" == "--coverage" ]]; then

    "${pwd}/bin/coverageall.sh" --all --clean

    gcovr -r . --txt-metric branch --sonarqube coverage/sonarqube.xml --gcov-exclude="rexo.h"

    . "${pwd}/bin/utils/install.sh" --sonar-scanner

    export PATH=$SONAR_SCANNER_HOME/bin:$PATH

    if [[ ! "${SONAR_TOKEN}" == "" ]]; then
        sonar-scanner
    fi
fi

if [[ "${install}" == "" ]]; then
    help
    exit
fi

[[ $SHLVL -eq 2 ]] && echo OK

cd "${pwd}"
