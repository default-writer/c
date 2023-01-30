#!/usr/bin/env bash
set -e

err_report() {
    echo "ERROR: on line $*: $(cat $0 | sed $1!d)" >&2
}

trap 'err_report $LINENO' ERR

uid=$(id -u)

if [ "${uid}" -eq 0 ]; then
    echo "Please run as user"
    exit
fi

pwd=$(pwd)

install="$1"

. "${pwd}/bin/scripts/load.sh"

## Uninstalls optional dependencies
## Usage: ${script} <option>
## ${commands}

case "${install}" in

    "--clangd") # uninstalls clangd 15.0.6
        rm -rf "${pwd}/clangd"
        ;;

    "--git") # unsets git config global environment variables
        git config --global --unset safe.directory
        git config --global --unset pull.rebase
        ;;

    "--submodule-rexo") # deinits git submodule rexo and cleans up rexo directories
        submodule-uninstall ${pwd} src/rexo
        ;;

    "--submodule-lldb-mi") # installs lldb-mi as git submodule
        submodule-uninstall ${pwd} src/lldb-mi
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

    *)
        help
        ;;

esac

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
