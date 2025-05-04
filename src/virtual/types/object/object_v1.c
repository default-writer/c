/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 12, 1961 at 09:07:34 PM GMT+3
 * Modified:
 *   May 3, 2025 at 12:24:08 AM GMT+3
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
static struct type_methods_definitions object_type_definitions = {
    .type_id = TYPE_OBJECT,
    .destructor = object_type_destructor
};

static void object_type_destructor(const_vm_ptr cvm, u64 address) {
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, object_type_definitions.type_id);
    CHECK_POINTER_NO_RETURN(const_ptr);
    CALL(pointer)->free(cvm, address);
}

static u64 object_alloc(const_vm_ptr cvm, u64 size) {
    CHECK_VM(cvm, NULL_ADDRESS);
    CHECK_ARG(size, NULL_ADDRESS);
    u64 address = CALL(allocator)->alloc(cvm, size, object_type_definitions.type_id);
    return address;
}

static u64 object_free(const_vm_ptr cvm, u64 address) {
    CHECK_VM(cvm, FALSE);
    CHECK_ARG(address, FALSE);
    object_type_destructor(cvm, address);
    return TRUE;
}

static void_ptr object_unsafe(const_vm_ptr cvm, u64 address) {
    CHECK_VM(cvm, NULL_PTR);
    CHECK_ARG(address, NULL_PTR);
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, object_type_definitions.type_id);
    CHECK_POINTER(const_ptr, NULL_PTR);
    safe_void_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr->data;
    void_ptr data_ptr = safe_ptr.ptr;
    return data_ptr;
}

static u64 object_load(const_vm_ptr cvm, const_void_ptr src_data, u64 size) {
    CHECK_VM(cvm, NULL_ADDRESS);
    CHECK_ARG(src_data, NULL_ADDRESS);
    CHECK_ARG(size, NULL_ADDRESS);
    u64 address = CALL(pointer)->copy(cvm, src_data, size, 0, object_type_definitions.type_id);
    return address;
}

static u64 object_size(const_vm_ptr cvm, u64 address) {
    CHECK_VM(cvm, NULL_VALUE);
    CHECK_ARG(address, NULL_VALUE);
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, object_type_definitions.type_id);
    CHECK_POINTER(const_ptr, NULL_VALUE);
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
    safe_ptr.const_ptr = &object_type_definitions;
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
