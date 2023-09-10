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

## Installs dependecies and hooks
## Usage: ${script} <option> [optional]
## ${commands}

case "${install}" in

    "")
        source="all"
        ;;

    "--all") # installs everything
        source="all"
        ;;

    "--hooks") # installs hooks
        hooks="--hooks"
        ;;

    "--clangd") # installs clangd
        clangd="--clangd"
        ;;

    "--cmake") # installs cmake
        cmake="--cmake"
        ;;

    "--submodule-musl") # installs musl
        musl="--submodule-musl"
        ;;

    "--submodule-rexo") # installs rexo
        rexo="--submodule-rexo"
        ;;

    "--submodule-vcpkg") # installs vcpkg
        vcpkg="--submodule-vcpkg"
        ;;

    "--submodule-raylib") # installs raylib
        raylib="--submodule-raylib"
        ;;

    "--help") # shows command desctiption
        help
        ;;

    *)
        help
        ;;

esac

if [[ "${source}" == "all" || "${hooks}" == "--hooks" ]]; then
    "${pwd}/bin/utils/install.sh" --hooks
fi
if [[ "${source}" == "all" || "${cclangd}" == "--clangd" ]]; then
    "${pwd}/bin/utils/install.sh" --clangd
fi
if [[ "${source}" == "all" || "${cmake}" == "--cmake" ]]; then
    "${pwd}/bin/utils/install.sh" --cmake
fi
if [[ "${source}" == "all" || "${musl}" == "--submodule-musl" ]]; then
    "${pwd}/bin/utils/install.sh" --submodule-musl
fi
if [[ "${source}" == "all" || "${rexo}" == "--submodule-rexo" ]]; then
    "${pwd}/bin/utils/install.sh" --submodule-rexo
fi
if [[ "${source}" == "all" || "${vcpkg}" == "--submodule-vcpkg" ]]; then
    "${pwd}/bin/utils/install.sh" --submodule-vcpkg
fi
if [[ "${source}" == "all" || "${raylib}" == "--submodule-raylib" ]]; then
    "${pwd}/bin/utils/install.sh" --submodule-raylib
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
