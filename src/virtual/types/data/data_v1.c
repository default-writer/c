/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 7, 2025 at 6:13:27 PM GMT+3
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

#include "data_v1.h"

#define USING_SYSTEM_ERROR_API
#include "system/error/error_v1.h"

#include "system/memory/memory_v1.h"

#include "virtual/pointer/pointer_v1.h"
#include "virtual/types/data/data_v1.h"

#include "internal/pointer_type_v1.h"

/* public */
static u64 data_alloc(const_vm_ptr cvm, u64 size);
static u64 data_size(const_vm_ptr cvm, u64 address);
static void_ptr data_unsafe(const_vm_ptr cvm, u64 address);
static u64 data_free(const_vm_ptr cvm, u64 address);

/* type */
static void data_type_destructor(const_vm_ptr cvm, u64 address);

/* implementation */
static struct type_methods_definitions data_type = {
    .type_id = TYPE_DATA,
    .destructor = data_type_destructor
};

static void data_type_destructor(const_vm_ptr cvm, u64 address) {
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, data_type.type_id);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)const_ptr, address, (u64)data_type.type_id);
        return;
    }
    CALL(pointer)->free(cvm, address);
}

static u64 data_alloc(const_vm_ptr cvm, u64 size) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (size == 0) {
        ERROR_INVALID_ARGUMENT("size == %lld", size);
        return FALSE;
    }
    void_ptr data = CALL(memory)->alloc(size);
    u64 address = CALL(pointer)->alloc(cvm, data, size, data_type.type_id);
    return address;
}

static u64 data_size(const_vm_ptr cvm, u64 address) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (address == 0) {
        ERROR_INVALID_ARGUMENT("address == %lld", address);
        return FALSE;
    }
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, data_type.type_id);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)const_ptr, address, (u64)data_type.type_id);
        return FALSE;
    }
    safe_pointer_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr;
    pointer_ptr ptr = safe_ptr.ptr;
    const_pointer_public_ptr public_ptr = &ptr->public;
    u64 size = public_ptr->size;
    return size;
}

static void_ptr data_unsafe(const_vm_ptr cvm, u64 address) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return NULL_PTR;
    }
    if (address == 0) {
        ERROR_INVALID_ARGUMENT("address == %lld", address);
        return FALSE;
    }
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, data_type.type_id);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)const_ptr, address, (u64)data_type.type_id);
        return NULL_PTR;
    }
    safe_void_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr->data;
    void_ptr data_ptr = safe_ptr.ptr;
    return data_ptr;
}

static u64 data_free(const_vm_ptr cvm, u64 address) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (address == 0) {
        ERROR_INVALID_ARGUMENT("address == %lld", address);
        return FALSE;
    }
    data_type_destructor(cvm, address);
    return TRUE;
}

/* public */
CVM_EXPORT void data_init(const_vm_ptr cvm) {
    safe_type_methods_definitions safe_ptr;
    safe_ptr.const_ptr = &data_type;
    CALL(type)->register_known_type(cvm, safe_ptr.ptr);
}

const virtual_data_methods PRIVATE_API(virtual_data_methods_definitions) = {
    .alloc = data_alloc,
    .size = data_size,
    .unsafe = data_unsafe,
    .free = data_free
};

const virtual_data_methods* PRIVATE_API(data) = &PRIVATE_API(virtual_data_methods_definitions);
const virtual_data_methods* CALL(data) {
    return PRIVATE_API(data);
}
