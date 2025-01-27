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
        git submodule add -f "$1" "$2"
        git submodule init
        git submodule update --recursive --remote
        git pull --recurse-submodules --quiet
    fi
}

function submodule-uninstall() {
    local pwd

    pwd=$(get-cwd)

    if [[ -d  "${pwd}/.git/modules/$2" ]]; then
        git submodule deinit -f "$2"
        rm -rf "${pwd}/.git/modules/$2"
        rm -r "${pwd}/$2"
        git reset --head
    fi
}

export -f submodule-install
export -f submodule-uninstall
