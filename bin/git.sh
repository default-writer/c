#!/bin/bash -e
pwd=$(pwd)

cd "${0%/*}"
./clean.sh
cd "${pwd}"

git submodule init || (git submodule add https://github.com/christophercrouzet/rexo.git src/rexo && git submodule init)
git submodule update --recursive --remote
git pull --recurse-submodules .

cd "${pwd}"
