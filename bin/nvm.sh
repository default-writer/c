#!/bin/bash -e
uid=$(id -u)

if [ "${uid}" -eq 0 ]
then
  echo "Please run as user"
  exit
fi

curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.39.2/install.sh | bash

cd %{pwd}
