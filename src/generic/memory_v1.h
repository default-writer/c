/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   January 30, 2024 at 4:58:43 PM GMT+3
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

#ifndef _GENERIC_MEMORY_H_
#define _GENERIC_MEMORY_H_

#include "std/compile.h"
#include "std/data.h"

#ifdef USE_MEMORY_DEBUG_INFO
extern void global_statistics(void);
#endif

struct generic_memory_v1 {
    void* (*alloc)(u64 size);
    void (*free)(void* ptr, u64 size);
    void* (*realloc)(void* old_ptr, u64 size, u64 new_size);
    void (*set)(void* dest, u8 c, u64 count);
};

extern const struct generic_memory_v1 generic_memory_definition_v1;

/* definition */
#if defined(INLINE)
const struct generic_memory_v1* generic_memory_v1 = &generic_memory_definition_v1;
#else
/* definition */
static const struct generic_memory_v1* generic_memory_v1 = &generic_memory_definition_v1;
#endif

#endif /* _GENERIC_MEMORY_H_ */
