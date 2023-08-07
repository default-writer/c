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
#include "common/alloc.h"

#ifdef USE_MEMORY_ALLOC
#include "playground/memory/memory.h"
#endif

static u64 total_alloc = 0;
static u64 total_used = 0;
static u64 total_free = 0;

static struct memory_info memory_info;
const struct memory_info* base = &memory_info;

static void* memory_alloc(u64 size);
static void memory_free(void* ptr, u64 size);

const struct memory_info* global_memory_info(void) {
    memory_info.alloc = memory_info.used > memory_info.alloc ? memory_info.used : memory_info.alloc;
    memory_info.free = total_free;
    memory_info.used = total_alloc - total_free;
    return base;
}

void* global_alloc(u64 size) {
    void* ptr = 0;
    if (size != 0) {
        ptr = calloc(1, size);
    }
    return ptr;
}

void global_free(void* ptr, u64 size) {
    if (ptr != 0) {
#ifdef USE_MEMORY_CLEANUP
        global_memset(ptr, 0, size); /* NOLINT */
#endif
        free(ptr);
    }
}

static void* memory_alloc(u64 size) {
    void* ptr = 0;
    if (size != 0) {
#ifdef USE_MEMORY_ALLOC
        ptr = memory_alloc(nmemb, size);
#else
        ptr = calloc(1, size);
#endif
#ifdef USE_MEMORY_DEBUG_INFO
        total_alloc += size;
        const struct memory_info* memory = global_memory_info();
        printf("   +: %016llx ! %16lld . %16lld : %16lld : %16lld : %16lld\n", (u64)ptr, size, memory->used, memory->alloc, memory->free, memory_info.used + memory_info.free);
#endif
    }
    return ptr;
}

static void memory_free(void* ptr, u64 size) {
    if (ptr != 0) {
#ifdef USE_MEMORY_CLEANUP
        global_memset(ptr, 0, size); /* NOLINT */
#endif
#ifdef USE_MEMORY_ALLOC
        memory_free(ptr, size);
#else
        free(ptr);
#endif
#ifdef USE_MEMORY_DEBUG_INFO
        total_free += size;
        const struct memory_info* memory = global_memory_info();
        printf("   -: %016llx ! %16lld . %16lld : %16lld : %16lld : %16lld\n", (u64)ptr, size, memory->used, memory->alloc, memory->free, memory_info.used + memory_info.free);
#endif
    }
}

#ifdef USE_MEMORY_DEBUG_INFO
void global_statistics(void) {
    const struct memory_info* memory = global_memory_info();
    printf("   .: %16s ! %16lld . %16lld : %16lld : %16lld : %16lld\n", "", (u64)0, memory->used, memory->alloc, memory->free, memory_info.used + memory_info.free);
}
#endif

void* global_realloc(void* old_ptr, u64 size, u64 new_size) {
    void* ptr = old_ptr;
    if (ptr != 0 && new_size > size) {
        ptr = realloc(ptr, new_size);
#ifdef USE_MEMORY_DEBUG_INFO
        const struct memory_info* memory = global_memory_info();
        printf("   -: %016llx ! %16lld . %16lld : %16lld : %16lld : %16lld\n", (u64)ptr, size, memory->used, memory->alloc, memory->free, memory_info.used + memory_info.free);
        total_alloc += new_size - size;
        printf("   +: %016llx ! %16lld . %16lld : %16lld : %16lld : %16lld\n", (u64)ptr, size, memory->used, memory->alloc, memory->free, memory_info.used + memory_info.free);
#endif
    }
    return ptr;
}

void* global_memset(void* dest, u8 c, size_t count) {
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

    if (!bytes_left)
        return dest;

    u8* dest_ptr1 = (u8*)&dest_ptr8[block_idx];
    for (block_idx = 0; block_idx < bytes_left; block_idx++)
        dest_ptr1[block_idx] = (u8)c_ull;

    return dest;
}

const struct memory memory_definition = {
    .alloc = memory_alloc,
    .free = memory_free
};