#!/usr/bin/env bash
set -e

uid=$(id -u)

if [ "${uid}" -eq 0 ]; then
	echo "Please run as user"
	exit
fi

pwd=$(pwd)

install="$1"

[ ! -d "${pwd}/coverage" ] && mkdir "${pwd}/coverage"

rm -rf ${pwd}/coverage/*.gcda
rm -rf ${pwd}/coverage/*.gcno

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

## compile with coverage metadata
for m in "${array[@]}"; do
	rm -f "${pwd}/coverage/coverage-main${m}.info"
done

## compile with coverage metadata
for m in "${array[@]}"; do
	rm -rf "${pwd}/coverage/main${m}.gcda"
	rm -rf "${pwd}/coverage/main${m}.gcno"
	gcc --coverage -g \
		-fsanitize=undefined,address \
		"${pwd}/tests/main${m}.c" \
		"${pwd}/src/list${m}/code.c" \
		"${pwd}/src/common/alloc.c" \
		"${pwd}/src/common/print.c" \
		"${pwd}/src/common/object.c" \
		-I"${pwd}/src/" \
		-I"${pwd}/src/std/" \
		-I"${pwd}/src/common/" \
		-I"${pwd}/src/list${m}/" \
		-DMEMORY_DEBUG_INFO \
		-DMEMORY_CLEANUP \
		-DMEMORY_LEAKS \
		-o "${pwd}/coverage/main${m}"

	"${pwd}/coverage/main${m}"
	lcov --capture --directory "${pwd}/coverage/" --output-file "${pwd}/coverage/coverage-main${m}.info"
	lcov --remove "${pwd}/coverage/coverage-main${m}.info" "${pwd}/src/rexo/*" -o "${pwd}/coverage/main${m}.info"
	rm "${pwd}/coverage/coverage-main${m}.info"
done

cd "${pwd}"
