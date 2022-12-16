#!/usr/bin/env bash
set -e

pwd=$(pwd)

git clean -f -q -d -x

cd "${pwd}"