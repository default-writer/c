/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   February 25, 2025 at 2:54:41 PM GMT+3
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

#include "vm/pointer/pointer_v1.h"
#include "vm/virtual/virtual_v1.h"

#define DEFAULT_SIZE 0x100

static const enum type id = TYPE_STRING_POINTER;

/* definition */
static void string_free(u64 ptr);

/* destructor */
static void type_desctructor(pointer_ptr ptr);

/* implementation */
static const struct type_methods_definitions _type = {
    .desctructor = type_desctructor
};

static void INIT init(void) {
    CALL(pointer)->register_known_type(id, &_type);
}

static void type_desctructor(pointer_ptr ptr) {
    CALL(pointer)->release(ptr);
}

/* api */
static void string_free(u64 ptr) {
    pointer_ptr* data_ptr = CALL(virtual)->read(ptr);
    if (data_ptr == 0 || *data_ptr == 0) {
        return;
    }
    if (CALL(pointer)->read_type(*data_ptr, TYPE_STRING_POINTER)) {
        type_desctructor(*data_ptr);
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

const virtual_string_pointer_methods* CALL(virtual_string_pointer) {
    return &PRIVATE_API(virtual_string_pointer_methods_definitions);
}
