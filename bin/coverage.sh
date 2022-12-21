#!/usr/bin/env bash
set -e

uid=$(id -u)

if [ "${uid}" -eq 0 ]; then
	echo "Please run as user"
	exit
fi

pwd=$(pwd)

[ ! -d "${pwd}/coverage" ] && mkdir "${pwd}/coverage"

array="undefined"
clean="undefined"

if [ "$1" == "--alloc" ]; then
	array=("-alloc")
fi

if [ "$1" == "--experimental" ]; then
	array=("-experimental")
fi

if [ "$1" == "--micro" ]; then
	array=("-micro")
fi

if [ "$1" == "--light" ]; then
	array=("-light")
fi

if [ "$1" == "--all" ]; then
	array=("" "-light" "-micro" "-experimental" "-alloc")
fi

if [ "$1" == "--default" ]; then
	array=("")
fi

if [ "$2" == "" ]; then
	clean=""
fi

if [ "$2" == "--clean" ]; then
	clean="--clean"
fi

if [ "$1" == "--help" ] || [ "$1" == "--?" ] || [ "${array}" == "undefined" ] || [ "${clean}" == "undfined" ]; then
	script="$(basename "$(test -L "$0" && readlink "$0" || echo "$0")")"
	help=$(\
cat << EOF
${script}
Generates coverage info file lcov.info into coverage folder
Usage: ${script} <option> [--clean]
    --all:
        builds and runs all (-default, -light, -micro -experimental -alloc) targets
    --default:
        builds and runs -default target
    --light:
        builds and runs -light target
    --micro:
        builds and runs -micro target
    --expermental:
        builds and runs -expermental target
    --light:
        builds and runs -light target
    --clean:
        cleans up cached directories before/after build
EOF
)
	echo "${help}"
	exit
fi

if [ "${clean}" == "--clean" ]; then
	## compile with coverage metadata
	for m in "${array[@]}"; do
		rm -f "${pwd}/coverage/coverage-main${m}.info"
	done
fi

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
	rm -rf "${pwd}/coverage/main${m}"
done

if [ "${clean}" == "--clean" ]; then
	find "${pwd}/coverage" -name "*.gcda" -delete
	find "${pwd}/coverage" -name "*.gcno" -delete
	find "${pwd}/coverage" -name "*.lcov" -exec echo -a {} \; | xargs lcov -o "${pwd}/coverage/lcov.info"
	find "${pwd}/coverage" -name "*.lcov" -delete
fi

cd "${pwd}"
