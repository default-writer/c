/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 14, 2025 at 11:34:07 AM GMT+3
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
    u64 (*alloc)(const_vm_ptr vm);
    u64 (*free)(const_vm_ptr vm, u64 ptr);
    u64 (*peek)(const_vm_ptr vm, u64 list_ptr);
    /*! returns a list of picked items
        @param list_ptr a list to get items from
        @param nelements number of elements to peek from the list
        @return returns new list which contains n elements from the list in reverse order
    */
    u64 (*peekn)(const_vm_ptr vm, u64 list_ptr, u64 nelements);
    u64 (*pop)(const_vm_ptr vm, u64 list_ptr);
    u64 (*popn)(const_vm_ptr vm, u64 list_ptr, u64 nelements);
    u64 (*push)(const_vm_ptr vm, u64 list_ptr, u64 ptr);
    u64 (*size)(const_vm_ptr vm, u64 ptr);
    u64 (*release)(const_vm_ptr vm, u64 ptr);
} virtual_stack_methods;

/* definition */
CVM_EXPORT extern const virtual_stack_methods* CALL(stack);

#endif /* _VIRTUAL_STACK_V1_H_ */
