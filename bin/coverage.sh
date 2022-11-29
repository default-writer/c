#!/bin/bash -e
pwd=$(pwd)

cd "${0%/*}"
./clean.sh
cd ${pwd}

if [ ! -d "${pwd}/coverage/" ]
then
    mkdir ${pwd}/coverage/
fi

rm -f ${pwd}/coverage/*.gcda

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
    -o ${pwd}/coverage/main \
    && ${pwd}/coverage/main && lcov --capture --directory ${pwd}/coverage --output-file ${pwd}/coverage/main.info

cd ${pwd}