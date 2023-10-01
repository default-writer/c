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

. "${pwd}/bin/scripts/load.sh"

## Installs project dependencies
## Usage: ${script} <option> [optional]
## ${commands}

while (($#)); do
    case "$1" in

        "")
            init="--init"
            ;;

        "--init") # initializes project dependencies
            init="--init"
            ;;
     
        "--clean") # cleans up directories before build
            clean="--clean"
            ;;

        "--setup") # installs required dependencies setup
            setup="--setup"
            ;;

        "--hooks") # installs git hooks
            hooks="--hooks"
            ;;

        "--optional") # installs optional dependencies
            optional="--optional"
            ;;

        "--silent") # suppress verbose output
            silent="--silent"
            shift
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

if [[ "${silent}" == "--silent" ]]; then
    exec 2>&1 >/dev/null
fi

if [[ "${clean}" == "--clean" ]]; then
    "${pwd}/bin/utils/cleanup.sh"
fi

if [[ "${setup}" == "--setup" ]]; then
    sudo "${pwd}/bin/setup.sh"
fi

if [[ "${hooks}" == "--hooks" ]]; then
    "${pwd}/bin/utils/install.sh" --hooks
fi

if [[ "${optional}" == "--optional" ]]; then
    "${pwd}/bin/utils/install.sh" --clangd
    "${pwd}/bin/utils/install.sh" --cmake
    "${pwd}/bin/utils/install.sh" --submodule-rexo
fi

if [[ "${init}" == "--init" ]]; then
    "${pwd}/bin/utils/init.sh" --all
fi

if [[ "${silent}" == "--silent" ]]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
