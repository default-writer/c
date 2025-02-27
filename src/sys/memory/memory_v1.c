/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   February 27, 2025 at 8:28:06 AM GMT+3
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef USE_MEMORY_DEBUG_INFO
static u64 total_alloc = 0;
static u64 total_free = 0;

struct memory_info_data {
    u64 alloc;
    u64 used;
    u64 free;
};

static struct memory_info_data memory_info;
static struct memory_info_data* base = &memory_info;
#endif

static void* memory_alloc(u64 size);
static void memory_free(void* ptr, u64 size);
static void* memory_realloc(void* old_ptr, u64 size, u64 new_size);
static void* mmap_alloc(u64 size);
static void mmap_free(void* ptr, u64 size);
static void* memory_vma_alloc(u64 size);
static void memory_vma_free(void* ptr, u64 size);
static void* memory_vma_realloc(void* old_ptr, u64 size, u64 new_size);
#ifdef USE_MEMORY_DEBUG_INFO
static void memory_set(void* dest, u8 c, u64 count);
#endif

/* definition */
struct vma {
    void* start;
    void* end;
    struct vma* prev;
    struct vma* next;
};

static struct vma* vma_list = 0;

#ifdef USE_MEMORY_DEBUG_INFO
static u64 total_alloc = 0;
static u64 total_free = 0;
#endif

static int can_merge(const struct vma* a, const struct vma* b) {
    return a->end == b->start;
}

static void vma_insert(struct vma* new_vma) {
    struct vma* current = vma_list;
    struct vma* prev = 0;

    while (current && current->start < new_vma->start) {
        prev = current;
        current = current->next;
    }

    new_vma->prev = prev;
    new_vma->next = current;
    if (prev)
        prev->next = new_vma;
    if (current)
        current->prev = new_vma;
    if (!prev)
        vma_list = new_vma;

    if (prev && can_merge(prev, new_vma)) {
        prev->end = new_vma->end;
        prev->next = new_vma->next;
        if (new_vma->next)
            new_vma->next->prev = prev;
        free(new_vma);
        new_vma = prev;
    }

    if (new_vma->next && can_merge(new_vma, new_vma->next)) {
        new_vma->end = new_vma->next->end;
        struct vma* to_free = new_vma->next;
        new_vma->next = to_free->next;
        if (to_free->next)
            to_free->next->prev = new_vma;
        free(to_free);
    }
}

static void vma_remove(struct vma* vma) {
    struct vma* prev = vma->prev;
    struct vma* next = vma->next;

    if (prev)
        prev->next = next;
    if (next)
        next->prev = prev;
    if (vma == vma_list)
        vma_list = next;

    free(vma);
}

/* api */
static void* memory_alloc(u64 size) {
    void* ptr = 0;
    if (size == 0) {
        return 0;
    }
    ptr = memory_vma_alloc(size);
#ifdef USE_MEMORY_DEBUG_INFO
    total_alloc += size;
    base->used = total_alloc - total_free;
    printf("   +: %016llx ! %16lld . %16lld : %16lld : %16lld\n", (u64)ptr, size, total_alloc - total_free, total_alloc, total_free);
#endif
    return ptr;
}

static void memory_free(void* ptr, u64 size) {
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
    memory_vma_free(ptr, size);
}

static void* memory_realloc(void* old_ptr, u64 size, u64 new_size) {
    void* ptr = old_ptr;
    if (ptr != 0 && new_size > size) {
        ptr = memory_vma_realloc(ptr, size, new_size);
#ifdef USE_MEMORY_DEBUG_INFO
        total_alloc += new_size - size;
        printf("  -+: %016llx ! %16lld . %16lld : %16lld : %16lld\n", (u64)ptr, size, total_alloc - total_free, total_alloc, total_free);
#endif
    }
    return ptr;
}

static void* memory_mmap_alloc(u64 size) {
    void* ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED) {
        return 0;
    }
#ifdef USE_MEMORY_DEBUG_INFO
    total_alloc += size;
    base->used = total_alloc - total_free;
    printf("   +: %016llx ! %16lld . %16lld : %16lld : %16lld\n", (u64)ptr, size, total_alloc - total_free, total_alloc, total_free);
#endif
    return ptr;
}

static u64 memory_mmap_free(void* ptr, u64 size) {
    if (ptr == 0) {
        return 0;
    }
#ifdef USE_MEMORY_DEBUG_INFO
    total_free += size;
    printf("   -: %016llx ! %16lld . %16lld : %16lld : %16lld\n", (u64)ptr, size, total_alloc - total_free, total_alloc, total_free);
#endif
    u64 result = (u64)munmap(ptr, size);
    if (result == (u64)MAP_FAILED) {
        return 0;
    }
    return result;
}

static void* memory_vma_alloc(u64 size) {
    if (size >= 0xffffffffffffffd7 || size == 0) {
        return 0;
    }

    void* ptr = CALL(sys_memory_mmap)->alloc(size);

    if (ptr == 0) {
        return 0;
    }

    struct vma* new_vma = malloc(sizeof(struct vma));

    void* end = (void*)((u8*)ptr + size);
    *new_vma = (struct vma) {
        .start = ptr,
        .end = end
    };

    vma_insert(new_vma);

#ifdef USE_MEMORY_DEBUG_INFO
    total_alloc += size;
    printf("allocated %p-%p (%llu bytes)\n", ptr, end, size);
#endif

    return ptr;
}

static void memory_vma_free(void* ptr, u64 size) {
    if (ptr == 0) {
        return;
    }
    struct vma* current = vma_list;
    void* end = (void*)((u8*)ptr + size);
    while (current) {
        if (current->start == ptr && current->end == end) {
#ifdef USE_MEMORY_DEBUG_INFO
            printf("freeing %p-%p (%llu bytes)\n", current->start, current->end, size);
#endif
            vma_remove(current);
            break;
        }
        if (current->start < ptr && current->end > end) {
            struct vma* new_vma = malloc(sizeof(struct vma));
            *new_vma = (struct vma) {
                .start = end,
                .end = current->end,
                .prev = current,
                .next = current->next
            };
            if (current->next) {
                current->next->prev = new_vma;
            }
            current->end = ptr;
            current->next = new_vma;
            break;
        }
        if (current->start == ptr && current->end > end) {
            current->start = end;
            break;
        }
        if (current->start < ptr && current->end == end) {
            current->end = ptr;
            break;
        }
        current = current->next;
    }
    CALL(sys_memory_mmap)->free(ptr, size);
}

static void* memory_vma_realloc(void* old_ptr, u64 old_size, u64 new_size) {
    if (old_ptr == 0 || new_size <= old_size) {
        return 0;
    }
    void* new_ptr = memory_vma_alloc(new_size);
    if (new_ptr == 0) {
        return 0;
    }

    u64 copy_size = old_size < new_size ? old_size : new_size;
    memcpy(new_ptr, old_ptr, copy_size);

    memory_vma_free(old_ptr, old_size);

#ifdef USE_MEMORY_DEBUG_INFO
    total_alloc += new_size - old_size;
    base->used = total_alloc - total_free;
    printf("  -+: %016llx ! %16lld . %16lld : %16lld : %16lld\n", (u64)new_ptr, new_size, total_alloc - total_free, total_alloc, total_free);
#endif

    return new_ptr;
}

#ifdef USE_MEMORY_DEBUG_INFO
static void memory_vma_print() {
    struct vma* current = vma_list;
    printf("current VMAs:\n");
    while (current) {
        printf("  %p-%p \n", current->start, current->end);
        current = current->next;
    }
}
#endif

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
static void global_statistics(void) {
    printf("   .: %16s ! %16lld . %16lld : %16lld : %16lld\n", "", (u64)0, total_alloc - total_free, total_alloc, total_free);
}
#endif

/* public */
const memory_methods PRIVATE_API(memory_methods_definitions) = {
    .alloc = memory_alloc,
    .free = memory_free,
    .realloc = memory_realloc,
    .vma_alloc = memory_vma_alloc,
    .vma_free = memory_vma_free,
    .vma_realloc = memory_vma_realloc,
#ifdef USE_MEMORY_DEBUG_INFO
    .vma_print = memory_vma_print,
#endif
#ifdef USE_MEMORY_CLEANUP
    .set = memory_set
#endif
};

const memory_methods* CALL(sys_memory) {
    return &PRIVATE_API(memory_methods_definitions);
}

/* public */
memory_mmap_methods PRIVATE_API(memory_mmap_methods_definitions) = {
    .alloc = memory_mmap_alloc,
    .free = memory_mmap_free,
};

memory_mmap_methods* CALL(sys_memory_mmap) {
    return &PRIVATE_API(memory_mmap_methods_definitions);
}
