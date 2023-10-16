#!/usr/bin/env bash
set -e

cwd=$(pwd)

path=$1
if [[ "${path}" == "" ]]; then
    path=$(dirname 0$)
fi

cd "${path}"

[[ ! -d "${path}/bin" ]] && mkdir "${path}/bin"

# Build the mojo program
mojo build "${path}/src/main.mojo" -o "${path}/bin/main"

echo "OK"

cd "${cwd}"
