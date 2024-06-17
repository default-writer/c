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

uid=$(id -u)

install="$1"

. "${pwd}/bin/scripts/load.sh"

if [[ "${install}" == "--help" ]]; then
    help
    exit
fi

## Builds binaries and creates coverage info
## Usage: ${script} <option> [optional]
## ${commands}

while (($#)); do
    case "$1" in

        "--all") # builds and runs all targets
            source="all"
            ;;

        "--target="*) # builds and runs specified target
            source=${1#*=}
            ;;

        "--silent") # [optional] suppress verbose output
            silent="--silent"
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

if [[ "${silent}" == "--silent" ]]; then
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

if [[ "${silent}" == "--silent" ]]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
