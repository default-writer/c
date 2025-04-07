/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 7, 2025 at 6:13:33 PM GMT+3
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

#include "string_pointer_v1.h"

#define USING_SYSTEM_ERROR_API
#include "system/error/error_v1.h"
#include "system/list/list_v1.h"

#include "virtual/pointer/pointer_v1.h"

/* public */
static u64 string_pointer_free(const_vm_ptr cvm, u64 address);

/* type */
static void string_pointer_type_destructor(const_vm_ptr cvm, u64 address);

/* implementation */
static struct type_methods_definitions string_pointer_type = {
    .type_id = TYPE_STRING_POINTER,
    .destructor = string_pointer_type_destructor
};

static void string_pointer_type_destructor(const_vm_ptr cvm, u64 address) {
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, string_pointer_type.type_id);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)const_ptr, address, (u64)string_pointer_type.type_id);
        return;
    }
    CALL(pointer)->free(cvm, address);
}

static u64 string_pointer_free(const_vm_ptr cvm, u64 address) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (address == 0) {
        ERROR_INVALID_ARGUMENT("address == %lld", address);
        return FALSE;
    }
    string_pointer_type_destructor(cvm, address);
    return TRUE;
}

/* public */
CVM_EXPORT void string_pointer_init(const_vm_ptr cvm) {
    safe_type_methods_definitions safe_ptr;
    safe_ptr.const_ptr = &string_pointer_type;
    CALL(type)->register_known_type(cvm, safe_ptr.ptr);
}

const virtual_string_pointer_methods PRIVATE_API(virtual_string_pointer_methods_definitions) = {
    .free = string_pointer_free
};

const virtual_string_pointer_methods* PRIVATE_API(string_pointer) = &PRIVATE_API(virtual_string_pointer_methods_definitions);
const virtual_string_pointer_methods* CALL(string_pointer) {
    return PRIVATE_API(string_pointer);
}
