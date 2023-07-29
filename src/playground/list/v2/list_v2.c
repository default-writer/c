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
#include "playground/list/v2/list_v2.h"
#include "common/alloc.h"

/* macros */
#define DEFAULT_SIZE 101
#define PTR_SIZE sizeof(void*) /* size of a pointer */
#define ALLOC_SIZE(size) (size * PTR_SIZE)

/* private */
struct list_data {
    void** ptr;
    void** base;
    void** max;
    u64 size;
};

static struct list_data* list_alloc(u64 size);
static void list_free(struct list_data* pointer);
static void* list_push(struct list_data* pointer, void* data);
static void* list_pop(struct list_data* pointer);
static void* list_peek(struct list_data* pointer);

static struct list_data* list_alloc(u64 size) {
    struct list_data* ptr = global_alloc(sizeof(struct list_data));
    ptr->base = global_alloc(ALLOC_SIZE(size));
    ptr->ptr = ptr->base;
    ptr->max = ptr->base + size;
    ptr->size = size;
    return ptr;
}

static void list_free(struct list_data* pointer) {
    global_free(pointer->base, ALLOC_SIZE(pointer->size));
    pointer->base = 0;
    pointer->ptr = 0;
    pointer->max = 0;
    global_free(pointer, sizeof(struct list_data));
    pointer = 0;
}

static void* list_push(struct list_data* pointer, void* data) {
    if (pointer->ptr != pointer->max) {
        *pointer->ptr++ = data;
    }
    return pointer->ptr != pointer->max ? pointer->ptr : 0;
}

static void* list_pop(struct list_data* pointer) {
    void* tmp = 0;
    if (pointer->ptr != pointer->base) {
        tmp = *--pointer->ptr;
        *pointer->ptr = 0;
    }
    return tmp;
}

static void* list_peek(struct list_data* pointer) {
    return pointer->ptr != pointer->base ? *(pointer->ptr - 1) : 0;
}

/* public */

const struct list list_v2 = {
    .alloc = list_alloc,
    .free = list_free,
    .push = list_push,
    .pop = list_pop,
    .peek = list_peek
};
