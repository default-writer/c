#!/usr/bin/env bash
set -e

err_report() {
    echo "ERROR: on line $*: $(cat $0 | sed $1!d)" >&2
}

trap 'err_report $LINENO' ERR

uid=$(id -u)

if [ ! "${uid}" -eq 0 ]; then
    echo "Please run as root"
    exit
fi

pwd=$(pwd)

"${pwd}/bin/utils/setup.sh" --update
"${pwd}/bin/utils/setup.sh" --zip --no-update --no-upgrade
"${pwd}/bin/utils/setup.sh" --configuration --no-update --no-upgrade
"${pwd}/bin/utils/setup.sh" --clang-format --no-update --no-upgrade
"${pwd}/bin/utils/setup.sh" --cmake --no-update --no-upgrade
"${pwd}/bin/utils/setup.sh" --gtk4 --no-update --no-upgrade
"${pwd}/bin/utils/setup.sh" --nodejs-ppa --no-update --no-upgrade
"${pwd}/bin/utils/setup.sh" --raylib --no-update --no-upgrade
"${pwd}/bin/utils/setup.sh" --doxygen --no-update --no-upgrade
"${pwd}/bin/utils/setup.sh" --upgrade

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
