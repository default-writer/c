#!/usr/bin/env bash
if [[ "${BASHOPTS}" != *extdebug* ]]; then
    set -e
fi

err_report() {
    echo "ERROR: $0:$*"
    exit 8
}

pwd=$(pwd)

if [[ "${BASHOPTS}" != *extdebug* ]]; then
    trap 'err_report $LINENO' ERR
fi

function cleanup-dirs() {
    local pwd

    pwd=$(get-cwd)

    find "${pwd}" -type f -name "callgrind.out.*" -delete
    find "${pwd}" -type f -name "*.s" -delete
}

function cleanup-python-dirs() {
    local pwd

    pwd=$(get-cwd)

    find "${pwd}" -type f -name "callgrind.out.*" -delete
    find "${pwd}" -type f -name "*.s" -delete
    find "${pwd}" -type d -not -path "${pwd}/.venv/*" -name "__pycache__" -exec rm -rf {} +
    find "${pwd}" -type d -not -path "${pwd}/.venv/*" -name ".pytest_cache" -exec rm -rf {} +
    find "${pwd}" -type d -not -path "${pwd}/.venv/*" -name "build" -exec rm -rf {} +
    find "${pwd}" -type d -not -path "${pwd}/.venv/*" -name "c.egg-info" -exec rm -rf {} +
}

export -f cleanup-dirs
export -f cleanup-python-dirs
