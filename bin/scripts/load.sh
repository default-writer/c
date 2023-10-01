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

function get-scripts() {
    local script
    local scripts
    local import
    scripts=$(find "${pwd}/bin/library" -type f -name "*.sh" -exec echo {} \;)
    for script in ${scripts[@]}; do
        import="$(echo ${script} | sed -n -e 's/^.*bin\/library\/\(.*\)$/\1/p')"
        . "${pwd}/bin/library/${import}"
    done
}

get-scripts
