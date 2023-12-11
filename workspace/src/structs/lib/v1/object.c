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

#include "common/memory.h"
#include "std/common.h"

#include "../../common/v1/object.h"
#include "../../common/v1/type.h"

static void* object_create(struct typeinfo* ti);
static void object_destroy(struct typeinfo* ti);

static void* object_create(struct typeinfo* ti) {
    ti->ptr = memory->alloc(ti->size);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("creating type %s (%ld bytes)\n", ti->name, ti->size);
#endif
    return ti->ptr;
}

static void object_destroy(struct typeinfo* ti) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("deleting type %s (%ld bytes)\n", ti->name, ti->size);
#endif
    memory->free(ti->ptr, ti->size);
}

const struct object_methods object_methods_definition = {
    .create = object_create,
    .destroy = object_destroy
};
