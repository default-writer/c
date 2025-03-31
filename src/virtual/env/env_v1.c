/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 31, 2025 at 12:35:34 AM GMT+3
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

#include "env_v1.h"

#define USING_SYSTEM_ERROR_API
#include "system/error/error_v1.h"

#include "system/memory/memory_v1.h"
#include "system/os/os_v1.h"

#include "virtual/pointer/pointer_v1.h"
#include "virtual/types/string/string_v1.h"

/* definition */
static u64 env_getenv(const_vm_ptr cvm, u64 name);
static u64 env_getcwd(const_vm_ptr cvm);
static u64 env_putc(const_vm_ptr cvm, u64 address);

/* implementation */
static u64 env_getenv(const_vm_ptr cvm, u64 address) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const void*)cvm);
        return FALSE;
    }
    if (address == 0) {
        ERROR_INVALID_ARGUMENT("address == %lld", address);
        return FALSE;
    }
    const char* name = CALL(pointer)->read(address, TYPE_STRING);
    if (name == 0) {
        ERROR_INVALID_POINTER("name == %p, address == %lld, type_id == %lld", (const void*)name, address, (u64)TYPE_STRING);
        return FALSE;
    }
    const char* ch = CALL(os)->getenv(name);
    return CALL(string)->load(cvm, ch);
}

static u64 env_getcwd(const_vm_ptr cvm) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const void*)cvm);
        return FALSE;
    }
    u64 data_ptr = 0;
    char* src = CALL(os)->calloc(1, PATH_MAX);
    src[PATH_MAX - 1] = 0;
    if (CALL(os)->getcwd(src, PATH_MAX - 1) != 0) {
        data_ptr = CALL(string)->load(cvm, src);
    }
    CALL(os)->free(src);
    return data_ptr;
}

static u64 env_putc(const_vm_ptr cvm, u64 address) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const void*)cvm);
        return FALSE;
    }
    if (address == 0) {
        ERROR_INVALID_ARGUMENT("address == %lld", address);
        return FALSE;
    }
    const char* data = CALL(string)->unsafe(cvm, address);
    if (data == 0) {
        ERROR_INVALID_ARGUMENT("data == %p", data);
        return FALSE;
    }
    CALL(os)->puts(data);
    return TRUE;
}

/* public */
const virtual_env_methods PRIVATE_API(virtual_env_methods_definitions) = {
    .getenv = env_getenv,
    .getcwd = env_getcwd,
    .putc = env_putc
};

const virtual_env_methods* PRIVATE_API(env) = &PRIVATE_API(virtual_env_methods_definitions);
const virtual_env_methods* CALL(env) {
    return PRIVATE_API(env);
}
