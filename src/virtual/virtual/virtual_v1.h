/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 6, 2025 at 12:33:11 AM GMT+3
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

#define USING_API

#include "std/api.h"

#include "vm/export.h"

typedef struct vm* vm_ptr;

typedef struct PRIVATE_API(virtual_methods) {
    void (*init)(vm_ptr*, u64 size);
    void (*destroy)(vm_ptr*);
    u64 (*alloc)(const_pointer_ptr ptr);
    void (*free)(const_pointer_ptr ptr);
    pointer_ptr (*read)(u64 address);
    pointer_ptr (*read_type)(u64 address, u64 id);
    void (*enumerator_init)(void);
    void (*enumerator_destroy)(void);
    u64 (*enumerator_next)(void);
#ifdef USE_MEMORY_DEBUG_INFO
    void (*dump)(void);
    void (*dump_ref)(void);
#endif
} virtual_methods;

/* definition */
CVM_EXPORT extern const virtual_methods* CALL(virtual);

#endif /* _VIRTUAL_V1_H_ */
