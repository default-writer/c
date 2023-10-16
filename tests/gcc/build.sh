#!/usr/bin/env bash
set -e

cwd=$(pwd)

path=$1
if [[ "${path}" == "" ]]; then
    path=$(dirname 0$)
fi

cd "${path}"

[[ ! -d "${path}/bin" ]] && mkdir "${path}/bin"

# Assemble the assembly program
nasm -f elf64 "${path}/src/my_asm_program.asm" -o "${path}/bin/my_asm_program.o"

# Compile the C program and link with the assembly object
gcc -o "${path}/bin/my_program" "${path}/src/main.c" "${path}/bin/my_asm_program.o" --save-temps --debug

cp "${path}/bin/my_program-main.s" "${path}/src/main.c.s"

echo "OK"

cd "${cwd}"
