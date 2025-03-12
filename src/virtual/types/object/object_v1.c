/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 12, 2025 at 9:19:14 PM GMT+3
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

#define USING_ERROR_API

#include "system/error/error_v1.h"

#include "virtual/pointer/pointer_v1.h"
#include "virtual/virtual/virtual_v1.h"

#define DEFAULT_SIZE 0x100

static const enum type type_id = TYPE_OBJECT;

/* internal */
static u64 object_alloc(const_vm_ptr vm, u64 size);
static u64 object_free(const_vm_ptr vm, u64 ptr);
static void* object_unsafe(const_vm_ptr vm, u64 ptr);
static u64 object_load(const_vm_ptr vm, const void* data, u64 size);
static u64 object_size(const_vm_ptr vm, u64 ptr);

/* destructor */
static void type_desctructor(const_pointer_ptr const_ptr);

/* implementation */
static struct type_methods_definitions object_type = {
    .desctructor = type_desctructor
};

static void INIT init(void) {
    safe_type_methods_definitions safe_ptr;
    safe_ptr.const_ptr = &object_type;
    CALL(pointer)->register_known_type(type_id, safe_ptr.ptr);
}

static void type_desctructor(const_pointer_ptr const_ptr) {
    CALL(pointer)->release(const_ptr);
}

static u64 object_alloc(const_vm_ptr vm, u64 size) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    if (size == 0) {
        ERROR_ARGUMENT_VALUE_NOT_INITIALIZED(size == 0);
        return FALSE;
    }
    return CALL(virtual)->pointer(vm, size, type_id);
}

static u64 object_free(const_vm_ptr vm, u64 ptr) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    const_pointer_ptr data_ptr = CALL(virtual)->read_type(vm, ptr, type_id);
    if (data_ptr == 0) {
        return FALSE;
    }
    type_desctructor(data_ptr);
    return TRUE;
}

static void* object_unsafe(const_vm_ptr vm, u64 ptr) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return NULL_PTR;
    }
    const_pointer_ptr data_ptr = CALL(virtual)->read_type(vm, ptr, type_id);
    if (data_ptr == 0) {
        return NULL_PTR;
    }
    void* object_data = CALL(pointer)->data(data_ptr);
    return object_data;
}

static u64 object_load(const_vm_ptr vm, const void* src_data, u64 size) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    if (src_data == 0) {
        return FALSE;
    }
    if (size == 0) {
        ERROR_ARGUMENT_VALUE_NOT_INITIALIZED(size == 0);
        return FALSE;
    }
    const_pointer_ptr data_ptr = CALL(pointer)->copy(src_data, size, type_id);
    return CALL(virtual)->alloc(vm, data_ptr);
}

static u64 object_size(const_vm_ptr vm, u64 ptr) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    const_pointer_ptr data_ptr = CALL(virtual)->read_type(vm, ptr, type_id);
    if (data_ptr == 0) {
        return FALSE;
    }
    return CALL(pointer)->size(data_ptr);
}

CVM_EXPORT void object_init(void) {
    init();
}

/* public */
const virtual_object_methods PRIVATE_API(virtual_object_methods_definitions) = {
    .alloc = object_alloc,
    .free = object_free,
    .load = object_load,
    .unsafe = object_unsafe,
    .size = object_size
};

const virtual_object_methods* object = &PRIVATE_API(virtual_object_methods_definitions);

const virtual_object_methods* CALL(object) {
    return object;
}
