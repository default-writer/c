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
#include "memory/api/v1/ref_v1.h"
#include "playground/memory/api/memory.h"
#include "playground/memory/list/v1/memory_list_v1.h"

#define MAX_MEMORY 0xffff /* 64K bytes */

/* offset for memory_ref structure */
extern struct memory_ref_methods memory_ref_definition_v1;

static struct memory_ref_methods* ref = &memory_ref_definition_v1;

/* api */

static void memory_init(void);
static void memory_destroy(void);
static void* memory_alloc(u64 size);
static void memory_free(void* data);

/* declaration */

/* implementation */

static void memory_init(void) {
    ref->init();
    memory_list_init();
}

static void memory_destroy(void) {
    void* data = 0;
    while ((data = memory_list_pop()) != 0) {
        ref->free(data);
    }
    memory_list_destroy();
    ref->destroy();
}

static void* memory_alloc(u64 size) {
    void* tmp = memory_list_peek();
    void* data = 0;
    u64 ptr_size = ref->size(tmp);
    if (ptr_size != 0 && ptr_size >= size) {
        data = memory_list_pop();
    } else {
        data = ref->alloc(size);
    }
    return data;
}

/* releases global memory */
static void memory_free(void* data) {
    memory_list_push(data);
}

/* public */

const union memory_allocator_api memory_allocator_v4 = {
    .v2.init = memory_init,
    .v2.destroy = memory_destroy,
    .v2.alloc = memory_alloc,
    .v2.free = memory_free
};
