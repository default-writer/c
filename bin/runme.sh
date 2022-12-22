#!/usr/bin/env bash
set -e

uid=$(id -u)

if [ "${uid}" -eq 0 ]; then
	echo "Please run as user"
	exit
fi

pwd=$(pwd)

./bin/clean.sh && ./bin/build.sh --all && ./bin/coverage.sh --all

cd "${pwd}"
