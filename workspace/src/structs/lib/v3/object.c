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

#include "../../common/v3/object.h"
#include "../../common/v3/type.h"

static void* object_create(const struct class* class);
static void object_destroy(void* ptr);

static void* object_create(const struct class* class_ptr) {
    struct typeinfo* ti = class_ptr->type();
    struct methodinfo* mi = class_ptr->method();
    void* data = memory->alloc(ti->size + sizeof(struct class*));
    struct class** b_class_ptr = (struct class**)data;
    memcpy(b_class_ptr, &class_ptr, sizeof(struct class*));
    void* ptr = (void*)(++b_class_ptr);
    mi->create(ptr);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("creating object of class %s at %016llx (%ld bytes)\n", ti->name, (u64)ptr, ti->size);
#endif
    return ptr;
}

static void object_destroy(void* ptr) {
    struct class** b_class_ptr = (struct class**)ptr;
    void** data = (void**)(--b_class_ptr);
    struct class* class_ptr = (struct class*)*data;
    struct typeinfo* ti = class_ptr->type();
    struct methodinfo* mi = class_ptr->method();
    mi->destroy(ptr);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("deleting object of class %s at %016llx (%ld bytes)\n", ti->name, (u64)ptr, ti->size);
#endif
    memory->free(data, ti->size + sizeof(struct typeinfo*));
}

const struct object_methods object_methods_definition = {
    .create = object_create,
    .destroy = object_destroy
};
