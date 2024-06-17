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

uid=$(id -u)

if [ "${uid}" -eq 0 ]; then
    echo "Please run as user"
    exit
fi

source=$(pwd)

pwd=$(cd "$(dirname $(dirname $(dirname "${BASH_SOURCE[0]}")))" &> /dev/null && pwd)

. "${pwd}/bin/scripts/load.sh"

if [ -f "${pwd}/.args" ]; then args=$(get-args); fi

echo "${pwd}/bin/config.sh" $args

"${pwd}/bin/config.sh" $args

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
