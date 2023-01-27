#!/usr/bin/env bash
set -e

err_report() {
    echo "ERROR: on line $*: $(cat $0 | sed $1!d)" >&2
}

trap 'err_report $LINENO' ERR

function submodule-install() {
    git submodule add -f $1 $2 # &>/dev/null || print
    git submodule init # || print
    git submodule update --recursive --remote # || print
    git pull --recurse-submodules . --quiet # || print
}

function submodule-uninstall() {
    git submodule deinit -f $2 &>/dev/null || print
    rm -rf .git/modules/$2 &>/dev/null || print
    git rm -f $2 &>/dev/null || print
    rm -rf $1/$2 || print
}

export -f submodule-install
export -f submodule-uninstall
