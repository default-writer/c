/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   12 December 2023 at 22:05:45 GMT+3
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

#include "vm/v1/types/data/data_v1.h"

#include "std/macros.h"

#include "vm/v1/pointer/pointer_v1.h"
#include "vm/v1/virtual/virtual_v1.h"
#include "vm/v1/vm_type.h"
#include "vm/v1/vm_v1.h"
#include "vm/vm_type.h"

#define DEFAULT_SIZE 0x100

static const enum type id = TYPE_DATA;

#ifndef ATTRIBUTE
void data_init(void);
#endif

/* internal */
static u64 data_alloc(u64 size);
static void data_free(u64 ptr);
static void data_vm_free(struct pointer* data_ptr);
static void* data_unsafe(u64 ptr);
static u64 data_size(u64 ptr);

/* implementation */
static u64 data_alloc(u64 size) {
    struct pointer* f_ptr = pointer_v1->alloc(size, id);
    u64 vm_data = virtual_v1->alloc(f_ptr);
    return vm_data;
}

static void data_free(u64 ptr) {
    struct pointer* data_ptr = virtual_v1->read_type(ptr, id);
    if (data_ptr == 0) {
        return;
    }
    data_vm_free(data_ptr);
}

static void data_vm_free(struct pointer* ptr) {
    pointer_v1->release(ptr);
}

static void* data_unsafe(u64 ptr) {
    const struct pointer* data_ptr = virtual_v1->read_type(ptr, id);
    if (data_ptr == 0) {
        return 0;
    }
    void* vm_data = pointer_v1->read(data_ptr);
    return vm_data;
}

static u64 data_size(u64 ptr) {
    const struct pointer* data_ptr = virtual_v1->read_type(ptr, id);
    if (data_ptr == 0) {
        return 0;
    }
    u64 size = pointer_v1->size(data_ptr);
    return size;
}

static const struct vm_type type = {
    .free = data_vm_free
};

static void INIT init(void) {
    pointer_v1->register_type(id, &type);
}

/* public */
const struct data_methods_v1 data_methods_definition_v1 = {
    .alloc = data_alloc,
    .free = data_free,
    .unsafe = data_unsafe,
    .size = data_size
};

#ifndef ATTRIBUTE
void data_init(void) {
    init();
}
#endif
