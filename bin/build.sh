#!/bin/bash -e
pwd=$(pwd)

cd "${0%/*}"
./clean.sh
cd ${pwd}

git submodule init
git submodule update --recursive --remote
git pull --recurse-submodules

rm -rf ${pwd}/build

if [ ! -d "${pwd}/build" ]
then
    cmake \
        --no-warn-unused-cli \
        -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
        -DCMAKE_BUILD_TYPE:STRING=Debug \
        -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/gcc \
        -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++ \
        -S${pwd} \
        -B${pwd}/build \
        -G "Unix Makefiles"
fi

export MAKEFLAGS=-j8

cmake --build ${pwd}/build --target all
