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
    *ptr = ptr + 1;
}

static void memory_destroy(void) {
    memory->free(mem, MAX_MEMORY);
    mem = 0;
    ptr = 0;
}

static void* memory_alloc(u64 size) {
    void** tmp = *ptr;
    ptr += size;
    ++ptr;
    *ptr = ptr + 1;
#ifdef USE_MEMORY_DEBUG_INFO
#if defined(VM_ALLOC_DEBUG_INFO)
    printf("  0+: %016llx > %016llx\n", (u64)tmp, (u64)(*(tmp + size)));
#endif
#endif
    return tmp;
}

/* releases global memory */
static void memory_free(void* data, u64 size) {
    CLEAN(data)
    --ptr;
    ptr -= size;
#ifdef USE_MEMORY_DEBUG_INFO
#if defined(VM_ALLOC_DEBUG_INFO)
    printf("  0-: %016llx ! %16lld\n", (u64)ptr, size);
#endif
#endif
}

const union memory_allocator_api memory_allocator_v2 = {
    .v1.init = memory_init,
    .v1.destroy = memory_destroy,
    .v1.alloc = memory_alloc,
    .v1.free = memory_free
};
