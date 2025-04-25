/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 12, 1961 at 09:07:34 PM GMT+3
 * Modified:
 *   April 17, 2025 at 11:57:23 AM GMT+3
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

#include "test_hashtable.h"

#include "system/hashtable/hashtable_v1.h"

#define USING_TESTS
#include "test.h"

/* definition */

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    hashtable_ptr ctx;
}* TEST_DATA;

/*api*/

RX_SET_UP(test_set_up) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    hashtable_ptr vm = CALL(hashtable)->init();
    rx->ctx = vm;
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    hashtable_ptr ht = rx->ctx;
    CALL(hashtable)->destroy(ht);
}

RX_SET_UP(test_set_up_pointer_init) {
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down_pointer_destroy) {
    /* nothing to cleanup */
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

/* Define the fixture. */
RX_FIXTURE(test_fixture_pointer, TEST_DATA, .set_up = test_set_up_pointer_init, .tear_down = test_tear_down_pointer_destroy);

/* test init */
RX_TEST_CASE(tests_hashtable_v1, test_hashtable, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    hashtable_ptr ht = rx->ctx;

    const char* str1 = "hello";
    const char* str2 = ", ";
    const char* str3 = "world";
    const char* str4 = "!";

    CALL(hashtable)->insert(ht, 10, str1);
    CALL(hashtable)->insert(ht, 20, str2);
    CALL(hashtable)->insert(ht, 30, str3);
    CALL(hashtable)->insert(ht, 40, str4);

    RX_ASSERT(strcmp((const char*)CALL(hashtable)->get(ht, 10), str1) == 0);
    RX_ASSERT(strcmp((const char*)CALL(hashtable)->get(ht, 20), str2) == 0);
    RX_ASSERT(strcmp((const char*)CALL(hashtable)->get(ht, 30), str3) == 0);
    RX_ASSERT(strcmp((const char*)CALL(hashtable)->get(ht, 40), str4) == 0);

    RX_ASSERT(CALL(hashtable)->get(ht, 60) == NULL);

    CALL(hashtable)->remove(ht, 20);
    CALL(hashtable)->insert(ht, 60 + HASHTABLE_INITIAL_CAPACITY, str2);
    CALL(hashtable)->insert(ht, 60, str3);
    CALL(hashtable)->remove(ht, 60 + HASHTABLE_INITIAL_CAPACITY);
    CALL(hashtable)->remove(ht, 61 + HASHTABLE_INITIAL_CAPACITY);

    RX_ASSERT(CALL(hashtable)->get(ht, 20) == NULL);
    RX_ASSERT(CALL(hashtable)->get(ht, 60) != NULL);

    RX_ASSERT(ht->size == 4);
}

/* test init */
RX_TEST_CASE(tests_hashtable_v1, test_hashtable_resize, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    hashtable_ptr ht = rx->ctx;
    for (size_t i = 0; i < HASHTABLE_INITIAL_CAPACITY * 2; ++i) {
        CALL(hashtable)->insert(ht, i, (void_ptr)i);
    }
    for (size_t i = 0; i < HASHTABLE_INITIAL_CAPACITY * 2; ++i) {
        RX_ASSERT((u64)CALL(hashtable)->get(ht, i) == i);
    }
}

/* test init */
RX_TEST_CASE(tests_hashtable_v1, test_hashtable_null, .fixture = test_fixture_pointer) {
    RX_ASSERT(CALL(hashtable)->insert(NULL, 10, "test") == NULL);
    RX_ASSERT(CALL(hashtable)->get(NULL, 10) == NULL);
    RX_ASSERT(CALL(hashtable)->remove(NULL, 10) == FALSE);
    CALL(hashtable)->destroy(NULL);
}

/* test init */
RX_TEST_CASE(tests_hashtable_v1, test_hashtable_collisions, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    hashtable_ptr ht = rx->ctx;
    CALL(hashtable)->insert(ht, 10, (void_ptr)10);
    CALL(hashtable)->insert(ht, 26, (void_ptr)26);
    RX_ASSERT((u64)CALL(hashtable)->get(ht, 10) == 10);
    RX_ASSERT((u64)CALL(hashtable)->get(ht, 26) == 26);
}

static void run(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- rexo unit test code %s\n", __FILE__);
#endif
}

const tests_hashtable_test_suite PRIVATE_API(tests_hashtable_test_suite_definitions) = {
    .run = run
};
