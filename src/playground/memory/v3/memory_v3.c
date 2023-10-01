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

/* macros */
#define DEFAULT_SIZE 0x0 /* 0 */
#define PTR_SIZE sizeof(void*) /* size of a pointer */
#define ALLOC_SIZE(size) ((size + 3) * PTR_SIZE)

/* private */

/* global allocated memory */
static void* mem = 0;
static void** ptr = 0;
static void* memory_alloc_internal(void** next, void* prev, u64 size);

static void memory_init(void);
static void memory_destroy(void);
static void* memory_alloc(u64 size);
static void memory_free(void* data, u64 size);

static void* memory_alloc_internal(void** next, void* prev, u64 size) {
    void** tmp = memory->alloc(ALLOC_SIZE(size)); //
    next = *next;
    *next = tmp;

    *tmp = prev;
    *(tmp + 1) = tmp + size + 2;
    *(tmp + 2) = next;

    return tmp + 2;
}

static u64 offset(const void* data) {
    const void* const* head = data;
    const void* const* next = *(head - 1);
    return (u64)(next - head);
}

static void memory_init(void) {
    ptr = &mem;
    *ptr = ptr;
    ptr = memory_alloc_internal(ptr, 0, DEFAULT_SIZE);
}

static void memory_destroy(void) {
    memory->free(mem, ALLOC_SIZE(DEFAULT_SIZE));
    mem = 0;
    ptr = 0;
}

static void* memory_alloc(u64 size) {
    ptr = memory_alloc_internal(ptr - 1, ptr, size);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("  0+: %016llx > %016llx\n", (u64)ptr, (u64)(*ptr));
#endif
    return ptr;
}

/* releases global memory */
static void memory_free(void* data, u64 size) {
    void** head = data;
    void** next = *(head - 1);
    void** last = *(head - 2);
    if (*next == 0) {
        ptr = last;
        next = ptr + offset(ptr);
        *next = 0;
    }
    CLEAN(size)
    size = offset(data);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("  0-: %016llx ! %16lld\n", (u64)last, size);
#endif
    memory->free(head - 2, ALLOC_SIZE(size));
}

const union memory_allocator_api memory_allocator_v3 = {
    .v1.init = memory_init,
    .v1.destroy = memory_destroy,
    .v1.alloc = memory_alloc,
    .v1.free = memory_free
};
