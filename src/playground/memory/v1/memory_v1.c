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
#include "common/memory.h"
#include "playground/memory/api/memory.h"

#define MAX_MEMORY 0xffff /* 64K bytes */

/* private */

/* global allocated memory */
static void* mem = 0;
static void** ptr = 0;

static void memory_init(void);
static void memory_destroy(void);
static void* memory_alloc(u64 size);
static void memory_free(void* data, u64 size);

static void memory_init(void) {
    ptr = &mem;
    *ptr = memory->alloc(MAX_MEMORY);
    ptr = *ptr;
}

static void memory_destroy(void) {
    memory->free(mem, MAX_MEMORY);
    mem = 0;
    ptr = 0;
}

static void* memory_alloc(u64 size) {
    void** tmp = ptr;
    ptr += size;
#ifdef USE_MEMORY_DEBUG_INFO
    printf("  0+: %016llx > %016llx\n", (u64)tmp, (u64)ptr);
#endif
    return tmp;
}

static void memory_free(void* data, u64 size) {
    CLEAN(data)
    ptr -= size;
#ifdef USE_MEMORY_DEBUG_INFO
    printf("  0-: %016llx ! %16lld\n", (u64)ptr, size);
#endif
}

const union memory_allocator_api memory_allocator_v1 = {
    .v1.init = memory_init,
    .v1.destroy = memory_destroy,
    .v1.alloc = memory_alloc,
    .v1.free = memory_free
};
