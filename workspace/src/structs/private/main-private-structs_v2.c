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

#include "../common/v2/object.h"
#include "../common/v2/public.h"
#include "../common/v2/type.h"

int main(void) {
#ifdef USE_MEMORY_DEBUG_INFO
#if defined(VM_GLOBAL_DEBUG_INFO)
    global_statistics();
#endif
#endif

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

    /*

    we moveed struct private_B into .c file and made it "private"
    of cause we just can recreate "private" struct in another file and are good to go.
    for keeping really "private" stucts we probably need SipHash and address randomisation with
    some sort of guard values to validate owr own data structures, as well as CPU page related protection
    but all of this can still be flawed and hacked, obviously.
    anyway it is a a good idea to ship to the end user only .h file with struct public_B whilts
    keeping struct private_B in .c file "hidden" (we assume reverse engineering not invented yet...)

    */

    /*
        we are not needed a structure aligment logic for private and public parts cause internal struct concatenation
        will do this automatically

        |       struct B       |  <-- great news, it is 13 bytes only!!!
        | private B | public B | < we actually got exactly the same 13 bytes-long data structure if divided into chunks

    */

    struct B* b1 = object->create(B->type());
    struct B* b2 = object->create(B->type());

    // now, since we use the same pointers for friend_B and B, we can obviously eliminate the need of friend_B
    // structure cause it is not needed anymore (we don't need it anywhere)

    struct public_B* public_ptr1 = (struct public_B*)b1;
    public_ptr1->base.counter_a = 1;
    B->set_counter_b(b1, 2);

    struct public_B* public_ptr2 = (struct public_B*)b2;
    public_ptr2->base.counter_a = 41;
    B->set_counter_b(b2, 42);

    printf("counter a: %016llx\n", public_ptr1->base.counter_a);
    printf("counter b: %016llx\n", B->get_counter_b(b1));

    printf("counter a: %016llx\n", public_ptr2->base.counter_a);
    printf("counter b: %016llx\n", B->get_counter_b(b2));

    object->destroy(b1);
    object->destroy(b2);

#ifdef USE_MEMORY_DEBUG_INFO
#if defined(VM_GLOBAL_DEBUG_INFO)
    global_statistics();
#endif
#endif
    return 0;
}
