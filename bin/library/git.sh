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
    if [ -d "$2" ]; then
        rm -rf "$2" || { echo "Failed to remove directory: $2"; err_report; }
    fi

    git submodule add -f "$1" "$2" &>/dev/null || { echo "Failed to add submodule: $1"; return 1; }
    git submodule init || { echo "Failed to initialize submodule"; return 1; }
    git submodule update --recursive --remote || { echo "Failed to update submodule"; return 1; }
    git pull --recurse-submodules --quiet || { echo "Failed to pull with submodules"; return 1; }
    if [ ! -d "$2" ]; then
        git add "$2" -f
    fi
}

function submodule-uninstall() {
    git submodule deinit -f "$2" &>/dev/null || { echo "Failed to deinitialize submodule: $2"; return 1; }
    if [[ -d ".git/modules/$2" ]]; then
        rm -rf ".git/modules/$2" &>/dev/null || { echo "Failed to remove .git/modules/$2"; return 1; }
    fi
    if [[ -d "$2" ]]; then
        git rm -f "$2" &>/dev/null || { echo "Failed to remove submodule from index: $2"; return 1; }
    fi
    if [[ -d "$1/$2" ]]; then
        rm -rf "$1/$2" || { echo "Failed to remove directory: $1/$2"; return 1; }
    fi
}

export -f submodule-install
export -f submodule-uninstall
