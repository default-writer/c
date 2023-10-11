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
#include "common/memory.h"
#include "std/common.h"
#include "../common/object.h"

int main(void) {
#ifdef USE_MEMORY_DEBUG_INFO
#if defined(VM_GLOBAL_DEBUG_INFO)
    global_statistics();
#endif
#endif

    struct A {
        u64 counter_a;
    };

    struct private_B {
        struct A base; /* private */
    };

    struct public_B {
        u64 counter_b;
    };

    struct B {
        // struct private_B private;
        struct public_B public;
    };

    struct typeinfo ti = {
        .size = sizeof(struct private_B) + sizeof(struct public_B),
#ifdef USE_MEMORY_DEBUG_INFO
        .name = "B"
#endif
    };

    struct B* b = (struct B*)object->create(&ti);

    // b->base.counter_a = 1;
    b->public.counter_b = 2;

    // printf("counter a: 0x%0llx\n", b->base.counter_a);
    printf("counter b: 0x%0llx\n", b->public.counter_b);

    object->destroy(&ti);
#ifdef USE_MEMORY_DEBUG_INFO
#if defined(VM_GLOBAL_DEBUG_INFO)
    global_statistics();
#endif
#endif
    return 0;
}
