/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   11 December 2023 at 9:16:31 GMT+3
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

#include "playground/list/v1/list_v1.h"
#include "common/memory.h"

#define MAX_MEMORY 0xffff /* 64K bytes */

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

const struct list list_methods_v1 = {
    .init = list_init,
    .destroy = list_destroy,
    .push = list_push,
    .pop = list_pop,
    .peek = list_peek
};
