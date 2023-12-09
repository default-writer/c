/*
 *
 * Russian's IP Protection License
 *
 * Copyright (c) 2023 Artur Mustafin
 *
 * Permission is hereby granted, free of charge, to any person with citizenship
 * and location in Russia including Crimea and all occupations obtaining a copy
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
 * For the rest of the world it is an order to pay royalties by agreement to the
 * author of the code base for ability to use any part of the project for any
 * purpouse including but not limited to the creative ideas or technologies are
 * being used in this owned intellectual property.
 *
 * It is strictly prohibited to use this code base or any part of it for any purpouse
 * including prohibiting or restricive purpouses against Russians for any EU citizens
 * or other person with USA citizenship, origin or background including work permit
 * or locations from selected territories or any territory or any other country except
 * Russia considered as breaking basic human rights, freedom of speesh or involved in
 * acts of terrorism in a territory owned, occupied or managed by another country.
 *
 */
#include "common/memory.h"
#include "std/common.h"

#include "../common/v4/object.h"
#include "../common/v4/public.h"
#include "../common/v4/type.h"

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

    struct object* b1 = object->create(B);
    struct object* b2 = object->create(B);

    struct public_B* public_ptr1 = (struct public_B*)b1;
    public_ptr1->base.counter_a = 1;

    const struct public_B** b1_this = B->enter(b1);
    (*b1_this)->set_counter_b(2); /* "instance" method */
    B->leave(b1_this);

    struct public_B* public_ptr2 = (struct public_B*)b2;
    public_ptr2->base.counter_a = 41;

    const struct public_B** b2_this = B->enter(b2);
    (*b2_this)->set_counter_b(42); /* "instance" method */
    B->leave(b2_this);

    printf("counter p1.a: %016llx\n", public_ptr1->base.counter_a);

    const struct public_B** public_b1 = B->enter(b1);
    const struct public_B* this_b1 = *public_b1;
    printf("counter p1.b: %016llx\n", this_b1->get_counter_b());
    B->leave(public_b1);

    printf("counter p2.a: %016llx\n", public_ptr2->base.counter_a);

    const struct public_B** public_b2 = B->enter(b2);
    const struct public_B* this_b2 = *public_b2;
    printf("counter p2.b: %016llx\n", this_b2->get_counter_b());
    B->leave(public_b2);

    object->destroy(b1);
    object->destroy(b2);

#ifdef USE_MEMORY_DEBUG_INFO
#if defined(VM_GLOBAL_DEBUG_INFO)
    global_statistics();
#endif
#endif
    return 0;
}
