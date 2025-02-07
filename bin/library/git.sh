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

    # Navigate to the submodule directory
    cd "${pwd}/$2"

    # Check for local changes
    if ! git diff-index --quiet HEAD --; then
        echo "Local changes detected in submodule '$2'. Resetting to remote state..."
        git fetch origin
        git reset --hard origin/master  # Replace 'main' with the correct branch if needed
    fi

    # Return to the main repository
    cd "${pwd}"

    # Pull with rebase
    git pull origin main --recurse-submodules --rebase --force
}

function submodule-uninstall() {
    local pwd

    pwd=$(get-cwd)

    if [[ -d "${pwd}/.git/modules/$2" ]]; then
        git submodule deinit -f "$2"
        rm -rf "${pwd}/.git/modules/$2"
    fi

    if [[ -d "${pwd}/$2" ]]; then
        rm -rf "${pwd}/$2"
    fi
}

export -f submodule-install
export -f submodule-uninstall
