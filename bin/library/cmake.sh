#!/usr/bin/env bash
if [[ "${BASHOPTS}" != *extdebug* ]]; then
    set -e
fi


function get_stack () {
   STACK=""
   local i message="${1:-""}"
   local stack_size=${#FUNCNAME[@]}
   # to avoid noise we start with 1 to skip the get_stack function
   for (( i=1; i<stack_size; i++ )); do
      local func="${FUNCNAME[$i]}"
      [[ $func = "" ]] && func=MAIN
      local linen="${BASH_LINENO[$(( i - 1 ))]}"
      local src="${BASH_SOURCE[$i]}"
      [[ "$src" = "" ]] && src=non_file_source

      STACK+=$'\n'"   at: $func $src "$linen
   done
   STACK="${message}${STACK}"
}

err_report() {
    cd ${source}
    get_stack
    echo "ERROR: on line $*: $(cat $(test -L "$0" && readlink "$0" || echo $0) | sed $1!d)" >&2
    exit 8
}

if [[ "${BASHOPTS}" != *extdebug* ]]; then
    trap 'err_report $LINENO' ERR
fi

function get-cmake() {
    local cmake
    [ -d "${pwd}/.tools/cmake-3.25/bin" ] && cmake=${pwd}/.tools/cmake-3.25/bin/cmake || cmake=${cmake}
    echo ${cmake}
}

function get-targets() {
    local file
    local files
    local target
    local targets
    local cmake
    local target
    local array

    if [[ ! -d "${pwd}/config" ]]; then
        exec 2>&1 >/dev/null

        cmake=$(get-cmake)
        if [[ "${cmake}" == "" ]]; then
            return
        fi

        ${cmake} \
            -DTARGETS:BOOL=ON \
            -S"${pwd}" \
            -B"${pwd}/config" \
            -G "Ninja" 2>&1 >/dev/null

        exec 1>&2 2>&-
    fi

    files=()
    if [[ -d "${pwd}/config" ]]; then
        files=$(find "${pwd}/config" -type f -name "sources.txt")
        for file in ${files[@]}; do
            array+=( $(basename $(dirname "${file}") ) )
        done
    fi

    printf '%s\n' ${array[@]} | cat -n | sort -uk2 | sort -nk1 | cut -f2-
}

function search() {
    local target_src
    local target_dest
    local target_link
    local source
    source=$1
    target_src=$(sed -n "s#target_link_libraries(\([^ ]*\) .*${source}.*)#\1#p" CMakeLists.txt)
    target_dest=$(sed -n "s#target_link_libraries(${source}* \w* \(.*\))#\1#p" CMakeLists.txt)
    if [[ ! "${target_dest[@]}" == "" ]]; then
        for target_link in "${target_dest[@]}"; do
            search ${target_link}
        done
        printf '%s\n' "${target_dest[@]}"
    fi
}

function get-linked-targets() {
    local link
    link=$1
    search ${link}
}

function get-cmake-targets() {
    local target
    local targets
    local source
    local sources
    local lib
    local exe
    local array
    
    target=$1
    targets=( $(get-targets) )

    if [[ "${target}" == "all" ]]; then
        printf '%s\n' "${targets[@]}"
    else
        lib=$(sed -n "s#add_library(\([^ ]*\)\s.*#\1#p" CMakeLists.txt)
        exe=$(sed -n "s#add_executable(\([^ ]*\) .*#\1#p" CMakeLists.txt)

        array+=( ${lib[@]} )
        array+=( ${exe[@]} )

        sources=$(echo "$(printf '%s\n' ${array[@]} | cat -n | sort -uk2 | sort -nk1 | cut -f2-)")
                        
        for source in ${sources[@]}; do
            if [[ " ${targets[*]} " == *" ${source} "* ]]; then
                printf '%s\n' "${source[@]}"
            fi
        done
    fi
}

function get-source-targets() {
    local source
    local cmake
    local target
    local targets
    local array
    local target
    local source
    local link
    local linked_targets
    local line
    local build
    local file
    local files

    source=$1

    cmake=$(get-cmake)
    if [[ "${cmake}" == "" ]]; then
        return
    fi

    if [[ ! -d "${pwd}/config" ]]; then
        exec 2>&1 >/dev/null

        build="${pwd}/config"
        ${cmake} \
            -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
            -DTARGETS:BOOL=ON \
            -S"${pwd}" \
            -B"${build}" \
            -G "Ninja" 2>&1 >/dev/null

        exec 1>&2 2>&-
    fi

    targets=( $(get-targets) )

    array=()
    if [[ "${source}" == "all" ]]; then
        array+=( ${targets[@]} )
    else
        for target in ${targets[@]}; do
            if [ "${target}" == "${source}" ]; then 
                array+=( ${target} )
                break
            fi
            files=$(find "${pwd}/config/${target}" -type f -name "sources.txt")
            for file in ${files[@]}; do
                while IFS= read -r line; do
                    if [[ "${source}" == "${line}" ]]; then
                        link=$(basename $(dirname "${file}"))
                        linked_targets=$(get-linked-targets ${link})
                        array+=( $(echo ${link} ${linked_targets[@]}) )
                    fi
                done <  $file
            done
        done
    fi

    printf '%s\n' ${array[@]} | cat -n | sort -uk2 | sort -nk1 | cut -f2-
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

            "--valgrind") # [optional] runs using valgrind
                valgrind="--valgrind"
                ;;

            "--callgrind") # [optional] runs using valgrind with tool callgrind
                callgrind="--callgrind"
                ;;

            "--debug") # [optional] runs using debug messaging
                debug="--debug"
                ;;

            "--vm-debug") # [optional] runs using vm debug messaging
                vm_debug="--vm-debug"
                ;;

            "--help") # [optional] shows command desctiption
                help="--help"
                ;;

            *)
                help
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
    local vm_debug_options

    if [ "${sanitize}" == "--sanitize" ] && [ "${valgrind}" == "" ]; then
        sanitize_options=-DCODE_SANITIZER:BOOL=TRUE
    fi

    if [ "${mocks}" == "--mocks" ]; then
        mocks_options=-DMOCKS:BOOL=TRUE
    fi

    if [ "${gc}" == "--gc" ]; then
        gc_options=-DCONFIG_GC:BOOL=TRUE
    fi

    if [ "${debug}" == "--debug" ]; then
        debug_options=-DCONFIG_MEMORY_DEBUG_INFO:BOOL=TRUE
    fi

    if [ "${vm_debug}" == "--vm-debug" ]; then
        vm_debug_options=-DCONFIG_VM_DEBUG_INFO:BOOL=TRUE
    fi

    echo " ${sanitize_options} ${mocs_options} ${gc_options} ${debug_options} ${verbose_options} ${vm_debug_options}"
}

function cmake-coverage-options() {
    local sanitize_options
    local mocks_options
    local debug_options
    local verbose_options
    local vm_debug_options

    if [ "${sanitize}" == "--sanitize" ] && [ "${valgrind}" == "" ]; then
        sanitize_options=-DCODE_SANITIZER:BOOL=TRUE
    fi

    if [ "${mocks}" == "--mocks" ]; then
        mocks_options=-DMOCKS:BOOL=TRUE
    fi

    if [ "${debug}" == "--debug" ]; then
        debug_options=-DCONFIG_MEMORY_DEBUG_INFO:BOOL=TRUE
    fi

    if [ "${vm_debug}" == "--vm-debug" ]; then
        vm_debug_options=-DCONFIG_VM_DEBUG_INFO:BOOL=TRUE
    fi

    echo " ${sanitize_options} ${mocs_options} ${debug_options} ${verbose_options} ${vm_debug_options}"
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
export -f get-source-targets
export -f get-cmake-targets
export -f get-options
export -f cmake-options
export -f cmake-coverage-options
export -f cmake-valgrind-options
