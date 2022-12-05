#!/bin/bash -e
uid=$(id -u)

if [ "${uid}" -eq 0 ]
then
  echo "Please run as user"
  exit
fi

pwd=$(pwd)

export USER_NAME=$USER
export USER_ID=$(id -u)
export USER_GROUP=$(id -gn)
export USER_PACKAGES="curl git python3 python3-dev python3-pip python3-venv"

cd ${pwd}