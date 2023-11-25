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
#include "memory/api/v2/ref_v2.h"
#include "playground/memory/api/memory.h"

#define MAX_MEMORY 0xffff /* 64K bytes */

/* offset for memory_ref structure */
extern struct memory_ref_methods memory_ref_definition_v2;

static struct memory_ref_methods* ref = &memory_ref_definition_v2;

/* api */

static void memory_init(void);
static void memory_destroy(void);
static void* memory_alloc(u64 size);
static void memory_free(void* data);

/* declaration */

/* implementation */

static void memory_init(void) {
    ref->init(0);
}

static void memory_destroy(void) {
    ref->destroy();
}

static void* memory_alloc(u64 size) {
    void* tmp = ref->peek();
    void* data = 0;
    u64 ptr_size = ref->size(tmp);
    if (ptr_size != 0 && ptr_size >= size) {
        data = ref->pop();
    } else {
        data = ref->alloc(size);
    }
    return data;
}

/* releases global memory */
static void memory_free(void* data) {
    ref->push(data);
}

/* public */

const union memory_allocator_api memory_allocator_v5 = {
    .v2.init = memory_init,
    .v2.destroy = memory_destroy,
    .v2.alloc = memory_alloc,
    .v2.free = memory_free
};
