#!/usr/bin/env bash
set -e

uid=$(id -u)

if [ "${uid}" -eq 0 ]; then
	echo "Please run as user"
	exit
fi

pwd=$(pwd)

install="$1"

if [ "${install}" == "venv" ]; then
	rm -rf venv
	python3 -m venv venv
fi

if [ "${install}" == "pyenv" ]; then
	pyenv install -f 3.11.1
	pyenv virtualenv -f 3.11.1 .venv
fi

cd "${pwd}"
