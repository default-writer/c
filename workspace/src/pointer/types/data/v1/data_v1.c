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

#include "pointer/types/data/v1/data_v1.h"
#include "pointer/types/types.h"
#include "pointer/v1/pointer_v1.h"
#include "pointer/v1/virtual_v1.h"

#define DEFAULT_SIZE 0x100

static const enum type id = TYPE_DATA;

/* api */
const struct data_methods data_methods_definition;

#ifndef ATTRIBUTE
void data_init(void);
#endif

/* definition */
static const struct vm_type type_definition;
static const struct vm_type* type = &type_definition;

/* internal */
static u64 data_alloc(u64 size);
static void data_free(u64 ptr);
static void data_vm_free(struct pointer* data_ptr);
static void* data_unsafe(u64 ptr);
static u64 data_size(u64 ptr);

/* implementation */
static u64 data_alloc(u64 size) {
    struct pointer* f_ptr = pointer->alloc(size, id);
    u64 vm_data = virtual->alloc(f_ptr);
    return vm_data;
}

static void data_free(u64 ptr) {
    struct pointer* data_ptr = virtual->read_type(ptr, id);
    if (data_ptr == 0) {
        return;
    }
    data_vm_free(data_ptr);
}

static void data_vm_free(struct pointer* ptr) {
    pointer->release(ptr);
}

static void* data_unsafe(u64 ptr) {
    const struct pointer* data_ptr = virtual->read_type(ptr, id);
    if (data_ptr == 0) {
        return 0;
    }
    void* vm_data = pointer->read(data_ptr);
    return vm_data;
}

static u64 data_size(u64 ptr) {
    const struct pointer* data_ptr = virtual->read_type(ptr, id);
    if (data_ptr == 0) {
        return 0;
    }
    u64 size = pointer->size(data_ptr);
    return size;
}

static const struct vm_type type_definition = {
    .free = data_vm_free
};

static void INIT init(void) {
    pointer->register_type(id, type);
}

/* public */
const struct data_methods data_methods_definition = {
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
