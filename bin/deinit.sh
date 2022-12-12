#!/usr/bin/env bash
set -e

git submodule deinit -f src/rexo
rm -rf .git/modules/src/rexo
git rm -f src/rexo
