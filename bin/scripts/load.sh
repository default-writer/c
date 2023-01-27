#!/usr/bin/env bash
set -e

err_report() {
    echo "ERROR: on line $*: $(cat $0 | sed $1!d)" >&2
}

trap 'err_report $LINENO' ERR

function load-libs() {
    local libs=$(find "${pwd}/bin/libs" -type f -name "*.sh" -exec echo {} \;)
    for i in $libs; do
        local import="$(echo $i | sed -n -e 's/^.*bin\/libs\/\(.*\)$/\1/p')"
        . "${pwd}/bin/libs/${import}"
    done
}

load-libs
