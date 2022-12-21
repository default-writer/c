#!/usr/bin/env bash
set -e

uid=$(id -u)

if [ "${uid}" -eq 0 ]; then
	echo "Please run as user"
	exit
fi

pwd=$(pwd)

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

if [ "$1" == "--help" ] || [ "$1" == "--?" ] || [ "${array}" == "undefined" ]; then
	script="$(basename "$(test -L "$0" && readlink "$0" || echo "$0")")"
	help=$(\
cat << EOF
${script}
Builds main test executables into build folder
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

[ ! -d "${pwd}/build" ] && mkdir "${pwd}/build"

if [ "${clean}" == "--clean" ]; then
	rm -rf "${pwd}/build"
	mkdir "${pwd}/build"
fi

cmake \
	-DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
	-DCMAKE_BUILD_TYPE:STRING=Debug \
	-DCMAKE_C_COMPILER:FILEPATH=/usr/bin/gcc \
	-DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++ \
	-S"${pwd}" \
	-B"${pwd}/cmake" \
	-G "Unix Makefiles"

export MAKEFLAGS=-j8

for m in "${array[@]}"; do
	cmake --build "${pwd}/cmake" --target "main${m}"
done

for m in "${array[@]}"; do
	cp "${pwd}/cmake/main${m}" "${pwd}/build/main${m}"
done

if [ "${clean}" == "--clean" ]; then
	rm -rf "${pwd}/cmake"
fi

cd "${pwd}"