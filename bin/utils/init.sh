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

if [ -f "${pwd}/.args" ]; then args=$(cat "${pwd}/.args"); fi

"${pwd}/bin/format.sh" --all
"${pwd}/bin/config.sh" $args
"${pwd}/bin/cmake.sh" $args
"${pwd}/bin/build.sh" $args
"${pwd}/bin/logs.sh" $args
"${pwd}/bin/coverage.sh" $args

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
