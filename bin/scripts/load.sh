#!/usr/bin/env bash
set -e


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
    get_stack
    echo "ERROR: on line $*: $(cat $0 | sed $1!d)" >&2
    exit 8
}

trap 'err_report $LINENO' ERR

function get-scripts() {
    local script
    local scripts
    local import
    scripts=$(find "${pwd}/bin/library" -type f -name "*.sh" -exec echo {} \;)
    for script in ${scripts[@]}; do
        import="$(echo ${script} | sed -n -e 's/^.*bin\/library\/\(.*\)$/\1/p')"
        . "${pwd}/bin/library/${import}"
    done
}

get-scripts
