/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 17, 2025 at 8:32:00 PM GMT+3
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
    void (*init)(const_vm_ptr vm, u64 size);
    void (*destroy)(const_vm_ptr vm);
    u64 (*alloc)(const_vm_ptr vm, const_pointer_ptr const_ptr);
    u64 (*memcpy)(const_vm_ptr vm, u64 size, const void* data, u64 type_id);
    u64 (*pointer)(const_vm_ptr vm, u64 size, u64 type_id);
    void (*free)(const_vm_ptr vm, u64 address);
    const_pointer_ptr (*read)(const_vm_ptr vm, u64 address);
    const_pointer_ptr (*read_type)(const_vm_ptr vm, u64 address, u64 type_id);
    void (*enumerator_init)(const_vm_ptr vm);
    void (*enumerator_destroy)(void);
    u64 (*enumerator_next)(const_vm_ptr vm);
#ifdef USE_MEMORY_DEBUG_INFO
    void (*dump)(const_vm_ptr vm);
    void (*dump_ref)(const_vm_ptr vm);
#endif
} virtual_methods;

/* definition */
CVM_EXPORT extern const virtual_methods* CALL(virtual);

#endif /* _VIRTUAL_V1_H_ */
