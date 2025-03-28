/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 27, 2025 at 4:38:10 PM GMT+3
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

static u64 type_id = TYPE_USER;

/* public */
static u64 user_alloc(const_vm_ptr vm);
static u64 user_free(const_vm_ptr vm, u64 ptr);

/* type */
static void user_type_destructor(u64 address);

/* implementation */
static struct type_methods_definitions user_type = {
    .destructor = user_type_destructor
};

static void INIT init(void) {
    safe_type_methods_definitions safe_ptr;
    safe_ptr.const_ptr = &user_type;
    CALL(vm)->register_user_type(safe_ptr.ptr);
    type_id = safe_ptr.const_ptr->type_id;
}

static void user_type_destructor(u64 address) {
    CALL(pointer)->free(address, type_id);
}

static u64 user_alloc(const_vm_ptr vm) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    u64 address = CALL(virtual)->alloc(vm, DEFAULT_SIZE, type_id);
    return address;
}

static u64 user_free(const_vm_ptr vm, u64 ptr) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    user_type_destructor(ptr);
    return TRUE;
}

CVM_EXPORT void user_init(void) {
    init();
}

/* public */
const virtual_user_methods PRIVATE_API(virtual_user_methods_definitions) = {
    .alloc = user_alloc,
    .free = user_free
};

const virtual_user_methods* user = &PRIVATE_API(virtual_user_methods_definitions);

const virtual_user_methods* CALL(user) {
    return user;
}
