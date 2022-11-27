#!/bin/bash -e
git submodule deinit -f rexo
rm -rf .git/modules/rexo
git rm -f rexo
