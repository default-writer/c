/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 4, 2025 at 10:03:02 AM GMT+3
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

#ifndef _TESTS_TEST_H_
#define _TESTS_TEST_H_

#define USING_TESTS

#define RXP_DEBUG_TESTS

#include "std/api.h"

#if !defined(RX_MAJOR_VERSION)
#if defined(_WIN32)
#define RXP_PLATFORM_WINDOWS
#elif defined(__unix__) || defined(__APPLE__)
#define RXP_PLATFORM_UNIX
#if defined(__APPLE__)
#define RXP_PLATFORM_DARWIN
#elif defined(__linux__)
#define RXP_PLATFORM_LINUX
#endif
#endif

#include "rexo.h"

#endif

typedef struct PRIVATE_API(test_suite) {
    int (*run)(void);
} test_suite;

#define TEST_RUN(variable, expression)           \
    int test_run_##variable = expression->run(); \
    if (test_run_##variable != 0) {              \
        return test_run_##variable;              \
    }                                            \
    int variable = test_run_##variable

#define CLEAN(ptr)  \
    if (ptr != 0) { \
        ptr = 0;    \
    }

#endif /* _TESTS_TEST_H_ */
