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
        git submodule update --recursive --remote --rebase --force
    fi

    # Stash any unstaged or uncommitted changes
    git stash

    # Pull with rebase
    git pull origin main --recurse-submodules --rebase --force

    # Reapply stashed changes (if any)
    git stash pop
}

function submodule-uninstall() {
    local pwd

    pwd=$(get-cwd)

    if [[ -d "${pwd}/$2" ]]; then
        rm -rf "${pwd}/$2"
    fi

    if [[ -d "${pwd}/.git/modules/$2" ]]; then
        git submodule deinit -f "$2"
        rm -rf "${pwd}/.git/modules/$2"
    fi
}

export -f submodule-install
export -f submodule-uninstall
