#!/usr/bin/env bash
set -e

err_report() {
    echo "ERROR: on line $*: $(cat $0 | sed $1!d)" >&2
}

trap 'err_report $LINENO' ERR

function update() {
    apt install -y --fix-broken
    apt update -y --fix-missing
}

function upgrade() {
    update
    apt upgrade -y
    apt autoremove -y
    apt full-upgrade -y
}

export -f update
export -f upgrade

