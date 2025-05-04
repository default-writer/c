/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 12, 1961 at 09:07:34 PM GMT+3
 * Modified:
 *   May 4, 2025 at 10:34:40 AM GMT+3
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

#include "internal/pointer_type_v1.h"
#include "internal/vm_type_v1.h"

/* private */
typedef struct vm_state {
    stack_v2_ptr vptr;
    pointer_ptr* ref;
} vm_state_type;

#ifdef USE_MEMORY_DEBUG_INFO
static void pointer_dump(pointer_ptr ptr);
static void pointer_dump_ref(pointer_ptr* ptr);
#endif

/* internal */
INLINE static u64 pointer_alloc_internal(const_pointer_ptr* tmp, const_vm_ptr cvm, const_void_ptr data, u64 size, u64 offset, u64 flags, u64 type_id);
INLINE static u64 pointer_find_type_id_internal(const_vm_ptr cvm, const_type_methods_definitions_ptr data_type);

struct file_handler {
    FILE* file;
#ifdef USE_MEMORY_DEBUG_INFO
    char* path;
#endif
};

/* public */
static void pointer_register_known_type(const_vm_ptr cvm, type_methods_definitions_ptr data_type);
static void pointer_register_user_type(const_vm_ptr cvm, type_methods_definitions_ptr data_type);
static u64 pointer_copy(const_vm_ptr cvm, const_void_ptr data, u64 size, u64 offset, u64 type_id);
static u64 pointer_alloc(const_vm_ptr cvm, const_void_ptr data, u64 size, u64 offset, u64 flags, u64 type_id);
static const_void_ptr pointer_read(const_vm_ptr cvm, u64 address, u64 type_id);
static u64 pointer_free(const_vm_ptr cvm, u64 address);
static u64 pointer_ref(const_vm_ptr cvm, u64 address);

static void pointer_register_known_type(const_vm_ptr cvm, type_methods_definitions_ptr data_type) {
    safe_vm_ptr safe_ptr;
    safe_ptr.const_ptr = cvm;
    vm_ptr ptr = *safe_ptr.ptr;
    u64 type_id = data_type->type_id;
    ptr->types[type_id - 1] = data_type;
}

static void pointer_register_user_type(const_vm_ptr cvm, type_methods_definitions_ptr data_type) {
    CHECK_TYPE_NO_RETURN(pointer_find_type_id_internal(cvm, data_type) != 0);
    safe_vm_ptr safe_ptr;
    safe_ptr.const_ptr = cvm;
    vm_ptr ptr = *safe_ptr.ptr;
    data_type->type_id = ptr->size + 1;
    u64 type_id = data_type->type_id;
    ptr->types = CALL(os)->realloc(ptr->types, TYPE_METHODS_ARRAY_SIZE(type_id));
    ptr->size = type_id;
    ptr->types[type_id - 1] = data_type;
}

/* internal */
INLINE static u64 pointer_alloc_internal(const_pointer_ptr* tmp, const_vm_ptr cvm, const_void_ptr data, u64 size, u64 offset, u64 flags, u64 type_id) {
    u64 address = CALL(allocator)->alloc(cvm, size, type_id);
    const_pointer_ptr const_ptr = CALL(allocator)->read(cvm, address);
    safe_pointer_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr;
    pointer_ptr ptr = safe_ptr.ptr;
    ptr->data = data;
    ptr->offset = offset;
    ptr->flags = flags;
    *tmp = ptr;
    return address;
}

INLINE static u64 pointer_find_type_id_internal(const_vm_ptr cvm, const_type_methods_definitions_ptr data_type) {
    for (u64 i = 0; i < (*cvm)->size; i++) {
        type_methods_definitions_ptr current = (*cvm)->types[i];
        if (current == data_type) {
            return current->type_id;
        }
    }
    return 0;
}

/* public */
static u64 pointer_alloc(const_vm_ptr cvm, const_void_ptr data, u64 size, u64 offset, u64 flags, u64 type_id) {
    CHECK_VM(cvm, NULL_ADDRESS);
    CHECK_ARG(data, NULL_ADDRESS);
    CHECK_ARG(size, NULL_ADDRESS);
    CHECK_ARG(type_id, NULL_ADDRESS);
    const_pointer_ptr tmp;
    u64 address = pointer_alloc_internal(&tmp, cvm, data, size, offset, flags, type_id);
    return address;
}

static u64 pointer_copy(const_vm_ptr cvm, const_void_ptr data, u64 size, u64 offset, u64 type_id) {
    CHECK_VM(cvm, NULL_ADDRESS);
    CHECK_ARG(data, NULL_ADDRESS);
    CHECK_ARG(size, NULL_ADDRESS);
    CHECK_ARG(type_id, NULL_ADDRESS);
    void_ptr dst_data = CALL(memory)->alloc(size);
    const_pointer_ptr tmp;
    u64 address = pointer_alloc_internal(&tmp, cvm, dst_data, size, 0, FLAG_MEMORY_PTR, type_id);
    safe_void_ptr safe_ptr;
    safe_ptr.const_ptr = data;
    u8* src_data = (u8*)safe_ptr.ptr + offset;
    CALL(os)->memcpy(dst_data, src_data, size);
    u64 data_size = size;
    safe_void_ptr safe_ptr2;
    safe_ptr2.const_ptr = tmp->data;
    u8* bytes = safe_ptr2.ptr;
    bytes[size - 1] = 0;
    return address;
}

static const_void_ptr pointer_read(const_vm_ptr cvm, u64 address, u64 type_id) {
    CHECK_VM(cvm, NULL_PTR);
    CHECK_ARG(address, NULL_PTR);
    CHECK_ARG(type_id, NULL_PTR);
    const_pointer_ptr const_ptr = CALL(allocator)->read(cvm, address);
    CHECK_POINTER(const_ptr, NULL_PTR);
    u64 type = const_ptr->public.type;
    CHECK_TYPE(type != type_id, NULL_PTR);
    return const_ptr;
}

static u64 pointer_ref(const_vm_ptr cvm, u64 address) {
    CHECK_VM(cvm, NULL_ADDRESS);
    CHECK_ARG(address, NULL_ADDRESS);
    const_pointer_ptr const_ptr = (const_pointer_ptr)address;
    return const_ptr->public.address;
}

static u64 pointer_free(const_vm_ptr cvm, u64 address) {
    CHECK_VM(cvm, FALSE);
    CHECK_ARG(address, FALSE);
    const_pointer_ptr const_ptr = CALL(allocator)->read(cvm, address);
    CHECK_POINTER(const_ptr, FALSE);
    safe_pointer_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr;
    pointer_ptr ptr = safe_ptr.ptr;
    u64 size = const_ptr->public.size;
    if (IS_FLAG_MEMORY_PTR(ptr->flags) && size != 0) {
        CALL(memory)->free(ptr->data, size);
        ptr->data = 0;
        ptr->flags &= ~FLAG_MEMORY_PTR;
    }
    CALL(allocator)->free(cvm, address);
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
