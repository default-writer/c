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

. "${pwd}/bin/scripts/load.sh"

init="--init"

## Installs project dependencies
## Usage: ${script} <option> [optional]
## ${commands}

while (($#)); do
    case "$1" in

        "--init") # initializes project dependencies
            ;;

        "--no-update") # [optional] skips system updates
            updateflags="--no-update"
            ;;

        "--no-upgrade") # [optional] skips system upgrades
            updgradeflags="--no-upgrade"
            ;;
     
        "--clean") # cleans up directories before build
            "${pwd}/bin/utils/cleanup.sh" --all
            ;;

        "--setup") # installs required dependencies setup
            setup="--setup"
            ;;

        "--hooks") # installs git hooks
            "${pwd}/bin/utils/install.sh" --hooks
            ;;

        "--clangd") # installs clangd
            "${pwd}/bin/utils/install.sh" --clangd
            ;;

        "--cmake") # installs cmake
            "${pwd}/bin/utils/install.sh" --cmake
            ;;

        "--submodule-rexo") # installs rexo
            "${pwd}/bin/utils/install.sh" --submodule-rexo
            ;;

        "--optional") # installs optional dependencies
            ;;

        "--lcov") # installs lcov
            sudo "${pwd}/bin/utils/install.sh" ${updateflags} ${updgradeflags} --lcov
            ;;

        "--setup") # setup requrements
            sudo "${pwd}/bin/setup.sh"
            help
            ;;

        "--help") # shows command description
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

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
