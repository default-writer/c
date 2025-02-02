/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   January 30, 2025 at 8:27:16 PM GMT+3
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

#include "generic/memory_v1.h"

#include "std/data.h"
#include "std/headers.h"

#include "vm/v1/os/os_v1.h"
#include "vm/v1/pointer/pointer_v1.h"
#include "vm/v1/types/string/string_v1.h"
#include "vm/v1/types/list/list_v1.h"
#include "vm/v1/virtual/virtual_v1.h"
#include "vm/v1/vm_type.h"
#include "vm/v1/vm_v1.h"

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
    const struct pointer* data_ptr = virtual_v1->read_type(ptr, TYPE_STRING);
    if (data_ptr == 0) {
        return 0;
    }
    const char* name_data = pointer_v1->read(data_ptr);
    u64 value = type_string_v1->load(getenv(name_data));
    return value;
}

static u64 os_getcwd(void) {
    u64 data_ptr = 0;
    char* src = generic_memory_v1->alloc(PATH_MAX + 1);
    if (getcwd(src, PATH_MAX + 1) != 0) {
        data_ptr = type_string_v1->load(src);
    }
    generic_memory_v1->free(src, PATH_MAX + 1);
    return data_ptr;
}

static void os_putc(u64 ptr) {
    const char* unsafe_data = type_string_v1->unsafe(ptr);
    if (unsafe_data == 0) {
        return;
    }
    puts(unsafe_data);
}

/* public */
const struct os_methods_v1 os_methods_definition_v1 = {
    .getenv = os_getenv,
    .getcwd = os_getcwd,
    .putc = os_putc
};
