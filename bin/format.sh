#!/usr/bin/env bash
set -e

err_report() {
    echo "ERROR: on line $*: $(cat $0 | sed $1!d)" >&2
}

trap 'err_report $LINENO' ERR

uid=$(id -u)

if [ "${uid}" -eq 0 ]; then
    echo "Please run as user"
    exit
fi

pwd=$(pwd)

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
 
    "--all") # Use the .clang-format file from the parent directory
        format=("InheritParentConfig")
        ;;

    "--help") # Use the .clang-format file from the parent directory
        help
        ;;

    *)
        help
        ;;

esac

main=$(find "${pwd}/src" -type f -name "*.[c|h]" -exec echo {} \; | grep -v -s "rexo" | sed -n -e 's/^.*\/\(src.*\)$/\1/p')
for i in $main; do
    clang-format -i "${pwd}/$i" --style="{BasedOnStyle: ${format}, IndentWidth: 4}"
done

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
