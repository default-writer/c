/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 12, 2025 at 9:10:33 AM GMT+3
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

typedef struct PRIVATE_API(virtual_pointer_methods) {
    const_vm_ptr (*init)(u64 size);
    void (*destroy)(void);
    void (*gc)(void);
    const_pointer_ptr (*alloc)(u64 size, u64 type_id);
    const_pointer_ptr (*copy)(const void* src, u64 size, u64 type_id);
    const_pointer_ptr (*copy_guard)(const void* src, u64 size, u64 offset, u64 type_id);
    u64 (*alloc_guard)(const void* src, u64 size, u64 offset, u64 type_id);
    u64 (*memcpy)(const_pointer_ptr const_ptr, const void* src, u64 size);
    u64 (*realloc)(const_pointer_ptr ptr, u64 size);
    void (*register_known_type)(u64 type_id, type_methods_definitions_ptr data_type);
    void (*register_user_type)(type_methods_definitions_type* data_type);
    u64 (*free)(u64 ptr);
    u64 (*release)(const_pointer_ptr const_ptr);
    u64 (*size)(const_pointer_ptr const_ptr);
    void* (*data)(const_pointer_ptr const_ptr);
    void* (*data_guard)(const_pointer_ptr const_ptr, u64 offset);
    u64 (*guard)(const_pointer_ptr const_ptr, u64 offset);
    u64 (*get_type)(const_pointer_ptr const_ptr);
    u64 (*set)(const_pointer_ptr const_ptr, const_virtual_pointer_ptr vptr, u64 address);
    u64 (*get_address)(const_pointer_ptr const_ptr);
#ifdef USE_MEMORY_DEBUG_INFO
    void (*dump)(pointer_ptr ptr);
    void (*dump_ref)(pointer_ptr* ptr);
#endif
} virtual_pointer_methods;

/* definition */
CVM_EXPORT extern const virtual_pointer_methods* CALL(pointer);

#endif /* _POINTER_V1_H_ */
