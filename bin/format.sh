#!/usr/bin/env bash
set -e

uid=$(id -u)

if [ "${uid}" -eq 0 ]; then
    echo "Please run as user"
    exit
fi

pwd=$(pwd)

array="undefined"
clean="undefined"

install="$1"

case "${install}" in

    "--llvm") # A style complying with the LLVM coding standards
        array=("LLVM")
        ;;

    "--google") # A style complying with Google’s C++ style guide
        array=("Google")
        ;;

    "--chromium") # A style complying with Chromium’s style guide
        array=("Chromium")
        ;;

    "--mozilla") # A style complying with Mozilla’s style guide
        array=("Mozilla")
        ;;

    "--webkit") # A style complying with WebKit’s style guide
        array=("WebKit")
        ;;

    "--microsoft") # A style complying with Microsoft’s style guide
        array=("Microsoft")
        ;;

    "--gnu") # A style complying with the GNU coding standards
        array=("GNU")
        ;;
 
    "--inherit-parent-config") # Not a real style, but allows to use the .clang-format file from the parent directory
        array=("InheritParentConfig")
        ;;

    *)
        commands=$(cat $0 | sed -e 's/^[ \t]*//;' | sed -e '/^[ \t]*$/d' | sed -n -e 's/^"\(.*\)".*#/    \1:/p' | sed -n -e 's/: /:\n        /p')
        script="$(basename "$(test -L "$0" && readlink "$0" || echo "$0")")"
        help=$(\
cat << EOF
Formats sources based on provided style guide
Usage: ${script} <option>
${commands}
EOF
)
        echo "${help}"
        exit
        ;;

esac

main=$(find "${pwd}/src" -name "*.[c|h]" -exec echo {} \; | grep -v -s "rexo" | sed -n -e 's/^.*\/\(src.*\)$/\1/p')
for i in $main; do
    echo "formatting ${pwd}/$i"
    clang-format -i "${pwd}/$i" --style="{BasedOnStyle: ${array}, IndentWidth: 4}"
done

cd "${pwd}"
