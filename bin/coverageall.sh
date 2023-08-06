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

        "--clean") # [optional] cleans up directories before coverage
            clean="--clean"
            ;;

        "--global") # [optional] merge to main output
            global="--global"
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

if [[ "${silent}" == "--silent" ]]; then
    exec 2>&1 >/dev/null
fi

"${pwd}/bin/coverage.sh" --target ${source} --dir=coverage-v1 --valgrind ${silent} ${opts[@]}
"${pwd}/bin/coverage.sh" --target ${source} --dir=coverage-v2 --sanitize ${silent} ${opts[@]}
"${pwd}/bin/coverage.sh" --target ${source} --dir=coverage-v3 ${silent}
"${pwd}/bin/coverage.sh" --target ${source} --dir=coverage-v4 --gc --valgrind ${silent} ${opts[@]}
"${pwd}/bin/coverage.sh" --target ${source} --dir=coverage-v5 --gc --sanitize ${silent} ${opts[@]}
"${pwd}/bin/coverage.sh" --target ${source} --dir=coverage-v6 --gc ${silent}

[ ! -d "${pwd}/coverage" ] && mkdir "${pwd}/coverage"

if [[ -f "${pwd}/coverage/${source}.info" ]]; then 
    rm "${pwd}/coverage/${source}.info"
fi

directories=( "coverage-v1" "coverage-v2" "coverage-v3" "coverage-v4" "coverage-v5" "coverage-v6" )
for directory in ${directories[@]}; do
    files=$(find "${directory}" -type f -name "lcov.info" -exec echo {} \;)
    for file in ${files[@]}; do
        link=$(basename $(dirname "${file}"))
        cp "${file}" "${pwd}/coverage/${link}-${source}-$(basename ${file})"
    done
done

find "${pwd}/coverage" -type f -name "*.info" -exec echo -a {} \; | xargs lcov -o "${pwd}/coverage/${source}.info"

directories=( "coverage-v1" "coverage-v2" "coverage-v3" "coverage-v4" "coverage-v5" "coverage-v6" )
for directory in ${directories[@]}; do
    files=$(find "${directory}" -type f -name "lcov.info" -exec echo {} \;)
    for file in ${files[@]}; do
        link=$(basename $(dirname "${file}"))
        rm "${pwd}/coverage/${link}-${source}-$(basename ${file})"
    done
done

if [[ "${source}" == "all" ]]; then
    mv "${pwd}/coverage/${source}.info" "${pwd}/coverage/lcov.info"
fi

if [ "${silent}" == "--silent" ]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
