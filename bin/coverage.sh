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
	lcov --capture --directory "${pwd}/coverage/" --output-file "${pwd}/coverage/main${m}.lcov"
	lcov --remove "${pwd}/coverage/main${m}.lcov" "${pwd}/src/rexo/*" -o "${pwd}/coverage/main${m}.lcov"
done

find "${pwd}/coverage" -name "*.gcda" -delete
find "${pwd}/coverage" -name "*.gcno" -delete
find "${pwd}/coverage" -name "*.lcov" -exec echo -a {} \; | xargs lcov -o "${pwd}/coverage/lcov.info"
find "${pwd}/coverage" -name "*.lcov" -delete

cd "${pwd}"
