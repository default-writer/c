/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 7, 2025 at 6:13:46 PM GMT+3
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

#include "internal/pointer_type_v1.h"

/* public */
static u64 object_alloc(const_vm_ptr cvm, u64 size);
static u64 object_free(const_vm_ptr cvm, u64 address);
static void_ptr object_unsafe(const_vm_ptr cvm, u64 address);
static u64 object_load(const_vm_ptr cvm, const_void_ptr data, u64 size);
static u64 object_size(const_vm_ptr cvm, u64 address);

/* type */
static void object_type_destructor(const_vm_ptr cvm, u64 address);

/* implementation */
static struct type_methods_definitions object_type = {
    .type_id = TYPE_OBJECT,
    .destructor = object_type_destructor
};

static void object_type_destructor(const_vm_ptr cvm, u64 address) {
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, object_type.type_id);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)const_ptr, address, (u64)object_type.type_id);
        return;
    }
    CALL(pointer)->free(cvm, address);
}

static u64 object_alloc(const_vm_ptr cvm, u64 size) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (size == 0) {
        ERROR_INVALID_ARGUMENT("size == %lld", size);
        return FALSE;
    }
    u64 address = CALL(virtual)->alloc(cvm, size, object_type.type_id);
    return address;
}

static u64 object_free(const_vm_ptr cvm, u64 address) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (address == 0) {
        ERROR_INVALID_ARGUMENT("address == %lld", address);
        return FALSE;
    }
    object_type_destructor(cvm, address);
    return TRUE;
}

static void_ptr object_unsafe(const_vm_ptr cvm, u64 address) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return NULL_PTR;
    }
    if (address == 0) {
        ERROR_INVALID_ARGUMENT("address == %lld", address);
        return NULL_PTR;
    }
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, object_type.type_id);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)const_ptr, address, (u64)object_type.type_id);
        return NULL_PTR;
    }
    safe_void_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr->data;
    void_ptr data_ptr = safe_ptr.ptr;
    return data_ptr;
}

static u64 object_load(const_vm_ptr cvm, const_void_ptr src_data, u64 size) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (src_data == 0) {
        ERROR_INVALID_ARGUMENT("src_data == %p", src_data);
        return FALSE;
    }
    if (size == 0) {
        ERROR_INVALID_ARGUMENT("size == %lld", size);
        return FALSE;
    }
    u64 address = CALL(pointer)->copy(cvm, src_data, size, 0, object_type.type_id);
    return address;
}

static u64 object_size(const_vm_ptr cvm, u64 address) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (address == 0) {
        ERROR_INVALID_ARGUMENT("address == %lld", address);
        return FALSE;
    }
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, object_type.type_id);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)const_ptr, address, (u64)object_type.type_id);
        return FALSE;
    }
    safe_pointer_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr;
    pointer_ptr ptr = safe_ptr.ptr;
    const_pointer_public_ptr public_ptr = &ptr->public;
    u64 size = public_ptr->size;
    return size;
}

/* public */
CVM_EXPORT void object_init(const_vm_ptr cvm) {
    safe_type_methods_definitions safe_ptr;
    safe_ptr.const_ptr = &object_type;
    CALL(type)->register_known_type(cvm, safe_ptr.ptr);
}

const virtual_object_methods PRIVATE_API(virtual_object_methods_definitions) = {
    .alloc = object_alloc,
    .free = object_free,
    .load = object_load,
    .unsafe = object_unsafe,
    .size = object_size
};

const virtual_object_methods* PRIVATE_API(object) = &PRIVATE_API(virtual_object_methods_definitions);
const virtual_object_methods* CALL(object) {
    return PRIVATE_API(object);
}
