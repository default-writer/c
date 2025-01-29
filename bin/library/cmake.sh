#!/usr/bin/env bash
if [[ "${BASHOPTS}" != *extdebug* ]]; then
    set -e
fi

err_report() {
    echo "ERROR: $0:$*"
    exit 8
}

if [[ "${BASHOPTS}" != *extdebug* ]]; then
    trap 'err_report $LINENO' ERR
fi

function get-cwd() {
    local pwd
    local source

    source=$(pwd)
    pwd=$(cd "$(dirname $(dirname $(dirname $(realpath --relative-to="${source}" "${BASH_SOURCE[0]}"))))" &> /dev/null)

    if [ "$pwd" == "" ]; then
        pwd="."
    fi

    echo ${pwd}
}

function get-ignore() {
    local line
    local array
    local pwd

    pwd=$(get-cwd)

    array=$(while read line ; do printf "%s\n" $line ; done < "${pwd}/.config/.ignore" | sed s'/:$//')
    echo $array
}

function get-args() {
    local line
    local array
    local pwd

    pwd=$(get-cwd)

    array=$(while read line ; do printf "%s\n" $line ; done < "${pwd}/.args" | sed s'/:$//')
    echo $array
}

function get-os-type() {
    local CMAKE_OS_TYPE
    case "$OSTYPE" in
      linux*)   CMAKE_OS_TYPE=linux ;;
      darwin*)  CMAKE_OS_TYPE=darwin ;;
      bsd*)     CMAKE_OS_TYPE=bsd ;;
      solaris*) CMAKE_OS_TYPE=solaris ;;
      win*)     CMAKE_OS_TYPE=win ;;
      msys*)    CMAKE_OS_TYPE=win ;;
      cygwin*)  CMAKE_OS_TYPE=win ;;
      *)
        echo "unknown" 
        exit 8
        ;;
    esac
    echo "${CMAKE_OS_TYPE}"
}

function get-cmake() {
    local cmake
    local pwd

    pwd=$(get-cwd)

    [ -d "${pwd}/.tools/cmake-3.25/bin" ] && cmake="${pwd}/.tools/cmake-3.25/ucrt64/bin/cmake" || cmake=${cmake}
    if [[ "${cmake}" == "" ]]; then
        cmake=$(command -v cmake)
    fi
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
    local config
    local pwd
    
    pwd=$(get-cwd)

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

    array=()
    files=()
    if [[ -d "${pwd}/config" ]]; then
        files=$(find "${pwd}/config" -type f -name "sources.txt")
        for file in ${files[@]}; do
            config=$(basename $(dirname "${file}"))
            array+=( ${config#*-} )
        done
    fi

    printf '%s\n' ${array[@]} | cat -n | sort -uk2 | sort -nk1 | cut -f2-
}

function get-cmake-c-compiler-path() {
    local CMAKE_COMPILER
    case "$OSTYPE" in
      linux*)   CMAKE_COMPILER=/usr/bin/gcc ;;
      darwin*)  CMAKE_COMPILER= ;;
      bsd*)     CMAKE_COMPILER= ;;
      solaris*) CMAKE_COMPILER= ;;
      win*)     CMAKE_COMPILER=cl ;;
      msys*)    CMAKE_COMPILER=gcc ;;
      cygwin*)  CMAKE_COMPILER=gcc ;;
      *)
        echo "unknown" 
        exit 8
        ;;
    esac
    echo "${CMAKE_COMPILER}"
}

function get-cmake-cxx-compiler-path() {
    local CMAKE_COMPILER
    case "$OSTYPE" in
      linux*)   CMAKE_COMPILER=/usr/bin/g++ ;;
      darwin*)  CMAKE_COMPILER= ;;
      bsd*)     CMAKE_COMPILER= ;;
      solaris*) CMAKE_COMPILER= ;;
      win*)     CMAKE_COMPILER=cl ;;
      msys*)    CMAKE_COMPILER=g++ ;;
      cygwin*)  CMAKE_COMPILER=g++ ;;
      *)
        echo "unknown"
        exit 8
      ;;
    esac
    echo "${CMAKE_COMPILER}"
}

function search-link() {
    local target_src
    local target_dest
    local target_link
    local array
    local source
    local result

    array=()
    source=$1
    target_src=$(sed -n "s#target_link_libraries(\([^ ]*\) .*${source}.*)#\1#p" CMakeLists.txt)
    target_dest=$(sed -n "s#target_link_libraries(${source}* \w* \(.*\))#\1#p" CMakeLists.txt)
    while [[ ! "${target_dest[@]}" == "" ]]; do
        for target_link in ${target_dest[@]}; do
            array+=( ${target_link} )
            source="${target_link}"
        done
        target_src=$(sed -n "s#target_link_libraries(\([^ ]*\) .*${source}.*)#\1#p" CMakeLists.txt)
        target_dest=$(sed -n "s#target_link_libraries(${source}* \w* \(.*\))#\1#p" CMakeLists.txt)
    done
    printf '%s\n' ${array[@]} | cat -n | sort -uk2 | sort -nk1 | cut -f2-
}

function get-linked-targets() {
    local link
    local linked_targets
    linked_targets=()
    link=$1
    linked_targets+=( $(search-link ${link}) )
    printf '%s\n' ${linked_targets[@]}
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
    local pwd

    pwd=$(get-cwd)

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
            if [[ -d "${pwd}/config/config-${target}" ]]; then
                files=$(find "${pwd}/config/config-${target}" -type f -name "sources.txt")
                for file in ${files[@]}; do
                    while IFS= read -r line; do
                        if [[ "${source}" == "${line}" ]]; then
                            config=$(basename $(dirname "${file}"))
                            link=${config#*-}
                            linked_targets=$(get-linked-targets ${link})
                            array+=( ${config#*-} )
                            array+=( ${linked_targets[@]} )
                        fi
                    done <  $file
                done
            fi
        done
    fi

    printf '%s\n' ${array[@]} | cat -n | sort -uk2 | sort -nk1 | cut -f2-
}

function get-options() {
    local opts=${@:1}
    for opt in ${opts[@]}; do
        case ${opt} in

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

            "--help") # [optional] shows command description
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

    if [ "$(cat /proc/version | grep -c MINGW64_NT)" == "0" ]; then
        echo " ${valgrind_options} ${callgrind_options}"
    else
        echo " echo"
    fi
}

export -f get-cwd
export -f get-ignore
export -f get-args
export -f get-os-type
export -f get-cmake
export -f get-targets
export -f get-linked-targets
export -f get-source-targets
export -f get-cmake-targets
export -f get-options
export -f cmake-options
export -f cmake-coverage-options
export -f cmake-valgrind-options
export -f get-cmake-c-compiler-path
export -f get-cmake-cxx-compiler-path
