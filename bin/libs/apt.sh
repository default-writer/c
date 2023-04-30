#!/usr/bin/env bash
set -e

err_report() {
    echo "ERROR: on line $*: $(cat $0 | sed $1!d)" >&2
}

trap 'err_report $LINENO' ERR

function update() {
    local opts=${@:1}
    local skip
    for opt in ${opts[@]}; do
        case ${opt} in

            "--no-update")
                skip="--skip"
                ;;

            *)
                ;;

        esac
    done

    if [ "${skip}" != "--skip" ]; then
        apt install -y --fix-broken
        apt update -y --fix-missing
        dpkg --configure -a
    fi

    export DEBIAN_FRONTEND=noninteractive
    set -a && eval "$(sudo tee --append /etc/environment <<<'DEBIAN_FRONTEND=noninteractive')" && set +a
}

function upgrade() {
    local opts=${@:1}
    local skip
    for opt in ${opts[@]}; do
        case ${opt} in

            "--no-upgrade")
                skip="--skip"            
                ;;

            *)
                ;;

        esac
    done

    if [ "${skip}" != "--skip" ]; then
        apt upgrade -y
        apt autoremove -y
        apt full-upgrade -y
    fi
}

export -f update
export -f upgrade

