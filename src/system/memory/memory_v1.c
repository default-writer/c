/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 9, 2025 at 11:03:42 AM GMT+3
 *
 */
/*
    Copyright (C) 2022-2047 Artur Mustafin (artur.mustafin@gmail.com)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its
       contributors may be used to endorse or promote products derived from
       this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
#ifdef USE_MEMORY_DEBUG_INFO
#ifdef USE_TTY
    const char* start = "\x1b[34m";
    const char* end = "\x1b[0m";
    fprintf(stderr, "%s[  m+ ]%s: %016llx ! %16lld . %16lld : %16lld : %16lld\n", start, end, (u64)ptr, size, total_alloc - total_free, total_free, total_alloc); /* NOLINT */
#else
    fprintf(stderr, "  m+ : %016llx ! %16lld . %16lld : %16lld : %16lld\n", (u64)ptr, size, total_alloc - total_free, total_free, total_alloc); /* NOLINT */
#endif
#endif
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
    total_alloc += new_size - size;
#ifdef USE_MEMORY_DEBUG_INFO
#ifdef USE_TTY
    const char* start = "\x1b[34m";
    const char* end = "\x1b[0m";
    fprintf(stderr, "%s[  m* ]%s: %016llx ! %16lld . %16lld : %16lld : %16lld\n", start, end, (u64)ptr, new_size, total_alloc - total_free, total_free, total_alloc); /* NOLINT */
#else
    fprintf(stderr, "  m* : %016llx ! %16lld . %16lld : %16lld : %16lld\n", (u64)ptr, new_size, total_alloc - total_free, total_free, total_alloc); /* NOLINT */
#endif
#endif
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
#ifdef USE_MEMORY_DEBUG_INFO
#ifdef USE_TTY
    const char* start = "\x1b[34m";
    const char* end = "\x1b[0m";
    fprintf(stderr, "%s[  m- ]%s: %016llx ! %16lld . %16lld : %16lld : %16lld\n", start, end, (u64)ptr, size, total_alloc - total_free, total_free, total_alloc); /* NOLINT */
#else
    fprintf(stderr, "  m- : %016llx ! %16lld . %16lld : %16lld : %16lld\n", (u64)ptr, size, total_alloc - total_free, total_free, total_alloc); /* NOLINT */
#endif
#endif
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
#ifdef USE_MEMORY_DEBUG_INFO
#ifdef USE_TTY
    const char* start = "\x1b[34m";
    const char* end = "\x1b[0m";
    fprintf(stderr, "%s[  m. ]%s: %16s ! %16lld . %16lld : %16lld : %16lld\n", start, end, "", (u64)0, total_alloc - total_free, total_free, total_alloc); /* NOLINT */
#else
    fprintf(stderr, "  m. : %16s ! %16lld . %16lld : %16lld : %16lld\n", "", (u64)0, total_alloc - total_free, total_free, total_alloc); /* NOLINT */
#endif
#endif
}

CSYS_EXPORT void result_statistics(void) {
#ifdef USE_MEMORY_DEBUG_INFO
#ifdef USE_TTY
    const char* start = "\x1b[34m";
    const char* end = "\x1b[0m";
    fprintf(stderr, "%s[  m. ]%s: %16s ! %16lld . %16lld : %16lld : %16lld\n", start, end, "", (u64)0, total_alloc - total_free, total_free, total_alloc); /* NOLINT */
#else
    fprintf(stderr, "  m. : %16s ! %16lld . %16lld : %16lld : %16lld\n", "", (u64)0, total_alloc - total_free, total_free, total_alloc); /* NOLINT */
#endif
#endif
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
