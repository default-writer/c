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

#include "../common/v1/object.h"
#include "../common/v1/type.h"

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
