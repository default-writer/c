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

#include "pointer/types/string_pointer/v1/string_pointer_v1.h"
#include "pointer/types/types.h"
#include "pointer/v1/pointer_v1.h"
#include "pointer/v1/virtual_v1.h"

#define DEFAULT_SIZE 0x100

static const enum type id = TYPE_STRING_POINTER;

/* api */
const struct string_pointer_methods string_pointer_methods_definition;

#ifndef ATTRIBUTE
void string_pointer_init(void);
#endif

/* definition */
static const struct vm_type type_definition;
static const struct vm_type* type = &type_definition;

/* definition */
static void string_free(u64 ptr);
static void string_vm_free(struct pointer* ptr);

/* internal */
static void string_virtual_free(struct pointer* ptr);

/* implementation */
static void string_virtual_free(struct pointer* ptr) {
    pointer->release(ptr);
}

static const struct vm_type type_definition = {
    .free = string_virtual_free
};

static void INIT init(void) {
    pointer->register_type(id, type);
}

/* api */
static void string_free(u64 ptr) {
    struct pointer* data_ptr = virtual->read(ptr);
    if (data_ptr == 0) {
        return;
    }
    if (pointer->read_type(data_ptr, TYPE_STRING_POINTER)) {
        string_vm_free(data_ptr);
        return;
    }
}

static void string_vm_free(struct pointer* ptr) {
    pointer->release(ptr);
}

/* public */
const struct string_pointer_methods string_pointer_methods_definition = {
    .free = string_free
};

#ifndef ATTRIBUTE
void string_pointer_init(void) {
    init();
}
#endif
