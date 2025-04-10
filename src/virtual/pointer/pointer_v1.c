/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 10, 2025 at 3:22:02 PM GMT+3
 *
 */
/*
    Copyright (C) 2022-2047 Artur Mustafin (artur.mustafin@gmail.com)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its
       contributors may be used to endorse or promote products derived from
       this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "std/data.h"

#include "pointer_v1.h"

#define USING_SYSTEM_ERROR_API
#include "system/error/error_v1.h"

#define VM_TYPE_SIZE sizeof(vm_type)

#include "system/memory/memory_v1.h"
#include "system/os/os_v1.h"

#include "virtual/list/list_v1.h"

#ifdef USE_MEMORY_DEBUG_INFO
#include "virtual/pointer/pointer_v1.h"
#endif

#include "virtual/virtual/virtual_v1.h"

/* macros */
#define DEFAULT_SIZE 0x8 /* 8 */
#define POINTER_TYPE_SIZE sizeof(pointer_type)
#define KNOWN_TYPES_TYPE_SIZE sizeof(known_types_type)
#define KNOWN_TYPES_TYPE_ARRAY_SIZE(size) (size * KNOWN_TYPES_TYPE_SIZE)
#define POINTER_TYPE_SIZE sizeof(pointer_type)

/* internal */
#include "internal/pointer_type_v1.h"
#include "internal/vm_type_v1.h"

/* private */
typedef struct vm_state {
    virtual_pointer_ptr vptr;
    pointer_ptr* ref;
} vm_state_type;

#ifdef USE_MEMORY_DEBUG_INFO
static void pointer_dump(pointer_ptr ptr);
static void pointer_dump_ref(pointer_ptr* ptr);
#endif

/* internal */
struct file_handler {
    FILE* file;
#ifdef USE_MEMORY_DEBUG_INFO
    char* path;
#endif
};

struct list_handler {
    stack_ptr list;
};

/* public */
static void pointer_register_known_type(const_vm_ptr cvm, type_methods_definitions_ptr data_type);
static void pointer_register_user_type(const_vm_ptr cvm, type_methods_definitions_ptr data_type);
static u64 pointer_copy(const_vm_ptr cvm, const_void_ptr data, u64 size, u64 offset, u64 type_id);
static u64 pointer_alloc(const_vm_ptr cvm, const_void_ptr data, u64 size, u64 type_id);
static const_void_ptr pointer_read(const_vm_ptr cvm, u64 address, u64 type_id);
static u64 pointer_free(const_vm_ptr cvm, u64 address);
static u64 pointer_ref(const_vm_ptr cvm, u64 address);

/* internal */
static u64 pointer_alloc_internal(const_vm_ptr cvm, const_void_ptr data, const_pointer_ptr* tmp, u64 size, u64 type_id);
static u64 pointer_find_type_id_internal(const_vm_ptr cvm, const_type_methods_definitions_ptr data_type);

static void pointer_register_known_type(const_vm_ptr cvm, type_methods_definitions_ptr data_type) {
    safe_vm_ptr safe_ptr;
    safe_ptr.const_ptr = cvm;
    vm_ptr ptr = *safe_ptr.ptr;
    u64 type_id = data_type->type_id;
    ptr->known_types[type_id - 1].methods = data_type;
}

static void pointer_register_user_type(const_vm_ptr cvm, type_methods_definitions_ptr data_type) {
    if (pointer_find_type_id_internal(cvm, data_type) != 0) {
        ERROR_INVALID_TYPE_ID("type_id == %lld", data_type->type_id);
        return;
    }
    safe_vm_ptr safe_ptr;
    safe_ptr.const_ptr = cvm;
    vm_ptr ptr = *safe_ptr.ptr;
    data_type->type_id = ptr->known_types_capacity + 1;
    u64 type_id = data_type->type_id;
    ptr->known_types = CALL(os)->realloc(ptr->known_types, KNOWN_TYPES_TYPE_ARRAY_SIZE(type_id));
    ptr->known_types_capacity = type_id;
    ptr->known_types[type_id - 1].methods = data_type;
}

/* internal */
static u64 pointer_alloc_internal(const_vm_ptr cvm, const_void_ptr data, const_pointer_ptr* tmp, u64 size, u64 type_id) {
    u64 address = CALL(virtual)->alloc(cvm, size, type_id);
    *tmp = CALL(virtual)->read(cvm, address);
    safe_pointer_ptr safe_ptr;
    safe_ptr.const_ptr = *tmp;
    pointer_ptr ptr = safe_ptr.ptr;
    ptr->data = data;
    return address;
}

static u64 pointer_find_type_id_internal(const_vm_ptr cvm, const_type_methods_definitions_ptr data_type) {
    for (u64 i = 0; i < (*cvm)->known_types_capacity; i++) {
        known_types_type current = (*cvm)->known_types[i];
        if (current.methods == data_type) {
            return current.methods->type_id;
        }
    }
    return 0;
}

/* public */
static u64 pointer_alloc(const_vm_ptr cvm, const_void_ptr data, u64 size, u64 type_id) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (data == 0) {
        ERROR_INVALID_ARGUMENT("data == %p", data);
        return FALSE;
    }
    if (size == 0) {
        ERROR_INVALID_ARGUMENT("size == %lld", size);
        return FALSE;
    }
    if (type_id == 0) {
        ERROR_INVALID_ARGUMENT("type_id == %lld", type_id);
        return FALSE;
    }
    const_pointer_ptr tmp;
    u64 address = pointer_alloc_internal(cvm, data, &tmp, size, type_id);
    return address;
}

static u64 pointer_copy(const_vm_ptr cvm, const_void_ptr data, u64 size, u64 offset, u64 type_id) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (data == 0) {
        ERROR_INVALID_ARGUMENT("data == %p", data);
        return FALSE;
    }
    if (size == 0) {
        ERROR_INVALID_ARGUMENT("size == %lld", size);
        return FALSE;
    }
    if (type_id == 0) {
        ERROR_INVALID_ARGUMENT("type_id == %lld", type_id);
        return FALSE;
    }
    void_ptr new_data = CALL(memory)->alloc(size);
    const_pointer_ptr tmp;
    u64 address = pointer_alloc_internal(cvm, new_data, &tmp, size, type_id);
    CALL(os)->memcpy(new_data, data, size);
    u64 data_size = size;
    if (offset > 0 && offset < data_size) {
        safe_void_ptr safe_ptr;
        safe_ptr.const_ptr = tmp->data;
        pointer_ptr ptr = safe_ptr.ptr;
        void_ptr data_ptr = ptr;
        if (data_ptr != 0) {
            u8* bytes = data_ptr;
            bytes[offset] = 0;
        }
    }
    return address;
}

static const_void_ptr pointer_read(const_vm_ptr cvm, u64 address, u64 type_id) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return NULL_PTR;
    }
    if (address == 0) {
        ERROR_INVALID_ARGUMENT("address == %lld", address);
        return NULL_PTR;
    }
    if (type_id == 0) {
        ERROR_INVALID_ARGUMENT("type_id == %lld", type_id);
        return NULL_PTR;
    }
    const_pointer_ptr const_ptr = CALL(virtual)->read(cvm, address);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)const_ptr, address, type_id);
        return NULL_PTR;
    }
    u64 type = const_ptr->public.type;
    if (type != type_id) {
        ERROR_INVALID_TYPE_ID("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)const_ptr, address, type_id);
        return NULL_PTR;
    }
    return const_ptr;
}

static u64 pointer_ref(const_vm_ptr cvm, u64 address) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (address == 0) {
        ERROR_INVALID_ARGUMENT("address == %lld", address);
        return FALSE;
    }
    const_pointer_ptr const_ptr = (const_pointer_ptr)address;
    return const_ptr->public.address;
}

static u64 pointer_free(const_vm_ptr cvm, u64 address) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (address == 0) {
        ERROR_INVALID_ARGUMENT("address == %lld", address);
        return FALSE;
    }
    const_pointer_ptr const_ptr = CALL(virtual)->read(cvm, address);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld", (const_void_ptr)const_ptr, address);
        return FALSE;
    }
    safe_pointer_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr;
    pointer_ptr ptr = safe_ptr.ptr;
    u64 size = const_ptr->public.size;
    if (size != 0) {
        CALL(memory)->free(ptr->data, size);
    }
    CALL(virtual)->free(cvm, address);
    CALL(os)->free(ptr);
    return TRUE;
}

/* code */

/* public */
const virtual_pointer_methods PRIVATE_API(virtual_pointer_methods_definitions) = {
    .alloc = pointer_alloc,
    .copy = pointer_copy,
    .read = pointer_read,
    .ref = pointer_ref,
    .free = pointer_free
};

const virtual_pointer_methods* PRIVATE_API(pointer) = &PRIVATE_API(virtual_pointer_methods_definitions);
const virtual_pointer_methods* CALL(pointer) {
    return PRIVATE_API(pointer);
}

const virtual_type_methods PRIVATE_API(virtual_type_methods_definitions) = {
    .register_known_type = pointer_register_known_type,
    .register_user_type = pointer_register_user_type
};

const virtual_type_methods* PRIVATE_API(type) = &PRIVATE_API(virtual_type_methods_definitions);
const virtual_type_methods* CALL(type) {
    return PRIVATE_API(type);
}
