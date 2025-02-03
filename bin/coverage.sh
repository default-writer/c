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


install="$1"

opts=( "${@:2}" )

. "${pwd}/bin/scripts/load.sh"

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
            opts=( "${@:2}" )
            ;;

        "--dir="*) # [optional] build directory
            dir=${1#*=}
            opts=( "${@:2}" )
            ;;

        "--clean") # [optional] cleans up directories before coverage
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

        "--index="*) # [optional] uses sharding coverage tests
            index="${1#*=}"
            opts=( "${@:2}" )
            if [[ "${index}" == "1" ]]; then
                gc=""
                sanitize=""
                valgrind=""
            fi
            if [[ "${index}" == "2" ]]; then
                gc="--gc"
                sanitize=""
                valgrind=""
            fi
            if [[ "${index}" == "3" ]]; then
                gc=""
                sanitize="--sanitize"
                valgrind=""
            fi
            if [[ "${index}" == "4" ]]; then
                gc="--gc"
                sanitize="--sanitize"
                valgrind=""
            fi
            if [[ "${index}" == "5" ]]; then
                gc=""
                sanitize=""
                valgrind="--valgrind"
            fi
            if [[ "${index}" == "6" ]]; then
                gc="--gc"
                sanitize=""
                valgrind="--valgrind"
            fi
            ;;

        "--coverage") # [optional] joins coverage info in coverage directory
            coverage="--coverage"
            opts=( "${@:2}" )
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

build=( "build/coverage-v1" "build/coverage-v2" "build/coverage-v3" "build/coverage-v4" "build/coverage-v5" "build/coverage-v6" )

if [[ ! "${dir}" == "" ]]; then
    build="${dir}"
fi

if [[ "${clean}" == "--clean" ]]; then
    if [[ -d "${dir}" ]]; then
        rm -rf "${dir}"
    fi
fi

directories=${build[@]}

if [[ "${coverage}" == "" ]]; then

    targets=( $(get-source-targets ${source}) )

    if [[ "${targets[@]}" == "" ]]; then
        if [[ "${help}" == "--help" ]]; then
            help
        fi
        echo no source targets: ${source} >&2
        exit 8
    fi

    coverage=( "*.gcda" "*.gcno" "*.s" "*.i" "*.o" )

    for directory in ${directories[@]}; do
        for f in ${coverage[@]}; do
            if [[ -d "${directory}" ]]; then
                find "${directory}" -type f -name "callgrind.out.*" -delete
                find "${directory}" -type f -name "*.s" -delete
                find "${directory}" -type f -name "${f}" -delete
            fi
        done
    done

    case "${index}" in

        "all")
            "${pwd}/bin/utils/coverage.sh" --target=${source} --dir=build/coverage-v1 ${opts[@]}
            "${pwd}/bin/utils/coverage.sh" --target=${source} --dir=build/coverage-v2 --gc ${opts[@]}
            "${pwd}/bin/utils/coverage.sh" --target=${source} --dir=build/coverage-v3 --sanitize ${opts[@]}
            "${pwd}/bin/utils/coverage.sh" --target=${source} --dir=build/coverage-v4 --gc --sanitize ${opts[@]}
            "${pwd}/bin/utils/coverage.sh" --target=${source} --dir=build/coverage-v5 --valgrind ${opts[@]}
            "${pwd}/bin/utils/coverage.sh" --target=${source} --dir=build/coverage-v6 --gc --valgrind ${opts[@]}
            ;;

        "1")
            "${pwd}/bin/utils/coverage.sh" --target=${source} --dir=build/coverage-v1 ${opts[@]}
            ;;

        "2")
            "${pwd}/bin/utils/coverage.sh" --target=${source} --dir=build/coverage-v2 --gc ${opts[@]}
            ;;

        "3")
            "${pwd}/bin/utils/coverage.sh" --target=${source} --dir=build/coverage-v3 --sanitize ${opts[@]}
            ;;

        "4")
            "${pwd}/bin/utils/coverage.sh" --target=${source} --dir=build/coverage-v4 --gc --sanitize ${opts[@]}
            ;;

        "5")
            "${pwd}/bin/utils/coverage.sh" --target=${source} --dir=build/coverage-v5 --valgrind ${opts[@]}
            ;;

        "6")
            "${pwd}/bin/utils/coverage.sh" --target=${source} --dir=build/coverage-v6 --gc --valgrind ${opts[@]}
            ;;

    esac

fi

if [[ "${coverage}" == "--coverage" ]]; then
    for directory in ${directories[@]}; do
        if [[ -d "${directory}" ]]; then
            files=$(find "${directory}" -type f -name "lcov.info" -exec echo {} \;)
            for file in ${files[@]}; do
                targets=( $(get-source-targets ${source}) )
                for target in ${targets[@]}; do
                    cp "${file}" "${directory}-${source}.info"
                done
            done
            find "${directory}" -type f -name "*.info" -delete
        fi
    done
fi

if [[ "${silent}" == "--silent" ]]; then
    exec 1>&2 2>&-
fi

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
