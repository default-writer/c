#!/usr/bin/env bash
set -e


function get_stack () {
   STACK=""
   local i message="${1:-""}"
   local stack_size=${#FUNCNAME[@]}
   # to avoid noise we start with 1 to skip the get_stack function
   for (( i=1; i<stack_size; i++ )); do
      local func="${FUNCNAME[$i]}"
      [[ $func = "" ]] && func=MAIN
      local linen="${BASH_LINENO[$(( i - 1 ))]}"
      local src="${BASH_SOURCE[$i]}"
      [[ "$src" = "" ]] && src=non_file_source

      STACK+=$'\n'"   at: $func $src "$linen
   done
   STACK="${message}${STACK}"
}

err_report() {
    get_stack
    echo "ERROR: on line $*: $(cat $0 | sed $1!d)" >&2
    exit 8
}

trap 'err_report $LINENO' ERR

uid=$(id -u)

if [ "${uid}" -eq 0 ]; then
    echo "Please run as user"
    exit
fi

pwd=$(cd "$(dirname $(dirname $(dirname "${BASH_SOURCE[0]}")))" &> /dev/null && pwd)

install="$1"

. "${pwd}/bin/scripts/load.sh"

## Uninstalls optional dependencies
## Usage: ${script} <option>
## ${commands}

case "${install}" in

    "--clangd") # uninstalls clangd 16.0.2
        rm -rf "${pwd}/clangd"
        ;;

    "--cmake") # uninstalls cmake cmake 3.25.3
        rm -rf "${pwd}/cmake-3.25"
        ;;

    "--git") # unsets git config global environment variables
        git config --global --unset safe.directory
        git config --global --unset pull.rebase
        ;;

    "--submodule-rexo") # uninstalls rexoas git submodule
        submodule-uninstall ${pwd} .deps/rexo
        ;;

    "--submodule-musl") # uninstalls muslas git submodule
        submodule-uninstall ${pwd} .deps/musl
        ;;

    "--submodule-vcpkg") # uninstalls vcpkg as git submodule
        submodule-uninstall ${pwd} .deps/vcpkg
        ;;

    "--submodule-raylib") # uninstalls rexo as raylib submodule
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

    "--help") # [optional] shows command desctiption
        help
        ;;

    *)
        help
        ;;

esac

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
