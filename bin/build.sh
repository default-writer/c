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

install="$1"
remove="$2"

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

if [ "${install}" == "--default" ]; then
	array=("")
fi

if [ "${remove}" == "" ]; then
	clean=""
fi

if [ "${remove}" == "--clean" ]; then
	clean="--clean"
fi

if [ "${install}" == "--help" ] || [ "${install}" == "--?" ] || [ "${array}" == "undefined" ] || [ "${clean}" == "undefined" ]; then
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

[ ! -d "${pwd}/cmake" ] && mkdir "${pwd}/cmake"

if [ "${clean}" == "--clean" ]; then
	rm -rf "${pwd}/cmake"
	mkdir "${pwd}/cmake"
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

cd "${pwd}"