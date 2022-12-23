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
Generates coverage info file lcov.info into coverage folder
Usage: ${script} <option>
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

[ ! -d "${pwd}/coverage" ] && mkdir "${pwd}/coverage"

if [ "${clean}" == "--clean" ]; then
    rm -rf "${pwd}/coverage"
    mkdir "${pwd}/coverage"
fi

for m in "${array[@]}"; do
    rm -f "${pwd}/coverage/main${m}.lcov"
done

find "${pwd}/coverage" -name "main*.gcda" -delete
find "${pwd}/coverage" -name "main*.gcno" -delete

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

find "${pwd}/coverage" -name "main*.gcda" -delete
find "${pwd}/coverage" -name "main*.gcno" -delete
find "${pwd}/coverage" -name "main*.lcov" -exec echo -a {} \; | xargs lcov -o "${pwd}/coverage/lcov.info"
find "${pwd}/coverage" -name "main*.lcov" -delete

cd "${pwd}"
