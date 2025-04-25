/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 12, 1961 at 09:07:34 PM GMT+3
 * Modified:
 *   April 24, 2025 at 7:54:58 PM GMT+3
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

#include "env_v1.h"

#define USING_SYSTEM_ERROR_API
#include "system/error/error_v1.h"

#include "system/memory/memory_v1.h"
#include "system/os/os_v1.h"

#include "virtual/pointer/pointer_v1.h"
#include "virtual/types/string/string_v1.h"
#include "virtual/virtual/virtual_v1.h"

#include "internal/pointer_type_v1.h"

/* definition */
static u64 env_getenv(const_vm_ptr cvm, u64 name);
static u64 env_getcwd(const_vm_ptr cvm);
static u64 env_puts(const_vm_ptr cvm, u64 address);

/* implementation */
static u64 env_getenv(const_vm_ptr cvm, u64 address) {
    CHECK_VM(cvm, NULL_ADDRESS);
    CHECK_ARG(address, NULL_ADDRESS);
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, TYPE_STRING);
    CHECK_POINTER(const_ptr, NULL_ADDRESS);
    const char* name = const_ptr->data;
    const char* ch = CALL(os)->getenv(name);
    return CALL(string)->load(cvm, ch);
}

static u64 env_getcwd(const_vm_ptr cvm) {
    CHECK_VM(cvm, NULL_ADDRESS);
    u64 data_ptr = 0;
    char* src = CALL(os)->calloc(1, PATH_MAX);
    src[PATH_MAX - 1] = 0;
    if (CALL(os)->getcwd(src, PATH_MAX - 1) != 0) {
        data_ptr = CALL(string)->load(cvm, src);
    }
    CALL(os)->free(src);
    return data_ptr;
}

static u64 env_puts(const_vm_ptr cvm, u64 address) {
    CHECK_VM(cvm, FALSE);
    CHECK_ARG(address, FALSE);
    u64 offset = 0;
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, TYPE_STRING);
    CHECK_POINTER(const_ptr, FALSE);
    const char* data = CALL(string)->unsafe(cvm, address);
    CHECK_VALUE(data, FALSE);
    data += offset;
    CALL(os)->puts(data);
    return TRUE;
}

/* public */
const virtual_env_methods PRIVATE_API(virtual_env_methods_definitions) = {
    .getenv = env_getenv,
    .getcwd = env_getcwd,
    .puts = env_puts
};

const virtual_env_methods* PRIVATE_API(env) = &PRIVATE_API(virtual_env_methods_definitions);
const virtual_env_methods* CALL(env) {
    return PRIVATE_API(env);
}
