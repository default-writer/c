#!/usr/bin/env bash
if [[ "${BASHOPTS}" != *extdebug* ]]; then
    set -e
fi

err_report() {
    echo "ERROR: $0:$*"
    exit 8
}

print() {
    echo "$0:$*"
}

if [[ "${BASHOPTS}" != *extdebug* ]]; then
    trap 'err_report $LINENO' ERR
fi

source=$(pwd)

pwd=$(cd "$(dirname $(dirname $(dirname "${BASH_SOURCE[0]}")))" &> /dev/null && pwd)

function submodule-install() {
    if [ -d "$2" ]; then
        rm -rf "$2" || { echo "Failed to remove directory: $2"; exit 8; }
    fi

    git submodule add -f "$1" "$2" &>/dev/null || { echo "Failed to add submodule: $1"; exit 8; }
    git submodule init || { echo "Failed to initialize submodule"; exit 8; }
    git submodule update --recursive --remote || { echo "Failed to update submodule"; exit 8; }
    git pull --recurse-submodules --quiet || { echo "Failed to pull with submodules"; exit 8; }
}

function submodule-uninstall() {
    if [ -d "$2" ]; then
        rm -rf "$2" || { echo "Failed to remove directory: $2"; exit 8; }
    fi
    git submodule deinit -f "$2" &>/dev/null || { echo "Failed to deinitialize submodule: $2"; exit 8; }
    rm -rf ".git/modules/$2" &>/dev/null || { echo "Failed to remove .git/modules/$2"; exit 8; }
    git rm -f "$2" &>/dev/null || { echo "Failed to remove submodule from index: $2"; exit 8; }
    rm -rf "$1/$2" || { echo "Failed to remove directory: $1/$2"; exit 8; }
}

export -f submodule-install
export -f submodule-uninstall
