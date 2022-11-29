#!/bin/bash -e
pwd=$(PWD)

cd "${0%/*}"
./clean.sh
cd ${pwd}

cmake . -DCMAKE_EXPORT_COMPILE_COMMANDS=1

cd ${pwd}