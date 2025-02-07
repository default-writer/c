/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   February 5, 2025 at 11:22:56 PM GMT+3
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

#include "std/api.h"

/*! @file pointer_v1.h
 *  @brief C API / pointer
 */

struct pointer;
struct vm_data;
struct vm_type;

typedef struct PRIVATE_API(pointer_methods) {
    void (*init)(u64 size);
    void (*destroy)(void);
    void (*gc)(void);
    struct pointer* (*alloc)(u64 size, u64 id);
    void (*realloc)(struct pointer* ptr, u64 size);
    u64 (*register_type)(u64 id, const struct vm_type* data_type);
    void (*free)(u64 ptr);
    u64 (*address)(const struct pointer* ptr);
    struct vm_data* (*vm)(const struct pointer* ptr);
    void (*release)(struct pointer* ptr);
    u64 (*size)(const struct pointer* ptr);
    void* (*read)(const struct pointer* ptr);
    u64 (*read_type)(const struct pointer* ptr, u64 id);
    void (*write)(struct pointer* ptr, struct vm_data* vm, u64 address);
#ifdef USE_MEMORY_DEBUG_INFO
    void (*dump)(struct pointer* ptr);
    void (*dump_ref)(void** ptr);
#endif
} pointer_methods;

/* definition */
extern const pointer_methods PRIVATE_API(pointer_methods_definition);

/* definition */
#ifdef INLINE
const spointer_methods* pointer = &PRIVATE_API(pointer_methods_definition);
#else
/* definition */
static const pointer_methods* pointer = &PRIVATE_API(pointer_methods_definition);
#endif

#endif /* _POINTER_V1_H_ */
