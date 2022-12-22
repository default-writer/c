#!/usr/bin/env bash
set -e

uid=$(id -u)

if [ "${uid}" -eq 0 ]; then
	echo "Please run as user"
	exit
fi

pwd=$(pwd)

echo Cleans up uncommited changes and non-gited files and folders
git clean -f -q -d -x

cd "${pwd}"