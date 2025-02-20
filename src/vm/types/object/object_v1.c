/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   February 20, 2025 at 5:52:48 AM GMT+3
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

#include "object_v1.h"

#include "std/api.h"

#include "vm/pointer/pointer_v1.h"
#include "vm/virtual/virtual_v1.h"

#include <string.h>

#define DEFAULT_SIZE 0x100

static const enum type id = TYPE_OBJECT;

/* internal */
static u64 object_alloc(u64 size);
static void object_free(u64 ptr);
static void* object_unsafe(u64 ptr);
static u64 object_load(const void* data, u64 size);
static u64 object_size(u64 ptr);

/* destructor */
static void virtual_free(pointer_ptr ptr);

/* implementation */
static u64 object_alloc(u64 size) {
    if (size == 0) {
        return 0;
    }
    pointer_ptr ptr = CALL(pointer)->alloc(size, id);
    u64 virtual_ptr = CALL(virtual)->alloc(ptr);
    return virtual_ptr;
}

static void object_free(u64 ptr) {
    pointer_ptr data_ptr = CALL(virtual)->read_type(ptr, id);
    if (data_ptr == 0) {
        return;
    }
    virtual_free(data_ptr);
}

static void virtual_free(pointer_ptr ptr) {
    CALL(pointer)->release(ptr);
}

static void* object_unsafe(u64 ptr) {
    const_pointer_ptr data_ptr = CALL(virtual)->read_type(ptr, id);
    if (data_ptr == 0) {
        return 0;
    }
    void* object_data = CALL(pointer)->read(data_ptr);
    return object_data;
}

static u64 object_load(const void* src_data, u64 size) {
    if (src_data == 0) {
        return 0;
    }
    if (size == 0) {
        return 0;
    }
    pointer_ptr data_ptr = CALL(pointer)->alloc(size, id);
    memcpy(CALL(pointer)->read(data_ptr), src_data, size); /* NOLINT */
    u64 virtual_ptr = CALL(virtual)->alloc(data_ptr);
    return virtual_ptr;
}

static u64 object_size(u64 ptr) {
    const_pointer_ptr data_ptr = CALL(virtual)->read_type(ptr, id);
    if (data_ptr == 0) {
        return 0;
    }
    u64 size = CALL(pointer)->size(data_ptr);
    return size;
}

static const struct type_methods_definitions _type = {
    .free = virtual_free
};

static void INIT init(void) {
    CALL(pointer)->register_type(id, &_type);
}

#ifndef ATTRIBUTE
void object_init(void) {
    init();
}
#endif

/* public */
const virtual_object_methods PRIVATE_API(virtual_object_methods_definitions) = {
    .alloc = object_alloc,
    .free = object_free,
    .load = object_load,
    .unsafe = object_unsafe,
    .size = object_size
};

const virtual_object_methods* _virtual_object() {
    return &PRIVATE_API(virtual_object_methods_definitions);
}
