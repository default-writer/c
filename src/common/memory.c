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

static u64 total_alloc = 0;
static u64 total_used = 0;
static u64 total_free = 0;

struct memory_info_data {
    u64 alloc;
    u64 used;
    u64 free;
};

static struct memory_info_data memory_info;
static struct memory_info_data* base = &memory_info;

static void* memory_alloc(u64 size);
static void memory_free(void* ptr, u64 size);
static void* memory_realloc(void* old_ptr, u64 size, u64 new_size);
static void memory_set(void* dest, u8 c, size_t count);

/* api */
void global_statistics(void);

static void* memory_alloc(u64 size) {
    void* ptr = 0;
    if (size != 0) {
        ptr = calloc(1, size);
#ifdef USE_MEMORY_DEBUG_INFO
        total_alloc += size;
        base->used = total_alloc - total_free;
#if defined(VM_ALLOC_DEBUG_INFO)
        printf("   +: %016llx ! %16lld . %16lld : %16lld : %16lld\n", (u64)ptr, size, total_alloc - total_free, total_alloc, total_free);
#endif
#endif
    }
    return ptr;
}

static void memory_free(void* ptr, u64 size) {
    if (ptr != 0) {
#ifdef USE_MEMORY_CLEANUP
        memory_set(ptr, 0, size); /* NOLINT */
#endif
       free(ptr);
#ifdef USE_MEMORY_DEBUG_INFO
        total_free += size;
#if defined(VM_ALLOC_DEBUG_INFO)
        printf("   -: %016llx ! %16lld . %16lld : %16lld : %16lld\n", (u64)ptr, size, total_alloc - total_free, total_alloc, total_free);
#endif
#endif
    }
}

static void* memory_realloc(void* old_ptr, u64 size, u64 new_size) {
    void* ptr = old_ptr;
    if (ptr != 0 && new_size > size) {
        ptr = realloc(ptr, new_size);
#ifdef USE_MEMORY_DEBUG_INFO
#if defined(VM_ALLOC_DEBUG_INFO)
        printf("   -: %016llx ! %16lld . %16lld : %16lld : %16lld\n", (u64)ptr, size, total_alloc - total_free, total_alloc, total_free);
#endif
        total_alloc += new_size - size;
#if defined(VM_ALLOC_DEBUG_INFO)
        printf("   +: %016llx ! %16lld . %16lld : %16lld : %16lld\n", (u64)ptr, size, total_alloc - total_free, total_alloc, total_free);
#endif
#endif
    }
    return ptr;
}

static void memory_set(void* dest, u8 c, size_t count) {
    size_t block_idx = 0;
    size_t blocks = count >> 3;
    size_t bytes_left = count - (blocks << 3);

    u64 c_ull = c
        | (((u64)c) << 8)
        | (((u64)c) << 16)
        | (((u64)c) << 24)
        | (((u64)c) << 32)
        | (((u64)c) << 40)
        | (((u64)c) << 48)
        | (((u64)c) << 56);

    u64* dest_ptr8 = (u64*)dest;
    for (block_idx = 0; block_idx < blocks; block_idx++)
        dest_ptr8[block_idx] = c_ull;

    u8* dest_ptr1 = (u8*)&dest_ptr8[block_idx];
    for (block_idx = 0; block_idx < bytes_left; block_idx++)
        dest_ptr1[block_idx] = (u8)c;
}

#ifdef USE_MEMORY_DEBUG_INFO
void global_statistics(void) {
    printf("   .: %16s ! %16lld . %16lld : %16lld : %16lld\n", "", (u64)0, total_alloc - total_free, total_alloc, total_free);
}
#endif

const struct memory memory_definition = {
    .alloc = memory_alloc,
    .free = memory_free,
    .realloc = memory_realloc,
    .set = memory_set
};
