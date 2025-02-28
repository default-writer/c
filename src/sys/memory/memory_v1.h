/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   February 28, 2025 at 9:26:54 AM GMT+3
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

#ifndef _SYS_MEMORY_H_
#define _SYS_MEMORY_H_

#define USING_MEMORY

#include "std/api.h"

#include "sys/export.h"

#ifdef USE_MEMORY_DEBUG_INFO
extern void global_statistics(void);
#endif

typedef struct PRIVATE_API(memory_methods) {
    void* (*alloc)(u64 size);
    void (*free)(void* ptr, u64 size);
    void* (*realloc)(void* old_ptr, u64 size, u64 new_size);
#ifdef USE_MEMORY_CLEANUP
    void (*set)(void* dest, u8 c, u64 count);
#endif
} memory_methods;

typedef struct memory_api {
    void* (*alloc)(size_t __nmemb, size_t __size);
    void (*free)(void* __ptr);
} memory_api;

/* api */
CSYS_EXPORT extern const memory_api* memory;

/* definition */
CSYS_EXPORT extern const memory_methods PRIVATE_API(memory_methods_definitions);
CSYS_EXPORT extern const memory_methods* CALL(sys_memory);

#endif /* _SYS_MEMORY_H_ */
