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

struct vm_data;

struct virtual_methods {
    void (*init)(struct vm_data** current, u64 size);
    void (*destroy)(struct vm_data** current);
    struct pointer* (*free)(struct vm_data** current, u64 address);
    struct pointer* (*read)(struct vm_data** current, u64 address);
    u64 (*write)(struct vm_data** current, struct pointer* value);
#if defined(VM_MEMORY_DEBUG_INFO)
    void (*dump)(struct vm_data* vm_ptr);
    void (*dump_ref)(struct vm_data* vm_ptr);
#endif
};

/* definition */
extern const struct virtual_methods virtual_methods_definition;

#if defined(INLINE)
const struct virtual_methods* virtual = &virtual_methods_definition;
#else
/* definition */
static const struct virtual_methods* virtual = &virtual_methods_definition;
#endif

#endif /* _VIRTUAL_H_ */
