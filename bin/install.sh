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

"${pwd}/bin/utils/install.sh" --hooks
"${pwd}/bin/utils/install.sh" --clangd
"${pwd}/bin/utils/install.sh" --cmake
"${pwd}/bin/utils/install.sh" --submodule-musl
"${pwd}/bin/utils/install.sh" --submodule-rexo
"${pwd}/bin/utils/install.sh" --submodule-vcpkg
"${pwd}/bin/utils/install.sh" --submodule-raylib

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
