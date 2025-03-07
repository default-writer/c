/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 6, 2025 at 12:37:04 AM GMT+3
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

#ifndef _system_api_H_
#define _system_api_H_

#define USING_API
#define USING_MEMORY

#include "std/api.h"

#include "sys/export.h"

#include <stdlib.h>

#ifdef USE_MEMORY_DEBUG_INFO
extern void global_statistics(void);
#endif

typedef struct system_api {
    void* (*alloc)(size_t __nmemb, size_t __size);
    void* (*realloc)(void* __ptr, size_t __size);
    void (*free)(void* __ptr);
} system_api_type;

/* api */
CSYS_EXPORT extern const system_api_type* system_api;

#endif /* _system_api_H_ */
