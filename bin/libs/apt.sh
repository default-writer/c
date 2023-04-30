#!/usr/bin/env bash
set -e

err_report() {
    echo "ERROR: on line $*: $(cat $0 | sed $1!d)" >&2
}

trap 'err_report $LINENO' ERR

function update() {
    local opts=${@:1}
    for opt in ${opts[@]}; do
        case ${opt} in

            "--no-update")
                ;;

            *)
                apt install -y --fix-broken
                apt update -y --fix-missing
                dpkg --configure -a
                ;;

        esac
    done
    export DEBIAN_FRONTEND=noninteractive
    set -a && eval "$(sudo tee --append /etc/environment <<<'DEBIAN_FRONTEND=noninteractive')" && set +a
}

function upgrade() {
    local opts=${@:1}
    for opt in ${opts[@]}; do
        case ${opt} in

            "--no-upgrade")
                ;;

            *)
                apt upgrade -y
                apt autoremove -y
                apt full-upgrade -y
                ;;

        esac
    done
}

export -f update
export -f upgrade

