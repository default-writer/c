/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   11 December 2023 at 9:15:15 GMT+3
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
#if defined(VM_ALLOC_DEBUG_INFO)
    printf("  0+: %016llx > %016llx\n", (u64)ptr, (u64)(*ptr));
#endif
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
#if defined(VM_ALLOC_DEBUG_INFO)
    printf("  0-: %016llx ! %16lld\n", (u64)last, size);
#endif
#endif
    memory->free(head - 2, ALLOC_SIZE(size));
}

const union memory_allocator_api memory_allocator_v3 = {
    .v1.init = memory_init,
    .v1.destroy = memory_destroy,
    .v1.alloc = memory_alloc,
    .v1.free = memory_free
};
