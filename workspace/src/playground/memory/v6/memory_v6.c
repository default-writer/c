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
#include "memory/api/v3/ref_v3.h"
#include "playground/memory/api/memory.h"

#define MAX_MEMORY 0xfffffc /* 16777215 (0xffffff) octa-bytes or 134217720 bytes (128MB) */

/* offset for memory_ref structure */
extern struct memory_ref_methods memory_ref_definition_v3;

static struct memory_ref_methods* ref = &memory_ref_definition_v3;

/* api */

static void memory_init(u64 size);
static void memory_destroy(void);
static void* memory_alloc(u64 size);
static void memory_free(void* data);

/* declaration */

/* implementation */

static void memory_init(u64 size) {
    ref->init(size);
}

static void memory_destroy(void) {
    ref->destroy();
}

static void* memory_alloc(u64 size) {
    return ref->alloc(size);
}

/* releases global memory */
static void memory_free(void* data) {
    ref->free(data);
}

/* public */

const union memory_allocator_api memory_allocator_v6 = {
    .v3.init = memory_init,
    .v3.destroy = memory_destroy,
    .v3.alloc = memory_alloc,
    .v3.free = memory_free
};