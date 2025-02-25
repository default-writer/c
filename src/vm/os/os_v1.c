/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   February 25, 2025 at 2:55:20 PM GMT+3
 *
 */
/*
    Copyright (C) 2022-2047 Artur Mustafin (artur.mustafin@gmail.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#define USING_LIST_V1

#include "os_v1.h"

#include "std/api.h"

#include "sys/memory/memory_v1.h"

#include "vm/pointer/pointer_v1.h"
#include "vm/types/string/string_v1.h"
#include "vm/virtual/virtual_v1.h"

#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <direct.h> // Windows-specific header for getcwd
#define __getcwd _getcwd // Map getcwd to _getcwd on Windows
#else
#define __getcwd getcwd // Map getcwd to _getcwd on Windows
#include <unistd.h> // POSIX header for getcwd
#endif
#define DEFAULT_SIZE 0x100

/* definition */
static u64 os_getenv(u64 name);
static u64 os_getcwd(void);
static void os_putc(u64 ptr);

/* implementation */
static u64 os_getenv(u64 ptr) {
    if (ptr == 0) {
        return 0;
    }
    pointer_ptr* ptr_ptr = CALL(virtual)->read_type(ptr, TYPE_STRING);
    if (ptr_ptr == 0 || *ptr_ptr == 0) {
        return 0;
    }
    const_pointer_ptr data_ptr = *ptr_ptr;
    const char* name_data = CALL(pointer)->read(data_ptr);
    u64 value = CALL(virtual_string)->load(getenv(name_data));
    return value;
}

static u64 os_getcwd(void) {
    u64 data_ptr = 0;
    char* src = CALL(sys_memory)->alloc(PATH_MAX);
    src[PATH_MAX - 1] = 0;
    if (__getcwd(src, PATH_MAX - 1) != 0) {
        data_ptr = CALL(virtual_string)->load(src);
    }
    CALL(sys_memory)->free(src, PATH_MAX);
    return data_ptr;
}

static void os_putc(u64 ptr) {
    const char* unsafe_data = CALL(virtual_string)->unsafe(ptr);
    if (unsafe_data == 0) {
        return;
    }
    puts(unsafe_data);
}

/* public */
const os_methods PRIVATE_API(os_methods_definitions) = {
    .getenv = os_getenv,
    .getcwd = os_getcwd,
    .putc = os_putc
};

const os_methods* CALL(os) {
    return &PRIVATE_API(os_methods_definitions);
}
