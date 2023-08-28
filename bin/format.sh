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

uid=$(id -u)

if [ "${uid}" -eq 0 ]; then
    echo "Please run as user"
    exit
fi

pwd=$(cd "$(dirname $(dirname "${BASH_SOURCE[0]}"))" &> /dev/null && pwd)

install="$1"

. "${pwd}/bin/scripts/load.sh"

## Formats sources based on provided style guide
## Usage: ${script} <option>
## ${commands}

case "${install}" in

    "--llvm") # A style complying with the LLVM coding standards
        format=("LLVM")
        ;;

    "--google") # A style complying with Google’s C++ style guide
        format=("Google")
        ;;

    "--chromium") # A style complying with Chromium’s style guide
        format=("Chromium")
        ;;

    "--mozilla") # A style complying with Mozilla’s style guide
        format=("Mozilla")
        ;;

    "--webkit") # A style complying with WebKit’s style guide
        format=("WebKit")
        ;;

    "--microsoft") # A style complying with Microsoft’s style guide
        format=("Microsoft")
        ;;

    "--gnu") # A style complying with the GNU coding standards
        format=("GNU")
        ;;
 
    "--clang-format") # Use the .clang-format file from the parent directory
        format=("InheritParentConfig")
        ;;

    "--help") # [optional] shows command desctiption
        help
        ;;

    *)
        help
        ;;

esac

main=$(find "${pwd}/src" -type f -name "*.[c|h]" -exec echo {} \; | grep -v -s "rexo" | sed -n -e 's/^.*\/\(src.*\)$/\1/p')
for i in ${main[@]}; do
    clang-format -i "${pwd}/$i" --style="{BasedOnStyle: ${format}, IndentWidth: 4}"
done

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
