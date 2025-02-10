/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   February 10, 2025 at 5:22:14 PM GMT+3
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

#ifndef ATTRIBUTE
void string_pointer_init(void);
#endif

/* definition */
static void string_free(u64 ptr);

/* destructor */
static void virtual_free(pointer_ptr ptr);

/* internal */
static void string_virtual_free(pointer_ptr ptr);

/* implementation */
static void string_virtual_free(pointer_ptr ptr) {
    pointer->release(ptr);
}

static const struct type_methods_definitions _type = {
    .free = string_virtual_free
};

static void INIT init(void) {
    pointer->register_type(id, &_type);
}

/* api */
static void string_free(u64 ptr) {
    pointer_ptr data_ptr = virtual->read(ptr);
    if (data_ptr == 0) {
        return;
    }
    if (pointer->read_type(data_ptr, TYPE_STRING_POINTER)) {
        virtual_free(data_ptr);
        return;
    }
}

static void virtual_free(pointer_ptr ptr) {
    pointer->release(ptr);
}

/* public */
const string_pointer_methods PRIVATE_API(string_pointer_methods_definitions) = {
    .free = string_free
};

#ifndef ATTRIBUTE
void string_pointer_init(void) {
    init();
}
#endif
