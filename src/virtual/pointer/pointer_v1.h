/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 27, 2025 at 4:52:40 PM GMT+3
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

#ifndef _VIRTUAL_POINTER_V1_H_
#define _VIRTUAL_POINTER_V1_H_

#define USING_STD_API
#include "std/api.h"

#include "virtual/export.h"

/*! @file pointer_v1.h
 *  @brief C API / pointer
 */

typedef struct PRIVATE_API(virtual_pointer_methods) {
    u64 (*alloc)(const_void_ptr data, u64 size, u64 type_id);
    u64 (*copy)(const_void_ptr src, u64 size, u64 offset, u64 type_id);
    const_void_ptr (*read)(u64 address, u64 type_id);
    u64 (*free)(u64 address, u64 type_id);
} virtual_pointer_methods;

typedef struct PRIVATE_API(virtual_vm_methods) {
    const_vm_ptr (*init)(u64 size);
    void (*destroy)(void);
    void (*gc)(void);
    void (*register_known_type)(u64 type_id, type_methods_definitions_ptr data_type);
    void (*register_user_type)(type_methods_definitions_type* data_type_builder);
    u64 (*release)(u64 ptr);
} virtual_vm_methods;

/* definition */
CVM_EXPORT extern const virtual_pointer_methods* CALL(pointer);
CVM_EXPORT extern const virtual_vm_methods* CALL(vm);

#endif /* _VIRTUAL_POINTER_V1_H_ */
