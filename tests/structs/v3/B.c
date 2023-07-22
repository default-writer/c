/*
 *
 *  MIT License
 *
 * Copyright (c) 2023 Artur Mustafin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
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
    writable_pointer_B b = (writable_pointer_B)ptr->object;
    instance_interface_A->set_counter(&b->a, 1);
    b->counter = 2;
    return ptr;
}

static void print(object_typeinfo ptr) {
    B b = (B)ptr->object;
    printf("counter a: 0x%0llx\n", b->a.counter);
    printf("counter b: 0x%0llx\n", b->counter);
}

static void finalize(object_typeinfo ptr) {
    writable_pointer_B b = (writable_pointer_B)ptr->object;
    instance_interface_A->set_counter(&b->a, 0);
    b->counter = 0;
    instance_class->destroy(ptr);
}

const struct_interface_B interface_B_definition = {
    .initialize = initialize,
    .print = print,
    .finalize = finalize
};
