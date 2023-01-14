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

"${pwd}/bin/clean.sh" && "${pwd}/bin/build.sh" --all && "${pwd}/bin/coverage.sh" --all

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
