#!/bin/bash -e
uid=$(id -u)

if [ "${uid}" -eq 0 ]
then
  echo "Please run as user"
  exit
fi

pwd=$(pwd)

${pwd}/bin/clean.sh

rm -rf venv

python3 -m venv venv

. venv/bin/activate
python3 -m pip install --upgrade pip
python3 -m pip install -r requirements.txt

cd ${pwd}