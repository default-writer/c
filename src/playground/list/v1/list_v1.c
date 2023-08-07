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
#include "playground/list/v1/list_v1.h"
#include "common/alloc.h"

#define MAX_MEMORY 0xffff /* 64K bytes */

/* definition */
extern const struct memory memory_definition;

/* definition */
static const struct memory* memory = &memory_definition;

/* implementation */

/* private */

/* global allocated memory */
static void** ptr = 0;
static void* base = 0;

static void list_init(void) {
    base = ptr = memory->alloc(MAX_MEMORY);
}

static void list_destroy(void) {
    memory->free(base, MAX_MEMORY);
    ptr = 0;
    base = 0;
}

static void list_push(void* data) {
    *ptr++ = data;
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   >: %016llx > %016llx\n", (u64)ptr - 1, (u64) * (ptr - 1));
#endif
}

static void* list_pop(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   <: %016llx > %016llx\n", (u64)ptr - 1, (u64) * (ptr - 1));
#endif
    return *--ptr;
}

static void* list_peek(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   *: %016llx > %016llx\n", (u64)ptr - 1, (u64) * (ptr - 1));
#endif
    return *(ptr - 1);
}

/* public */

const struct list list_v1 = {
    .init = list_init,
    .destroy = list_destroy,
    .push = list_push,
    .pop = list_pop,
    .peek = list_peek
};
