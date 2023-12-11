/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   11 December 2023 at 9:15:15 GMT+3
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

#include "std/common.h"

struct vm;
struct pointer;

struct vm_default_options {
    const u64 size;
};

struct virtual_methods {
    void (*init)(struct vm**, u64 size);
    void (*destroy)(struct vm**);
    u64 (*alloc)(struct pointer* ptr);
    void (*free)(struct pointer* ptr);
    struct pointer* (*read)(u64 address);
    struct pointer* (*read_type)(u64 address, u64 id);
    void (*enumerator_init)(void);
    void (*enumerator_destroy)(void);
    u64 (*enumerator_next)(void);
#if defined(VM_MEMORY_DEBUG_INFO)
    void (*dump)(void);
    void (*dump_ref)(void);
#endif
};

/* definition */
extern const struct virtual_methods virtual_methods_definition;
extern const struct vm_default_options vm_default_options_definition;

/* definition */

#if defined(INLINE)
const struct virtual_methods* virtual = &virtual_methods_definition;
const struct vm_default_options* vm_options = &vm_default_options_definition;
#else
/* definition */
static const struct virtual_methods* virtual = &virtual_methods_definition;
static const struct vm_default_options* vm_options = &vm_default_options_definition;
#endif

#endif /* _VIRTUAL_H_ */
