/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 16, 2025 at 1:11:14 AM GMT+3
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

#include "string_reference_v1.h"

#define USING_SYSTEM_ERROR_API
#include "system/error/error_v1.h"

#include "virtual/list/list_v1.h"
#include "virtual/pointer/pointer_v1.h"

/* public */
static u64 string_reference_free(const_vm_ptr cvm, u64 address);

/* type */
static void string_reference_type_destructor(const_vm_ptr cvm, u64 address);

/* implementation */
static struct type_methods_definitions string_reference_type_definitions = {
    .type_id = TYPE_STRING_REFERENCE,
    .destructor = string_reference_type_destructor
};

static void string_reference_type_destructor(const_vm_ptr cvm, u64 address) {
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, string_reference_type_definitions.type_id);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)const_ptr, address, (u64)string_reference_type_definitions.type_id);
        return;
    }
    CALL(pointer)->free(cvm, address);
}

static u64 string_reference_free(const_vm_ptr cvm, u64 address) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (address == 0) {
        ERROR_INVALID_ARGUMENT("address == %lld", address);
        return FALSE;
    }
    string_reference_type_destructor(cvm, address);
    return TRUE;
}

/* public */
CVM_EXPORT void string_reference_init(const_vm_ptr cvm) {
    safe_type_methods_definitions safe_ptr;
    safe_ptr.const_ptr = &string_reference_type_definitions;
    CALL(type)->register_known_type(cvm, safe_ptr.ptr);
}

const virtual_string_reference_methods PRIVATE_API(virtual_string_reference_methods_definitions) = {
    .free = string_reference_free
};

const virtual_string_reference_methods* PRIVATE_API(string_reference) = &PRIVATE_API(virtual_string_reference_methods_definitions);
const virtual_string_reference_methods* CALL(string_reference) {
    return PRIVATE_API(string_reference);
}
