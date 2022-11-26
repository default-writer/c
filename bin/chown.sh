#!/bin/bash -e

git submodule init
git submodule update --recursive --remote
git pull --recurse-submodules

cd "${0%/*}"

sudo chown -R vscode:vscode ../