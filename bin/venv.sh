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
	. venv/bin/activate
	python3 -m pip install --upgrade pip
	[ -f "requirements.txt" ] && python3 -m pip install -r requirements.txt
fi

if [ "${install}" == "pyenv" ]; then
	pyenv insall 3.11.1
	pyenv virtualenv 3.11.1 .venv
	pyenv activate .venv
fi

cd "${pwd}"
