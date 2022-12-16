#!/usr/bin/env bash
set -e

pwd=$(pwd)

install="$1"

if [ "${install}" == "submodule" ]; then
    git submodule deinit -f src/rexo
    rm -rf .git/modules/src/rexo
    git rm -f src/rexo
fi

cd "${pwd}"
