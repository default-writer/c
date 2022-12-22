#!/usr/bin/env bash
set -e

uid=$(id -u)

if [ "${uid}" -eq 0 ]; then
	echo "Please run as user"
	exit
fi

pwd=$(pwd)

./bin/install.sh git
./bin/install.sh submodule
./bin/install.sh hooks
./bin/nvm.sh

nvm --version
nvm install node
node --version

./bin/runme.sh

cd "${pwd}"