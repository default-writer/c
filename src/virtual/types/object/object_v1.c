/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 27, 2025 at 4:36:58 PM GMT+3
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

#define USING_SYSTEM_ERROR_API
#include "system/error/error_v1.h"

#include "virtual/pointer/pointer_v1.h"
#include "virtual/virtual/virtual_v1.h"

#include "internal/internal_v1.h"

static const enum type type_id = TYPE_OBJECT;

/* public */
static u64 object_alloc(const_vm_ptr vm, u64 size);
static u64 object_free(const_vm_ptr vm, u64 address);
static void* object_unsafe(const_vm_ptr vm, u64 address);
static u64 object_load(const_vm_ptr vm, const void* data, u64 size);
static u64 object_size(const_vm_ptr vm, u64 address);

/* type */
static void object_type_destructor(u64 address);

/* implementation */
static struct type_methods_definitions object_type = {
    .destructor = object_type_destructor
};

static void INIT init(void) {
    safe_type_methods_definitions safe_ptr;
    safe_ptr.const_ptr = &object_type;
    CALL(vm)->register_known_type(type_id, safe_ptr.ptr);
}

static void object_type_destructor(u64 address) {
    CALL(pointer)->free(address, type_id);
}

static u64 object_alloc(const_vm_ptr vm, u64 size) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    if (size == 0) {
        ERROR_INVALID_ARGUMENT(size == 0);
        return FALSE;
    }
    u64 address = CALL(virtual)->alloc(vm, size, type_id);
    return address;
}

static u64 object_free(const_vm_ptr vm, u64 address) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    object_type_destructor(address);
    return TRUE;
}

static void* object_unsafe(const_vm_ptr vm, u64 address) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return NULL_PTR;
    }
    const_void_ptr const_data_ptr = CALL(pointer)->read(address, type_id);
    safe_void_ptr void_ptr;
    void_ptr.const_ptr = const_data_ptr;
    void* data_ptr = void_ptr.ptr;
    if (data_ptr == 0) {
        ERROR_INVALID_POINTER(data_ptr == 0);
        return NULL_PTR;
    }
    return data_ptr;
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
        ERROR_INVALID_ARGUMENT(size == 0);
        return FALSE;
    }
    u64 address = CALL(pointer)->copy(src_data, size, 0, type_id);
    return address;
}

static u64 object_size(const_vm_ptr vm, u64 address) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    const_pointer_ptr const_ptr = CALL(virtual)->read(vm, address, type_id);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER(const_ptr == 0);
        return FALSE;
    }
    safe_pointer_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr;
    pointer_ptr ptr = safe_ptr.ptr;
    const_pointer_public_ptr public_ptr = &ptr->public;
    u64 size = public_ptr->size;
    return size;
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
