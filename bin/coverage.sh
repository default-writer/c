#!/bin/bash -e
pwd=$(pwd)

gcc --coverage -g \
    ${pwd}/tests/main.c \
    ${pwd}/src/list/code.c \
    ${pwd}/src/common/alloc.c \
    ${pwd}/src/common/print.c \
    ${pwd}/src/common/object.c \
    -I${pwd}/src/ \
    -I${pwd}/src/std/ \
    -I${pwd}/src/common/ \
    -I${pwd}/src/list/ \
    -I${pwd}/rexo/include/ \
    -o ${pwd}/build/main \
    && lcov --capture --directory ${pwd}/ --output-file lcov.info

cd ${pwd}