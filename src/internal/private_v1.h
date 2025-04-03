/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 3, 2025 at 9:20:22 AM GMT+3
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

#ifndef _INTERNAL_PRIVATE_V1_H_
#define _INTERNAL_PRIVATE_V1_H_

#define USING_STD_API
#include "std/api.h"

typedef struct virtual_pointer {
    pointer_ptr* sp;
    pointer_ptr* bp;
    virtual_pointer_ptr next;
    u64 offset;
} virtual_pointer_type;

typedef struct vm {
#ifndef USE_GC
    stack_ptr* cache;
#endif
    u64 known_types_counter;
    known_types_ptr known_types;
    known_types_ptr default_types;
    virtual_pointer_ptr next;
} vm_type;

#endif /* _INTERNAL_PRIVATE_V1_H_ */
