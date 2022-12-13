#!/usr/bin/env bash
set -e

pwd=$(pwd)

cd "${0%/*}"
./clean.sh
cd "${pwd}"

rm -rf "${pwd}/coverage"
mkdir "${pwd}/coverage"

## define an array ##
array=('' '-light' '-micro')

## get item count using ${array[@]} ##
for m in ${array[@]}; do
	gcc --coverage -g \
		"${pwd}/tests/main${m}.c" \
		"${pwd}/src/list${m}/code.c" \
		"${pwd}/src/common/alloc.c" \
		"${pwd}/src/common/print.c" \
		"${pwd}/src/common/object.c" \
		-I"${pwd}/src/" \
		-I"${pwd}/src/std/" \
		-I"${pwd}/src/common/" \
		-I"${pwd}/src/list${m}/" \
		-I"${pwd}/rexo/include/" \
		-DMEMORY_DEBUG_INFO \
		-DMEMORY_CLEANUP \
		-DMEMORY_LEAKS \
		-o "${pwd}/coverage/main${m}" &&
		"${pwd}/coverage/main${m}" &&
		lcov --capture --directory "${pwd}/coverage/" --output-file "${pwd}/coverage/main${m}.info"
done

cd "${pwd}"
