/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 12, 1961 at 09:07:34 PM GMT+3
 * Modified:
 *   May 1, 2025 at 8:59:28 PM GMT+3
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

#define USING_COMPILE

#ifndef STD_COMPILE_H
#define STD_COMPILE_H

#ifdef CONFIG_TTY
#if CONFIG_TTY == 1
#define USE_TTY
#endif
#endif

#ifdef CONFIG_MEMORY_DEBUG_INFO
#if CONFIG_MEMORY_DEBUG_INFO == 1
#define USE_MEMORY_DEBUG_INFO
#endif
#endif

#ifdef CONFIG_MEMORY_CLEANUP
#if CONFIG_MEMORY_CLEANUP == 1
#define USE_MEMORY_CLEANUP
#endif
#endif

#ifdef CONFIG_MEMORY_ALLOC
#if CONFIG_MEMORY_ALLOC == 1
#define USE_MEMORY_ALLOC
#endif
#endif

#ifdef CONFIG_GC
#if CONFIG_GC == 1
#define USE_GC
#endif
#endif

#ifdef CONFIG_MOCKS
#if CONFIG_MOCKS == 1
#define USE_MOCKS
#endif
#endif

#ifdef CONFIG_MEMCPY
#if CONFIG_MEMCPY == 1
#define USE_MEMCPY
#endif
#endif

#endif /* STD_COMPILE_H */
