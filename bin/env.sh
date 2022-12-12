#!/bin/bash -e
uid=$(id -u)
gid=$(id -gn)

if [ "${uid}" -eq 0 ]
then
  echo "Please run as user"
  exit
fi

pwd=$(pwd)

export USER_NAME=$USER
export USER_ID="${uid}"
export USER_GROUP="${gid}"

cd "${pwd}"