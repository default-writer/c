/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   12 December 2023 at 22:06:31 GMT+3
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

#include "string_pointer_v1.h"

#include "std/macros.h"

#include "vm/v1/pointer/pointer_v1.h"
#include "vm/v1/virtual/virtual_v1.h"
#include "vm/v1/vm_type.h"
#include "vm/v1/vm_v1.h"
#include "vm/vm_type.h"

#define DEFAULT_SIZE 0x100

static const enum type id = TYPE_STRING_POINTER;

#ifndef ATTRIBUTE
void string_pointer_init(void);
#endif

/* definition */
static void string_free(u64 ptr);
static void string_vm_free(struct pointer* ptr);

/* internal */
static void string_virtual_free(struct pointer* ptr);

/* implementation */
static void string_virtual_free(struct pointer* ptr) {
    pointer_v1->release(ptr);
}

static const struct vm_type type = {
    .free = string_virtual_free
};

static void INIT init(void) {
    pointer_v1->register_type(id, &type);
}

/* api */
static void string_free(u64 ptr) {
    struct pointer* data_ptr = virtual_v1->read(ptr);
    if (data_ptr == 0) {
        return;
    }
    if (pointer_v1->read_type(data_ptr, TYPE_STRING_POINTER)) {
        string_vm_free(data_ptr);
        return;
    }
}

static void string_vm_free(struct pointer* ptr) {
    pointer_v1->release(ptr);
}

/* public */
const struct string_pointer_methods_v1 string_pointer_methods_definition_v1 = {
    .free = string_free
};

#ifndef ATTRIBUTE
void string_pointer_init(void) {
    init();
}
#endif
