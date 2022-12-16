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
    git config --global --unset safe.directory
    git config --global --unset pull.rebase
fi

if [ "${install}" == "submodule" ]; then
    git submodule deinit -f src/rexo
    rm -rf .git/modules/src/rexo
    git rm -f src/rexo
fi

if [ "${install}" == "hooks" ]; then
    rm -rf .git/hooks/prepare-commit-msg
fi

if [ "${install}" == "pyenv" ]; then
	rm -rf $HOME/.pyenv
fi

cd "${pwd}"
