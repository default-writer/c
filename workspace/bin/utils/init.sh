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

pwd=$(cd "$(dirname $(dirname $(dirname "${BASH_SOURCE[0]}")))" &> /dev/null && pwd)

if [ -f "${pwd}/.args" ]; then args=$(cat "${pwd}/.args"); fi

"${pwd}/bin/config.sh" $args

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
