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

opts=()

. "${pwd}/bin/scripts/load.sh"

init=""

## Installs project dependencies
## Usage: ${script} <option> [optional]
## ${commands}

while (($#)); do
    case "$1" in

        "--init") # initializes project dependencies
            init="--init"
            opts+=( "$1" )
            ;;

        "--no-update") # [optional] skips system updates
            updateflags="--no-update"
            opts+=( "$1" )
            ;;

        "--no-upgrade") # [optional] skips system upgrades
            updgradeflags="--no-upgrade"
            opts+=( "$1" )
            ;;
     
        "--clean") # cleans up directories before build
            clean="--clean"
            opts+=( "$1" )
            ;;

        "--setup") # installs required dependencies setup
            setup="--setup"
            opts+=( "$1" )
            ;;

        "--hooks") # installs git hooks
            hooks="--hooks"
            opts+=( "$1" )
            ;;

        "--clangd") # installs clangd
            clangd="--clangd"
            opts+=( "$1" )
            ;;

        "--lcov") # installs lcov
            lcov="--lcov"
            opts+=( "$1" )
            ;;

        "--cmake") # installs cmake
            cmake="--cmake"
            opts+=( "$1" )
            ;;

        "--submodule-rexo") # installs rexo
            rexo="--submodule-rexo"
            opts+=( "$1" )
            ;;

        "--optional") # installs optional dependencies
            optional="--optional"
            opts+=( "$1" )
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

if [[ "${init}" == "" ]]; then
    help
    exit;
fi

if [[ "${hooks}" == "--hooks" ]]; then
    "${pwd}/bin/utils/install.sh" --hooks
fi

if [[ "${init}" == "--init" ]]; then
    "${pwd}/bin/utils/install.sh" --submodule-rexo
fi

if [[ "${clean}" == "--clean" ]]; then
    "${pwd}/bin/utils/cleanup.sh" --all
fi

if [[ "${cmake}" == "--cmake" ]]; then
    "${pwd}/bin/utils/install.sh" --cmake
fi

if [[ "${clangd}" == "--clangd" ]]; then
    "${pwd}/bin/utils/install.sh" --clangd
fi

if [[ "${lcov}" == "--lcov" ]]; then
   echo sudo "${pwd}/bin/setup.sh" ${updateflags} ${updgradeflags} --lcov
fi

if [[ "${silent}" == "--silent" ]]; then
    exec 1>&2 2>&-
fi

cd "${pwd}"
