#!/bin/bash -e
pwd=${PWD}
cd ..
cmake . -DCMAKE_EXPORT_COMPILE_COMMANDS=1
cd $(pwd)