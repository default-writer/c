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

./bin/cleanup.sh --python

./bin/cmake.sh --recommended

cwd=$(pwd)
unset LD_PRELOAD
export LD_PRELOAD=${cwd}/lib/libc-sys.so:${cwd}/lib/libc-vm.so
export LD_LIBRARY_PATH=${cwd}/lib

if [[ ! -d ".venv" ]]; then
    python3 -m venv .venv

    source .venv/bin/activate

    pip install -r requirements.txt
fi

source .venv/bin/activate

black .

cd "${pwd}/src"

if [[ "$(pip list | grep -c '^c ')" == "1" ]]; then
    pip uninstall c -y
fi

pip install .

[[ $SHLVL -eq 2 ]] && echo OK

cd "${pwd}"
