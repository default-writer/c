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

opts=( "${@:2}" )

. "${pwd}/bin/scripts/load.sh"

## Builds binaries
## Usage: ${script} <option> [optional]
## ${commands}

case "${install}" in

    "")
        target="--all"
        ;;

    "--target") # builds and runs specified target
        opts=( "${@:3}" )
        target="--target $2"
        ;;

    "--all") # builds and runs all targets
        target="--all"
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

        "--help") # shows command desctiption
            help
            ;;

        *)
            help
            ;;

    esac
done

if [[ "${silent}" == "--silent" ]]; then
    exec 2>&1 >/dev/null
fi

"${pwd}/bin/build.sh" ${target} --gc --clean ${silent}
"${pwd}/bin/build.sh" ${target} --gc --clean --sanitize ${silent}
"${pwd}/bin/build.sh" ${target} --gc --clean --valgrind ${silent}
"${pwd}/bin/build.sh" ${target} --clean ${silent}
"${pwd}/bin/build.sh" ${target} --clean --sanitize ${silent}
"${pwd}/bin/build.sh" ${target} --clean --valgrind ${silent}

if [ "${silent}" == "--silent" ]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
