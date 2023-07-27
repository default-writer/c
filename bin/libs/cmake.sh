#!/usr/bin/env bash
set -e

err_report() {
    echo "ERROR: on line $*: $(cat $0 | sed $1!d)" >&2
}

trap 'err_report $LINENO' ERR

function get-cmake() {
    local cmake=${cmake-cmake}
    [ -d "${pwd}/.tools/cmake-3.25/bin" ] && cmake=${pwd}/.tools/cmake-3.25/bin/cmake || cmake=${cmake}
    echo ${cmake}
}

function get-targets() {
    local array=()
    local source=$0
    local script="$(basename "$(test -L "${source}" && readlink "${source}" || echo "${source}")")"
    local commands=$(echo $(cat ${source} | sed -e 's/^[ \t]*//;' | sed -e '/^[ \t]*$/d' | sed -n -e 's/^"--\(.*\)".*/\1/p') | sed -n -e 's/^\(.*\)\sall\s.*$/\1/p')
    local targets=$(echo ${commands})

    for target in ${targets[@]}; do
        if [ "${target}" != "target" ]; then
            array+=( "${target}" )
        fi
    done

    [ ! -d "${pwd}/config" ] && mkdir "${pwd}/config"

    exec 2>&1 >/dev/null

    ${cmake} \
        -DTARGETS:BOOL=ON \
        -S"${pwd}" \
        -B"${pwd}/config" \
        -G "Ninja" 2>&1 >/dev/null

    exec 1>&2 2>&-

    if [ -f "${pwd}/config/targets.txt" ]; then
        targets=$(cat "${pwd}/config/targets.txt")
        for target in ${targets[@]}; do
            array+=("${target}")
        done
    fi

    printf '%s\n' "${array[@]}"
}

function search() {
    local target=$1
    if [[ ! "${target}" == "" ]]; then
        target=$(sed -n "s#target_link_libraries(\([^ ]*\) .*${target}.*)#\1#p" CMakeLists.txt)
        if [[ ! "${target}" == "" ]]; then
            printf '%s\n' "${target}"
            search ${target}
        fi
    fi
}

function get-linked-targets() {
    local target=$1
    if [[ ! "${target}" == "" ]]; then
        printf '%s\n' "${target}"
        search ${target}
    fi
}

function get-cmake-targets() {
    local target=$1
    local targets=()
    local lib=""
    local exe=""
    lib=$(sed -n "s#add_library(\([^ ]*\) .*#\1#p" CMakeLists.txt)
    exe=$(sed -n "s#add_executable(\([^ ]*\) .*#\1#p" CMakeLists.txt)
    targets+=( ${lib[@]} )
    targets+=( ${exe[@]} )
    targets=$(echo "${targets[@]}" | xargs -n1 | sort -u | xargs)
    if [[ ! "${target}" == "" ]]; then
        if [[ " ${targets[*]} " == *" ${target} "* ]]; then
            printf '%s\n' "${target}"
        fi
    fi
    if [[ "${target}" == "" ]]; then
        printf '%s\n' "${targets[@]}"
    fi
}

function get-gtktargets() {
    local array=()
    local source=$0
    local script="$(basename "$(test -L "${source}" && readlink "${source}" || echo "${source}")")"
    local commands=$(echo $(cat ${source} | sed -e 's/^[ \t]*//;' | sed -e '/^[ \t]*$/d' | sed -n -e 's/^"--\(.*\)".*/\1/p') | sed -n -e 's/^\(.*\)\sall\s.*$/\1/p')
    local targets=$(echo ${commands})

    for target in ${targets[@]}; do
        if [ "${target}" != "target" ]; then
            array+=( "${target}" )
        fi
    done

    [ ! -d "${pwd}/config" ] && mkdir "${pwd}/config"

    exec 2>&1 >/dev/null

    ${cmake} \
        -DTARGETS:BOOL=ON \
        -S"${pwd}" \
        -B"${pwd}/config" \
        -G "Ninja" 2>&1 >/dev/null

    exec 1>&2 2>&-

    if [ -f "${pwd}/config/gtktargets.txt" ]; then
        gtktargets=$(cat "${pwd}/config/gtktargets.txt")
        for target in ${gtktargets[@]}; do
            array+=("${target}")
        done
    fi

    printf '%s\n' "${array[@]}"
}

function get-options() {
    local opts=${@:1}
    for opt in ${opts[@]}; do
        case ${opt} in

            "")
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

            "--help") # [optional] shows command desctiption
                help="--help"
                ;;

            *)
                help
                exit 8
                ;;

        esac
    done

    echo " ${target} ${clean} ${sanitize} ${mocks} ${gc} ${silent} ${valgrind} ${callgrind} ${debug} ${help}"
}

function cmake-options() {
    local sanitize_options
    local mocks_options
    local gc_options
    local verbose_options

    if [ "${sanitize}" == "--sanitize" ] && [ "${valgrind}" == "" ]; then
        sanitize_options=-DCODE_SANITIZER:BOOL=TRUE
    fi

    if [ "${mocks}" == "--mocks" ]; then
        mocks_options=-DMOCKS:BOOL=TRUE
    fi

    if [ "${gc}" == "--gc" ]; then
        gc_options=-DGC:BOOL=TRUE
    fi

    if [ "${debug}" == "--debug" ]; then
        debug_options=-DCONFIG_MEMORY_DEBUG_INFO:BOOL=TRUE
    fi

    if [[ "${verbose}" == "--verbose" ]]; then
        verbose_options=-DVERBOSE:BOOL=TRUE
    fi

    echo " ${sanitize_options} ${mocs_options} ${gc_options} ${debug_options} ${verbose_options}"
}

function cmake-coverage-options() {
    local sanitize_options
    local mocks_options
    local debug_options
    local verbose_options

    if [ "${sanitize}" == "--sanitize" ] && [ "${valgrind}" == "" ]; then
        sanitize_options=-DCODE_SANITIZER:BOOL=TRUE
    fi

    if [ "${mocks}" == "--mocks" ]; then
        mocks_options=-DMOCKS:BOOL=TRUE
    fi

    if [ "${debug}" == "--debug" ]; then
        debug_options=-DCONFIG_MEMORY_DEBUG_INFO:BOOL=TRUE
    fi

    if [[ "${verbose}" == "--verbose" ]]; then
        verbose_options=-DVERBOSE:BOOL=TRUE
    fi

    echo " ${sanitize_options} ${mocs_options} ${debug_options} ${verbose_options}"
}

function cmake-valgrind-options() {
    local valgrind_options
    local callgrind_options

    if [ "${valgrind}" == "--valgrind" ] && [ "${sanitize}" == "" ]; then
        valgrind_options="valgrind --leak-check=full -q -s"
        if [ "${callgrind}" == "--callgrind" ]; then
            callgrind_options=" --tool=callgrind"
        fi
    fi

    echo " ${valgrind_options} ${callgrind_options}"
}
export -f get-cmake
export -f get-targets
export -f get-linked-targets
export -f get-gtktargets
export -f get-cmake-targets
export -f get-options
export -f cmake-options
export -f cmake-coverage-options
export -f cmake-valgrind-options