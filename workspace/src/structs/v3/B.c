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

#include "B.h"

extern struct_class class_definition;

extern struct_interface_A interface_A_definition;
static const interface_A instance_interface_A = &interface_A_definition;

static const class instance_class = &class_definition;

static object_typeinfo initialize(void);
static void print(object_typeinfo ptr);
static void finalize(object_typeinfo ptr);

typedef struct writable_B {
    A a;
    u64 counter;
} *writable_pointer_B, writable_struct_B;

struct_typeinfo b_typeinfo = {
    .size = sizeof(struct B),
#ifdef USE_MEMORY_DEBUG_INFO
    .name = "B"
#endif
};

static object_typeinfo initialize(void) {
    object_typeinfo ptr = instance_class->create(&b_typeinfo);
    writable_pointer_B b = (writable_pointer_B)(void*)ptr->object;
    instance_interface_A->set_counter(&b->a, 1);
    b->counter = 2;
    return ptr;
}

static void print(object_typeinfo ptr) {
    B b = (B)(void*)ptr->object;
    printf("counter a: 0x%0llx\n", b->a.counter);
    printf("counter b: 0x%0llx\n", b->counter);
}

static void finalize(object_typeinfo ptr) {
    writable_pointer_B b = (writable_pointer_B)(void*)ptr->object;
    instance_interface_A->set_counter(&b->a, 0);
    b->counter = 0;
    instance_class->destroy(ptr);
}

const struct_interface_B interface_B_definition = {
    .initialize = initialize,
    .print = print,
    .finalize = finalize
};
