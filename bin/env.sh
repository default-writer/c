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

cd ${pwd}