/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   February 19, 2025 at 10:17:41 PM GMT+3
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

#ifndef _OBJECT_V1_H_
#define _OBJECT_V1_H_

#define USING_API

#include "std/api.h"

#include "vm/export.h"

/*! @file object_v1.h
 *  @brief C API / object
 */

typedef struct API(object_methods) {
    u64 (*alloc)(u64 size);
    void (*free)(u64 ptr);
    void* (*unsafe)(u64 ptr);
    u64 (*load)(const void* data, u64 size);
    u64 (*size)(u64 ptr);
} virtual_object_methods;

/* definition */
CVM_EXPORT extern const virtual_object_methods PRIVATE_API(virtual_object_methods_definitions);
CVM_EXPORT extern const virtual_object_methods* _virtual_object();

#endif /* _OBJECT_V1_H_ */
