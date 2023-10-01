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
