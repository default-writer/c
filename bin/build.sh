#!/usr/bin/env bash
set -e

uid=$(id -u)

if [ "${uid}" -eq 0 ]; then
	echo "Please run as user"
	exit
fi

pwd=$(pwd)

install="$1"

[ ! -d "${pwd}/build" ] && mkdir "${pwd}/build"

array=()

if [ "${install}" == "--alloc" ]; then
	array=("-alloc")
fi

if [ "${install}" == "--experimental" ]; then
	array=("-experimental")
fi

if [ "${install}" == "--micro" ]; then
	array=("-micro")
fi

if [ "${install}" == "--light" ]; then
	array=("-light")
fi

if [ "${install}" == "--all" ]; then
	array=("" "-light" "-micro" "-experimental" "-alloc")
fi

if [ "${install}" == "" ]; then
	exit
fi

cmake \
	-DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
	-DCMAKE_BUILD_TYPE:STRING=Debug \
	-DCMAKE_C_COMPILER:FILEPATH=/usr/bin/gcc \
	-DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++ \
	-S"${pwd}" \
	-B"${pwd}/build" \
	-G "Unix Makefiles"

export MAKEFLAGS=-j8

for m in "${array[@]}"; do
	cmake --build "${pwd}/build" --target "main${m}"
done

cd "${pwd}"