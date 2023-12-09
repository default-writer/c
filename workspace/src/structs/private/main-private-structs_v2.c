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
