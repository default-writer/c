#!/bin/bash -e

git submodule init
git submodule update --recursive --remote
git pull --recurse-submodules

cd "${0%/*}"

rm -rf $(pwd)/../build

if [ ! -d "$(pwd)/../build" ]
then
    mkdir $(pwd)/../build
    cp ../CMakeLists.txt $(pwd)/../build/
fi

cd $(pwd)/../build
cmake ../

cd $(pwd)/../build
make