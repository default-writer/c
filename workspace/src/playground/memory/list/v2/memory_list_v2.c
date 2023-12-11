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

#include "memory_list_v2.h"
#include "common/memory.h"

/* macros */
#define PTR_SIZE sizeof(void*) /* size of a pointer */

/* private */

static struct memory_ref** current;

void memory_list_init(void) {
    current = memory->alloc(PTR_SIZE);
}

struct memory_ref* memory_list_peek(void) {
    return *current;
}

void memory_list_push(struct memory_ref* ptr) {
    if (ptr != 0) {
        ptr->cache = *current;
    }
    *current = ptr;
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
