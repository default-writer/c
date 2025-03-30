/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 28, 2025 at 4:58:21 PM GMT+3
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

#include "test_api.h"

#include "system/api/api_v1.h"
#include "virtual/api/api_v1.h"

#define USING_TESTS
#include "test.h"

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    stack_ptr ctx;
}* TEST_DATA;

/* Initialize the data structure. Its allocation is handled by Rexo. */
RX_SET_UP(test_set_up) {
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    /* nothing to cleanup */
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

/* test init */
RX_TEST_CASE(tests_api_v1, test_api, .fixture = test_fixture) {
    RX_ASSERT(PUBLIC_API(error) != 0);
    RX_ASSERT(PUBLIC_API(info) != 0);
    RX_ASSERT(PUBLIC_API(list) != 0);
    RX_ASSERT(PUBLIC_API(memory) != 0);
    RX_ASSERT(PUBLIC_API(options) != 0);
    RX_ASSERT(PUBLIC_API(os) != 0);
    RX_ASSERT(PUBLIC_API(vm) != 0);
    RX_ASSERT(PUBLIC_API(virtual) != 0);
    RX_ASSERT(PUBLIC_API(pointer) != 0);
    RX_ASSERT(PUBLIC_API(env) != 0);
    RX_ASSERT(PUBLIC_API(hashtable) != 0);
    RX_ASSERT(PUBLIC_API(data) != 0);
    RX_ASSERT(PUBLIC_API(file) != 0);
    RX_ASSERT(PUBLIC_API(object) != 0);
    RX_ASSERT(PUBLIC_API(stack) != 0);
    RX_ASSERT(PUBLIC_API(string) != 0);
    RX_ASSERT(PUBLIC_API(string_pointer) != 0);
    RX_ASSERT(PUBLIC_API(user) != 0);
}

static void run(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- rexo unit test code %s\n", __FILE__);
#endif
}

const tests_api_test_suite PRIVATE_API(tests_api_test_suite_definitions) = {
    .run = run
};
