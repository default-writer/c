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
        exit 8
        ;;

esac

for opt in ${opts[@]}; do
    case ${opt} in

        "")
            ;;

        "--keep") # [optional] keeps coverage files and merges them
            keep="--keep"
            ;;

        "--clean") # [optional] cleans up directories before build
            clean="--clean"
            ;;

        "--sanitize") # [optional] builds using sanitizer
            sanitize="--sanitize"
            ;;

        "--mocks") # [optional] builds with mocks
            mocks="--mocks"
            ;;

        "--gc") # [optional] builds with garbage collector
            gc="--gc"
            ;;

        "--silent") # [optional] suppress verbose output
            silent="--silent"
            ;;

        "--valgrind") # [optional] runs using valgrind (disables --sanitize on build)
            valgrind="--valgrind"
            ;;

        "--callgrind") # [optional] runs using valgrind with tool callgrind (disables --sanitize on build)
            callgrind="--callgrind"
            ;;

        "--debug") # [optional] runs using debug memory debug info
            debug="--debug"
            ;;

        "--verbose") # [optional] shows verbose messages
            verbose="--verbose"
            ;;

        "--help") # [optional] shows command desctiption
            help
            exit 8
            ;;

        *)
            echo "Error: unknown argyment ${opt}"
            help
            exit 8
            ;;

    esac
done

if [ "${silent}" == "--silent" ]; then
    exec 2>&1 >/dev/null
fi

build="${pwd}/build"

if [ "${gc}" == "--gc" ]; then
    build="${build}-gc"
fi

if [ "${sanitize}" == "--sanitize" ]; then
    build="${build}-sanitize"
fi

if [ "${valgrind}" == "--valgrind" ]; then
    build="${build}-valgrind"
fi

[ ! -d "${build}" ] && mkdir "${build}"

if [ "${keep}" == "" ]; then
if [ "${clean}" == "--clean" ]; then
    rm -rf "${build}"
    mkdir "${build}"
fi
fi

find "${pwd}" -type f -name "callgrind.out.*" -delete
find "${pwd}/src" -type f -name "*.s" -delete
find "${pwd}/tests" -type f -name "*.s" -delete

cmake=$(get-cmake)
targets=( $(get-cmake-targets) )

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

if [[ "${target_base}" == "" ]]; then
    target_base="${targets[@]}"
fi

targets=$(echo "${targets[@]} ${linked_targets[@]}" | xargs -n1 | sort -u | xargs)

if [[ "${targets[@]}" == "" ]]; then
    if [[ "${help}" == "--help" ]]; then
        help
    fi
    echo ERROR
    exit 8
fi

[ ! -d "${build}" ] && mkdir "${build}"

coverage=( "*.gcda" "*.gcno" "*.s" "*.i" "*.o" )
for f in ${coverage}; do
    find "${build}" -type f -name "${f}" -delete
done

export MAKEFLAGS=-j8

${cmake} \
    -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
    -DCMAKE_BUILD_TYPE:STRING=Debug \
    -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/gcc \
    -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++ \
    -DVERBOSE:BOOL=FALSE \
    $(cmake-options) \
    -S"${pwd}" \
    -B"${build}" \
    -G "Ninja" 2>&1 >/dev/null


for target in ${targets[@]}; do
    if [[ "${verbose}" == "--verbose" ]]; then
        echo Building target ${target}
        echo Building with options $(cmake-options)
    fi
    if [ "${silent}" == "--silent" ]; then
        ${cmake} --build "${build}" --target "${target}" 2>&1 >/dev/null || (echo ERROR: "${target}" && exit 1)
    else
        ${cmake} --build "${build}" --target "${target}" || (echo ERROR: "${target}" && exit 1)
    fi
    case "${target}" in main-*)
        timeout --foreground 180 $(cmake-valgrind-options) "${build}/${target}" 2>&1 >"${build}/log-${target}.txt" || (echo ERROR: "${target}" && exit 1)
    esac
done

main=$(find "${build}" -type f -name "*.s" -exec echo {} \;)
for i in $main; do
    path="${pwd}/$(echo $i | sed -n -e 's/^.*.dir\/\(.*\)$/\1/p')"
    cp "${i}" "${path}"
done

if [ "${silent}" == "--silent" ]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
