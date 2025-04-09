/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 9, 2025 at 11:03:43 AM GMT+3
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
