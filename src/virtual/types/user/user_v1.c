/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 12, 1961 at 09:07:34 PM GMT+3
 * Modified:
 *   April 27, 2025 at 9:09:07 PM GMT+3
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

#include "user_v1.h"

#define USING_SYSTEM_ERROR_API
#include "system/error/error_v1.h"

#include "virtual/pointer/pointer_v1.h"
#include "virtual/virtual/virtual_v1.h"

#define DEFAULT_SIZE 0x8 /* 8 */

/* public */
static u64 user_alloc(const_vm_ptr cvm);
static u64 user_free(const_vm_ptr cvm, u64 ptr);

/* type */
static void user_type_destructor(const_vm_ptr cvm, u64 address);

/* implementation */
static struct type_methods_definitions user_type_definitions = {
    .type_id = TYPE_USER,
    .destructor = user_type_destructor
};

static void user_type_destructor(const_vm_ptr cvm, u64 address) {
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, user_type_definitions.type_id);
    CHECK_POINTER_NO_RETURN(const_ptr);
    CALL(pointer)->free(cvm, address);
}

static u64 user_alloc(const_vm_ptr cvm) {
    CHECK_VM(cvm, NULL_ADDRESS);
    u64 address = CALL(virtual)->alloc(cvm, DEFAULT_SIZE, user_type_definitions.type_id);
    return address;
}

static u64 user_free(const_vm_ptr cvm, u64 address) {
    CHECK_VM(cvm, FALSE);
    CHECK_ARG(address, FALSE);
    user_type_destructor(cvm, address);
    return TRUE;
}

/* public */
CVM_EXPORT void user_init(const_vm_ptr cvm) {
    CALL(type)->register_user_type(cvm, &user_type_definitions);
}

const virtual_user_methods PRIVATE_API(virtual_user_methods_definitions) = {
    .alloc = user_alloc,
    .free = user_free
};

const virtual_user_methods* PRIVATE_API(user) = &PRIVATE_API(virtual_user_methods_definitions);
const virtual_user_methods* CALL(user) {
    return PRIVATE_API(user);
}
