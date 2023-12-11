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

#include "memory/api/v1/ref_v1.h"
#include "playground/memory/api/memory.h"
#include "playground/memory/list/v1/memory_list_v1.h"

#define MAX_MEMORY 0xffff /* 64K bytes */

/* offset for memory_ref structure */
extern struct memory_ref_methods memory_ref_definition_v1;

static struct memory_ref_methods* ref = &memory_ref_definition_v1;

/* api */

static void memory_init(void);
static void memory_destroy(void);
static void* memory_alloc(u64 size);
static void memory_free(void* data);

/* declaration */

/* implementation */

static void memory_init(void) {
    ref->init();
    memory_list_init();
}

static void memory_destroy(void) {
    void* data = 0;
    while ((data = memory_list_pop()) != 0) {
        ref->free(data);
    }
    memory_list_destroy();
    ref->destroy();
}

static void* memory_alloc(u64 size) {
    void* tmp = memory_list_peek();
    void* data = 0;
    u64 ptr_size = ref->size(tmp);
    if (ptr_size != 0 && ptr_size >= size) {
        data = memory_list_pop();
    } else {
        data = ref->alloc(size);
    }
    return data;
}

/* releases global memory */
static void memory_free(void* data) {
    memory_list_push(data);
}

/* public */

const union memory_allocator_api memory_allocator_v4 = {
    .v2.init = memory_init,
    .v2.destroy = memory_destroy,
    .v2.alloc = memory_alloc,
    .v2.free = memory_free
};
