/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 7, 2025 at 2:46:59 PM GMT+3
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

#include "std/api.h"

#include "virtual/pointer/pointer_v1.h"
#include "virtual/virtual/virtual_v1.h"

#define DEFAULT_SIZE 0x100

static const enum type id = TYPE_STRING_POINTER;

/* definition */
static void string_free(u64 ptr);

/* destructor */
static void type_desctructor(const_pointer_ptr const_ptr);

/* implementation */
static const struct type_methods_definitions string_pointer_type = {
    .desctructor = type_desctructor
};

static void INIT init(void) {
    CALL(pointer)->register_known_type(id, &string_pointer_type);
}

static void type_desctructor(const_pointer_ptr const_ptr) {
    CALL(pointer)->release(const_ptr);
}

/* api */
static void string_free(u64 ptr) {
    const_pointer_ptr data_ptr = CALL(virtual)->read(ptr);
    if (data_ptr == 0) {
        return;
    }
    if (CALL(pointer)->read_type(data_ptr, TYPE_STRING_POINTER)) {
        type_desctructor(data_ptr);
    }
}

#ifndef ATTRIBUTE
void string_pointer_init(void) {
    init();
}
#endif

/* public */
const virtual_string_pointer_methods PRIVATE_API(virtual_string_pointer_methods_definitions) = {
    .free = string_free
};

const virtual_string_pointer_methods* string_pointer = &PRIVATE_API(virtual_string_pointer_methods_definitions);

const virtual_string_pointer_methods* CALL(string_pointer) {
    return string_pointer;
}
