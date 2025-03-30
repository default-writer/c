/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 28, 2025 at 12:49:50 PM GMT+3
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
#include "virtual/virtual/virtual_v1.h"

#include "internal/internal_v1.h"

static const enum type type_id = TYPE_DATA;

/* public */
static u64 data_alloc(const_vm_ptr cvm, u64 size);
static u64 data_free(const_vm_ptr cvm, u64 address);
static void* data_unsafe(const_vm_ptr cvm, u64 address);
static u64 data_size(const_vm_ptr cvm, u64 address);

/* type */
static void data_type_destructor(u64 address);

/* implementation */
static struct type_methods_definitions data_type = {
    .destructor = data_type_destructor
};

static void INIT init(void) {
    safe_type_methods_definitions safe_ptr;
    safe_ptr.const_ptr = &data_type;
    CALL(type)->register_known_type(type_id, safe_ptr.ptr);
}

static void data_type_destructor(u64 address) {
    CALL(pointer)->free(address, type_id);
}

static u64 data_alloc(const_vm_ptr cvm, u64 size) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", cvm);
        return FALSE;
    }
    if (size == 0) {
        ERROR_INVALID_ARGUMENT("size == %lld", size);
        return FALSE;
    }
    void* data = CALL(memory)->alloc(size);
    u64 address = CALL(pointer)->alloc(data, size, type_id);
    return address;
}

static u64 data_free(const_vm_ptr cvm, u64 address) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", cvm);
        return FALSE;
    }
    if (address == 0) {
        ERROR_INVALID_ARGUMENT("address == %lld", address);
        return FALSE;
    }
    data_type_destructor(address);
    return TRUE;
}

static void* data_unsafe(const_vm_ptr cvm, u64 address) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", cvm);
        return NULL_PTR;
    }
    if (address == 0) {
        ERROR_INVALID_ARGUMENT("address == %lld", address);
        return FALSE;
    }
    const_void_ptr const_ptr = CALL(pointer)->read(address, type_id);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", const_ptr, address, type_id);
        return NULL_PTR;
    }
    safe_void_ptr void_ptr;
    void_ptr.const_ptr = const_ptr;
    void* data_ptr = void_ptr.ptr;
    return data_ptr;
}

static u64 data_size(const_vm_ptr cvm, u64 address) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", cvm);
        return FALSE;
    }
    if (address == 0) {
        ERROR_INVALID_ARGUMENT("address == %lld", address);
        return FALSE;
    }
    const_pointer_ptr const_ptr = CALL(virtual)->read(cvm, address, type_id);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", const_ptr, address, type_id);
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
CVM_EXPORT void data_init(void) {
    init();
}

const virtual_data_methods PRIVATE_API(virtual_data_methods_definitions) = {
    .alloc = data_alloc,
    .free = data_free,
    .unsafe = data_unsafe,
    .size = data_size
};

const virtual_data_methods* PRIVATE_API(data) = &PRIVATE_API(virtual_data_methods_definitions);
const virtual_data_methods* CALL(data) {
    return PRIVATE_API(data);
}
