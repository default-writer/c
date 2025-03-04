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

function branch-change-main-to-master() {
  git branch -m main master
  git fetch origin
  git branch -u origin/master master
  git remote set-head origin -a
}

function submodule-install() {
    local pwd

    pwd=$(get-cwd)

    if [[ ! -d  "${pwd}/.git/modules/$2" ]]; then
        git submodule add -f "$1" "$2"
        git submodule update --init --recursive --remote
    fi

    # Navigate to the submodule directory
    cd "${pwd}/$2"

    # Check for local changes
    if ! git diff-index --quiet HEAD --; then
        echo "Local changes detected in submodule '$2'. Resetting to remote state..."
        git fetch origin
        git reset --hard # Replace 'master' with the correct branch if needed
    fi

    # Return to the main repository
    cd "${pwd}"

    # sync
    git submodule sync --recursive

    # Pull with rebase
    git pull origin --recurse-submodules --force
}

function submodule-uninstall() {
    local pwd

    pwd=$(get-cwd)

    if [[ -d "${pwd}/$2" ]]; then
        git submodule deinit -f "$2"
        git rm -f "$2"
    fi

    if [[ -d "${pwd}/.git/modules/$2" ]]; then
        rm -rf "${pwd}/.git/modules/$2"

    fi

    if [[ -d "${pwd}/$2" ]]; then
        rm -rf "${pwd}/$2"
    fi
}

export -f branch-change-main-to-master
export -f submodule-install
export -f submodule-uninstall
