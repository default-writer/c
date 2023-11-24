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

#include "../common/v3/type.h"
#include "../common/v3/public.h"
#include "../common/v3/object.h"

int main(void) {
#ifdef USE_MEMORY_DEBUG_INFO
#if defined(VM_GLOBAL_DEBUG_INFO)
    global_statistics();
#endif
#endif

    /* 
    
    object methods

    in advance we can use special section to be added for "objects" or "instances", which are opposite
    to class methods. in example below, that is called a "class" methods:

    B->get_counter_b(b)
    
    it is true because B is a collection of methods around the "class" of pointers of the same type.
    key difference that we cannot replace or override default behavior for the "instance" object associated
    to the particular "class" methods for specific classes of pointers for the data type.


    we break our struct into chunks, and label them as "private" and "public" parts, as well as allocating
    "invisible" part and "private" part


    | pointer type information - "class" methods | "pubic" part of a struct | ["instance" pointer_1 | method_1  .. "instance" pointer_n | "instance" method_n] | "private" part of a struct |
                                                 ^ 
                                                 | pointer returns the start of "public" section
    |<- "invisible"                            ->|

    we can address that "instance" part as pointer from "public" section, like this:

    b->set_counter(2);

    */

    struct B* b = object->create(B);

    struct public_B* public_ptr = (struct public_B*)b; 
    public_ptr->base.counter_a = 1;
    public_ptr->set_counter_b(b, 2); /* "instance" method */

    printf("counter a: %016llx\n", public_ptr->base.counter_a);
    printf("counter b: %016llx\n", public_ptr->get_counter_b(b)); // private_ptr->private.counter_b

    object->destroy(b);

#ifdef USE_MEMORY_DEBUG_INFO
#if defined(VM_GLOBAL_DEBUG_INFO)
    global_statistics();
#endif
#endif
    return 0;
}
