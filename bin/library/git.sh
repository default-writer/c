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

function submodule-install() {
    git submodule add -f "$1" "$2" &>/dev/null
    git submodule init
    git submodule update --recursive --remote
    git pull --recurse-submodules --quiet
}

function submodule-uninstall() {
    git submodule deinit -f "$2" &>/dev/null
    rm -rf ".git/modules/$2" &>/dev/null
    rm -rf "$2"
    rm -rf "$1/$2"
}


export -f submodule-install
export -f submodule-uninstall
