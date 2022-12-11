#!/bin/bash -e
uid=$(id -u)

if [ "${uid}" -eq 0 ]; then
	echo "Please run as user"
	exit
fi

pwd=$(pwd)

curl -O https://get.bzl.io/linux_amd64/bzl && chmod +x ./bzl

./bzl install

cd "${pwd}"
