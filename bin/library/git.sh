#!/usr/bin/env bash
if [[ "${BASHOPTS}" != *extdebug* ]]; then
    set -e
fi

err_report() {
    echo "ERROR: $0:$*"
    exit 8
}

pwd=$(pwd)

if [[ "${BASHOPTS}" != *extdebug* ]]; then
    trap 'err_report $LINENO' ERR
fi

function submodule-install() {
    local pwd

    pwd=$(get-cwd)

    if [[ ! -d  "${pwd}/.git/modules/$2" ]]; then
        git submodule add -f "$1" "$2" 2>&1 >/dev/null
        git submodule init 2>&1 >/dev/null
        git submodule update --recursive --remote 2>&1 >/dev/null
        git pull origin main --recurse-submodules --quiet 2>&1 >/dev/null
    fi
}

function submodule-uninstall() {
    local pwd

    pwd=$(get-cwd)

    if [[ -d  "${pwd}/.git/modules/$2" ]]; then
        git submodule deinit -f "$2" 2>&1 >/dev/null
        rm -rf "${pwd}/.git/modules/$2" 2>&1 >/dev/null
        rm -r "${pwd}/$2" 2>&1 >/dev/null
    fi
}

export -f submodule-install
export -f submodule-uninstall
