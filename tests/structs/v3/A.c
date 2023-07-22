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
