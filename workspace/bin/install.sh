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

install="$1"

. "${pwd}/bin/scripts/load.sh"

## Installs dependecies and hooks
## Usage: ${script} <option> [optional]
## ${commands}

while (($#)); do
    case "$1" in

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

        "--submodule-rexo") # installs rexo
            rexo="--submodule-rexo"
            ;;

        "--submodule-musl") # installs musl
            musl="--submodule-musl"
            ;;

        "--submodule-vcpkg") # installs vcpkg
            vcpkg="--submodule-vcpkg"
            ;;

        "--submodule-raylib") # installs raylib
            raylib="--submodule-raylib"
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

if [[ "${source}" == "all" || "${hooks}" == "--hooks" ]]; then
    "${pwd}/bin/utils/install.sh" --hooks
fi
if [[ "${source}" == "all" || "${clangd}" == "--clangd" ]]; then
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