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

. "${pwd}/bin/scripts/load.sh"

init="--init"

## Installs project dependencies
## Usage: ${script} <option> [optional]
## ${commands}

while (($#)); do
    case "$1" in

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

        "--clangd") # installs clangd
            clangd="--clangd"
            ;;

        "--lcov") # installs lcov
            lcov="--lcov"
            ;;

        "--cmake") # installs cmake
            cmake="--cmake"
            ;;

        "--submodule-rexo") # installs rexo
            rexo="--submodule-rexo"
            ;;

        "--optional") # installs optional dependencies
            optional="--optional"
            ;;

        "--silent") # suppress verbose output
            silent="--silent"
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

if [[ "${silent}" == "--silent" ]]; then
    exec 2>&1 >/dev/null
fi

if [[ "${clean}" == "--clean" ]]; then
    "${pwd}/bin/utils/cleanup.sh" --all
fi

if [[ "${setup}" == "--setup" ]]; then
    sudo "${pwd}/bin/setup.sh"
fi

if [[ "${hooks}" == "--hooks" ]]; then
    "${pwd}/bin/utils/install.sh" --hooks
fi

if [[ "${clangd}" == "--clangd" ]]; then
    "${pwd}/bin/utils/install.sh" --clangd
fi

if [[ "${lcov}" == "--lcov" ]]; then
    sudo "${pwd}/bin/utils/install.sh" --lcov
fi

if [[ "${cmake}" == "--cmake" ]]; then
    "${pwd}/bin/utils/install.sh" --cmake
fi

if [[ "${init}" == "--init" ]]; then
    "${pwd}/bin/utils/install.sh" --submodule-rexo
    "${pwd}/bin/utils/init.sh" --all
fi

if [[ "${silent}" == "--silent" ]]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
