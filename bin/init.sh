#!/usr/bin/env bash
if [[ "${BASHOPTS}" != *extdebug* ]]; then
    set -e
fi

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
    cd ${source}
    get_stack
    echo "ERROR: on line $*: $(cat $(test -L "$0" && readlink "$0" || echo $0) | sed $1!d)" >&2
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

install="$1"

. "${pwd}/bin/scripts/load.sh"

## Installs project dependencies
## Usage: ${script} [optional]
## ${commands}

case "${install}" in

    "")
        ;;

    "--clean") # [optional] cleans up directories before build
        clean="--clean"
        ;;

    "--setup") # [optional] installs required dependencies setup
        setup="--setup"
        ;;

    "--hooks") # [optional] installs git hooks
        hooks="--hooks"
        ;;

    "--optional") # [optional] installs optional dependencies
        optional="--optional"
        ;;

    "--silent") # [optional] suppress verbose output
        silent="--silent"
        ;;

    "--help") # [optional] shows command description
        help
        ;;

    *)
        help
        ;;

esac


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

if [[ "${optional}" == "--optional" ]]; then
    "${pwd}/bin/utils/install.sh" --clangd
    "${pwd}/bin/utils/install.sh" --cmake
    "${pwd}/bin/utils/install.sh" --submodule-rexo
fi

"${pwd}/bin/utils/init.sh" --all

if [[ "${silent}" == "--silent" ]]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
