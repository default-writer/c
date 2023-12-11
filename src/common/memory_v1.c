/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   11 December 2023 at 9:16:01 GMT+3
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

#include "common/memory_v1.h"
#include "std/data.h"
#include <stdio.h>
#include <stdlib.h>

static u64 total_alloc = 0;
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
static void memory_set(void* dest, u8 c, u64 count);

/* api */

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

#ifdef USE_MEMORY_DEBUG_INFO
void global_statistics(void) {
    printf("   .: %16s ! %16lld . %16lld : %16lld : %16lld\n", "", (u64)0, total_alloc - total_free, total_alloc, total_free);
}
#endif

const struct memory_v1 memory_definition_v1 = {
    .alloc = memory_alloc,
    .free = memory_free,
    .realloc = memory_realloc,
    .set = memory_set
};
