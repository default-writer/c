#!/usr/bin/env bash
set -e

err_report() {
    echo "ERROR: on line $*: $(cat $0 | sed $1!d)" >&2
}

trap 'err_report $LINENO' ERR

uid=$(id -u)

pwd=$(pwd)

install="$1"

opts=( "${@:2}" )

. "${pwd}/bin/scripts/load.sh"

## Builds binaries and creates coverage info
## Usage: ${script} <option> [optional]
## ${commands}

case "${install}" in

    "")
        source="all"
        ;;

    "--target") # builds and runs specified target
        source="$2"
        opts=( "${@:3}" )
        ;;

    "--all") # builds and runs specified target
        source="all"
        opts=( "${@:2}" )
        ;;

    *)
        help
        ;;

esac

for opt in ${opts[@]}; do
    case ${opt} in

        "")
            ;;

        "--silent") # [optional] suppress verbose output
            silent="--silent"
            ;;

        "--help") # [optional] shows command desctiption
            help
            ;;

        *)
            help
            ;;

    esac
done

if [ "${silent}" == "--silent" ]; then
    exec 2>&1 >/dev/null
fi

if [[ "${source}" == "musl" ]]; then

    if [ ! "${uid}" -eq 0 ]; then
        echo "Please run as root"
        exit
    fi

    cd "${pwd}/.deps/musl"
    ./configure 2>&1
    make
    make install

    cat > hello.c <<EOF
#include <stdio.h>
int main()
{
    printf("hello, world!\n");
    return 0;
}
EOF
    /usr/local/musl/bin/musl-gcc --pipe ./hello.c
    ./a.out
    rm hello.c
    rm ./a.out

fi

if [ "${silent}" == "--silent" ]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
