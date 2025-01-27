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

function submodule-install() {
    [ -d $2 ] && rm -rf $2
    git submodule add -f $1 $2
    git submodule init
    git submodule update --recursive --remote
    git pull --recurse-submodules $(pwd) --verbose
}

function submodule-uninstall() {
    [ -d $2 ] && rm -rf $2
    git submodule deinit -f $2 &>/dev/null
    rm -rf .git/modules/$2 &>/dev/null
    git rm -f $2 &>/dev/null
    rm -rf $1/$2
}


export -f submodule-install
export -f submodule-uninstall
