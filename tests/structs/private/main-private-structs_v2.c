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
#include "public.h"

int main(void) {
#ifdef USE_MEMORY_DEBUG_INFO
#if defined(VM_GLOBAL_DEBUG_INFO)
    global_statistics();
#endif
#endif

    /* 
    
    we have to split B into two parts.
    it is essential to keep in mind that first part will always be private
    this is a requirement because of how we actually done a pointer correction
    arithmetic. if we prefer not to do a pointer corrections then we can keep
    first part public (which makes sense to me cause that will helps us to
    keep things unchanged if we tend to change private structure...) that's is
    a Liskov principle to our data applcation programming interface. Lets try 
    this in the next commit

    
    | private | public |

    */

    /* 

    we need to move struct private_B into .c file to make it "private"
    of cause we just can recreate "private" struct in another file and good to go.
    for keeping really "private" stucts we probably need SipHash and address randomisation with
    some sort of guard values to validate owr own data structures, as well as CPU page related protection
    but all of this can still be flawed and hacked, obviously.
    anyway it is a a good idea to ship to the end user only .h file with struct public_B whilts 
    keeping struct private_B in .c file "hidden" (we assume reverse engineering not invented yet...)

    */

    struct B {
        // struct private_B private;
        struct public_B public;
    };

    /* 
        we still can requre this stucture in case aligment setup changes its boundaries so we do not match 
        
        |      B               |  <-- oops it is 13 bytes only
        | private B | . . . | public B | < we actually will get aligned to 16 bytes, and 42 at the end
                           ^
                           | 
         8 byte boundaries aligment (if somehow private B is not a 8n size long)

    */

    struct B* b = (struct B*)object->create(B_type);
    struct friend_B* private_ptr = (struct friend_B*)b;
    b->public.base.counter_a = 1;
    B_methods->set_counter_b(private_ptr, 2); // private_ptr->private.counter_b = 2

    printf("counter a: 0x%0llx\n", b->public.base.counter_a);
    printf("counter b: 0x%0llx\n",B_methods->get_counter_b(private_ptr)); // private_ptr->private.counter_b

    object->destroy(B_type);
#ifdef USE_MEMORY_DEBUG_INFO
#if defined(VM_GLOBAL_DEBUG_INFO)
    global_statistics();
#endif
#endif
    return 0;
}
