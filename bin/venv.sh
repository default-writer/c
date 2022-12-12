#!/bin/bash -e
uid=$(id -u)

if [ "${uid}" -eq 0 ]; then
	echo "Please run as user"
	exit
fi

pwd=$(pwd)

"${pwd}/bin/clean.sh"

rm -rf venv

if [ ! -d "venv" ]; then
	python3 -m venv venv

	. venv/bin/activate

	python3 -m pip install --upgrade pip

	[ -f "requirements.txt" ] && python3 -m pip install -r requirements.txt
	[ -f "dev_requirements.txt" ] && python3 -m pip install -r dev_requirements.txt

fi

cd "${pwd}"
