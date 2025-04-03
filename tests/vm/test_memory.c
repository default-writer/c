/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 3, 2025 at 11:19:43 AM GMT+3
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

#include "system/error/error_v1.h"
#include "system/memory/memory_v1.h"
#include "system/os/os_v1.h"

#include "virtual/env/env_v1.h"
#include "virtual/types/data/data_v1.h"
#include "virtual/types/file/file_v1.h"
#include "virtual/types/string/string_v1.h"
#include "virtual/vm/vm_v1.h"

#define USING_TESTS
#include "test.h"

/* definition */
CSYS_EXPORT extern const system_memory_methods* PRIVATE_API(memory);
CSYS_EXPORT extern const system_os_methods* PRIVATE_API(os);

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    stack_ptr ctx;
}* TEST_DATA;

/* Data structure to use at the core of our fixture. */
typedef struct test_vm_data {
    const_vm_ptr ctx;
}* TEST_VM_DATA;

/*api*/
static const system_memory_methods* memory_methods_ptr;
static const system_os_methods* memory_os_ptr;
static const system_os_methods* temp_api;

/* mocks */
static void_ptr mock_memory_alloc(u64 address);
static void mock_memory_free(const_void_ptr const_ptr, u64 size);
static void_ptr mock_os_calloc(size_t __nmemb, size_t __size);
static int mock_os_fseek(FILE* __stream, long int __off, int __whence);
static void mock_os_free(void_ptr __ptr);

/* implementation */
static u64 last_alloc_size = 0;
static void_ptr last_alloc_ptr = 0;
static u8 memory_buffer[8] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };

static void_ptr mock_memory_alloc(u64 size) {
    last_alloc_size = size;
    last_alloc_ptr = memory_buffer;
    void_ptr ptr = last_alloc_ptr;
    memset(ptr, 0, sizeof(u64)); /* NOLINT: sizeof(u64) */
    return ptr;
}

static void mock_memory_free(const_void_ptr const_ptr, u64 size) {
    if (last_alloc_ptr == const_ptr && last_alloc_size == size) {
        const_void_ptr const_data_ptr = const_ptr;
        safe_void_ptr safe_ptr;
        safe_ptr.const_ptr = const_data_ptr;
        void_ptr ptr = safe_ptr.ptr;
        memset(ptr, 0xfe, sizeof(u64)); /* NOLINT: sizeof(u64) */
    }
}

static void_ptr mock_os_calloc(size_t __nmemb, size_t __size) {
    return NULL_PTR;
}

static int mock_os_fseek(FILE* __stream, long int __off, int __whence) {
    return -1;
}

static void mock_os_free(void_ptr __ptr) {
    memset(__ptr, 0xef, sizeof(u64)); /* NOLINT: sizeof(u64) */
}

/* Initialize the data structure. Its allocation is handled by Rexo. */
RX_SET_UP(test_set_up) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    *ctx = CALL(os)->calloc(1, sizeof(stack_element_type));
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

RX_SET_UP(test_vm_set_up) {
    TEST_VM_DATA rx = (TEST_VM_DATA)RX_DATA;
    const_vm_ptr cvm = CALL(vm)->init(8);
    rx->ctx = cvm;
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_vm_tear_down) {
    TEST_VM_DATA rx = (TEST_VM_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    CALL(vm)->gc(cvm);
    CALL(vm)->destroy(cvm);
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

/* Define the fixture. */
RX_FIXTURE(test_vm_fixture, TEST_VM_DATA, .set_up = test_vm_set_up, .tear_down = test_vm_tear_down);

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
    const_void_ptr data_ptr = CALL(memory)->alloc(0);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(tests_memory_v1, test_error_api_stdout, .fixture = test_fixture) {
    CALL(error)->clear();
    FILE* f = CALL(error)->stdout();
    RX_ASSERT(f == stdout);
}

/* test case */
RX_TEST_CASE(tests_memory_v1, test_error_api_stderr, .fixture = test_fixture) {
    CALL(error)->clear();
    FILE* f = CALL(error)->stderr();
    RX_ASSERT(f == stderr);
}

/* test case */
RX_TEST_CASE(tests_memory_v1, test_api_clear_get_has_0, .fixture = test_fixture) {
    CALL(error)->clear();
    const char* ex = CALL(error)->get();
    RX_ASSERT(ex == 0);
    RX_ASSERT(CALL(error)->type() == 0);
}

/* test case */
RX_TEST_CASE(tests_memory_v1, test_api_error_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    const char* ch = "hello, world!";
    char* very_long_message = CALL(os)->calloc(1, ERROR_MESSAGE_SIZE);
    CALL(os)->memset(very_long_message, 0xfe, ERROR_MESSAGE_SIZE); /* NOLINT: sizeof(u64); */
    CALL(os)->memcpy(very_long_message, ch, 13); /* NOLINT: sizeof(u64); */
    very_long_message[14] = 0;
    u64 very_long_message_size = CALL(os)->strlen(ch);
    CALL(error)->throw(ID_ERROR_NO_ERROR, very_long_message, very_long_message_size);
    RX_ASSERT(CALL(error)->type() == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
    CALL(os)->free(very_long_message); /* NOLINT: sizeof(u64) */
}

/* test case */
RX_TEST_CASE(tests_memory_v1, test_api_error_throw_error_max, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    char* very_long_message = CALL(os)->calloc(1, 8192);
    CALL(os)->memset(very_long_message, 0xfe, 8192); /* NOLINT: sizeof(u64); */
    u64 very_long_message_size = ERROR_MESSAGE_SIZE;
    CALL(error)->throw(ID_ERROR_INVALID_ARGUMENT, very_long_message, very_long_message_size);
    RX_ASSERT(CALL(error)->type() != 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
    CALL(os)->free(very_long_message); /* NOLINT: sizeof(u64) */
}

/* test case */
RX_TEST_CASE(tests_memory_v1, test_api_error_next, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    const char* ch = "hello, world!";
    char* very_long_message = CALL(os)->calloc(1, ERROR_MESSAGE_SIZE);
    CALL(os)->memset(very_long_message, 0xfe, ERROR_MESSAGE_SIZE); /* NOLINT: sizeof(u64); */
    CALL(os)->memcpy(very_long_message, ch, 13); /* NOLINT: sizeof(u64); */
    very_long_message[14] = 0;
    u64 very_long_message_size = CALL(os)->strlen(ch);
    CALL(error)->clear();
    CALL(error)->throw(1, very_long_message, very_long_message_size);
    for (int i = 0; i < ERROR_MESSAGE_COUNT + 1; i++) {
        CALL(error)->throw((u64)(i + 2), very_long_message, very_long_message_size);
    }
    u64 count = CALL(error)->count();
    RX_ASSERT(count == ERROR_MESSAGE_COUNT);
    do {
        RX_ASSERT(CALL(error)->get() != 0);
        RX_ASSERT(CALL(error)->type() != 0);
        fprintf(CALL(error)->stderr(), "message=%s, type=%lld\n", CALL(error)->get(), CALL(error)->type()); /* NOLINT: fprintf */
        CALL(error)->next();
    } while (CALL(error)->count() != 0);
    CALL(error)->next();
    RX_ASSERT(CALL(error)->get() == 0);
    RX_ASSERT(CALL(error)->type() == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
    CALL(os)->free(very_long_message); /* NOLINT: sizeof(u64) */
}

/* test case */
RX_TEST_CASE(tests_memory_v1, test_vm_init, .fixture = test_fixture) {
    CALL(error)->clear();
    const_vm_ptr buffer[VM_PTR_ARRAY_SIZE] = { 0 };
    for (int i = 0; i < VM_PTR_ARRAY_SIZE; i++) {
        buffer[i] = CALL(vm)->init(0);
    }
    RX_ASSERT(CALL(vm)->init(0) == 0);
    for (int i = 0; i < VM_PTR_ARRAY_SIZE; i++) {
        CALL(vm)->destroy(buffer[i]);
    }
}

/* test case */
RX_TEST_CASE(tests_memory_v1, test_api_file_data_error_0, .fixture = test_vm_fixture) {
    TEST_VM_DATA rx = (TEST_VM_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    static system_os_methods mock_os_methods_definitions;
    /*api */
    memcpy(&mock_os_methods_definitions, PRIVATE_API(os), sizeof(system_os_methods)); /* NOLINT: sizeof(system_os_methods*) */
    /* setup mocks */
    mock_os_methods_definitions.fseek = mock_os_fseek;
    /* setup api endpoint */
    static const system_os_methods* mock_os_methods = &mock_os_methods_definitions;
    /* backup api calls */
    memcpy(&memory_os_ptr, &PRIVATE_API(os), sizeof(system_os_methods*)); /* NOLINT: sizeof(system_os_methods*) */
    /* prepare to mock api calls */
    memcpy(&PRIVATE_API(os), &mock_os_methods, sizeof(system_os_methods*)); /* NOLINT: sizeof(system_os_methods*) */
    /* pushed to the list */
    u64 file_path_ptr = CALL(env)->getcwd(cvm);
    u64 file_name_ptr = CALL(string)->load(cvm, "/data/all_english_words.txt");
    CALL(string)->strcat(cvm, file_path_ptr, file_name_ptr);
    CALL(string)->free(cvm, file_name_ptr);
    u64 mode_ptr = CALL(string)->load(cvm, "rb");
    u64 f_ptr = CALL(file)->alloc(cvm, file_path_ptr, mode_ptr);
    CALL(string)->free(cvm, file_path_ptr);
    CALL(string)->free(cvm, mode_ptr);
    u64 data_ptr = CALL(file)->data(cvm, f_ptr);
    CALL(file)->free(cvm, f_ptr);
    CALL(env)->puts(cvm, data_ptr);
#ifndef USE_GC
    CALL(data)->free(cvm, data_ptr);
#endif
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(cvm != 0);
    /* restore api calls */
    memcpy(&PRIVATE_API(os), &memory_os_ptr, sizeof(system_os_methods*)); /* NOLINT: sizeof(system_os_methods*) */
}

/* test case */
RX_TEST_CASE(tests_memory_v1, test_api_alloc_ret_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    static system_os_methods mock_os_methods_definitions;
    /*api */
    memcpy(&mock_os_methods_definitions, PRIVATE_API(os), sizeof(system_os_methods)); /* NOLINT: sizeof(system_os_methods*) */
    /* setup mocks */
    mock_os_methods_definitions.calloc = mock_os_calloc;
    mock_os_methods_definitions.free = mock_os_free;
    /* setup api endpoint */
    static const system_os_methods* mock_os_methods = &mock_os_methods_definitions;
    /* backup api calls */
    memcpy(&memory_os_ptr, &PRIVATE_API(os), sizeof(system_os_methods*)); /* NOLINT: sizeof(system_os_methods*) */
    /* prepare to mock api calls */
    memcpy(&PRIVATE_API(os), &mock_os_methods, sizeof(system_os_methods*)); /* NOLINT: sizeof(system_os_methods*) */
    /* pushed to the list */
    const_void_ptr data_ptr = CALL(memory)->alloc(16);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
    /* restore api calls */
    memcpy(&PRIVATE_API(os), &memory_os_ptr, sizeof(system_os_methods*)); /* NOLINT: sizeof(system_os_methods*) */
}

/* test case */
RX_TEST_CASE(tests_memory_v1, test_api_free_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    static system_os_methods mock_os_methods_definitions;
    /*api */
    memcpy(&mock_os_methods_definitions, PRIVATE_API(os), sizeof(system_os_methods)); /* NOLINT: sizeof(system_os_methods*) */
    /* setup mocks */
    mock_os_methods_definitions.calloc = mock_os_calloc;
    mock_os_methods_definitions.free = mock_os_free;
    /* setup api endpoint */
    static const system_os_methods* mock_os_methods = &mock_os_methods_definitions;
    /* backup api calls */
    memcpy(&memory_os_ptr, &PRIVATE_API(os), sizeof(system_os_methods*)); /* NOLINT: sizeof(system_os_methods*) */
    /* prepare to mock api calls */
    memcpy(&PRIVATE_API(os), &mock_os_methods, sizeof(system_os_methods*)); /* NOLINT: sizeof(system_os_methods*) */
    /* pushed to the list */
    CALL(memory)->free((void_ptr)0, 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
    /* restore api calls */
    memcpy(&PRIVATE_API(os), &memory_os_ptr, sizeof(system_os_methods*)); /* NOLINT: sizeof(system_os_methods*) */
}

/* test case */
RX_TEST_CASE(tests_memory_v1, test_api_free_size_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    static system_os_methods mock_os_methods_definitions;
    /*api */
    memcpy(&mock_os_methods_definitions, PRIVATE_API(os), sizeof(system_os_methods)); /* NOLINT: sizeof(system_os_methods*) */
    /* setup mocks */
    mock_os_methods_definitions.calloc = mock_os_calloc;
    mock_os_methods_definitions.free = mock_os_free;
    /* setup api endpoint */
    static const system_os_methods* mock_os_methods = &mock_os_methods_definitions;
    /* backup api calls */
    memcpy(&memory_os_ptr, &PRIVATE_API(os), sizeof(system_os_methods*)); /* NOLINT: sizeof(system_os_methods*) */
    /* prepare to mock api calls */
    memcpy(&PRIVATE_API(os), &mock_os_methods, sizeof(system_os_methods*)); /* NOLINT: sizeof(system_os_methods*) */
    /* pushed to the list */
    CALL(memory)->free((void_ptr)0xdeadbeef, 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
    /* restore api calls */
    memcpy(&PRIVATE_API(os), &memory_os_ptr, sizeof(system_os_methods*)); /* NOLINT: sizeof(system_os_methods*) */
}

/* test case */
RX_TEST_CASE(tests_memory_v1, test_api_alloc_free_8, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    uint64_t expected_pattern = 0xefefefefefefefef;
    static system_os_methods mock_os_methods_definitions;
    /*api */
    memcpy(&mock_os_methods_definitions, PRIVATE_API(os), sizeof(system_os_methods)); /* NOLINT: sizeof(system_os_methods*) */
    /* setup mocks */
    mock_os_methods_definitions.calloc = mock_os_calloc;
    mock_os_methods_definitions.free = mock_os_free;
    /* setup api endpoint */
    static const system_os_methods* mock_os_methods = &mock_os_methods_definitions;
    /* backup api calls */
    memcpy(&temp_api, &PRIVATE_API(os), sizeof(system_os_methods*)); /* NOLINT: sizeof(system_os_methods*) */
    /* prepare to mock api calls */
    memcpy(&PRIVATE_API(os), &mock_os_methods, sizeof(system_os_methods*)); /* NOLINT: sizeof(system_os_methods*) */
    void_ptr ptr = calloc(1, 16);
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
    u64 pointer_size = sizeof(void_ptr);
    void_ptr ptr = CALL(memory)->alloc(pointer_size);
    u64 value = (u64)0xdeadbeef;
    memcpy(ptr, &value, 8); /* NOLINT: sizeof(void_ptr) */
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
    const_void_ptr data_ptr = CALL(memory)->realloc(null_ptr, 0, 0);
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
    const_void_ptr data_ptr = CALL(memory)->realloc(null_ptr, 1, 0);
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
    const_void_ptr data_ptr = CALL(memory)->realloc(null_ptr, 2, 1);
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
    const_void_ptr data_ptr = CALL(memory)->realloc(0, 24, 8);
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
