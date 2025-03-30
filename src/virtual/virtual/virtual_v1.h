/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 28, 2025 at 1:00:54 PM GMT+3
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

#ifndef _VIRTUAL_V1_H_
#define _VIRTUAL_V1_H_

#define USING_STD_API
#include "std/api.h"

#include "virtual/export.h"

typedef struct PRIVATE_API(virtual_methods) {
    u64 (*alloc)(const_vm_ptr cvm, u64 size, u64 type_id);
    const_pointer_ptr (*read)(const_vm_ptr cvm, u64 address, u64 type_id);
    u64 (*type)(const_vm_ptr cvm, u64 address);
    u64 (*free)(const_vm_ptr cvm, u64 address);
} virtual_methods;

typedef struct PRIVATE_API(virtual_system_methods) {
    void (*init)(const_vm_ptr cvm, u64 size);
    void (*destroy)(const_vm_ptr cvm);
} virtual_system_methods;

/* definition */
CVM_EXPORT extern const virtual_methods* CALL(virtual);
CVM_EXPORT extern const virtual_system_methods* CALL(system);

#endif /* _VIRTUAL_V1_H_ */
