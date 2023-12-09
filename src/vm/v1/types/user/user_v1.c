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
 * including prohibiting or restricive purpouses agains Russians for any EU citizens
 * or other person with USA citizenship, origin or background including work permit
 * or locations from selected territories or any territory or any other country except
 * Russia considered as breaking basic human rights, freedom of speesh or involved in
 * acts of terrorism in a territory owned, occupied or managed by another country.
 *
 */
#include "common/memory.h"

#include "vm/v1/pointer/pointer_v1.h"
#include "vm/v1/system/types_v1.h"
#include "vm/v1/virtual/virtual_v1.h"

#define DEFAULT_SIZE 0x100

static u64 id = TYPE_NULL;

/* api */
const struct user_methods user_methods_definition;

#ifndef ATTRIBUTE
void user_type_init(void);
#endif

/* definition */
static struct vm_type type_definition;
static const struct vm_type* type = &type_definition;

/* internal */
static u64 user_alloc(void);
static void user_free(u64 ptr);
static void user_vm_free(struct pointer* ptr);

/* implementation */
static u64 user_alloc(void) {
    struct pointer* ptr = pointer->alloc(0, id);
    u64 virtual_ptr = virtual->alloc(ptr);
    return virtual_ptr;
}

static void user_free(u64 ptr) {
    struct pointer* data_ptr = virtual->read_type(ptr, id);
    if (data_ptr == 0) {
        return;
    }
    user_vm_free(data_ptr);
}

static void user_vm_free(struct pointer* ptr) {
    pointer->release(ptr);
}

static struct vm_type type_definition = {
    .free = user_vm_free
};

static void INIT init(void) {
    id = pointer->register_type(TYPE_NULL, type);
}

/* public */
const struct user_methods user_methods_definition = {
    .alloc = user_alloc,
    .free = user_free
};

#ifndef ATTRIBUTE
void user_type_init(void) {
    init();
}
#endif
