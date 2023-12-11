/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   11 December 2023 at 9:15:15 GMT+3
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

#include "common/memory.h"
#include "list-micro/data.h"

#include "pointer/v1/pointer_v1.h"
#include "pointer/v1/virtual_v1.h"

#include "pointer/types/data/v1/data_v1.h"
#include "pointer/types/list/v1/list_v1.h"
#include "pointer/types/object/v1/object_v1.h"
#include "pointer/types/string/v1/string_v1.h"
#include "pointer/types/string_pointer/v1/string_pointer_v1.h"
#include "pointer/types/types.h"

#include "pointer/os/v1/os_v1.h"

#define DEFAULT_SIZE 0x100

/* api */
const struct os_methods os_methods_definition;

/* definition */
static u64 os_getenv(u64 name);
static u64 os_getcwd(void);
static void os_putc(u64 ptr);

/* implementation */
static u64 os_getenv(u64 ptr) {
    if (ptr == 0) {
        return 0;
    }
    struct pointer* data_ptr = virtual->read_type(ptr, TYPE_STRING);
    if (data_ptr == 0) {
        return 0;
    }
    const char* name_data = pointer->read(data_ptr);
    u64 value = string->load(getenv(name_data));
    return value;
}

static u64 os_getcwd(void) {
    u64 data_ptr = 0;
    char* src = memory->alloc(PATH_MAX + 1);
    data_ptr = string->load(getcwd(src, PATH_MAX));
    memory->free(src, PATH_MAX + 1);
    return data_ptr;
}

static void os_putc(u64 ptr) {
    const char* unsafe_data = string->unsafe(ptr);
    if (unsafe_data == 0) {
        return;
    }
    puts(unsafe_data);
}

/* public */
const struct os_methods os_methods_definition = {
    .getenv = os_getenv,
    .getcwd = os_getcwd,
    .putc = os_putc
};
