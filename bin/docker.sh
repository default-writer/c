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

pwd=$(cd "$(dirname "$(dirname "${BASH_SOURCE[0]}")")" &> /dev/null && pwd)

cd "${pwd}"

install="$1"

opts=( "${@:2}" )

. "${pwd}/bin/scripts/load.sh"

## Builds docker image
## Usage: ${script} <option> [optional]
## ${commands}

while (($#)); do
    case "$1" in

        "--build") # [optional] builds docker image and pushes it into repository
            build="--build"
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
    exit
fi

if [[ "${build}" == "--build" ]]; then
    export CR_PAT="$CR_PAT"
    echo $CR_PAT | docker login -u defaultwriter --password-stdin
    IMAGE_NAME="defaultwriter/c-image-repo"
    GIT_TAG=$(git rev-parse --short HEAD)
    docker build . -t $IMAGE_NAME:$GIT_TAG
    docker tag $IMAGE_NAME:$GIT_TAG $IMAGE_NAME:latest
    docker push $IMAGE_NAME:$GIT_TAG
    docker push $IMAGE_NAME:latest
fi

[[ $SHLVL -eq 2 ]] && echo OK

cd "${pwd}"
