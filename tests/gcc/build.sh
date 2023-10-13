#!/usr/bin/env bash
set -e

cwd=$(pwd)

path=$1
if [[ "${path}" == "" ]]; then
    path=$(dirname 0$)
fi

[[ -d "${path}/bin" ]] || mkdir "${path}/bin"

gcc "${path}/main.c" -o "${path}/bin/main" --debug -save-temps -std=gnu89 -fgnu89-inline -Winline
mv "${path}/bin/main.s" "${path}/main.c.s"

echo OK

cd "${cwd}"
