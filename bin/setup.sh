#!/bin/bash -e
uid=$(id -u)

if [ ! "${uid}" -eq 0 ]
then
  echo "Please run as root"
  exit
fi

pwd=$(pwd)

install="$1"

if [ "${install}" == "build" ]
then
  git config --global --add safe.directory ${pwd}
  git config --global pull.rebase false

  apt-get update
  apt install -y build-essential curl git ca-certificates python3 python3-dev python3-pip python3-venv python3-behave
fi

if [ "${install}" == "docker" ]
then
  apt-get update
  apt-get install -y \
    ca-certificates \
    curl \
    gnupg \
    lsb-release
  mkdir -p /etc/apt/keyrings
  curl -fsSL --use-ascii --retry 5 --retry-all-errors https://download.docker.com/linux/ubuntu/gpg | gpg --dearmor -o /etc/apt/keyrings/docker.gpg
  echo \
  "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/ubuntu \
  $(lsb_release -cs) stable" | tee /etc/apt/sources.list.d/docker.list > /dev/null

  apt-get update -y
  chmod a+r /etc/apt/keyrings/docker.gpg

  apt-get update -y
  apt-get install -y docker-ce docker-ce-cli containerd.io docker-compose-plugin
fi

cd ${pwd}