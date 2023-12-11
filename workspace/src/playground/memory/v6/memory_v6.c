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

#include "memory/api/v3/ref_v3.h"
#include "playground/memory/api/memory.h"

#define MAX_MEMORY 0xfffffc /* 16777215 (0xffffff) octa-bytes or 134217720 bytes (128MB) */

/* offset for memory_ref structure */
extern struct memory_ref_methods memory_ref_definition_v3;

static struct memory_ref_methods* ref = &memory_ref_definition_v3;

/* api */

static void memory_init(u64 size);
static void memory_destroy(void);
static void* memory_alloc(u64 size);
static void memory_free(void* data);

/* declaration */

/* implementation */

static void memory_init(u64 size) {
    ref->init(size);
}

static void memory_destroy(void) {
    ref->destroy();
}

static void* memory_alloc(u64 size) {
    return ref->alloc(size);
}

/* releases global memory */
static void memory_free(void* data) {
    ref->free(data);
}

/* public */

const union memory_allocator_api memory_allocator_v6 = {
    .v3.init = memory_init,
    .v3.destroy = memory_destroy,
    .v3.alloc = memory_alloc,
    .v3.free = memory_free
};
