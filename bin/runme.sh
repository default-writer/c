#!/usr/bin/env bash
set -e

uid=$(id -u)

if [ "${uid}" -eq 0 ]; then
	echo "Please run as user"
	exit
fi

pwd=$(pwd)

"${pwd}/bin/clean.sh" && "${pwd}/bin/build.sh" --all && "${pwd}/bin/coverage.sh" --all

cd "${pwd}"
