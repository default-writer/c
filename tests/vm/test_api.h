/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 28, 2025 at 10:19:11 AM GMT+3
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

#define USING_TESTS_API

#ifndef _TEST_API_H_
#define _TEST_API_H_

#define USING_STD_API
#include "std/api.h"

typedef struct PRIVATE_API(test_suite) tests_api_test_suite;

/* definition */
extern const tests_api_test_suite PRIVATE_API(tests_api_test_suite_definitions);

/* definition */
#ifdef INLINE
const tests_api_test_suite* tests_api_test_suite = &PRIVATE_API(tests_api_test_suite_definitions);
#else
/* definition */
static const tests_api_test_suite* _tests_api_test_suite = &PRIVATE_API(tests_api_test_suite_definitions);
#endif

#endif /* _TEST_API_H_ */
