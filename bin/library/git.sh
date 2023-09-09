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

function submodule-install() {
    [ -d $2 ] && rm -rf $2
    git submodule add -f $1 $2 # &>/dev/null || print
    git submodule init # || print
    git submodule update --recursive --remote # || print
    git pull --recurse-submodules . --quiet # || print
}

function submodule-uninstall() {
    [ -d $2 ] && rm -rf $2
    git submodule deinit -f $2 &>/dev/null || print
    rm -rf .git/modules/$2 &>/dev/null || print
    git rm -f $2 &>/dev/null || print
    rm -rf $1/$2 || print
}

export -f submodule-install
export -f submodule-uninstall
