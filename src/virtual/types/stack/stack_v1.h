/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 8, 2025 at 8:40:22 AM GMT+3
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

#ifndef _VIRTUAL_STACK_V1_H_
#define _VIRTUAL_STACK_V1_H_

#define USING_STD_API
#include "std/api.h"

#include "virtual/export.h"

/*! @file list_v1.h
 *  @brief C API / list
 *
 *  structure for organizing @ref stack functions
 *
 */

typedef struct PRIVATE_API(virtual_stack_methods) {
    u64 (*alloc)(const_vm_ptr cvm);
    u64 (*push)(const_vm_ptr cvm, u64 list_ptr, u64 ptr);
    u64 (*peek)(const_vm_ptr cvm, u64 list_ptr);
    u64 (*peekn)(const_vm_ptr cvm, u64 list_ptr, u64 nelements);
    u64 (*pop)(const_vm_ptr cvm, u64 list_ptr);
    u64 (*popn)(const_vm_ptr cvm, u64 list_ptr, u64 nelements);
    u64 (*size)(const_vm_ptr cvm, u64 ptr);
    u64 (*release)(const_vm_ptr cvm, u64 ptr);
    u64 (*free)(const_vm_ptr cvm, u64 ptr);
} virtual_stack_methods;

/* definition */
CVM_EXPORT extern const virtual_stack_methods* CALL(stack);

#endif /* _VIRTUAL_STACK_V1_H_ */
