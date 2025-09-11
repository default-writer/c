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

        "--optional") # [optional] installs optional dependencies
            optional="--optional"
            ;;            
     
        "--clean") # cleans up git directory
            "${pwd}/bin/utils/git.sh" --clean
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

        "--python") # installs python .venv
            "${pwd}/bin/utils/install.sh" --python
            ;;

        "--submodule-rexo") # installs rexo
            "${pwd}/bin/utils/install.sh" --submodule-rexo
            ;;

        "--lcov") # installs lcov
            lcov="--lcov"
            ;;

        "--setup") # installs required dependencies setup
            setup="--setup"
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
    exit
fi

if [[ "${silent}" == "--silent" ]]; then
    exec >/dev/null 2>&1
fi

if [[ "${clean}" == "--clean" ]]; then
    "${pwd}/bin/utils/cleanup.sh" --all
fi

if [[ "${setup}" == "--setup" ]]; then
    sudo "${pwd}/bin/setup.sh" ${updateflags} ${updgradeflags} ${optional}
fi

if [[ "${env}" == "--env" ]]; then
    ${pwd}/bin/utils/env.sh --python
    source ${pwd}/bin/env.sh
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
    "${pwd}/bin/utils/init.sh" --init ${updateflags} ${updgradeflags} ${optional}
fi

[[ $SHLVL -eq 2 ]] && echo OK

cd "${pwd}"
