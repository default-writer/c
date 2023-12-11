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
