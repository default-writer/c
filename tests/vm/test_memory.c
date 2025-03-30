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

#include "test_memory.h"

#include "system/memory/memory_v1.h"
#include "system/os/os_v1.h"

#define USING_TESTS
#include "test.h"

/* definition */
CSYS_EXPORT extern const system_memory_methods* PRIVATE_API(memory);
CSYS_EXPORT extern const system_os_methods* PRIVATE_API(os);

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    stack_ptr ctx;
}* TEST_DATA;

/*api*/
static const system_memory_methods* memory_methods_ptr;
static const system_os_methods* temp_api;

/* mocks */
static void* mock_alloc(size_t __nmemb, size_t __size);
static void mock_free(void* __ptr);
static void* mock_memory_alloc(u64 address);
static void mock_memory_free(const_void_ptr const_ptr, u64 size);

/* implementation */
static void* mock_alloc(size_t __nmemb, size_t __size) {
    return NULL_PTR;
}

static void mock_free(void* __ptr) {
    memset(__ptr, 0xef, sizeof(u64)); /* NOLINT: sizeof(u64) */
}

static const system_os_methods mock_api_methods_definitions = {
    .alloc = mock_alloc,
    .free = mock_free
};

static u64 last_alloc_size = 0;
static void* last_alloc_ptr = 0;
static u8 memory_buffer[8] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };

static void* mock_memory_alloc(u64 size) {
    last_alloc_size = size;
    last_alloc_ptr = memory_buffer;
    void* ptr = last_alloc_ptr;
    memset(ptr, 0, sizeof(u64)); /* NOLINT: sizeof(u64) */
    return ptr;
}

static void mock_memory_free(const_void_ptr const_ptr, u64 size) {
    if (last_alloc_ptr == const_ptr && last_alloc_size == size) {
        const_void_ptr const_data_ptr = const_ptr;
        safe_void_ptr void_ptr;
        void_ptr.const_ptr = const_data_ptr;
        void* ptr = void_ptr.ptr;
        memset(ptr, 0xfe, sizeof(u64)); /* NOLINT: sizeof(u64) */
    }
}

static const system_os_methods* mock_api = &mock_api_methods_definitions;

/* Initialize the data structure. Its allocation is handled by Rexo. */
RX_SET_UP(test_set_up) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    *ctx = CALL(os)->alloc(1, sizeof(stack_element_type));
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* destroy list */
    CALL(os)->free(*ctx);
    /* initializes to 0 */
    *ctx = 0;
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

/* test init */
RX_TEST_CASE(tests_memory_v1, test_empty_memory_count_equals_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* ensures counter is initialized to 0 */
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(tests_memory_v1, test_alloc_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* pushed to the list */
    const void* data_ptr = CALL(memory)->alloc(0);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(tests_memory_v1, test_api_alloc_ret_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* backup api calls */
    memcpy(&temp_api, &PRIVATE_API(os), sizeof(system_os_methods*)); /* NOLINT: sizeof(system_os_methods*) */
    /* prepare to mock api calls */
    memcpy(&PRIVATE_API(os), &mock_api, sizeof(system_os_methods*)); /* NOLINT: sizeof(system_os_methods*) */
    /* pushed to the list */
    const void* data_ptr = CALL(memory)->alloc(16);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
    /* restore api calls */
    memcpy(&PRIVATE_API(os), &temp_api, sizeof(system_os_methods*)); /* NOLINT: sizeof(system_os_methods*) */
}

/* test case */
RX_TEST_CASE(tests_memory_v1, test_api_free_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* backup api calls */
    memcpy(&temp_api, &PRIVATE_API(os), sizeof(system_os_methods*)); /* NOLINT: sizeof(system_os_methods*) */
    /* prepare to mock api calls */
    memcpy(&PRIVATE_API(os), &mock_api, sizeof(system_os_methods*)); /* NOLINT: sizeof(system_os_methods*) */
    /* pushed to the list */
    CALL(memory)->free((void*)0, 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
    /* restore api calls */
    memcpy(&PRIVATE_API(os), &temp_api, sizeof(system_os_methods*)); /* NOLINT: sizeof(system_os_methods*) */
}

/* test case */
RX_TEST_CASE(tests_memory_v1, test_api_free_size_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* backup api calls */
    memcpy(&temp_api, &PRIVATE_API(os), sizeof(system_os_methods*)); /* NOLINT: sizeof(system_os_methods*) */
    /* prepare to mock api calls */
    memcpy(&PRIVATE_API(os), &mock_api, sizeof(system_os_methods*)); /* NOLINT: sizeof(system_os_methods*) */
    /* pushed to the list */
    CALL(memory)->free((void*)0xdeadbeef, 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
    /* restore api calls */
    memcpy(&PRIVATE_API(os), &temp_api, sizeof(system_os_methods*)); /* NOLINT: sizeof(system_os_methods*) */
}

/* test case */
RX_TEST_CASE(tests_memory_v1, test_api_alloc_free_8, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    uint64_t expected_pattern = 0xefefefefefefefef;
    /* backup api calls */
    memcpy(&temp_api, &PRIVATE_API(os), sizeof(system_os_methods*)); /* NOLINT: sizeof(system_os_methods*) */
    /* prepare to mock api calls */
    memcpy(&PRIVATE_API(os), &mock_api, sizeof(system_os_methods*)); /* NOLINT: sizeof(system_os_methods*) */
    void* ptr = calloc(1, 16);
    u64 value = (u64)0xdeadbeef;
    memcpy(ptr, &value, 8); /* NOLINT: sizeof(system_os_methods*) */
    CALL(os)->free(ptr); /* NOLINT: mock api */
    /* compare to make sure mock memory free function is called */
    RX_ASSERT(memcmp(ptr, &expected_pattern, sizeof(expected_pattern)) == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
    /* free ptr */
    free(ptr);
    /* restore api calls */
    memcpy(&PRIVATE_API(os), &temp_api, sizeof(system_os_methods*)); /* NOLINT: sizeof(system_os_methods*) */
}

/* test case */
RX_TEST_CASE(tests_memory_v1, test_alloc_free_debug, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    uint64_t expected_pattern = 0xfefefefefefefefe;
    static system_memory_methods mock_memory_methods_definitions;
    /*api */
    memcpy(&mock_memory_methods_definitions, PRIVATE_API(memory), sizeof(system_memory_methods)); /* NOLINT: sizeof(memory_methods*) */
    /* setup mocks */
    mock_memory_methods_definitions.alloc = mock_memory_alloc;
    mock_memory_methods_definitions.free = mock_memory_free;
    /* setup api endpoint */
    static const system_memory_methods* mock_memory_methods = &mock_memory_methods_definitions;
    /* backup api calls */
    memcpy(&memory_methods_ptr, &PRIVATE_API(memory), sizeof(system_memory_methods*)); /* NOLINT: sizeof(memory_methods*) */
    /* prepare to mock api calls */
    memcpy(&PRIVATE_API(memory), &mock_memory_methods, sizeof(system_memory_methods*)); /* NOLINT: sizeof(memory_methods*) */
    u64 pointer_size = sizeof(void*);
    void* ptr = CALL(memory)->alloc(pointer_size);
    u64 value = (u64)0xdeadbeef;
    memcpy(ptr, &value, 8); /* NOLINT: sizeof(void*) */
    /* compare to make sure mock memory free function is called */
    CALL(memory)->free(ptr, pointer_size); /* NOLINT: mock api */
    RX_ASSERT(memcmp(ptr, &expected_pattern, sizeof(expected_pattern)) == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
    /* restore api calls */
    memcpy(&PRIVATE_API(memory), &memory_methods_ptr, sizeof(system_memory_methods*)); /* NOLINT: sizeof(memory_methods*) */
}

/* test case */
RX_TEST_CASE(tests_memory_v1, test_realloc_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* pushed to the list */
    stack_ptr ptr = 0;
    stack_ptr* null_ptr = &ptr;
    const void* data_ptr = CALL(memory)->realloc(null_ptr, 0, 0);
    /* ensures there is no result on 0 */
    RX_ASSERT(null_ptr != 0);
    RX_ASSERT(data_ptr == 0);
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(tests_memory_v1, test_realloc_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* pushed to the list */
    stack_ptr ptr = 0;
    stack_ptr* null_ptr = &ptr;
    const void* data_ptr = CALL(memory)->realloc(null_ptr, 1, 0);
    /* ensures there is no result on 0 */
    RX_ASSERT(null_ptr != 0);
    RX_ASSERT(data_ptr == 0);
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(tests_memory_v1, test_realloc_2_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* pushed to the list */
    stack_ptr ptr = 0;
    stack_ptr* null_ptr = &ptr;
    const void* data_ptr = CALL(memory)->realloc(null_ptr, 2, 1);
    /* ensures there is no result on 0 */
    RX_ASSERT(null_ptr != 0);
    RX_ASSERT(data_ptr == 0);
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(tests_memory_v1, test_realloc_0_ptr, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* pushed to the list */
    stack_ptr ptr = 0;
    stack_ptr* null_ptr = &ptr;
    const void* data_ptr = CALL(memory)->realloc(0, 24, 8);
    /* ensures there is no result on 0 */
    RX_ASSERT(null_ptr != 0);
    RX_ASSERT(data_ptr == 0);
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(tests_memory_v1, test_alloc_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    stack_ptr element = CALL(memory)->alloc(sizeof(stack_element_type));
    /* pushed to the list */
    CALL(memory)->free(element, sizeof(stack_element_type));
    RX_ASSERT(*ctx != 0);
}

static void run(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- rexo unit test code %s\n", __FILE__);
#endif
}

const tests_memory_test_suite PRIVATE_API(tests_memory_test_suite_definitions) = {
    .run = run
};
