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

#include "playground/list/v2/list_v2.h"
#include "common/memory.h"

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
    struct list_data* ptr = memory->alloc(sizeof(struct list_data));
    ptr->base = memory->alloc(ALLOC_SIZE(size));
    ptr->ptr = ptr->base;
    ptr->max = ptr->base + size;
    ptr->size = size;
    return ptr;
}

static void list_free(struct list_data* pointer) {
    memory->free(pointer->base, ALLOC_SIZE(pointer->size));
    pointer->base = 0;
    pointer->ptr = 0;
    pointer->max = 0;
    memory->free(pointer, sizeof(struct list_data));
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
