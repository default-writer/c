/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 28, 2025 at 4:57:40 PM GMT+3
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

#define USING_API

#ifndef _SYSTEM_OS_V1_H_
#define _SYSTEM_OS_V1_H_

#define USING_STD_API
#include "std/api.h"

#include "system/export.h"

#ifdef USE_MEMORY_DEBUG_INFO
CSYS_EXPORT extern void init_statistics(void);
CSYS_EXPORT extern void result_statistics(void);
#endif

typedef struct PRIVATE_API(system_os_methods) {
    void* (*alloc)(size_t __nmemb, size_t __size);
    void* (*realloc)(void* __ptr, size_t __size);
    void (*free)(void* __ptr);
    int (*fclose)(FILE* __stream);
    FILE* (*fopen)(const char* __filename, const char* __modes);
    size_t (*fread)(void* __ptr, size_t __size, size_t __n, FILE* __stream);
    int (*fseek)(FILE* __stream, long int __off, int __whence);
    long (*ftell)(FILE* __stream);
    char* (*getcwd)(char* __buf, size_t __size);
    char* (*getenv)(const char* __name);
    void* (*memcpy)(void* __dest, const_void_ptr __src, size_t __n);
    void* (*memset)(void* __s, int __c, size_t __n);
    int (*puts)(const char* __s);
    int (*strcmp)(const char* __s1, const char* __s2);
    char* (*strncat)(char* __dest, const char* __src, size_t __n);
    char* (*strncpy)(char* __dest, const char* __src, size_t __n);
    size_t (*strlen)(const char* __s);
} system_os_methods;

/* public */
CSYS_EXPORT extern const system_os_methods* CALL(os);

#endif /* _SYSTEM_OS_V1_H_ */
