/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 7, 2025 at 6:13:17 PM GMT+3
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
static struct type_methods_definitions user_type = {
    .type_id = TYPE_USER,
    .destructor = user_type_destructor
};

static void user_type_destructor(const_vm_ptr cvm, u64 address) {
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, user_type.type_id);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)const_ptr, address, (u64)user_type.type_id);
        return;
    }
    CALL(pointer)->free(cvm, address);
}

static u64 user_alloc(const_vm_ptr cvm) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    u64 address = CALL(virtual)->alloc(cvm, DEFAULT_SIZE, user_type.type_id);
    return address;
}

static u64 user_free(const_vm_ptr cvm, u64 address) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (address == 0) {
        ERROR_INVALID_ARGUMENT("address == %lld", address);
        return FALSE;
    }
    user_type_destructor(cvm, address);
    return TRUE;
}

/* public */
CVM_EXPORT void user_init(const_vm_ptr cvm) {
    CALL(type)->register_user_type(cvm, &user_type);
}

const virtual_user_methods PRIVATE_API(virtual_user_methods_definitions) = {
    .alloc = user_alloc,
    .free = user_free
};

const virtual_user_methods* PRIVATE_API(user) = &PRIVATE_API(virtual_user_methods_definitions);
const virtual_user_methods* CALL(user) {
    return PRIVATE_API(user);
}
