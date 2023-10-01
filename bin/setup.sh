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

if [ ! "${uid}" -eq 0 ]; then
    echo "Please run as root"
    exit
fi

"${pwd}/bin/utils/setup.sh" --update
"${pwd}/bin/utils/setup.sh" --curl-openssl --no-update --no-upgrade
"${pwd}/bin/utils/setup.sh" --zip --no-update --no-upgrade
"${pwd}/bin/utils/setup.sh" --configuration --no-update --no-upgrade
"${pwd}/bin/utils/setup.sh" --clang-format --no-update --no-upgrade
"${pwd}/bin/utils/setup.sh" --cmake --no-update --no-upgrade
"${pwd}/bin/utils/setup.sh" --gtk4 --no-update --no-upgrade
"${pwd}/bin/utils/setup.sh" --nodejs --no-update --no-upgrade
"${pwd}/bin/utils/setup.sh" --raylib --no-update --no-upgrade
"${pwd}/bin/utils/setup.sh" --doxygen --no-update --no-upgrade
"${pwd}/bin/utils/setup.sh" --upgrade

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
