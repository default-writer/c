#!/bin/bash -e
pwd=${PWD}
cd src/rexo
git submodule update --recursive --remote
git pull --recurse-submodules
cd $pwd