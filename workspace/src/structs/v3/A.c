/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   11 December 2023 at 9:15:15 GMT+3
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

#include "std/common.h"

#include "A.h"

extern struct_class class_definition;

static const class instance_class = &class_definition;

typedef struct writable_A {
    u64 counter;
} *writable_pointer_A, writable_struct_A;

static void set_counter(A* a, u64 value);

static void set_counter(A* a, u64 value) {
    writable_pointer_A ptr = (writable_pointer_A)a;
    ptr->counter = value;
}

struct_typeinfo a_typeinfo = {
    .size = sizeof(struct A),
#ifdef USE_MEMORY_DEBUG_INFO
    .name = "A"
#endif
};

const struct_interface_A interface_A_definition = {
    .set_counter = set_counter
};
