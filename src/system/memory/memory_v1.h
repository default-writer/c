/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 28, 2025 at 2:05:28 PM GMT+3
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

#define USING_SYSTEM_MEMORY

#ifndef _SYSTEM_MEMORY_H_
#define _SYSTEM_MEMORY_H_

#define USING_STD_API
#include "std/api.h"

#include "system/export.h"

#ifdef USE_MEMORY_DEBUG_INFO
CSYS_EXPORT extern void init_statistics(void);
CSYS_EXPORT extern void result_statistics(void);
#endif

typedef struct PRIVATE_API(system_memory_methods) {
    void* (*alloc)(u64 size);
    void* (*realloc)(const_void_ptr const_ptr, u64 size, u64 new_size);
    void (*free)(const_void_ptr const_ptr, u64 size);
#ifdef USE_MEMORY_CLEANUP
    void (*set)(void* dest, u8 c, u64 count);
#endif
} system_memory_methods;

/* definition */
CSYS_EXPORT extern const system_memory_methods* CALL(memory);

#endif /* _SYSTEM_MEMORY_H_ */
