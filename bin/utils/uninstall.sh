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

pwd=$(cd "$(dirname $(dirname $(dirname "${BASH_SOURCE[0]}")))" &> /dev/null && pwd)

install="$1"

. "${pwd}/bin/scripts/load.sh"

## Uninstalls optional dependencies
## Usage: ${script} <option>
## ${commands}

while (($#)); do
    case "$1" in

        "--clangd") # uninstalls clangd
            rm -rf "${pwd}/.tools/clangd"
            ;;

        "--cmake") # uninstalls cmake cmake 3.25.3
            rm -rf "${pwd}/.tools/cmake-3.25"
            ;;

        "--git") # unsets git config global environment variables
            git config --global --unset safe.directory
            git config --global --unset pull.rebase
            ;;

        "--submodule-rexo") # uninstalls rexo as git submodule
            submodule-uninstall ${pwd} .rexo
            ;;

        "--submodule-musl") # uninstalls muslas git submodule
            submodule-uninstall ${pwd} .deps/musl
            ;;

        "--submodule-vcpkg") # uninstalls vcpkg as git submodule
            submodule-uninstall ${pwd} .deps/vcpkg
            ;;

        "--submodule-raylib") # uninstalls raylib as git submodule
            submodule-uninstall ${pwd} .deps/raylib
            ;;

        "--submodule-lldb-mi") # uninstalls lldb-mi as git submodule
            submodule-uninstall ${pwd} .deps/lldb-mi
            ;;

        "--hooks") # removes prepare-commit-msg hook from .git
            rm -rf .git/hooks/prepare-commit-msg
            ;;

        "--pyenv") # removes .pyenv folder
            rm -rf $HOME/.pyenv
            ;;

        "--devcontainer-extensions") # installs vs code .devcontainers extensions
            code --log error --uninstall-extension ms-vscode-remote.remote-containers &>/dev/null
            ;;

        "--docker-extensions") # installs vs code docker extensions
            code --log error --uninstall-extension ms-azuretools.vscode-docker &>/dev/null
            ;;

        "--marp-extention") # istalls marp vscode extension
            code --log error --uninstall-extension marp-team.marp-vscode &>/dev/null
            ;;

        "--run-on-save") # istalls run on save vsode extension
            code --log error --uninstall-extension emeraldwalk.RunOnSave &>/dev/null
            ;;

        "--help") # [optional] shows command description
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
