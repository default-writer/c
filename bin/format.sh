#!/usr/bin/env bash
if [[ "${BASHOPTS}" != *extdebug* ]]; then
    set -e
fi

err_report() {
    cd ${source}
    echo "ERROR: $0:$*"
    exit 8
}

if [[ "${BASHOPTS}" != *extdebug* ]]; then
    trap 'err_report $LINENO' ERR
fi

source=$(pwd)

pwd=$(cd "$(dirname $(dirname "${BASH_SOURCE[0]}"))" &> /dev/null && pwd)

cd "${pwd}"


install="$1"

. "${pwd}/bin/scripts/load.sh"

## Formats sources based on provided style guide
## Usage: ${script} <option>
## ${commands}


while (($#)); do
    case "$1" in

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

        "--help") # [optional] shows command description
            help
            ;;

        *)
            help
            ;;

    esac
    shift
done

if [[ "${install}" == "" ]]; then
    help
    exit;
fi

main=$(find "${pwd}/src" -type f -name "*.[c|h]" -exec echo {} \; | grep -v -s "rexo" | sed -n -e 's/^.*\/\(src.*\)$/\1/p')
for i in ${main[@]}; do
    clang-format -i "${pwd}/$i" --style="{BasedOnStyle: ${format}, IndentWidth: 4}"
done

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
