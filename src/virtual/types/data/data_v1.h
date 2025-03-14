/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 14, 2025 at 11:34:06 AM GMT+3
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

#ifndef _VIRTUAL_DATA_V1_H_
#define _VIRTUAL_DATA_V1_H_

#define USING_STD_API
#include "std/api.h"

#include "virtual/export.h"

/*! @file data_v1.h
 *  @brief C API / data
 */

typedef struct PRIVATE_API(virtual_data_methods) {
    u64 (*alloc)(const_vm_ptr vm, u64 size);
    u64 (*free)(const_vm_ptr vm, u64 ptr);
    void* (*unsafe)(const_vm_ptr vm, u64 ptr);
    u64 (*size)(const_vm_ptr vm, u64 ptr);
} virtual_data_methods;

/* definition */
CVM_EXPORT extern const virtual_data_methods* CALL(data);

#endif /* _VIRTUAL_DATA_V1_H_ */
