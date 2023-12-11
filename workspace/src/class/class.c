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

#include "class/class.h"
#include "common/memory.h"

static object_typeinfo class_create(typeinfo ti);
static void class_destroy(object_typeinfo ptr);

typedef struct writeable_object_typeinfo {
    object object;
    typeinfo typeinfo;
} struct_writeable_object_typeinfo, *writeable_object_typeinfo;

static object_typeinfo class_create(typeinfo ti) {
    object_typeinfo ptr;
#ifndef USE_MEMCPY
    writeable_object_typeinfo type = memory->alloc(sizeof(struct_object_typeinfo));
    type->object = memory->alloc(ti->size);
    type->typeinfo = ti;
    ptr = (object_typeinfo)type;
#else
    ptr = memory->alloc(sizeof(object_typeinfo));
    struct_object_typeinfo object_ti = {
        .object = memory->alloc(t->size),
        .typeinfo = ti
    };
    memcpy(ptr, &object_ti, sizeof(object_typeinfo));
#endif
#ifdef USE_MEMORY_DEBUG_INFO
    printf("creating type %s of size %ld (+ %ld)\n", ti->name, ti->size, sizeof(struct_object_typeinfo));
#endif
    return ptr;
}

static void class_destroy(object_typeinfo ptr) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("deleting type %s of size %ld (+ %ld)\n", ptr->typeinfo->name, ptr->typeinfo->size, sizeof(struct_object_typeinfo));
#endif
    memory->free(ptr->object, ptr->typeinfo->size);
    memory->free(ptr, sizeof(struct_object_typeinfo));
}

const struct_class class_definition = {
    .create = class_create,
    .destroy = class_destroy
};
