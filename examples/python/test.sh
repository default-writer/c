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

pwd=$(cd "$(dirname "${BASH_SOURCE[0]}")" &> /dev/null && pwd)

cd "${pwd}/../../"

if [[ ! -d ".venv" ]]; then
    python3 -m venv .venv

    source .venv/bin/activate

    pip install -r requirements.txt
fi

source .venv/bin/activate

pytest

[[ $SHLVL -eq 2 ]] && echo OK

cd "${pwd}"
