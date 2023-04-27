#!/usr/bin/env bash
set -e

err_report() {
    echo "ERROR: on line $*: $(cat $0 | sed $1!d)" >&2
}

trap 'err_report $LINENO' ERR

uid=$(id -u)

pwd=$(pwd)

"${pwd}/bin/utils/install.sh" --git
"${pwd}/bin/utils/install.sh" --hooks

"${pwd}/bin/cleanup.sh" --all
"${pwd}/bin/utils/install.sh" --clangd
"${pwd}/bin/utils/install.sh" --cmake
"${pwd}/bin/utils/install.sh" --submodule-rexo
"${pwd}/bin/utils/install.sh" --submodule-vcpkg

sudo "${pwd}/bin/utils/setup.sh" --configuration
sudo "${pwd}/bin/utils/setup.sh" --clang-format
sudo "${pwd}/bin/utils/setup.sh" --cmake
sudo "${pwd}/bin/utils/setup.sh" --gtk4

"${pwd}/bin/utils/runme.sh"

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
