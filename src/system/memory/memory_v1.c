/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 3, 2025 at 11:20:29 AM GMT+3
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

#include "memory_v1.h"

#define USING_SYSTEM_ERROR_API
#include "system/error/error_v1.h"

#include "system/os/os_v1.h"

#ifdef USE_MEMORY_DEBUG_INFO
static u64 total_alloc = 0;
static u64 total_free = 0;
#endif

struct memory_info_data {
    u64 alloc;
    u64 free;
};

#ifdef USE_MEMORY_DEBUG_INFO
static struct memory_info_data memory_info;
static struct memory_info_data* base = &memory_info;
#endif

static void_ptr memory_alloc(u64 size);
static void_ptr memory_realloc(const_void_ptr const_ptr, u64 size, u64 new_size);
static void memory_free(const_void_ptr const_ptr, u64 size);
#ifdef USE_MEMORY_CLEANUP
static void memory_set(void_ptr dest, u8 c, u64 count);
#endif

static void_ptr memory_alloc(u64 size) {
    if (size == 0) {
        ERROR_INVALID_ARGUMENT("size == %lld", size);
        return NULL_PTR;
    }
    void_ptr ptr = CALL(os)->calloc(1, size);
    if (ptr == 0) {
        ERROR_INVALID_VALUE("ptr == %p, size == %lld", (const_void_ptr)ptr, size);
        return NULL_PTR;
    }
#ifdef USE_MEMORY_DEBUG_INFO
    total_alloc += size;
    printf("  m+: %016llx ! %16lld . %16lld : %16lld : %16lld\n", (u64)ptr, size, total_alloc - total_free, total_free, total_alloc);
#endif
    return ptr;
}

static void_ptr memory_realloc(const_void_ptr const_ptr, u64 size, u64 new_size) {
    if (const_ptr == 0) {
        ERROR_INVALID_ARGUMENT("const_ptr == %p", const_ptr);
        return NULL_PTR;
    }
    if (size == 0) {
        ERROR_INVALID_ARGUMENT("size == %lld", size);
        return NULL_PTR;
    }
    if (new_size == 0) {
        ERROR_INVALID_ARGUMENT("new_size == %lld", new_size);
        return NULL_PTR;
    }
    if (size >= new_size) {
        ERROR_INVALID_VALUE("size == %lld, new_size == %lld", size, new_size);
        return NULL_PTR;
    }
    const_void_ptr const_data_ptr = const_ptr;
    safe_void_ptr safe_ptr;
    safe_ptr.const_ptr = const_data_ptr;
    void_ptr ptr = safe_ptr.ptr;
    ptr = CALL(os)->realloc(ptr, new_size);
    CALL(os)->memset((u8*)ptr + size, 0x00, new_size - size);
#ifdef USE_MEMORY_DEBUG_INFO
    total_alloc += new_size;
    total_free += size;
    printf("  m*: %016llx ! %16lld . %16lld : %16lld : %16lld\n", (u64)ptr, size, total_alloc - total_free, total_free, total_alloc);
#endif
    return ptr;
}

static void memory_free(const_void_ptr const_ptr, u64 size) {
    if (const_ptr == 0) {
        ERROR_INVALID_ARGUMENT("const_ptr == %p", const_ptr);
        return;
    }
    if (size == 0) {
        ERROR_INVALID_ARGUMENT("size == %lld", size);
        return;
    }
    const_void_ptr const_data_ptr = const_ptr;
    safe_void_ptr safe_ptr;
    safe_ptr.const_ptr = const_data_ptr;
    void_ptr ptr = safe_ptr.ptr;
#if !defined(USE_MEMORY_CLEANUP) && !defined(USE_MEMORY_DEBUG_INFO)
    (void)size; /* mark as unused when not in debug/cleanup mode */
#endif
#ifdef USE_MEMORY_CLEANUP
    memory_set(ptr, 0, size); /* NOLINT */
#endif
#ifdef USE_MEMORY_DEBUG_INFO
    total_free += size;
    printf("  m-: %016llx ! %16lld . %16lld : %16lld : %16lld\n", (u64)ptr, size, total_alloc - total_free, total_free, total_alloc);
#endif
    CALL(os)->free(ptr);
}

#ifdef USE_MEMORY_CLEANUP
static void memory_set(void_ptr dest, u8 c, u64 count) {
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
CSYS_EXPORT void init_statistics(void) {
    total_alloc = 0;
    total_free = 0;
    printf("  m.: %16s ! %16lld . %16lld : %16lld : %16lld\n", "", (u64)0, total_alloc - total_free, total_free, total_alloc);
}

CSYS_EXPORT void result_statistics(void) {
    printf("  m.: %16s ! %16lld . %16lld : %16lld : %16lld\n", "", (u64)0, total_alloc - total_free, total_free, total_alloc);
}
#endif

/* public */
const system_memory_methods PRIVATE_API(system_memory_methods_definitions) = {
    .alloc = memory_alloc,
    .free = memory_free,
    .realloc = memory_realloc,
#ifdef USE_MEMORY_CLEANUP
    .set = memory_set
#endif
};

const system_memory_methods* PRIVATE_API(memory) = &PRIVATE_API(system_memory_methods_definitions);
const system_memory_methods* CALL(memory) {
    return PRIVATE_API(memory);
}
