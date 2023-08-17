#!/usr/bin/env bash
set -e

err_report() {
    echo "ERROR: on line $*: $(cat $0 | sed $1!d)" >&2
}

trap 'err_report $LINENO' ERR

function get-scripts() {
    local scripts=$(find "${pwd}/bin/library" -type f -name "*.sh" -exec echo {} \;)
    for i in ${scripts[@]}; do
        local import="$(echo $i | sed -n -e 's/^.*bin\/library\/\(.*\)$/\1/p')"
        . "${pwd}/bin/library/${import}"
    done
}

get-scripts
