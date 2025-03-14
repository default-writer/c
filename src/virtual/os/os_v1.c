/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 14, 2025 at 7:04:11 AM GMT+3
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

#include "os_v1.h"

#define USING_SYSTEM_ERROR_API
#include "system/error/error_v1.h"

#include "system/memory/memory_v1.h"

#include "virtual/api/api_v1.h"
#include "virtual/pointer/pointer_v1.h"
#include "virtual/types/string/string_v1.h"
#include "virtual/virtual/virtual_v1.h"

/* definition */
static u64 os_getenv(const_vm_ptr vm, u64 name);
static u64 os_getcwd(const_vm_ptr vm);
static u64 os_putc(const_vm_ptr vm, u64 ptr);

/* implementation */
static u64 os_getenv(const_vm_ptr vm, u64 address) {
    if (vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm);
        return FALSE;
    }
    if (address == 0) {
        ERROR_ADDRESS_NOT_INITIALIZED(address == 0);
        return FALSE;
    }
    const_pointer_ptr const_ptr = CALL(virtual)->read_type(vm, address, TYPE_STRING);
    if (const_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(const_ptr == 0);
        return FALSE;
    }
    const_pointer_ptr data_ptr = const_ptr;
    const char* name_data = CALL(pointer)->data(data_ptr);
    const char* ch = virtual_api->getenv(name_data);
    return CALL(string)->load(vm, ch);
}

static u64 os_getcwd(const_vm_ptr vm) {
    u64 data_ptr = 0;
    char* src = CALL(system_memory)->alloc(PATH_MAX);
    src[PATH_MAX - 1] = 0;
    if (virtual_api->getcwd(src, PATH_MAX - 1) != 0) {
        data_ptr = CALL(string)->load(vm, src);
    }
    CALL(system_memory)->free(src, PATH_MAX);
    return data_ptr;
}

static u64 os_putc(const_vm_ptr vm, u64 ptr) {
    const char* unsafe_data = CALL(string)->unsafe(vm, ptr);
    if (unsafe_data == 0) {
        ERROR_POINTER_NOT_INITIALIZED(unsafe_data == 0);
        return FALSE;
    }
    virtual_api->puts(unsafe_data);
    return TRUE;
}

/* public */
const virtual_os_methods PRIVATE_API(virtual_os_methods_definitions) = {
    .getenv = os_getenv,
    .getcwd = os_getcwd,
    .putc = os_putc
};

const virtual_os_methods* os = &PRIVATE_API(virtual_os_methods_definitions);

const virtual_os_methods* CALL(os) {
    return os;
}
