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

uid=$(id -u)

if [ "${uid}" -eq 0 ]; then
	echo "Please run as user"
	exit
fi

rm -rf "${pwd}/doxygen"

doxygen Doxyfile

cd "${pwd}/doxygen/latex"

make

cd "${pwd}"

cp "${pwd}/doxygen/latex/refman.pdf" "${pwd}/docs/"

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
