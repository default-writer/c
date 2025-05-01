/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 12, 1961 at 09:07:34 PM GMT+3
 * Modified:
 *   May 1, 2025 at 12:02:39 PM GMT+3
 *
 */
/*
    Copyright (C) 2022-2047 Artur Mustafin (artur.mustafin@gmail.com)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its
       contributors may be used to endorse or promote products derived from
       this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#define USING_SYSTEM_OS

#ifndef SYSTEM_OS_V1_H
#define SYSTEM_OS_V1_H

#define USING_STD_API
#include "std/api.h"

#include "system/platform.h"

#ifdef USE_MEMORY_DEBUG_INFO
CSYS_EXPORT extern void init_statistics(void);
CSYS_EXPORT extern void result_statistics(void);
#endif

#include <stdio.h>

typedef struct PRIVATE_API(system_os_methods) {
    void* (*calloc)(size_t __nmemb, size_t __size);
    void* (*realloc)(void* __ptr, size_t __size);
    void (*free)(void* __ptr);
    int (*fclose)(FILE* __stream);
    FILE* (*fopen)(const char* __filename, const char* __modes);
    size_t (*fread)(void* __ptr, size_t __size, size_t __n, FILE* __stream);
    int (*fseek)(FILE* __stream, long int __off, int __whence);
    long (*ftell)(FILE* __stream);
#if defined(_WIN32)
    char* (*getcwd)(char* __buf, int __size);
#else
    char* (*getcwd)(char* __buf, size_t __size);
#endif
    char* (*getenv)(const char* __name);
    void* (*memcpy)(void* __dest, const void* __src, size_t __n);
    void* (*memmove)(void* __dest, const void* __src, size_t __n);
    void* (*memset)(void* __s, int __c, size_t __n);
    int (*puts)(const char* __s);
    int (*strcmp)(const char* __s1, const char* __s2);
    char* (*strncat)(char* __dest, const char* __src, size_t __n);
    char* (*strncpy)(char* __dest, const char* __src, size_t __n);
    size_t (*strlen)(const char* __s);
} system_os_methods;

/* public */
CSYS_EXPORT extern const system_os_methods* CALL(os);

#endif /* SYSTEM_OS_V1_H */
