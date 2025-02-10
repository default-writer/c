/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   February 9, 2025 at 12:22:59 PM GMT+3
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

#ifndef _VIRTUAL_H_
#define _VIRTUAL_H_

#include "std/api.h"

struct vm;
struct pointer;

typedef struct PRIVATE_API(virtual_methods) {
    void (*init)(struct vm**, u64 size);
    void (*destroy)(struct vm**);
    u64 (*alloc)(pointer_ptr ptr);
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
extern const virtual_methods PRIVATE_API(virtual_methods_definitions);

/* definition */
#ifdef INLINE
const virtual_methods* virtual = &PRIVATE_API(virtual_methods_definitions);
#else
/* definition */
static const virtual_methods* virtual = &PRIVATE_API(virtual_methods_definitions);
#endif

#endif /* _VIRTUAL_H_ */
