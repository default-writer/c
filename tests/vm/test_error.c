/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 12, 1961 at 09:07:34 PM GMT+3
 * Modified:
 *   May 5, 2025 at 3:50:41 PM GMT+3
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

#include "test_error.h"

#include "system/error/error_v1.h"

#define USING_TESTS
#include "test.h"

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    void* data;
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
RX_TEST_CASE(tests_error, test_measure_int, .fixture = test_fixture) {
    u64 written = CALL(error)->print(NULL, 0, "%d", -2147483648);
    RX_ASSERT(written == strlen("-2147483648") + 1);
}

/* test init */
RX_TEST_CASE(tests_error, test_print_int, .fixture = test_fixture) {
    u64 bytes = CALL(error)->print(NULL, 0, "%d", -2147483648);
    char* buffer = (char*)malloc(bytes);
    CALL(error)->print(buffer, bytes, "%d", -2147483648);
    RX_ASSERT(strcmp(buffer, "-2147483648") == 0);
    free(buffer);
}

/* test init */
RX_TEST_CASE(tests_error, test_measure_string_empty, .fixture = test_fixture) {
    const char* text = "\0";
    u64 written = CALL(error)->print(NULL, 0, "%s", text);
    RX_ASSERT(written == strlen(text) + 1);
}

RX_TEST_CASE(tests_error, test_print_string_empty, .fixture = test_fixture) {
    const char* text = "\0";
    u64 bytes = CALL(error)->print(NULL, 0, "%s", text);
    char* buffer = (char*)malloc(bytes);
    CALL(error)->print(buffer, bytes, "%s", text);
    RX_ASSERT(strcmp(buffer, text) == 0);
    free(buffer);
}

/* test init */
RX_TEST_CASE(tests_error, test_measure_string, .fixture = test_fixture) {
    const char* text = "hello, world!";
    u64 written = CALL(error)->print(NULL, 0, "%s", text);
    RX_ASSERT(written == strlen(text) + 1);
}

/* test init */
RX_TEST_CASE(tests_error, test_print_string, .fixture = test_fixture) {
    const char* text = "hello, world!";
    u64 bytes = CALL(error)->print(NULL, 0, "%s", text);
    char* buffer = (char*)malloc(bytes);
    CALL(error)->print(buffer, bytes, "%s", text);
    RX_ASSERT(strcmp(buffer, text) == 0);
    free(buffer);
}

/* test init */
RX_TEST_CASE(tests_error, test_measure_unknown_format, .fixture = test_fixture) {
    const char* text = "hello, world!";
    const char* format = "%016llx";
    u64 written = CALL(error)->print(NULL, 0, format, text);
    RX_ASSERT(written == strlen(format) + 1);
}

/* test init */
RX_TEST_CASE(tests_error, test_print_unknown_format, .fixture = test_fixture) {
    const char* text = "hello, world!";
    const char* format = "%016llx";
    u64 bytes = CALL(error)->print(NULL, 0, format, text);
    char* buffer = (char*)malloc(bytes);
    CALL(error)->print(buffer, bytes, format, text);
    RX_ASSERT(strcmp(buffer, format) == 0);
    free(buffer);
}

/* test init */
RX_TEST_CASE(tests_error, test_measure_unknown_format_string, .fixture = test_fixture) {
    const char* text = "hello, world!";
    const char* format = "%016llx %s";
    u64 written = CALL(error)->print(NULL, 0, format, text);
    RX_ASSERT(written == strlen("%016llx hello, world!") + 1);
}

/* test init */
RX_TEST_CASE(tests_error, test_print_unknown_format_string, .fixture = test_fixture) {
    const char* text = "hello, world!";
    const char* format = "%016llx %s";
    u64 bytes = CALL(error)->print(NULL, 0, format, text);
    char* buffer = (char*)malloc(bytes);
    CALL(error)->print(buffer, bytes, "%s", text);
    RX_ASSERT(strcmp(buffer, text) == 0);
    free(buffer);
}

/* test init */
RX_TEST_CASE(tests_error, test_print_unknown_format_overflow_string, .fixture = test_fixture) {
    const char* text = "hello, world!";
    const char* format = "%016llx %s";
    u64 bytes = CALL(error)->print(NULL, 0, format, text);
    char* buffer = (char*)malloc(bytes);
    CALL(error)->print(buffer, 2, "%s", text);
    RX_ASSERT(strcmp(buffer, "h") == 0);
    free(buffer);
}

/* test init */
RX_TEST_CASE(tests_error, test_print_unknown_format_overflow_string_empty, .fixture = test_fixture) {
    const char* text = "\0";
    const char* format = "%016llx %s";
    u64 bytes = CALL(error)->print(NULL, 0, format, text);
    char* buffer = (char*)malloc(bytes);
    CALL(error)->print(buffer, 1, "hello, world!%s", text);
    RX_ASSERT(strcmp(buffer, "\0") == 0);
    free(buffer);
}

/* test init */
RX_TEST_CASE(tests_error, test_print_unknown_format_overflow_number, .fixture = test_fixture) {
    int number = -2147483648;
    const char* format = "   %d";
    u64 bytes = CALL(error)->print(NULL, 0, format, number);
    char* buffer = (char*)malloc(bytes);
    CALL(error)->print(buffer, 14, format, number);
    RX_ASSERT(strcmp(buffer, "   -214748364\0") == 0);
    free(buffer);
}

/* test init */
RX_TEST_CASE(tests_error, test_print_unknown_format_underflow_number, .fixture = test_fixture) {
    int number = -2147483648;
    const char* format = "   %d";
    u64 bytes = CALL(error)->print(NULL, 0, format, number);
    char* buffer = (char*)malloc(bytes);
    CALL(error)->print(buffer, 24, format, number);
    RX_ASSERT(strcmp(buffer, "   -2147483648\0") == 0);
    free(buffer);
}

/* test init */
RX_TEST_CASE(tests_error, test_print_unknown_format_overflow_number_empty, .fixture = test_fixture) {
    int number = -2147483648;
    const char* format = "%016llx %d";
    u64 bytes = CALL(error)->print(NULL, 0, format, number);
    char* buffer = (char*)malloc(bytes);
    CALL(error)->print(buffer, 1, "hello, world!%d", number);
    RX_ASSERT(strcmp(buffer, "\0") == 0);
    free(buffer);
}

/* test init */
RX_TEST_CASE(tests_error, test_measure_unknown_format_number, .fixture = test_fixture) {
    int number = -2147483648;
    const char* format = "%016llx %d";
    u64 written = CALL(error)->print(NULL, 0, format, number);
    RX_ASSERT(written == strlen("%016llx -2147483648") + 1);
}

/* test init */
RX_TEST_CASE(tests_error, test_print_string_text_empty, .fixture = test_fixture) {
    const char* text = "\0";
    u64 bytes = CALL(error)->print(NULL, 0, "hello, world!%s", text);
    char* buffer = (char*)malloc(bytes);
    CALL(error)->print(buffer, bytes, "hello, world!%s", text);
    RX_ASSERT(strcmp(buffer, "hello, world!") == 0);
    free(buffer);
}

/* test init */
RX_TEST_CASE(tests_error, test_print_string_text, .fixture = test_fixture) {
    const char* text = "world!";
    u64 bytes = CALL(error)->print(NULL, 0, "hello, %s", text);
    char* buffer = (char*)malloc(bytes);
    CALL(error)->print(buffer, bytes, "hello, %s", text);
    RX_ASSERT(strcmp(buffer, "hello, world!") == 0);
    free(buffer);
}

static void run(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- rexo unit test code %s\n", __FILE__);
#endif
}

const tests_error_test_suite PRIVATE_API(tests_error_test_suite_definitions) = {
    .run = run
};
