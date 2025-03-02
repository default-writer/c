/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 2, 2025 at 9:13:24 PM GMT+3
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

#include <stdlib.h>
#define USING_TESTS

#include "test_memory.h"

#include "sys/memory/memory_v1.h"

#include "test.h"

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    stack_ptr ctx;
}* TEST_DATA;

/*api*/
static void* mock_alloc(size_t __nmemb, size_t __size) {
    return 0;
}

static void mock_free(void* __ptr) {
    memset(__ptr, 0xef, sizeof(u64)); /* NOLINT: sizeof(u64) */
}

static const memory_api mock_memory_api_methods_definitions = {
    .alloc = mock_alloc,
    .free = mock_free
};

static const memory_api* mock_memory_api = &mock_memory_api_methods_definitions;
static const memory_api* temp_memory_api;

/* Initialize the data structure. Its allocation is handled by Rexo. */
RX_SET_UP(test_set_up) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    *ctx = CALL(sys_memory)->alloc(sizeof(stack_element));
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* destroy list */
    CALL(sys_memory)->free(*ctx, sizeof(stack_element));
    /* initializes to 0 */
    *ctx = 0;
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

/* test init */
RX_TEST_CASE(memory_micro_tests, test_empty_memory_count_equals_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* ensures counter is initialized to 0 */
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(memory_micro_tests, test_alloc_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* pushed to the list */
    const void* data_ptr = CALL(sys_memory)->alloc(0);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(memory_micro_tests, test_alloc_ret_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* backup api calls */
    memcpy(&temp_memory_api, &memory, sizeof(memory_api*)); /* NOLINT: sizeof(memory_api*) */
    /* prepare to mock api calls */
    memcpy(&memory, &mock_memory_api, sizeof(memory_api*)); /* NOLINT: sizeof(memory_api*) */
    /* pushed to the list */
    const void* data_ptr = CALL(sys_memory)->alloc(16);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
    /* restore api calls */
    memcpy(&memory, &temp_memory_api, sizeof(memory_api*)); /* NOLINT: sizeof(memory_api*) */
}

/* test case */
RX_TEST_CASE(memory_micro_tests, test_free_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* backup api calls */
    memcpy(&temp_memory_api, &memory, sizeof(memory_api*)); /* NOLINT: sizeof(memory_api*) */
    /* prepare to mock api calls */
    memcpy(&memory, &mock_memory_api, sizeof(memory_api*)); /* NOLINT: sizeof(memory_api*) */
    /* pushed to the list */
    CALL(sys_memory)->free((void*)0, 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
    /* restore api calls */
    memcpy(&memory, &temp_memory_api, sizeof(memory_api*)); /* NOLINT: sizeof(memory_api*) */
}

/* test case */
RX_TEST_CASE(memory_micro_tests, test_free_size_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* backup api calls */
    memcpy(&temp_memory_api, &memory, sizeof(memory_api*)); /* NOLINT: sizeof(memory_api*) */
    /* prepare to mock api calls */
    memcpy(&memory, &mock_memory_api, sizeof(memory_api*)); /* NOLINT: sizeof(memory_api*) */
    /* pushed to the list */
    CALL(sys_memory)->free((void*)0xdeadbeef, 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
    /* restore api calls */
    memcpy(&memory, &temp_memory_api, sizeof(memory_api*)); /* NOLINT: sizeof(memory_api*) */
}

/* test case */
RX_TEST_CASE(memory_micro_tests, test_free_size_8, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    uint64_t expected_pattern = 0xefefefefefefefef;
    /* backup api calls */
    memcpy(&temp_memory_api, &memory, sizeof(memory_api*)); /* NOLINT: sizeof(memory_api*) */
    /* prepare to mock api calls */
    memcpy(&memory, &mock_memory_api, sizeof(memory_api*)); /* NOLINT: sizeof(memory_api*) */
    void* ptr = calloc(1, 16);
    u64 value = (u64)0xdeadbeef;
    memcpy(ptr, &value, 8); /* NOLINT: sizeof(memory_api*) */
    CALL(sys_memory)->free(ptr, 8); /* NOLINT: mock api */
    /* compare to make sure mock memory free function is called */
    RX_ASSERT(memcmp(ptr, &expected_pattern, sizeof(expected_pattern)) == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
    /* free ptr */
    free(ptr);
    /* restore api calls */
    memcpy(&memory, &temp_memory_api, sizeof(memory_api*)); /* NOLINT: sizeof(memory_api*) */
}

/* test case */
RX_TEST_CASE(memory_micro_tests, test_realloc_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* pushed to the list */
    stack_ptr ptr = 0;
    stack_ptr* null_ptr = &ptr;
    const void* data_ptr = CALL(sys_memory)->realloc(null_ptr, 0, 0);
    /* ensures there is no result on 0 */
    RX_ASSERT(null_ptr != 0);
    RX_ASSERT(data_ptr == 0);
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(memory_micro_tests, test_realloc_0_ptr, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* pushed to the list */
    stack_ptr ptr = 0;
    stack_ptr* null_ptr = &ptr;
    const void* data_ptr = CALL(sys_memory)->realloc(0, 24, 8);
    /* ensures there is no result on 0 */
    RX_ASSERT(null_ptr != 0);
    RX_ASSERT(data_ptr == 0);
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(memory_micro_tests, test_alloc_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    stack_ptr new = CALL(sys_memory)->alloc(sizeof(stack_element));
    /* pushed to the list */
    CALL(sys_memory)->free(new, sizeof(stack_element));
    RX_ASSERT(*ctx != 0);
}

static int run(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- rexo unit test code\n");
#endif
    /* Execute the main function that runs the test cases found. */
    int result = rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;
    return result;
}

const memory_micro_test_suite PRIVATE_API(memory_micro_test_suite_definitions) = {
    .run = run
};
