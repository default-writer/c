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

#ifndef _POINTER_TYPES_OBJECT_H_
#define _POINTER_TYPES_OBJECT_H_

#include "std/common.h"

/*! @file object_v1.h
 *  @brief C API / object
 */

struct object_methods {
    u64 (*alloc)(u64 size);
    void (*free)(u64 ptr);
    void* (*unsafe)(u64 ptr);
    u64 (*load)(const void* data, u64 size);
    u64 (*size)(u64 ptr);
};

/* definition */
extern const struct object_methods object_methods_definition;

/* definition */
#if defined(INLINE)
const struct object_methods* object = &object_methods_definition;
#else
static const struct object_methods* object = &object_methods_definition;
#endif

#endif /* _POINTER_TYPES_OBJECT_H_ */
