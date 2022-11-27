#!/bin/bash -e
git submodule deinit -f deps/rexo
rm -rf .git/modules/deps/rexo
git rm -f deps/rexo
