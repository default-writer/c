#!/usr/bin/env bash
set -e

err_report() {
    echo "ERROR: on line $*: $(cat $0 | sed $1!d)" >&2
}

trap 'err_report $LINENO' ERR

uid=$(id -u)

if [ "${uid}" -eq 0 ]; then
    echo "Please run as user"
    exit
fi

pwd=$(pwd)

function git-submodule-install() {
    commit=$(git ls-remote $1 | grep -s HEAD | { read _commit_ _; echo $_commit_; })
    export commit="${commit}"
    git submodule add $1 $2 &>/dev/null || print
    git submodule init || print
    git submodule update --recursive --remote || print
    git pull --recurse-submodules . --quiet || print
}

function git-submodule-uninstall() {
    git submodule deinit -f $1 &>/dev/null || print
    rm -rf .git/modules/$1 &>/dev/null || print
    git rm -f $1 &>/dev/null || print
    rm -rf "${pwd}/$1" || print
}

export -f git-submodule-install
export -f git-submodule-uninstall

cd "${pwd}"
