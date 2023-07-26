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
        target="--target $2"
        opts=( "${@:3}" )
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

        "--keep") # [optional] keeps coverage files and merges them
            keep="--keep"
            ;;

        "--silent") # [optional] suppress verbose output
            silent="--silent"
            ;;

        "--help") # shows command desctiption
            help
            ;;

        *)
            echo "Error: unknown argyment ${opt}"
            help
            ;;

    esac
done

if [[ "${silent}" == "--silent" ]]; then
    exec 2>&1 >/dev/null
fi

cmake=$(get-cmake)
targets=( $(get-targets) )

found_target=false
for target in ${targets[@]}; do
    if [ "${target}" == "$2" ]; then 
        array=( ${target} )
        found_target=true
    fi
done

if [[ "${found_target}" == false ]]; then
    array=()
    target=$2
    found_target_base=false
    sources=$(find "${pwd}/config" -type f -name "sources.txt")
    for source in ${sources[@]}; do
        # Find target directory for the given source file
        while IFS= read -r line; do
            if [[ "${target}" == "${line}" ]]; then
                found_target_base=true
                target_base=$(basename $(dirname "${source}"))
                array+=( ${target_base} )
                break
            fi
        done <  $source
        if [[ "${found_target_base}" == true ]]; then
            linked_targets=$(get-linked-targets ${target_base})
            if [ "${target}" == "--target ${line}" ]; then
                for linked_target in ${linked_targets[@]}; do
                    if [ "${linked_target}" == "$2" ]; then 
                        array+=( ${linked_target} )
                    fi
                done
            fi
        fi
    done
fi

targets=( ${array[@]} )

if [ "$(echo ${targets[@]})" == "" ]; then
    echo "Error: no targets found for $2"
    help
    exit 8
fi

echo =============================================================================
echo -1/6------- building with garbage collector ---------------------------------
"${pwd}/bin/build.sh" --target ${target} --gc ${opts[@]}
echo -2/6------- building with garbage collector / with sanitizer ----------------
"${pwd}/bin/build.sh" --target ${target} --gc --sanitize ${opts[@]}
echo -3/6------- building with garbage collector / with valgrind -----------------
"${pwd}/bin/build.sh" --target ${target} --gc --valgrind ${opts[@]}
echo -4/6------- building without garbage collector ------------------------------
"${pwd}/bin/build.sh" --target ${target} ${silent} ${opts[@]}
echo -5/6------- building without garbage collector / with sanitizer -------------
"${pwd}/bin/build.sh" --target ${target} --sanitize ${opts[@]}
echo -6/6------- building without garbage collector / with valgrind --------------
"${pwd}/bin/build.sh" --target ${target} --valgrind ${opts[@]}
echo =============================================================================

if [ "${silent}" == "--silent" ]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
