/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 12, 1961 at 09:07:34 PM GMT+3
 * Modified:
 *   May 6, 2025 at 7:55:49 AM GMT+3
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
static u64 data_type_destructor(const_vm_ptr cvm, u64 address);

/* implementation */
static struct type_methods_definitions data_type_definitions = {
    .type_id = TYPE_DATA,
    .destructor = data_type_destructor
};

static u64 data_type_destructor(const_vm_ptr cvm, u64 address) {
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, data_type_definitions.type_id);
    CHECK_POINTER(const_ptr, FALSE);
    CALL(pointer)->free(cvm, address);
    return TRUE;
}

static u64 data_alloc(const_vm_ptr cvm, u64 size) {
    CHECK_VM(cvm, NULL_ADDRESS);
    CHECK_ARG(size, NULL_ADDRESS);
    void_ptr data = CALL(memory)->alloc(size);
    u64 address = CALL(pointer)->alloc(cvm, data, size, 0, FLAG_MEMORY_PTR, data_type_definitions.type_id);
    return address;
}

static u64 data_size(const_vm_ptr cvm, u64 address) {
    CHECK_VM(cvm, NULL_VALUE);
    CHECK_ARG(address, NULL_VALUE);
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, data_type_definitions.type_id);
    CHECK_POINTER(const_ptr, NULL_VALUE);
    safe_pointer_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr;
    pointer_ptr ptr = safe_ptr.ptr;
    const_pointer_public_ptr public_ptr = &ptr->public;
    u64 size = public_ptr->size;
    return size;
}

static void_ptr data_unsafe(const_vm_ptr cvm, u64 address) {
    CHECK_VM(cvm, NULL_PTR);
    CHECK_ARG(address, NULL_PTR);
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, data_type_definitions.type_id);
    CHECK_POINTER(const_ptr, NULL_PTR);
    safe_void_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr->data;
    void_ptr data_ptr = safe_ptr.ptr;
    return data_ptr;
}

static u64 data_free(const_vm_ptr cvm, u64 address) {
    CHECK_VM(cvm, FALSE);
    CHECK_ARG(address, FALSE);
    return data_type_destructor(cvm, address);
}

/* public */
#ifdef USE_DYNAMIC_TYPES
void data_init(const_vm_ptr cvm) {
    safe_type_methods_definitions safe_ptr;
    safe_ptr.const_ptr = &data_type_definitions;
    CALL(type)->register_known_type(cvm, safe_ptr.ptr);
}
#else
INIT_TYPE(TYPE_DATA)
void data_init(void) {
    safe_type_methods_definitions safe_ptr;
    safe_ptr.const_ptr = &data_type_definitions;
    CALL(type)->register_known_type(safe_ptr.ptr);
}
#endif

const virtual_data_methods PRIVATE_API(virtual_data_methods_definitions) = {
    .alloc = data_alloc,
    .size = data_size,
    .unsafe = data_unsafe,
    .free = data_free
};

const virtual_data_methods* PRIVATE_API(data) = &PRIVATE_API(virtual_data_methods_definitions);
PUBLIC const virtual_data_methods* CALL(data) {
    return PRIVATE_API(data);
}
