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
#include "memory/api/v3/ref_v3.h"
#include "common/alloc.h"
#include "playground/memory/list/v3/memory_list_v3.h"

/* macros */
#define PTR_SIZE sizeof(void*) /* size of a pointer */
#define MEMORY_REF_SIZE sizeof(struct memory_ref)
#define ALLOC_SIZE(size) (MEMORY_REF_SIZE + size * PTR_SIZE)

/* declaration */

static struct memory_ref* memory_ptr;

struct memory_ref_ptr {
    struct memory_ref* ptr;
    u64 size;
};

static void* memory_ref_alloc(u64 size);
static void memory_ref_init(u64 size);
static void memory_ref_destroy(void);
static void memory_ref_free(void* data);

static struct memory_ref* memory_alloc_internal(u64 size);
static void memory_ref_free_internal(void* data);
static void* memory_ref_ptr_internal(struct memory_ref* ptr);
static void* memory_ref_peek_internal(void);
static void* memory_ref_pop_internal(void);

/* definition */
extern const struct memory memory_definition;

/* definition */
static const struct memory* memory = &memory_definition;

/* implementation */

static void* memory_ref_ptr_internal(struct memory_ref* ptr) {
    void* data = 0;
    if (ptr != 0) {
        struct memory_ref* tmp = ptr;
        data = (void*)++tmp;
    }
    return data;
}

static void* memory_ref_alloc(u64 size) {
    struct memory_ref* ptr = memory_ref_peek_internal();
    void* data = 0;
    if (ptr != 0 && size > 0 && ptr->size < size) {
        ptr = memory_alloc_internal(size);
    }
    ptr->size -= size;
#ifdef USE_MEMORY_DEBUG_INFO
    printf("  c.: %016llx . %016llx > %16lld ! %16lld\n", (u64)ptr, (u64)ptr->cache, ptr->offset, ptr->size); /* NOLINT */
#endif
    data = memory_ref_ptr_internal(ptr);
    data = (u64*)data + ptr->size;
    return data;
}

static void memory_ref_free_internal(void* data) {
    if (data != 0) {
        struct memory_ref* ptr = data;
        memory->free(ptr, ALLOC_SIZE(ptr->offset));
    }
}

static void memory_ref_init(u64 size) {
    memory_list_init();
    memory_ptr = memory->alloc(MEMORY_REF_SIZE);
    if (size > 0) {
        memory_ptr->next = memory->alloc(ALLOC_SIZE(size));
        memory_ptr->next->size = size;
        memory_ptr->next->offset = size;
        memory_list_push(memory_ptr->next);
    }
}

static void memory_ref_destroy(void) {
    struct memory_ref* ptr = 0;
    while ((ptr = memory_ref_pop_internal()) != 0) {
        memory_ref_free_internal(ptr);
    }
    memory->free(memory_ptr, MEMORY_REF_SIZE);
#ifdef USE_MEMORY_CLEANUP
    memory_ptr = 0;
#endif
    memory_list_destroy();
}

static struct memory_ref* memory_alloc_internal(u64 size) {
    struct memory_ref* ptr = memory_list_peek();
#ifdef USE_MEMORY_DEBUG_INFO
    void* data = ptr;
#endif
    struct memory_ref* next_ptr = memory->alloc(ALLOC_SIZE(size));
    next_ptr->size = size;
    next_ptr->offset = size;
    next_ptr->prev = memory_ref_ptr_internal(ptr);
    if (ptr != 0) {
        ptr->next = memory_ref_ptr_internal(next_ptr);
#ifdef USE_MEMORY_DEBUG_INFO
        printf("  p.: %016llx . %016llx . %016llx\n", (u64)data, (u64)ptr->prev, (u64)ptr->next);
#endif
    }
    ptr = next_ptr;
    memory_list_push(ptr);
    return ptr;
}

static void memory_ref_free(void* data) {
    if (data != 0) {
        struct memory_ref* ptr = memory_list_peek();
        const void* current = memory_ref_ptr_internal(ptr);
        if (data == current) {
            ptr->size = ptr->offset;
        }
    }
}

static void* memory_ref_pop_internal(void) {
    struct memory_ref* ptr = 0;
    void* data = memory_list_pop();
    if (data != 0) {
        ptr = data;
    }
    return ptr;
}

static void* memory_ref_peek_internal(void) {
    void* data = memory_list_peek();
    void* ptr = 0;
    if (data != 0) {
        ptr = data;
    }
    return ptr;
}

/* public */

const struct memory_ref_methods memory_ref_definition_v3 = {
    .init = memory_ref_init,
    .destroy = memory_ref_destroy,
    .alloc = memory_ref_alloc,
    .free = memory_ref_free
};
