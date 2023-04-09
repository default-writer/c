#!/usr/bin/env bash
set -e

err_report() {
    echo "ERROR: on line $*: $(cat $0 | sed $1!d)" >&2
}

trap 'err_report $LINENO' ERR

function update() {
    apt install -y --fix-broken
    apt update -y --fix-missing
    dpkg --configure -a
    export DEBIAN_FRONTEND=noninteractive
    set -a && eval "$(sudo tee --append /etc/environment <<<'DEBIAN_FRONTEND=noninteractive')" && set +a
}

function upgrade() {
    update
    apt upgrade -y
    apt autoremove -y
    apt full-upgrade -y
}

export -f update
export -f upgrade

