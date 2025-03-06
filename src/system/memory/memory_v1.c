/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 6, 2025 at 12:37:41 AM GMT+3
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

#include "system/api/api_v1.h"

#include "memory_v1.h"

#include <stdlib.h>

#ifdef USE_MEMORY_DEBUG_INFO
static u64 total_alloc = 0;
static u64 total_free = 0;
#endif

struct memory_info_data {
    u64 alloc;
    u64 used;
    u64 free;
};

#ifdef USE_MEMORY_DEBUG_INFO
static struct memory_info_data memory_info;
static struct memory_info_data* base = &memory_info;
#endif

static void* memory_alloc(u64 size);
static void memory_free(void* ptr, u64 size);
static void* memory_realloc(void* old_ptr, u64 size, u64 new_size);
#ifdef USE_MEMORY_DEBUG_INFO
static void memory_set(void* dest, u8 c, u64 count);
#endif

static void* memory_alloc(u64 size) {
    if (size == 0) {
        return 0;
    }
    void* ptr = system_api->alloc(1, size);
    if (ptr == 0) {
        return 0;
    }
#ifdef USE_MEMORY_DEBUG_INFO
    total_alloc += size;
    base->used = total_alloc - total_free;
    printf("   +: %016llx ! %16lld . %16lld : %16lld : %16lld\n", (u64)ptr, size, total_alloc - total_free, total_alloc, total_free);
#endif
    return ptr;
}

static void memory_free(void* ptr, u64 size) {
    if (ptr == 0) {
        return;
    }
    if (size == 0) {
        return;
    }
#if !defined(USE_MEMORY_CLEANUP) && !defined(USE_MEMORY_DEBUG_INFO)
    (void)size; /* mark as unused when not in debug/cleanup mode */
#endif
#ifdef USE_MEMORY_CLEANUP
    memory_set(ptr, 0, size); /* NOLINT */
#endif
#ifdef USE_MEMORY_DEBUG_INFO
    total_free += size;
    printf("   -: %016llx ! %16lld . %16lld : %16lld : %16lld\n", (u64)ptr, size, total_alloc - total_free, total_alloc, total_free);
#endif
    system_api->free(ptr);
}

static void* memory_realloc(void* old_ptr, u64 size, u64 new_size) {
    if (old_ptr == 0) {
        return 0;
    }
    if (new_size <= size) {
        return 0;
    }
    void* ptr = old_ptr;
    ptr = system_api->realloc(ptr, new_size);
#ifdef USE_MEMORY_DEBUG_INFO
    total_alloc += new_size - size;
    printf("  -+: %016llx ! %16lld . %16lld : %16lld : %16lld\n", (u64)ptr, size, total_alloc - total_free, total_alloc, total_free);
#endif
    return ptr;
}

#ifdef USE_MEMORY_CLEANUP
static void memory_set(void* dest, u8 c, u64 count) {
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
        dest_ptr1[block_idx] = c;
}
#endif

#ifdef USE_MEMORY_DEBUG_INFO
void global_statistics(void) {
    printf("   .: %16s ! %16lld . %16lld : %16lld : %16lld\n", "", (u64)0, total_alloc - total_free, total_alloc, total_free);
}
#endif

/* public */
const memory_methods PRIVATE_API(system_memory_methods_definitions) = {
    .alloc = memory_alloc,
    .free = memory_free,
    .realloc = memory_realloc,
#ifdef USE_MEMORY_CLEANUP
    .set = memory_set
#endif
};

const memory_methods* CALL(system_memory) {
    return &PRIVATE_API(system_memory_methods_definitions);
}
