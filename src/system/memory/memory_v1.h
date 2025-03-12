/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 12, 2025 at 5:31:06 PM GMT+3
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

#ifndef _system_memory_H_
#define _system_memory_H_

#define USING_MEMORY
#define USING_API

#include "std/api.h"

#include "sys/export.h"

#ifdef USE_MEMORY_DEBUG_INFO
CSYS_EXPORT extern void init_statistics(void);
CSYS_EXPORT extern void result_statistics(void);
#endif

typedef struct PRIVATE_API(memory_methods) {
    void* (*alloc)(u64 size);
    void (*free)(void* ptr, u64 size);
    void* (*realloc)(void* old_ptr, u64 size, u64 new_size);
#ifdef USE_MEMORY_CLEANUP
    void (*set)(void* dest, u8 c, u64 count);
#endif
} memory_methods;

/* definition */
CSYS_EXPORT extern const memory_methods* CALL(system_memory);

#endif /* _system_memory_H_ */
