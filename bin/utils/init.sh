#!/usr/bin/env bash
set -e

err_report() {
    echo "ERROR: on line $*: $(cat $0 | sed $1!d)" >&2
}

trap 'err_report $LINENO' ERR

uid=$(id -u)

if [ "${uid}" -eq 0 ]; then
    echo "Please run as user"
    exit
fi

pwd=$(pwd)

"${pwd}/bin/utils/install.sh" --git
"${pwd}/bin/utils/install.sh" --submodule
"${pwd}/bin/utils/install.sh" --hooks
"${pwd}/bin/utils/env.sh" --nvm
"${pwd}/bin/utils/nvm.sh"

. "${pwd}/bin/utils/nvm.sh"

nvm --version
nvm install 19.3.0
nvm use 19.3.0
npm install -g npm@9.2.0
echo "Node version $(node --version)"
echo "NPM version $(npm --version)"

"${pwd}/bin/utils/runme.sh"

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"