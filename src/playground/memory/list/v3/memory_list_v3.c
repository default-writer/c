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
#include "memory_list_v3.h"
#include "common/alloc.h"

/* macros */
#define PTR_SIZE sizeof(void*) /* size of a pointer */

/* definition */
extern const struct memory memory_definition;
static const struct memory* memory = &memory_definition;

/* private */

static struct memory_ref** current;

void memory_list_init(void) {
    current = memory->alloc(PTR_SIZE);
}

struct memory_ref* memory_list_peek(void) {
    return *current;
}

void memory_list_push(struct memory_ref* ptr) {
    struct memory_ref* current_ptr = *current;
    while (current_ptr != 0 && current_ptr->offset < ptr->offset) {
        current_ptr = current_ptr->cache; /* get next element in a list */
    }
    if (ptr != 0) {
        if (current_ptr == 0) {
            ptr->cache = *current;
            *current = ptr;
        } else {
            struct memory_ref* next_ptr = current_ptr->cache; /* get next to current element in a list */
            ptr->cache = next_ptr; /* inject ptr into a list */
            current_ptr->cache = ptr;
        }
    }
#ifdef USE_MEMORY_DEBUG_INFO
    struct memory_ref* tmp_ptr = *current;
    while (tmp_ptr != 0) {
        printf("  c.: %016llx . %016llx > %16lld ! %16lld\n", (u64)tmp_ptr, (u64)tmp_ptr->cache, tmp_ptr->offset, tmp_ptr->size); /* NOLINT */
        tmp_ptr = tmp_ptr->cache;
    }
#endif
}

struct memory_ref* memory_list_pop(void) {
    struct memory_ref* ptr = *current;
    if (ptr != 0) {
        *current = ptr->cache;
    }
    return ptr;
}

void memory_list_destroy(void) {
    memory->free(current, PTR_SIZE);
#ifdef USE_MEMORY_CLEANUP
    current = 0;
#endif
}
