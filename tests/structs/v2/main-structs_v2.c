/*
 *
 * MIT License
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
#include "class/class.h"
#include "common/alloc.h"

extern struct_class class_definition;

const class instance = &class_definition;

int main(void) {
    global_statistics();
    typedef struct B* B;

    struct A {
        u64 counter_a;
    };

    struct B {
        struct A base;
        u64 counter_b;
    };

    struct_typeinfo b_typeinfo = {
        .size = sizeof(struct B),
#ifdef USE_MEMORY_DEBUG_INFO
        .name = "B"
#endif
    };

    object_typeinfo class_object = instance->create(&b_typeinfo);

    B b = (B)class_object->object;

    b->base.counter_a = 1;
    b->counter_b = 2;

    printf("counter a: 0x%0llx\n", b->base.counter_a);
    printf("counter b: 0x%0llx\n", b->counter_b);

    instance->destroy(class_object);
    global_statistics();
    return 0;
}
