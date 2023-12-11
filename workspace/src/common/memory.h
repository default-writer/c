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

#ifndef _COMMON_ALLOC_H_
#define _COMMON_ALLOC_H_

#include "std/common.h"

#ifdef USE_MEMORY_DEBUG_INFO
void global_statistics(void);
#endif

typedef void* (*alloc_func)(u64 size);
typedef void (*free_func)(void* ptr, u64 size);

struct memory {
    void* (*alloc)(u64 size);
    void (*free)(void* ptr, u64 size);
    void* (*realloc)(void* old_ptr, u64 size, u64 new_size);
    void (*set)(void* dest, u8 c, size_t count);
    alloc_func (*set_alloc)(alloc_func alloc);
    free_func (*set_free)(free_func free);
};

/* definition */
extern const struct memory memory_definition;

/* definition */
static const struct memory* memory = &memory_definition;

#endif /* _COMMON_ALLOC_H_ */
