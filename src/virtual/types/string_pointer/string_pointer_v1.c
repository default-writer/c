/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 28, 2025 at 12:50:07 PM GMT+3
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

#include "virtual/pointer/pointer_v1.h"
#include "virtual/virtual/virtual_v1.h"

static const enum type type_id = TYPE_STRING_POINTER;

/* public */
static u64 string_free(const_vm_ptr vm, u64 address);

/* type */
static void string_type_destructor(u64 address);

/* implementation */
static struct type_methods_definitions string_pointer_type = {
    .destructor = string_type_destructor
};

static void INIT init(void) {
    safe_type_methods_definitions safe_ptr;
    safe_ptr.const_ptr = &string_pointer_type;
    CALL(vm)->register_known_type(type_id, safe_ptr.ptr);
}

static void string_type_destructor(u64 address) {
    CALL(pointer)->free(address, type_id);
}

/* api */
static u64 string_free(const_vm_ptr vm, u64 address) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    const_pointer_ptr data_ptr = CALL(virtual)->read(vm, address, TYPE_STRING_POINTER);
    if (data_ptr == 0) {
        return FALSE;
    }
    string_type_destructor(address);
    return TRUE;
}

CVM_EXPORT void string_pointer_init(void) {
    init();
}

/* public */
const virtual_string_pointer_methods PRIVATE_API(virtual_string_pointer_methods_definitions) = {
    .free = string_free
};

const virtual_string_pointer_methods* string_pointer = &PRIVATE_API(virtual_string_pointer_methods_definitions);

const virtual_string_pointer_methods* CALL(string_pointer) {
    return string_pointer;
}
