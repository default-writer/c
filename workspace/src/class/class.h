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

#ifndef _CLASS_H_
#define _CLASS_H_

#include "std/common.h"

struct object;
struct class;
struct typeinfo;
struct object_typeinfo;

typedef struct object struct_object;
typedef struct typeinfo struct_typeinfo;
typedef struct object_typeinfo struct_object_typeinfo;
typedef struct class struct_class;

typedef struct_object* object;
typedef struct_typeinfo* typeinfo;
typedef struct_object_typeinfo* object_typeinfo;
typedef struct_class* class;

struct object {
    void* ptr;
};

struct typeinfo {
    const size_t size;
#ifdef USE_MEMORY_DEBUG_INFO
    const char* name;
#endif
};

struct class {
    object_typeinfo (*create)(typeinfo ti);
    void (*destroy)(object_typeinfo ptr);
};

struct object_typeinfo {
    const object object;
    const typeinfo typeinfo;
};

#endif /* _CLASS_H_ */
