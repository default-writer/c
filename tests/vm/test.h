/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 12, 1961 at 09:07:34 PM GMT+3
 * Modified:
 *   May 11, 2025 at 2:08:42 PM GMT+3
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

#define USING_TESTS

#ifndef TESTS_TEST_H
#define TESTS_TEST_H

#define USING_STD_API
#include "std/api.h"

#if !defined(RX_MAJOR_VERSION)
#if defined(__unix__) || defined(__APPLE__)
#define RXP_PLATFORM_UNIX
#if defined(__APPLE__)
#define RXP_PLATFORM_DARWIN
#elif defined(__linux__)
#define RXP_PLATFORM_LINUX
#endif
#endif

#define RXP_DEBUG_TESTS

#include "system/platform/default/export.h"

#include "rexo.h" /* NOLINT */

#endif

typedef struct PRIVATE_API(test_suite) {
    void (*run)(void);
} test_suite;

#define TEST_RUN(expression) expression->run();

#define CLEAN(ptr)  \
    if (ptr != 0) { \
        ptr = 0;    \
    }

CSYS_EXPORT void init_statistics(void);
CSYS_EXPORT void result_statistics(void);

#endif /* TESTS_TEST_H */
