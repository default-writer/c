/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   February 25, 2025 at 2:49:12 PM GMT+3
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

#ifndef _POINTER_V1_H_
#define _POINTER_V1_H_

#define USING_API

#include "std/api.h"

#include "vm/export.h"

/*! @file pointer_v1.h
 *  @brief C API / pointer
 */

typedef struct PRIVATE_API(pointer_methods) {
    void (*init)(u64 size);
    void (*destroy)(void);
    void (*gc)(void);
    pointer_ptr (*alloc)(u64 size, u64 id);
    void (*realloc)(pointer_ptr ptr, u64 size);
    void (*register_known_type)(u64 id, const struct type_methods_definitions* data_type);
    u64 (*register_user_type)(const struct type_methods_definitions* data_type);
    void (*free)(u64 ptr);
    u64 (*address)(const_pointer_ptr ptr);
    virtual_pointer_ptr (*ref)(const_pointer_ptr ptr);
    void (*release)(pointer_ptr ptr);
    u64 (*size)(const_pointer_ptr ptr);
    void* (*read)(const_pointer_ptr ptr);
    u64 (*read_type)(const_pointer_ptr ptr, u64 id);
    void (*write)(pointer_ptr ptr, virtual_pointer_ptr m, u64 address);
#ifdef USE_MEMORY_DEBUG_INFO
    void (*dump)(pointer_ptr ptr);
    void (*dump_ref)(void** ptr);
#endif
} pointer_methods;

/* definition */
CVM_EXPORT extern const pointer_methods PRIVATE_API(pointer_methods_definitions);
CVM_EXPORT extern const pointer_methods* CALL(pointer);

#endif /* _POINTER_V1_H_ */
