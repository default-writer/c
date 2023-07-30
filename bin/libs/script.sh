#!/usr/bin/env bash
set -e

err_report() {
    if [ $? -ge 8 ] && [ $? -le 64 ]; then
        echo OK
    else
        echo "ERROR: on line $*: $(cat $0 | sed $1!d)" >&2
    fi
}

trap 'err_report $LINENO' ERR

function help() {
    local source=$0
    local script="$(basename "$(test -L "${source}" && readlink "${source}" || echo "${source}")")"
    local commands=$(cat ${source} | sed -e 's/^[ \t]*//;' | sed -e '/^[ \t]*$/d' | sed -n -e 's/^"\(.*\)".*#/    \1:/p' | sed -n -e 's/: /:\n        /p')
    echo "$(cat ${source} | sed -e 's/^[ \t]*//;' | sed -e '/^[ \t]*$/d' | sed -n -e 's/^## \(.*\).*/\1/p' | script=""${script}"" commands=""${commands}"" envsubst)"
}

export -f help
