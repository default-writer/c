#!/bin/bash -e
pwd=${PWD}

cd "${0%/*}"
./clean.sh
cd ..

git submodule init
git submodule update --recursive --remote
git pull --recurse-submodules

rm -rf $(pwd)/build

cd $(pwd)

cmake .

make
