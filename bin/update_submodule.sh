#!/bin/bash -e
cd "${0%/*}"
cd deps
git submodule update --recursive --remote
git pull --recurse-submodules