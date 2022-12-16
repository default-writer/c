#!/usr/bin/env bash
set -e

uid=$(id -u)

if [ "${uid}" -eq 0 ]; then
	echo "Please run as user"
	exit
fi

pwd=$(pwd)

install="$1"

if [ "${install}" == "git" ]; then
	git config --global --add safe.directory "${pwd}"
	git config --global pull.rebase false
fi

if [ "${install}" == "submodule" ]; then
    git submodule init || (git submodule add https://github.com/christophercrouzet/rexo.git src/rexo && git submodule init)
    git submodule update --recursive --remote
    git pull --recurse-submodules .
fi

if [ "${install}" == "hooks" ]; then
    cp .hooks/pre-commit .git/hooks/prepare-commit-msg
fi

cd "${pwd}"
