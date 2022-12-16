#!/usr/bin/env bash
set -e

pwd=$(pwd)

install="$1"

if [ "${install}" == "submodule" ]; then
    git submodule init || (git submodule add https://github.com/christophercrouzet/rexo.git src/rexo && git submodule init)
    git submodule update --recursive --remote
    git pull --recurse-submodules .
fi

cd "${pwd}"
