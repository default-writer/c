#!/usr/bin/env bash
set -e

uid=$(id -u)

if [ "${uid}" -eq 0 ]; then
	echo "Please run as user"
	exit
fi

pwd=$(pwd)

"${pwd}/bin/install.sh" git
"${pwd}/bin/install.sh" submodule
"${pwd}/bin/install.sh" hooks
"${pwd}/bin/env.sh" nvm
"${pwd}/bin/nvm.sh"

. "${HOME}/.bashrc"

nvm --version
nvm install node
node --version

./bin/runme.sh

cd "${pwd}"