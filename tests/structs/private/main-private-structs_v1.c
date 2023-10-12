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

#include "../common/v1/type.h"
#include "../common/v1/public.h"
#include "../common/v1/object.h"

int main(void) {
#ifdef USE_MEMORY_DEBUG_INFO
#if defined(VM_GLOBAL_DEBUG_INFO)
    global_statistics();
#endif
#endif

    struct A {
        u64 counter_a;
    };

    /* 
    
    we have to split B into two parts.
    it is essential to keep in mind that first part will always be public
    this is a requirement because of how we actually omit a pointer correction
    arithmetic. if we prefer to do a pointer corrections then we can keep
    first part private (which makes sense to me cause that will helps us to
    keep things unchanged if we tend to change private structure...) thats the open/closed
    principle applied to our data applcation programming interface, i.e.: 

    we break our struct into chunks, and label them as "private" and "public" parts

    |      struct      |
    | public | private | 

    */
    struct private_B {
        struct A base; /* private */
    };

    struct public_B { /* public */
        u64 counter_b;
    };

    /* 

    now we actually can move struct private_B into .c file while keeping struct public_B in .h file...

    */

    struct B {
        // struct private_B private;
        struct public_B public;
    };

    struct friend_B {
        struct private_B private;
        struct public_B public;
    };

    struct typeinfo ti = {
        .size = sizeof(struct friend_B),
#ifdef USE_MEMORY_DEBUG_INFO
        .name = "B"
#endif
    };

    struct friend_B* b = (struct friend_B*)object->create(&ti);
    u64 offset = sizeof(struct private_B);

    struct B* public_ptr = (struct B*)((u64*)((void*)((u8*)b + offset)));
    b->private.base.counter_a = 1;
    public_ptr->public.counter_b = 2;

    printf("counter a: 0x%0llx\n", b->private.base.counter_a);
    printf("counter b: 0x%0llx\n", b->public.counter_b);

    object->destroy(&ti);
#ifdef USE_MEMORY_DEBUG_INFO
#if defined(VM_GLOBAL_DEBUG_INFO)
    global_statistics();
#endif
#endif
    return 0;
}
