#!/usr/bin/env bash
if [[ "${BASHOPTS}" != *extdebug* ]]; then
    set -e
fi

function get_stack () {
   STACK=""
   local i message="${1:-""}"
   local stack_size=${#FUNCNAME[@]}
   # to avoid noise we start with 1 to skip the get_stack function
   for (( i=1; i<stack_size; i++ )); do
      local func="${FUNCNAME[$i]}"
      [[ $func = "" ]] && func=MAIN
      local linen="${BASH_LINENO[$(( i - 1 ))]}"
      local src="${BASH_SOURCE[$i]}"
      [[ "$src" = "" ]] && src=non_file_source

      STACK+=$'\n'"   at: $func $src "$linen
   done
   STACK="${message}${STACK}"
}

err_report() {
    cd ${source}
    get_stack
    echo "ERROR: on line $*: $(cat $(test -L "$0" && readlink "$0" || echo $0) | sed $1!d)" >&2
    exit 8
}

if [[ "${BASHOPTS}" != *extdebug* ]]; then
    trap 'err_report $LINENO' ERR
fi

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
