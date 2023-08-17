#!/usr/bin/env bash
set -e

err_report() {
    echo "ERROR: on line $*: $(cat $0 | sed $1!d)" >&2
}

trap 'err_report $LINENO' ERR

uid=$(id -u)

pwd=$(pwd)

"${pwd}/bin/utils/install.sh" --hooks

sudo "${pwd}/bin/utils/setup.sh" --update
sudo "${pwd}/bin/utils/setup.sh" --zip --no-update --no-upgrade
sudo "${pwd}/bin/utils/setup.sh" --configuration --no-update --no-upgrade
sudo "${pwd}/bin/utils/setup.sh" --clang-format --no-update --no-upgrade
sudo "${pwd}/bin/utils/setup.sh" --cmake --no-update --no-upgrade
sudo "${pwd}/bin/utils/setup.sh" --gtk4 --no-update --no-upgrade
sudo "${pwd}/bin/utils/setup.sh" --nodejs-ppa --no-update --no-upgrade
sudo "${pwd}/bin/utils/setup.sh" --raylib --no-update --no-upgrade
sudo "${pwd}/bin/utils/setup.sh" --doxygen --no-update --no-upgrade
sudo "${pwd}/bin/utils/setup.sh" --upgrade

"${pwd}/bin/cleanup.sh" --all
"${pwd}/bin/utils/install.sh" --clangd
"${pwd}/bin/utils/install.sh" --cmake
"${pwd}/bin/utils/install.sh" --submodule-musl
"${pwd}/bin/utils/install.sh" --submodule-rexo
"${pwd}/bin/utils/install.sh" --submodule-vcpkg
"${pwd}/bin/utils/install.sh" --submodule-raylib
"${pwd}/bin/utils/runme.sh" --all

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
