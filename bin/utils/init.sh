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

trap 'get_stack' ERR

uid=$(id -u)

if [ "${uid}" -eq 0 ]; then
    echo "Please run as user"
    exit
fi

if [ -f "$(pwd)/.args" ]; then args=$(cat "$(pwd)/.args"); fi

"$(pwd)/bin/format.sh" --clang-format
"$(pwd)/bin/config.sh" $args

# "$(pwd)/bin/cmake.sh"  $args
# "$(pwd)/bin/logs.sh" $args
# "$(pwd)/bin/build.sh"  --all
# "$(pwd)/bin/coverage.sh" --all

[[ $SHLVL -gt 2 ]] || echo OK

cd "$(pwd)"