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

#include "../common/v3/object.h"
#include "../common/v3/public.h"
#include "../common/v3/type.h"

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
