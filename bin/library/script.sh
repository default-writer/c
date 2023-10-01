#!/usr/bin/env bash
if [[ "${BASHOPTS}" != *extdebug* ]]; then
    set -e
fi

err_report() {
    echo "ERROR: $0:$*"
    exit 8
}

if [[ "${BASHOPTS}" != *extdebug* ]]; then
    trap 'err_report $LINENO' ERR
fi

source=$(pwd)

pwd=$(cd "$(dirname $(dirname $(dirname "${BASH_SOURCE[0]}")))" &> /dev/null && pwd)

function help() {
    local source=$0
    local script="$(basename "$(test -L "${source}" && readlink "${source}" || echo "${source}")")"
    local commands=$(cat ${source} | sed -e 's/^[ \t]*//;' | sed -e '/^[ \t]*$/d' | sed -n -e 's/^"\(.*\)".*#/    \1:/p' | sed -n -e 's/: /:\n        /p')
    echo "$(cat ${source} | sed -e 's/^[ \t]*//;' | sed -e '/^[ \t]*$/d' | sed -n -e 's/^## \(.*\).*/\1/p' | script=""${script}"" commands=""${commands}"" envsubst)"
    exit 8
}

export -f help
