#!/usr/bin/env bash
set -e

uid=$(id -u)

if [ "${uid}" -eq 0 ]; then
	echo "Please run as user"
	exit
fi

pwd=$(pwd)

[ ! -d "${pwd}/build" ] && mkdir "${pwd}/build"

rm -rf ${pwd}/coverage/*.gcda
rm -rf ${pwd}/coverage/*.gcno

cmake \
	-DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
	-DCMAKE_BUILD_TYPE:STRING=Debug \
	-DCMAKE_C_COMPILER:FILEPATH=/usr/bin/gcc \
	-DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++ \
	-S"${pwd}" \
	-B"${pwd}/build" \
	-G "Unix Makefiles"

export MAKEFLAGS=-j8

cmake --build "${pwd}/build" --target all

cd "${pwd}"