/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   February 25, 2025 at 2:21:34 PM GMT+3
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

#define USING_TESTS

#include "test_pointer.h"

#include "vm/os/os_v1.h"
#include "vm/pointer/pointer_v1.h"
#include "vm/types/data/data_v1.h"
#include "vm/types/file/file_v1.h"
#include "vm/types/object/object_v1.h"
#include "vm/types/stack/stack_v1.h"
#include "vm/types/string/string_v1.h"
#include "vm/types/string_pointer/string_pointer_v1.h"
#include "vm/types/user/user_v1.h"

#include "test.h"

#define DEFAULT_SIZE 0x100

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    void* ptr;
}* TEST_DATA;

RX_SET_UP(test_set_up) {
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    // nothing to cleanup
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

/* test init */
RX_TEST_CASE(tests_v1, test_print_0, .fixture = test_fixture) {
    CALL(os)->putc(0);
}

/* test init */
RX_TEST_CASE(tests_v1, test_load_0, .fixture = test_fixture) {
    u64 empty_ptr = CALL(virtual_string)->load("\0");
    RX_ASSERT(empty_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_v1, test_load_null_ptr, .fixture = test_fixture) {
    u64 null_ptr = 0;
#ifndef USE_GC
    CALL(pointer)->free(null_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_v1, test_load_null, .fixture = test_fixture) {
    u64 empty_ptr = CALL(virtual_string)->load(0);
    RX_ASSERT(empty_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_v1, test_load_empty, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 empty_ptr = CALL(virtual_string)->load("");
    RX_ASSERT(empty_ptr == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(empty_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_print_load_empty, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 empty_ptr = CALL(virtual_string)->load("");
    CALL(os)->putc(empty_ptr);
    RX_ASSERT(empty_ptr == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(empty_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_load_copy, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 char_ptr = CALL(virtual_string)->load("/");
    u64 copy_ptr = CALL(virtual_string)->copy(char_ptr);
    RX_ASSERT(char_ptr != 0);
    RX_ASSERT(copy_ptr != 0);
    RX_ASSERT(char_ptr != copy_ptr);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(char_ptr), "/") == 0);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(copy_ptr), "/") == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(char_ptr);
    CALL(virtual_string)->free(copy_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_load_push_peek_pop, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 char_ptr = CALL(virtual_string)->load("/");
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(char_ptr), "/") == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(char_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

RX_TEST_CASE(tests_v1, test_load_free_free, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 char_ptr = CALL(virtual_string)->load("/");
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(char_ptr), "/") == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(char_ptr);
    CALL(virtual_string)->free(char_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_strcat_load_alloc, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 char_ptr = CALL(virtual_string)->load("/");
    u64 pattern_ptr = CALL(virtual_string)->load("\0");
    CALL(virtual_string)->strcat(pattern_ptr, char_ptr);
    RX_ASSERT(pattern_ptr == 0);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(char_ptr), "/") == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(char_ptr);
    CALL(virtual_string)->free(pattern_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_size, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 char_ptr = CALL(virtual_string)->load("/");
    u64 pattern_ptr = CALL(virtual_string)->load("\0");
    u64 string_size = CALL(virtual_data)->size(pattern_ptr);
    RX_ASSERT(string_size == 0);
    RX_ASSERT(pattern_ptr == 0);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(char_ptr), "/") == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(char_ptr);
    CALL(virtual_string)->free(pattern_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_data_size, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 data_ptr = CALL(virtual_data)->alloc(2);
    u64 data_size = CALL(virtual_data)->size(data_ptr);
    RX_ASSERT(data_size == 2);
#ifndef USE_GC
    CALL(virtual_data)->free(data_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_pointer_size_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 char_ptr = CALL(virtual_string)->load("/");
    u64 pattern_ptr = CALL(virtual_string)->load("\0");
    u64 string_size = CALL(virtual_string)->size(0);
    RX_ASSERT(string_size == 0);
    RX_ASSERT(pattern_ptr == 0);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(char_ptr), "/") == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(char_ptr);
    CALL(virtual_string)->free(pattern_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_pointer_size_empty, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 char_ptr = CALL(virtual_string)->load(" ");
    u64 pattern_ptr = CALL(virtual_string)->load("\0");
    u64 string_size = CALL(virtual_string)->size(char_ptr);
    RX_ASSERT(string_size == 1);
    RX_ASSERT(pattern_ptr == 0);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(char_ptr), " ") == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(char_ptr);
    CALL(virtual_string)->free(pattern_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_pointer_size_object, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 object_ptr = CALL(virtual_object)->alloc(1);
    u64 pattern_ptr = CALL(virtual_string)->load("\0");
    u64 object_size = CALL(virtual_object)->size(object_ptr);
    RX_ASSERT(object_size == 1);
    RX_ASSERT(object_ptr != 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(object_ptr);
    CALL(virtual_string)->free(pattern_ptr);
    CALL(virtual_object)->free(object_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_object_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 object_ptr = CALL(virtual_object)->alloc(0);
    u64 pattern_ptr = CALL(virtual_string)->load("\0");
    u64 object_size = CALL(virtual_object)->size(object_ptr);
    RX_ASSERT(object_size == 0);
    RX_ASSERT(object_ptr == 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(object_ptr);
    CALL(virtual_string)->free(pattern_ptr);
    CALL(virtual_object)->free(object_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_object_alloc_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 object_ptr = CALL(virtual_object)->alloc(0);
    u64 pattern_ptr = CALL(virtual_string)->load("\0");
    u64 object_size = CALL(virtual_object)->size(object_ptr);
    RX_ASSERT(object_size == 0);
    RX_ASSERT(object_ptr == 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(object_ptr);
    CALL(virtual_string)->free(pattern_ptr);
    CALL(virtual_object)->free(object_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_object_load_free, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    const char* test_data = "Hello, world!";
    const void* ptr = (const void*)test_data;
    u64 object_ptr = CALL(virtual_object)->load(ptr, strlen(ptr));
    CALL(virtual_object)->free(object_ptr);
#ifndef USE_GC
    CALL(virtual_string)->free(object_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_object_free_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 object_ptr = 0;
    CALL(virtual_object)->free(object_ptr);
#ifndef USE_GC
    CALL(virtual_string)->free(object_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_object_load_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 object_ptr = CALL(virtual_object)->load(0, 0x01234567);
    u64 pattern_ptr = CALL(virtual_string)->load("\0");
    u64 object_size = CALL(virtual_object)->size(object_ptr);
    RX_ASSERT(object_size == 0);
    RX_ASSERT(object_ptr == 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(object_ptr);
    CALL(virtual_string)->free(pattern_ptr);
    CALL(virtual_object)->free(object_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_object_load_string_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    const char* test_data = "Hello, world!";
    const void* ptr = (const void*)test_data;
    u64 object_ptr = CALL(virtual_object)->load(ptr, 0);
    u64 pattern_ptr = CALL(virtual_string)->load("\0");
    u64 object_size = CALL(virtual_object)->size(object_ptr);
    RX_ASSERT(object_size == 0);
    RX_ASSERT(object_ptr == 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(object_ptr);
    CALL(virtual_string)->free(pattern_ptr);
    CALL(virtual_object)->free(object_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_object_load_string, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    const char* test_data = "Hello, world!";
    const void* ptr = (const void*)test_data;
    u64 object_ptr = CALL(virtual_object)->load(ptr, strlen(ptr));
    u64 pattern_ptr = CALL(virtual_string)->load("\0");
    u64 object_size = CALL(virtual_object)->size(object_ptr);
    RX_ASSERT(object_size == strlen(ptr));
    RX_ASSERT(object_ptr != 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(object_ptr);
    CALL(virtual_string)->free(pattern_ptr);
    CALL(virtual_object)->free(object_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_object_load_string_unsafe_alloc_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 object_ptr = CALL(virtual_object)->alloc(0);
    u64 pattern_ptr = CALL(virtual_string)->load("\0");
    u64 object_size = CALL(virtual_object)->size(object_ptr);
    const char* ch = (char*)CALL(virtual_object)->unsafe(object_ptr);
    RX_ASSERT(ch == 0);
    RX_ASSERT(object_size == 0);
    RX_ASSERT(object_ptr == 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(object_ptr);
    CALL(virtual_string)->free(pattern_ptr);
    CALL(virtual_object)->free(object_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_object_load_string_unsafe_string, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 object_ptr = CALL(virtual_object)->alloc(0);
    u64 pattern_ptr = CALL(virtual_string)->load("\0");
    u64 object_size = CALL(virtual_object)->size(object_ptr);
    const char* ch = (char*)CALL(virtual_object)->unsafe(pattern_ptr);
    RX_ASSERT(ch == 0);
    RX_ASSERT(object_size == 0);
    RX_ASSERT(object_ptr == 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(object_ptr);
    CALL(virtual_string)->free(pattern_ptr);
    CALL(virtual_object)->free(object_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_object_load_string_unsafe_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    const char* test_data = "Hello, world!";
    const void* ptr = (const void*)test_data;
    u64 object_ptr = CALL(virtual_object)->load(ptr, strlen(ptr));
    u64 pattern_ptr = CALL(virtual_string)->load("\0");
    u64 object_size = CALL(virtual_object)->size(object_ptr);
    const char* ch = (char*)CALL(virtual_object)->unsafe(0);
    RX_ASSERT(ch == 0);
    RX_ASSERT(object_size == strlen(ptr));
    RX_ASSERT(object_ptr != 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(object_ptr);
    CALL(virtual_string)->free(pattern_ptr);
    CALL(virtual_object)->free(object_ptr);
#endif
    CALL(pointer)->gc();
    CALL(virtual_object)->free(object_ptr);
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_object_load_string_unsafe, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    const char* test_data = "Hello, world!";
    const void* ptr = (const void*)test_data;
    u64 object_ptr = CALL(virtual_object)->load(ptr, strlen(ptr));
    u64 pattern_ptr = CALL(virtual_string)->load("\0");
    u64 object_size = CALL(virtual_object)->size(object_ptr);
    char* ch = (char*)CALL(virtual_object)->unsafe(object_ptr);
    *ch = 'Z';
    RX_ASSERT(object_size == strlen(ptr));
    RX_ASSERT(object_ptr != 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(object_ptr);
    CALL(virtual_string)->free(pattern_ptr);
    CALL(virtual_object)->free(object_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_strcat_alloc_alloc, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 char_ptr = CALL(virtual_string)->load("\0");
    u64 pattern_ptr = CALL(virtual_string)->load("\0");
    CALL(virtual_string)->strcat(pattern_ptr, char_ptr);
    RX_ASSERT(char_ptr == 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(char_ptr);
    CALL(virtual_string)->free(pattern_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_offset_ptr_diff, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 string_ptr1 = CALL(virtual_string)->load("192.168.0.111");
    u64 string_ptr2 = CALL(virtual_string)->load("192.168.0.12");
    u64 last_matched_ptr1 = CALL(virtual_string)->offset(string_ptr1, string_ptr2);
    RX_ASSERT(last_matched_ptr1 == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(last_matched_ptr1);
    CALL(virtual_string)->free(string_ptr1);
    CALL(virtual_string)->free(string_ptr2);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_offset_ptr_less, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 string_ptr1 = CALL(virtual_string)->load("192.168.0.11");
    u64 string_ptr2 = CALL(virtual_string)->load("8.0.112");
    u64 last_matched_ptr1 = CALL(virtual_string)->offset(string_ptr1, string_ptr2);
    RX_ASSERT(last_matched_ptr1 == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(last_matched_ptr1);
    CALL(virtual_string)->free(string_ptr1);
    CALL(virtual_string)->free(string_ptr2);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_offset_ptr_offset, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 string_ptr1 = CALL(virtual_string)->load("192.168.0.11");
    u64 string_ptr2 = CALL(virtual_string)->load("8.0.12");
    u64 last_matched_ptr1 = CALL(virtual_string)->offset(string_ptr1, string_ptr2);
    RX_ASSERT(last_matched_ptr1 == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(last_matched_ptr1);
    CALL(virtual_string)->free(string_ptr1);
    CALL(virtual_string)->free(string_ptr2);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_offset_ptr_offset_found, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 string_ptr1 = CALL(virtual_string)->load("192.168.0.111");
    u64 string_ptr2 = CALL(virtual_string)->load("8.0.");
    u64 last_matched_ptr1 = CALL(virtual_string)->offset(string_ptr1, string_ptr2);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(last_matched_ptr1), "111") == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(last_matched_ptr1);
    CALL(virtual_string)->free(string_ptr1);
    CALL(virtual_string)->free(string_ptr2);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_free_list, .fixture = test_fixture) {
    CALL(pointer)->init(8);
#ifndef USE_GC
    u64 list_ptr = CALL(virtual_stack)->alloc();
    u64 string_ptr = CALL(virtual_string)->load("hello");
    u64 e_ptr = CALL(virtual_string)->load("e");
    u64 string_pointer_ptr = CALL(virtual_string)->offset(string_ptr, e_ptr);
    CALL(virtual_string)->free(list_ptr);
    CALL(virtual_string)->free(string_pointer_ptr);
    CALL(virtual_string)->free(string_ptr);
    CALL(virtual_string)->free(e_ptr);
    CALL(virtual_stack)->free(list_ptr);
#else
    CALL(virtual_stack)->alloc();
    u64 string_ptr = CALL(virtual_string)->load("hello");
    u64 e_ptr = CALL(virtual_string)->load("e");
    CALL(virtual_string)->offset(string_ptr, e_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_free, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr = CALL(virtual_stack)->alloc();
    u64 string_ptr = CALL(virtual_string)->load("hello");
    u64 e_ptr = CALL(virtual_string)->load("e");
    u64 string_pointer_ptr = CALL(virtual_string)->offset(string_ptr, e_ptr);
    CALL(virtual_stack)->push(list_ptr, string_pointer_ptr);
    CALL(virtual_stack)->push(list_ptr, string_ptr);
#ifndef USE_GC
    CALL(virtual_string)->free(e_ptr);
    CALL(virtual_stack)->free(list_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_offset_list, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr = CALL(virtual_stack)->alloc();
    u64 string_ptr = CALL(virtual_string)->load("hello");
    u64 last_matched_ptr = CALL(virtual_string)->offset(list_ptr, string_ptr);
    RX_ASSERT(last_matched_ptr == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(string_ptr);
    CALL(virtual_stack)->free(list_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_match, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr = CALL(virtual_stack)->alloc();
    u64 string_ptr = CALL(virtual_string)->load("192.192.0.1");
    u64 dot_ptr = CALL(virtual_string)->load("192.");
    u64 string_pointer_ptr1 = CALL(virtual_string)->match(string_ptr, dot_ptr);
    u64 string_pointer_ptr2 = CALL(virtual_string)->match(string_pointer_ptr1, dot_ptr);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(string_pointer_ptr1), "192.0.1") == 0);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(string_pointer_ptr2), "0.1") == 0);
    CALL(virtual_stack)->push(list_ptr, string_pointer_ptr1);
    CALL(virtual_stack)->push(list_ptr, string_pointer_ptr2);
    CALL(virtual_stack)->push(list_ptr, string_ptr);
#ifndef USE_GC
    CALL(virtual_string)->free(dot_ptr);
    CALL(virtual_stack)->free(list_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_match_miss, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr = CALL(virtual_stack)->alloc();
    u64 string_ptr = CALL(virtual_string)->load("192.168.0.1");
    u64 dot_ptr = CALL(virtual_string)->load(":");
    u64 string_pointer_ptr1 = CALL(virtual_string)->match(string_ptr, dot_ptr);
    RX_ASSERT(string_pointer_ptr1 == 0);
    CALL(virtual_stack)->push(list_ptr, string_pointer_ptr1);
    CALL(virtual_stack)->push(list_ptr, string_ptr);
#ifndef USE_GC
    CALL(virtual_string)->free(dot_ptr);
    CALL(virtual_stack)->free(list_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_match_pattern, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr = CALL(virtual_stack)->alloc();
    u64 string_ptr = CALL(virtual_string)->load("192.168.0.112");
    u64 dot_ptr = CALL(virtual_string)->load("192.168.0.");
    u64 string_pointer_ptr1 = CALL(virtual_string)->match(string_ptr, dot_ptr);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(string_pointer_ptr1), "112") == 0);
    CALL(virtual_stack)->push(list_ptr, string_pointer_ptr1);
    CALL(virtual_stack)->push(list_ptr, string_ptr);
#ifndef USE_GC
    CALL(virtual_string)->free(dot_ptr);
    CALL(virtual_stack)->free(list_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_match_pattern_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr = CALL(virtual_stack)->alloc();
    u64 string_ptr = CALL(virtual_string)->load("192.168.0.112");
    u64 dot_ptr = CALL(virtual_string)->load("193");
    u64 string_pointer_ptr1 = CALL(virtual_string)->match(string_ptr, dot_ptr);
    RX_ASSERT(string_pointer_ptr1 == 0);
    CALL(virtual_stack)->push(list_ptr, string_pointer_ptr1);
    CALL(virtual_stack)->push(list_ptr, string_ptr);
#ifndef USE_GC
    CALL(virtual_string)->free(dot_ptr);
    CALL(virtual_stack)->free(list_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_match_pattern_none, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr = CALL(virtual_stack)->alloc();
    u64 string_ptr = CALL(virtual_string)->load("192.168.0.11");
    u64 dot_ptr = CALL(virtual_string)->load("192.168.0.111");
    u64 string_pointer_ptr1 = CALL(virtual_string)->match(string_ptr, dot_ptr);
    RX_ASSERT(string_pointer_ptr1 == 0);
    CALL(virtual_stack)->push(list_ptr, string_pointer_ptr1);
    CALL(virtual_stack)->push(list_ptr, string_ptr);
#ifndef USE_GC
    CALL(virtual_string)->free(dot_ptr);
    CALL(virtual_stack)->free(list_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_release_string, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 string_ptr = CALL(virtual_string)->load("192.168.0.11");
    CALL(virtual_stack)->release(string_ptr);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_free_string, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 string_ptr = CALL(virtual_string)->load("192.168.0.11");
    CALL(virtual_stack)->free(string_ptr);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_match_pattern_load_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 string_ptr = CALL(virtual_string)->load("192.168.0.1");
    u64 dot_ptr = CALL(virtual_string)->load("57.34.7.2");
    u64 error_ptr = CALL(virtual_string)->match(string_ptr, dot_ptr);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_offset_pattern_load_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 string_ptr = CALL(virtual_string)->load("192.168.0.1");
    u64 dot_ptr = CALL(virtual_string)->load("57.34.7.2");
    u64 error_ptr = CALL(virtual_string)->offset(string_ptr, dot_ptr);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_match_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 string_ptr = CALL(virtual_string)->load("192.168.0.1");
    u64 string_pointer_ptr = CALL(virtual_string)->match(string_ptr, 0);
    RX_ASSERT(string_pointer_ptr == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(string_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_match_0_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
#ifndef USE_GC
    u64 string_ptr = CALL(virtual_string)->load("192.168.0.1");
    u64 string_pointer_ptr = CALL(virtual_string)->match(0, 0);
    RX_ASSERT(string_pointer_ptr == 0);
    CALL(virtual_string)->free(string_ptr);
#else
    CALL(virtual_string)->load("192.168.0.1");
    u64 string_pointer_ptr = CALL(virtual_string)->match(0, 0);
    RX_ASSERT(string_pointer_ptr == 0);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_match_0_string, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 string_ptr = CALL(virtual_string)->load("192.168.0.1");
    u64 string_pointer_ptr = CALL(virtual_string)->match(0, string_ptr);
    RX_ASSERT(string_pointer_ptr == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(string_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_match_list, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr = CALL(virtual_stack)->alloc();
    u64 string_ptr = CALL(virtual_string)->load("hello");
    u64 last_matched_ptr = CALL(virtual_string)->match(list_ptr, string_ptr);
    RX_ASSERT(last_matched_ptr == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(string_ptr);
    CALL(virtual_stack)->free(list_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_strchr, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr = CALL(virtual_stack)->alloc();
    u64 string_ptr = CALL(virtual_string)->load("a.b.c/2");
    u64 dot_ptr = CALL(virtual_string)->load(".");
    u64 string_pointer_ptr1 = CALL(virtual_string)->strchr(string_ptr, dot_ptr);
    u64 string_pointer_ptr2 = CALL(virtual_string)->match(string_pointer_ptr1, dot_ptr);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(string_pointer_ptr1), ".b.c/2") == 0);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(string_pointer_ptr2), "b.c/2") == 0);
    CALL(virtual_stack)->push(list_ptr, string_pointer_ptr1);
    CALL(virtual_stack)->push(list_ptr, string_pointer_ptr2);
    CALL(virtual_stack)->push(list_ptr, string_ptr);
#ifndef USE_GC
    CALL(virtual_string)->free(dot_ptr);
    CALL(virtual_stack)->free(list_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_strchr_miss, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr = CALL(virtual_stack)->alloc();
    u64 string_ptr = CALL(virtual_string)->load("192.168.0.1");
    u64 dot_ptr = CALL(virtual_string)->load(":");
    u64 string_pointer_ptr1 = CALL(virtual_string)->strchr(string_ptr, dot_ptr);
    RX_ASSERT(string_pointer_ptr1 == 0);
    CALL(virtual_stack)->push(list_ptr, string_pointer_ptr1);
    CALL(virtual_stack)->push(list_ptr, string_ptr);
#ifndef USE_GC
    CALL(virtual_string)->free(dot_ptr);
    CALL(virtual_stack)->free(list_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_strchr_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 string_ptr = CALL(virtual_string)->load("192.168.0.1");
    u64 string_pointer_ptr = CALL(virtual_string)->strchr(string_ptr, 0);
    RX_ASSERT(string_pointer_ptr == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(string_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_strchr_0_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
#ifndef USE_GC
    u64 string_ptr = CALL(virtual_string)->load("192.168.0.1");
    u64 string_pointer_ptr = CALL(virtual_string)->strchr(0, 0);
    RX_ASSERT(string_pointer_ptr == 0);
    CALL(virtual_string)->free(string_ptr);
#else
    CALL(virtual_string)->load("192.168.0.1");
    u64 string_pointer_ptr = CALL(virtual_string)->strchr(0, 0);
    RX_ASSERT(string_pointer_ptr == 0);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_strchr_0_string, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 string_ptr = CALL(virtual_string)->load("192.168.0.1");
    u64 string_pointer_ptr = CALL(virtual_string)->strchr(0, string_ptr);
    RX_ASSERT(string_pointer_ptr == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(string_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_strchr_list, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr = CALL(virtual_stack)->alloc();
    u64 string_ptr = CALL(virtual_string)->load("hello");
    u64 last_matched_ptr = CALL(virtual_string)->strchr(list_ptr, string_ptr);
    RX_ASSERT(last_matched_ptr == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(string_ptr);
    CALL(virtual_stack)->free(list_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_strrchr, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 string_ptr = CALL(virtual_string)->load("192.168.0.1");
    u64 dot_ptr = CALL(virtual_string)->load(".");
    u64 string_pointer_ptr1 = CALL(virtual_string)->strrchr(string_ptr, dot_ptr);
    u64 string_pointer_ptr2 = CALL(virtual_string)->match(string_pointer_ptr1, dot_ptr);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(string_pointer_ptr1), ".1") == 0);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(string_pointer_ptr2), "1") == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(string_pointer_ptr1);
    CALL(virtual_string)->free(string_pointer_ptr2);
    CALL(virtual_string)->free(string_ptr);
    CALL(virtual_string)->free(dot_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_strrchr_match_offset, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 string_ptr = CALL(virtual_string)->load("192.168.0.111");
    u64 dot_ptr = CALL(virtual_string)->load(".");
    u64 string_pointer_ptr1 = CALL(virtual_string)->strrchr(string_ptr, dot_ptr);
    u64 string_pointer_ptr2 = CALL(virtual_string)->match(string_pointer_ptr1, dot_ptr);
    u64 string_pointer_ptr3 = CALL(virtual_string)->offset(string_ptr, dot_ptr);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(string_pointer_ptr1), ".111") == 0);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(string_pointer_ptr2), "111") == 0);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(string_pointer_ptr3), "111") == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(string_pointer_ptr1);
    CALL(virtual_string)->free(string_pointer_ptr2);
    CALL(virtual_string)->free(string_pointer_ptr3);
    CALL(virtual_string)->free(string_ptr);
    CALL(virtual_string)->free(dot_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_peekn_0_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr = CALL(virtual_stack)->alloc();
    ASSERT_DEBUG(list_ptr != 0);
    u64 error_ptr = CALL(virtual_stack)->peekn(0, 0);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_peekn_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr = CALL(virtual_stack)->alloc();
    u64 error_ptr = CALL(virtual_stack)->peekn(list_ptr, 0);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_peekn_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr1 = CALL(virtual_stack)->alloc();
    u64 string_ptr1 = CALL(virtual_string)->load("192.168.0.1");
    CALL(virtual_stack)->push(list_ptr1, string_ptr1);
    u64 list_ptr2 = CALL(virtual_stack)->peekn(list_ptr1, 1);
    RX_ASSERT(list_ptr2 != 0);
    RX_ASSERT(CALL(virtual_stack)->size(list_ptr2) != 0);
    u64 string_ptr2 = CALL(virtual_stack)->peek(list_ptr2);
    RX_ASSERT(string_ptr2 != 0);
    RX_ASSERT(string_ptr1 == string_ptr2);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_peekn_2, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 string_ptr = CALL(virtual_string)->load("192.168.0.1");
    u64 list_ptr = CALL(virtual_stack)->alloc();
    CALL(virtual_stack)->push(list_ptr, string_ptr);
    u64 error_ptr = CALL(virtual_stack)->peekn(list_ptr, 2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_popn_0_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr = CALL(virtual_stack)->alloc();
    ASSERT_DEBUG(list_ptr != 0);
    u64 error_ptr = CALL(virtual_stack)->popn(0, 0);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_popn_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr = CALL(virtual_stack)->alloc();
    u64 error_ptr = CALL(virtual_stack)->popn(list_ptr, 0);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_popn_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr1 = CALL(virtual_stack)->alloc();
    u64 string_ptr1 = CALL(virtual_string)->load("192.168.0.1");
    CALL(virtual_stack)->push(list_ptr1, string_ptr1);
    u64 list_ptr2 = CALL(virtual_stack)->popn(list_ptr1, 1);
    RX_ASSERT(list_ptr2 != 0);
    RX_ASSERT(CALL(virtual_stack)->size(list_ptr2) != 0);
    u64 string_ptr2 = CALL(virtual_stack)->peek(list_ptr2);
    RX_ASSERT(string_ptr2 != 0);
    RX_ASSERT(string_ptr1 == string_ptr2);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_popn_2, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 string_ptr = CALL(virtual_string)->load("192.168.0.1");
    u64 list_ptr = CALL(virtual_stack)->alloc();
    CALL(virtual_stack)->push(list_ptr, string_ptr);
    u64 error_ptr = CALL(virtual_stack)->popn(list_ptr, 2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_strchr_match_offset, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr = CALL(virtual_stack)->alloc();
    u64 list_match_ptr = CALL(virtual_stack)->alloc();
    u64 string_ptr = CALL(virtual_string)->load("a.bc.bb.ba.a");
    u64 pattern_ptr = CALL(virtual_string)->load(".b");
    u64 string_pointer_ptr = 0;
    u64 match_ptr = string_ptr;
    u64 current_ptr = match_ptr;
    while ((string_pointer_ptr = CALL(virtual_string)->strchr(current_ptr, pattern_ptr)) != 0) {
        CALL(virtual_stack)->push(list_ptr, string_pointer_ptr);
        match_ptr = CALL(virtual_string)->match(string_pointer_ptr, pattern_ptr);
        CALL(virtual_stack)->push(list_match_ptr, match_ptr);
        current_ptr = match_ptr;
    }
    u64 size = CALL(virtual_stack)->size(list_ptr);
    RX_ASSERT(size == 4);
    u64 string_ptr1 = CALL(virtual_stack)->pop(list_ptr);
    u64 string_ptr2 = CALL(virtual_stack)->pop(list_ptr);
    u64 string_ptr3 = CALL(virtual_stack)->pop(list_ptr);
    u64 string_ptr4 = CALL(virtual_stack)->pop(list_ptr);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(string_ptr1), ".a") == 0);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(string_ptr2), ".ba.a") == 0);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(string_ptr3), ".bb.ba.a") == 0);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(string_ptr4), ".bc.bb.ba.a") == 0);
    RX_ASSERT(CALL(virtual_stack)->size(list_ptr) == 0);
    u64 match_size = CALL(virtual_stack)->size(list_match_ptr);
    RX_ASSERT(match_size == 3);
    u64 string_match_ptr1 = CALL(virtual_stack)->pop(list_match_ptr);
    u64 string_match_ptr2 = CALL(virtual_stack)->pop(list_match_ptr);
    u64 string_match_ptr3 = CALL(virtual_stack)->pop(list_match_ptr);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(string_match_ptr1), "a.a") == 0);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(string_match_ptr2), "b.ba.a") == 0);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(string_match_ptr3), "c.bb.ba.a") == 0);
    RX_ASSERT(CALL(virtual_stack)->size(list_match_ptr) == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(string_ptr1);
    CALL(virtual_string)->free(string_ptr2);
    CALL(virtual_string)->free(string_ptr3);
    CALL(virtual_string)->free(string_ptr4);
    CALL(virtual_string)->free(string_match_ptr1);
    CALL(virtual_string)->free(string_match_ptr2);
    CALL(virtual_string)->free(string_match_ptr3);
    CALL(virtual_string)->free(string_ptr);
    CALL(virtual_string)->free(pattern_ptr);
    CALL(virtual_stack)->free(list_match_ptr);
    CALL(virtual_stack)->free(list_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_strrchr_miss, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr = CALL(virtual_stack)->alloc();
    u64 string_ptr = CALL(virtual_string)->load("192.168.0.1");
    u64 dot_ptr = CALL(virtual_string)->load(":");
    u64 string_pointer_ptr1 = CALL(virtual_string)->strrchr(string_ptr, dot_ptr);
    RX_ASSERT(string_pointer_ptr1 == 0);
    CALL(virtual_stack)->push(list_ptr, string_pointer_ptr1);
    CALL(virtual_stack)->push(list_ptr, string_ptr);
#ifndef USE_GC
    CALL(virtual_string)->free(dot_ptr);
    CALL(virtual_stack)->free(list_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_strrchr_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 string_ptr = CALL(virtual_string)->load("192.168.0.1");
    u64 string_pointer_ptr = CALL(virtual_string)->strrchr(string_ptr, 0);
    RX_ASSERT(string_pointer_ptr == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(string_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_strrchr_0_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
#ifndef USE_GC
    u64 string_ptr = CALL(virtual_string)->load("192.168.0.1");
    u64 string_pointer_ptr = CALL(virtual_string)->strrchr(0, 0);
    RX_ASSERT(string_pointer_ptr == 0);
    CALL(virtual_string)->free(string_ptr);
#else
    CALL(virtual_string)->load("192.168.0.1");
    u64 string_pointer_ptr = CALL(virtual_string)->strrchr(0, 0);
    RX_ASSERT(string_pointer_ptr == 0);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_strrchr_0_string, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 string_ptr = CALL(virtual_string)->load("192.168.0.1");
    u64 string_pointer_ptr = CALL(virtual_string)->strrchr(0, string_ptr);
    RX_ASSERT(string_pointer_ptr == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(string_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_strrchr_list, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr = CALL(virtual_stack)->alloc();
    u64 string_ptr = CALL(virtual_string)->load("hello");
    u64 last_matched_ptr = CALL(virtual_string)->strrchr(list_ptr, string_ptr);
    RX_ASSERT(last_matched_ptr == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(string_ptr);
    CALL(virtual_stack)->free(list_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_push_0_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    CALL(virtual_stack)->push(0, 1);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_push_1_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    CALL(virtual_stack)->push(1, 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_push_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr = CALL(virtual_stack)->alloc();
    CALL(virtual_stack)->push(list_ptr, 0);
#ifndef USE_GC
    CALL(virtual_stack)->free(list_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_push_list, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr = CALL(virtual_stack)->alloc();
    CALL(virtual_stack)->push(list_ptr, list_ptr);
#ifndef USE_GC
    CALL(virtual_stack)->free(list_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_push_string, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 string_ptr = CALL(virtual_string)->load("@");
    u64 list_ptr = CALL(virtual_stack)->alloc();
    CALL(virtual_stack)->push(string_ptr, list_ptr);
#ifndef USE_GC
    CALL(virtual_stack)->free(list_ptr);
    CALL(virtual_string)->free(string_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_size_string, .fixture = test_fixture) {
    CALL(pointer)->init(8);
#ifndef USE_GC
    u64 string_ptr = CALL(virtual_string)->load("@");
    u64 list_ptr = CALL(virtual_stack)->alloc();
    u64 value = CALL(virtual_stack)->size(string_ptr);
    RX_ASSERT(value == 0);
    CALL(virtual_stack)->free(list_ptr);
    CALL(virtual_string)->free(string_ptr);
#else
    u64 string_ptr = CALL(virtual_string)->load("@");
    CALL(virtual_stack)->alloc();
    u64 value = CALL(virtual_stack)->size(string_ptr);
    RX_ASSERT(value == 0);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_push_size, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 string_ptr = CALL(virtual_string)->load("@");
    u64 list_ptr = CALL(virtual_stack)->alloc();
    CALL(virtual_stack)->push(list_ptr, string_ptr);
    u64 value = CALL(virtual_stack)->size(list_ptr);
    RX_ASSERT(value != 0);
#ifndef USE_GC
    CALL(virtual_stack)->free(list_ptr);
    CALL(virtual_string)->free(string_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_pop_0_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 value_ptr = CALL(virtual_stack)->pop(0);
    RX_ASSERT(value_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_pop_1_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 value_ptr = CALL(virtual_stack)->pop(1);
    RX_ASSERT(value_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_pop_list, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr = CALL(virtual_stack)->alloc();
    u64 value_ptr = CALL(virtual_stack)->pop(list_ptr);
    RX_ASSERT(value_ptr == 0);
#ifndef USE_GC
    CALL(virtual_stack)->free(list_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_pop_string, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 string_ptr = CALL(virtual_string)->load("@");
    u64 list_ptr = CALL(virtual_stack)->alloc();
    CALL(virtual_stack)->push(list_ptr, string_ptr);
    u64 value_ptr = CALL(virtual_stack)->pop(list_ptr);
    RX_ASSERT(value_ptr != 0);
#ifndef USE_GC
    CALL(virtual_stack)->free(list_ptr);
    CALL(virtual_string)->free(string_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_peek_0_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 value_ptr = CALL(virtual_stack)->peek(0);
    RX_ASSERT(value_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_peek_1_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 value_ptr = CALL(virtual_stack)->peek(1);
    RX_ASSERT(value_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_peek_list, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr = CALL(virtual_stack)->alloc();
    u64 value_ptr = CALL(virtual_stack)->peek(list_ptr);
    RX_ASSERT(value_ptr == 0);
#ifndef USE_GC
    CALL(virtual_stack)->free(list_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_peek_string, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 string_ptr = CALL(virtual_string)->load("@");
    u64 list_ptr = CALL(virtual_stack)->alloc();
    CALL(virtual_stack)->push(list_ptr, string_ptr);
    u64 value_ptr = CALL(virtual_stack)->peek(list_ptr);
    RX_ASSERT(value_ptr != 0);
#ifndef USE_GC
    CALL(virtual_stack)->free(list_ptr);
    CALL(virtual_string)->free(string_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_strcpy, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 path_ptr1 = CALL(virtual_string)->load("/");
    u64 path_ptr2 = CALL(virtual_string)->load("@");
    u64 path_copy_ptr = CALL(virtual_string)->copy(path_ptr1);
    CALL(virtual_string)->strcat(path_copy_ptr, path_ptr2);

    char* path1 = CALL(virtual_string)->unsafe(path_ptr1);
    char* path2 = CALL(virtual_string)->unsafe(path_ptr2);
    u64 path1_len = strlen(path1);
    u64 path2_len = strlen(path2);
    RX_ASSERT(path1_len > 0);
    RX_ASSERT(path2_len > 0);
    char* buf = calloc(1, path1_len + path2_len + 1);
#if defined(_WIN32)
    strcpy_s(buf, path1_len + path2_len + 1, path1); /* NOLINT */
    strcat_s(buf, path1_len + path2_len + 1, path2); /* NOLINT */
#else
    strcpy(buf, path1); /* NOLINT */
    strcat(buf, path2); /* NOLINT */
#endif
    char* path_copy = CALL(virtual_string)->unsafe(path_copy_ptr);
    RX_ASSERT(strlen(path_copy) == strlen(buf));
    RX_ASSERT(strcmp(path_copy, buf) == 0);
    free(buf);
#ifndef USE_GC
    CALL(virtual_string)->free(path_ptr1);
    CALL(virtual_string)->free(path_ptr2);
    CALL(virtual_string)->free(path_copy_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_strcat_load_alloc_copy, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 string_ptr = CALL(virtual_string)->load("/all_english_words.txt");
    u64 zero_ptr = CALL(virtual_string)->load("\0");
    u64 data_ptr = CALL(virtual_data)->alloc(1);
    u64 list_ptr = CALL(virtual_stack)->alloc();
    u64 empty_ptr = CALL(virtual_string)->load("\0");

    u64 null_ptr = 0;
    u64 none_ptr = 0xfffffffff;

    u64 data_ptr1 = CALL(virtual_string)->copy(null_ptr);
    u64 data_ptr2 = CALL(virtual_string)->copy(zero_ptr);
    u64 data_ptr3 = CALL(virtual_string)->copy(string_ptr);
    u64 data_ptr4 = CALL(virtual_string)->copy(none_ptr);

    CALL(os)->putc(string_ptr);
    CALL(os)->putc(zero_ptr);
    CALL(os)->putc(data_ptr);
    CALL(os)->putc(list_ptr);
    CALL(os)->putc(empty_ptr);
    CALL(os)->putc(null_ptr);
    CALL(os)->putc(none_ptr);

    CALL(virtual_string)->put_char(string_ptr, 'a');
    CALL(virtual_string)->put_char(zero_ptr, 'a');
    CALL(virtual_string)->put_char(data_ptr, 'a');
    CALL(virtual_string)->put_char(list_ptr, 'a');
    CALL(virtual_string)->put_char(empty_ptr, 'a');
    CALL(virtual_string)->put_char(null_ptr, 'a');
    CALL(virtual_string)->put_char(none_ptr, 'a');

    CALL(virtual_string)->put_char(string_ptr, '\0');
    CALL(virtual_string)->put_char(zero_ptr, '\0');
    CALL(virtual_string)->put_char(data_ptr, '\0');
    CALL(virtual_string)->put_char(list_ptr, '\0');
    CALL(virtual_string)->put_char(empty_ptr, '\0');
    CALL(virtual_string)->put_char(null_ptr, '\0');
    CALL(virtual_string)->put_char(none_ptr, '\0');

    CALL(virtual_string)->put_char(string_ptr, '/');
    CALL(virtual_string)->strcpy(string_ptr, string_ptr);
    CALL(virtual_string)->strcpy(string_ptr, zero_ptr);
    CALL(virtual_string)->strcpy(zero_ptr, string_ptr);
    CALL(virtual_string)->strcpy(data_ptr, null_ptr);
    CALL(virtual_string)->strcpy(list_ptr, null_ptr);
    CALL(virtual_string)->strcpy(string_ptr, data_ptr);
    CALL(virtual_string)->strcpy(string_ptr, empty_ptr);
    CALL(virtual_string)->strcpy(data_ptr, null_ptr);
    CALL(virtual_string)->strcpy(list_ptr, string_ptr);
    CALL(virtual_string)->strcpy(data_ptr, null_ptr);
    CALL(virtual_string)->strcpy(list_ptr, null_ptr);
    CALL(virtual_string)->strcpy(string_ptr, data_ptr);
    CALL(virtual_string)->strcpy(data_ptr, null_ptr);
    CALL(virtual_string)->strcpy(list_ptr, string_ptr);
    CALL(virtual_string)->strcpy(null_ptr, null_ptr);
    CALL(virtual_string)->strcpy(string_ptr, null_ptr);
    CALL(virtual_string)->strcpy(string_ptr, none_ptr);
    CALL(virtual_string)->strcpy(none_ptr, string_ptr);

    CALL(virtual_string)->strcat(string_ptr, string_ptr);
    CALL(virtual_string)->strcat(string_ptr, zero_ptr);
    CALL(virtual_string)->strcat(zero_ptr, string_ptr);
    CALL(virtual_string)->strcat(data_ptr, null_ptr);
    CALL(virtual_string)->strcat(list_ptr, null_ptr);
    CALL(virtual_string)->strcat(string_ptr, data_ptr);
    CALL(virtual_string)->strcat(string_ptr, empty_ptr);
    CALL(virtual_string)->strcat(data_ptr, null_ptr);
    CALL(virtual_string)->strcat(list_ptr, string_ptr);
    CALL(virtual_string)->strcat(data_ptr, null_ptr);
    CALL(virtual_string)->strcat(list_ptr, null_ptr);
    CALL(virtual_string)->strcat(string_ptr, data_ptr);
    CALL(virtual_string)->strcat(data_ptr, null_ptr);
    CALL(virtual_string)->strcat(list_ptr, string_ptr);
    CALL(virtual_string)->strcat(null_ptr, null_ptr);
    CALL(virtual_string)->strcat(string_ptr, null_ptr);
    CALL(virtual_string)->strcat(string_ptr, none_ptr);
    CALL(virtual_string)->strcat(none_ptr, string_ptr);

    u64 last_matched_ptr1 = CALL(virtual_string)->offset(string_ptr, string_ptr);
    CALL(virtual_string)->offset(data_ptr, null_ptr);
    CALL(virtual_string)->offset(list_ptr, null_ptr);
    CALL(virtual_string)->offset(string_ptr, data_ptr);
    CALL(virtual_string)->offset(string_ptr, empty_ptr);
    CALL(virtual_string)->offset(data_ptr, null_ptr);
    CALL(virtual_string)->offset(list_ptr, string_ptr);
    CALL(virtual_string)->offset(data_ptr, null_ptr);
    CALL(virtual_string)->offset(list_ptr, null_ptr);
    CALL(virtual_string)->offset(string_ptr, data_ptr);
    CALL(virtual_string)->offset(data_ptr, null_ptr);
    CALL(virtual_string)->offset(list_ptr, string_ptr);
    CALL(virtual_string)->offset(null_ptr, null_ptr);
    CALL(virtual_string)->offset(string_ptr, null_ptr);

    u64 last_matched_ptr2 = CALL(virtual_string)->offset(string_ptr, string_ptr);
    CALL(virtual_string)->offset(string_ptr, zero_ptr);
    CALL(virtual_string)->offset(data_ptr, zero_ptr);
    CALL(virtual_string)->offset(list_ptr, zero_ptr);
    CALL(virtual_string)->offset(string_ptr, data_ptr);
    CALL(virtual_string)->offset(zero_ptr, zero_ptr);
    CALL(virtual_string)->offset(zero_ptr, data_ptr);
    CALL(virtual_string)->offset(zero_ptr, list_ptr);
    CALL(virtual_string)->offset(zero_ptr, string_ptr);
    CALL(virtual_string)->offset(empty_ptr, empty_ptr);
    CALL(virtual_string)->offset(empty_ptr, string_ptr);
    CALL(virtual_string)->offset(empty_ptr, zero_ptr);

    CALL(virtual_string)->offset(null_ptr, null_ptr);
    CALL(virtual_string)->offset(null_ptr, empty_ptr);
    CALL(virtual_string)->offset(null_ptr, string_ptr);
    CALL(virtual_string)->offset(null_ptr, data_ptr);
    CALL(virtual_string)->offset(null_ptr, list_ptr);
    CALL(virtual_string)->offset(empty_ptr, null_ptr);
    CALL(virtual_string)->offset(empty_ptr, empty_ptr);
    CALL(virtual_string)->offset(empty_ptr, string_ptr);
    CALL(virtual_string)->offset(empty_ptr, data_ptr);
    CALL(virtual_string)->offset(empty_ptr, list_ptr);
    CALL(virtual_string)->offset(string_ptr, null_ptr);
    CALL(virtual_string)->offset(string_ptr, empty_ptr);

    u64 last_matched_ptr3 = CALL(virtual_string)->offset(string_ptr, string_ptr);
    CALL(virtual_string)->offset(string_ptr, data_ptr);
    CALL(virtual_string)->offset(string_ptr, list_ptr);
    CALL(virtual_string)->offset(data_ptr, null_ptr);
    CALL(virtual_string)->offset(data_ptr, empty_ptr);
    CALL(virtual_string)->offset(data_ptr, string_ptr);
    CALL(virtual_string)->offset(data_ptr, data_ptr);
    CALL(virtual_string)->offset(data_ptr, list_ptr);
    CALL(virtual_string)->offset(list_ptr, null_ptr);
    CALL(virtual_string)->offset(list_ptr, empty_ptr);
    CALL(virtual_string)->offset(list_ptr, string_ptr);
    CALL(virtual_string)->offset(list_ptr, data_ptr);
    CALL(virtual_string)->offset(list_ptr, list_ptr);
    CALL(virtual_string)->offset(string_ptr, none_ptr);
    CALL(virtual_string)->offset(none_ptr, string_ptr);

    CALL(virtual_string)->unsafe(null_ptr);
    CALL(virtual_string)->unsafe(empty_ptr);
    CALL(virtual_string)->unsafe(string_ptr);
    CALL(virtual_string)->unsafe(data_ptr);
    CALL(virtual_string)->unsafe(list_ptr);
    CALL(virtual_string)->unsafe(none_ptr);

    CALL(virtual_file)->alloc(zero_ptr, zero_ptr);
    CALL(virtual_file)->alloc(string_ptr, zero_ptr);
    CALL(virtual_file)->alloc(data_ptr, zero_ptr);
    CALL(virtual_file)->alloc(list_ptr, zero_ptr);
    CALL(virtual_file)->alloc(zero_ptr, string_ptr);
    CALL(virtual_file)->alloc(zero_ptr, data_ptr);
    CALL(virtual_file)->alloc(zero_ptr, list_ptr);
    CALL(virtual_file)->alloc(string_ptr, string_ptr);
    CALL(virtual_file)->alloc(data_ptr, null_ptr);
    CALL(virtual_file)->alloc(list_ptr, null_ptr);
    CALL(virtual_file)->alloc(string_ptr, data_ptr);
    CALL(virtual_file)->alloc(string_ptr, empty_ptr);
    CALL(virtual_file)->alloc(data_ptr, null_ptr);
    CALL(virtual_file)->alloc(list_ptr, string_ptr);
    CALL(virtual_file)->alloc(data_ptr, null_ptr);
    CALL(virtual_file)->alloc(list_ptr, null_ptr);
    CALL(virtual_file)->alloc(string_ptr, data_ptr);
    CALL(virtual_file)->alloc(data_ptr, null_ptr);
    CALL(virtual_file)->alloc(list_ptr, string_ptr);
    CALL(virtual_file)->alloc(null_ptr, null_ptr);
    CALL(virtual_file)->alloc(string_ptr, null_ptr);
    CALL(virtual_file)->alloc(string_ptr, none_ptr);
    CALL(virtual_file)->alloc(none_ptr, string_ptr);

    const u64* data_unsafe_ptr1 = CALL(virtual_data)->unsafe(empty_ptr);
    const u64* data_unsafe_ptr2 = CALL(virtual_data)->unsafe(null_ptr);
    const u64* data_unsafe_ptr3 = CALL(virtual_data)->unsafe(string_ptr);
    const u64* data_unsafe_ptr4 = CALL(virtual_data)->unsafe(list_ptr);
    const u64* data_unsafe_ptr5 = CALL(virtual_data)->unsafe(none_ptr);

    RX_ASSERT(data_unsafe_ptr1 == 0);
    RX_ASSERT(data_unsafe_ptr2 == 0);
    RX_ASSERT(data_unsafe_ptr3 == 0);
    RX_ASSERT(data_unsafe_ptr4 == 0);
    RX_ASSERT(data_unsafe_ptr5 == 0);

    RX_ASSERT(string_ptr != 0);
    RX_ASSERT(data_ptr1 == 0);
    RX_ASSERT(data_ptr2 == 0);
    RX_ASSERT(data_ptr3 != 0);
    RX_ASSERT(data_ptr3 != 0);
    RX_ASSERT(data_ptr4 == 0);

    u64 undefined_ptr = CALL(virtual_string)->copy(list_ptr);

    RX_ASSERT(undefined_ptr == 0);
    RX_ASSERT(last_matched_ptr1 != 0);
    RX_ASSERT(last_matched_ptr2 != 0);
    RX_ASSERT(last_matched_ptr3 != 0);
#ifndef USE_GC
    CALL(virtual_string)->free(last_matched_ptr1);
    CALL(virtual_string)->free(last_matched_ptr2);
    CALL(virtual_string)->free(last_matched_ptr3);
    CALL(virtual_string)->free(last_matched_ptr2);
    CALL(virtual_string)->free(last_matched_ptr3);
    CALL(virtual_string)->free(null_ptr);
    CALL(virtual_string)->free(zero_ptr);
    CALL(virtual_string)->free(string_ptr);
    CALL(virtual_string)->free(data_ptr3);
    CALL(virtual_string)->free(list_ptr);
    CALL(virtual_string)->free(empty_ptr);
    CALL(virtual_string)->free(data_ptr);
    CALL(virtual_string)->free(string_ptr);
    CALL(virtual_string)->free(data_ptr3);
    CALL(virtual_string)->free(empty_ptr);
    CALL(virtual_string)->free(data_ptr);
    CALL(virtual_data)->free(zero_ptr);
    CALL(virtual_data)->free(data_ptr);
    CALL(virtual_data)->free(data_ptr3);
    CALL(virtual_data)->free(empty_ptr);
    CALL(virtual_data)->free(null_ptr);
    CALL(virtual_data)->free(data_ptr);
    CALL(virtual_data)->free(list_ptr);
    CALL(virtual_data)->free(data_ptr);
    CALL(virtual_data)->free(data_ptr3);
    CALL(virtual_data)->free(none_ptr);
    CALL(virtual_stack)->free(zero_ptr);
    CALL(virtual_stack)->free(data_ptr);
    CALL(virtual_stack)->free(empty_ptr);
    CALL(virtual_stack)->free(null_ptr);
    CALL(virtual_stack)->free(data_ptr);
    CALL(virtual_stack)->free(list_ptr);
    CALL(virtual_stack)->free(data_ptr);
    CALL(virtual_stack)->free(list_ptr);
    CALL(virtual_stack)->free(none_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_strcat_alloc_load, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 zero_ptr = CALL(virtual_string)->load("\0");
    u64 char_ptr = CALL(virtual_string)->load("/");
    CALL(virtual_string)->strcat(zero_ptr, char_ptr);
    RX_ASSERT(zero_ptr == 0);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(char_ptr), "/") == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(zero_ptr);
    CALL(virtual_string)->free(char_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_strcat_load_alloc_alloc, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 char_ptr = CALL(virtual_string)->load("/");
    u64 pattern_ptr = CALL(virtual_string)->load("\0");
    u64 empty_ptr = CALL(virtual_string)->load("\0");
    CALL(virtual_string)->strcat(pattern_ptr, char_ptr);
    CALL(virtual_string)->strcpy(empty_ptr, pattern_ptr);
    RX_ASSERT(pattern_ptr == 0);
    RX_ASSERT(empty_ptr == 0);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(char_ptr), "/") == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(empty_ptr);
    CALL(virtual_string)->free(pattern_ptr);
    CALL(virtual_string)->free(char_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_strcat_load_load, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 char_ptr = CALL(virtual_string)->load("/");
    u64 pattern_ptr = CALL(virtual_string)->load("*");
    CALL(virtual_string)->strcat(pattern_ptr, char_ptr);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(pattern_ptr), "*/") == 0);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(char_ptr), "/") == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(pattern_ptr);
    CALL(virtual_string)->free(char_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_strcpy_load_alloc, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("/");
    u64 empty_ptr = CALL(virtual_string)->load("\0");
    CALL(virtual_string)->strcpy(empty_ptr, pattern_ptr);
    RX_ASSERT(empty_ptr == 0);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(pattern_ptr), "/") == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(empty_ptr);
    CALL(virtual_string)->free(pattern_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_strcpy_load_load, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 char_ptr = CALL(virtual_string)->load("/input.txt");
    u64 pattern_ptr = CALL(virtual_string)->load("*");
    CALL(virtual_string)->strcpy(pattern_ptr, char_ptr);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(pattern_ptr), "/input.txt") == 0);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(char_ptr), "/input.txt") == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(pattern_ptr);
    CALL(virtual_string)->free(char_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_os_getenv, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 ui_mode_ptr = CALL(virtual_string)->load("ASDASD_UI_MODE");
    u64 file_path_ptr = CALL(os)->getenv(ui_mode_ptr);
    ASSERT_DEBUG(file_path_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_os_getenv_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 file_path_ptr = CALL(os)->getenv(0);
    ASSERT_DEBUG(file_path_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_os_getenv_list, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr = CALL(virtual_stack)->alloc();
    u64 file_path_ptr = CALL(os)->getenv(list_ptr);
    ASSERT_DEBUG(file_path_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_load_open_file_close_file, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 file_path_ptr = CALL(os)->getcwd();
    u64 file_name_ptr = CALL(virtual_string)->load("/nonexistent.txt");
    CALL(virtual_string)->strcat(file_path_ptr, file_name_ptr);
    CALL(virtual_string)->free(file_name_ptr);
    u64 mode_ptr = CALL(virtual_string)->load("rb");
    u64 f_ptr = CALL(virtual_file)->alloc(file_path_ptr, mode_ptr);
    u64 data_ptr = CALL(virtual_file)->data(f_ptr);
    RX_ASSERT(f_ptr == 0);
    RX_ASSERT(data_ptr == 0);
#ifndef USE_GC
    CALL(virtual_file)->free(f_ptr);
    CALL(virtual_data)->free(data_ptr);
    CALL(virtual_string)->free(mode_ptr);
    CALL(virtual_string)->free(file_name_ptr);
    CALL(virtual_string)->free(file_path_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_offset_strcpy, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 name_ptr = CALL(virtual_string)->load("name");
    u64 at_ptr = CALL(virtual_string)->load("@");
    u64 domain_ptr = CALL(virtual_string)->load("domain.org");
    CALL(virtual_string)->strcat(name_ptr, at_ptr);
    CALL(virtual_string)->strcat(name_ptr, domain_ptr);

    u64 path_ptr1 = CALL(virtual_string)->load("name@domain.org");
    u64 path_ptr2 = CALL(virtual_string)->copy(at_ptr);

    u64 domain_name = CALL(virtual_string)->offset(path_ptr1, path_ptr2);
    CALL(virtual_string)->strcpy(name_ptr, domain_name);

    char* domain = CALL(virtual_string)->unsafe(domain_ptr);
    char* name = CALL(virtual_string)->unsafe(name_ptr);
    RX_ASSERT(strcmp(domain, "domain.org") == 0);
    RX_ASSERT(strcmp(name, "domain.org") == 0);

#ifndef USE_GC
    CALL(virtual_string)->free(domain_name);
    CALL(virtual_string)->free(name_ptr);
    CALL(virtual_string)->free(path_ptr1);
    CALL(virtual_string)->free(path_ptr2);
    CALL(virtual_string)->free(name_ptr);
    CALL(virtual_string)->free(domain_ptr);
    CALL(virtual_string)->free(at_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_offset_strcpy_free, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 name_ptr = CALL(virtual_string)->load("name");
    u64 at_ptr = CALL(virtual_string)->load("@");
    u64 domain_ptr = CALL(virtual_string)->load("domain.org");
    CALL(virtual_string)->strcat(name_ptr, at_ptr);
    CALL(virtual_string)->strcat(name_ptr, domain_ptr);

    u64 path_ptr1 = CALL(virtual_string)->load("name@domain.org");
    u64 path_ptr2 = CALL(virtual_string)->copy(at_ptr);

    u64 domain_name = CALL(virtual_string)->offset(path_ptr1, path_ptr2);
    CALL(virtual_string)->free(path_ptr1);
    CALL(virtual_string)->strcpy(name_ptr, domain_name);

    char* domain = CALL(virtual_string)->unsafe(domain_ptr);
    char* name = CALL(virtual_string)->unsafe(name_ptr);
    RX_ASSERT(strcmp(domain, "domain.org") == 0);
    RX_ASSERT(strcmp(name, "name@domain.org") == 0);

#ifndef USE_GC
    CALL(virtual_string)->free(domain_name);
    CALL(virtual_string)->free(name_ptr);
    CALL(virtual_string)->free(path_ptr2);
    CALL(virtual_string)->free(name_ptr);
    CALL(virtual_string)->free(domain_ptr);
    CALL(virtual_string)->free(at_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_print_free, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 printing_ptr = CALL(virtual_string)->load("hello, world!");
    CALL(virtual_string)->free(printing_ptr);
    CALL(os)->putc(printing_ptr);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_print, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 printing_ptr = CALL(virtual_string)->load("hello, world!");
    CALL(os)->putc(printing_ptr);
#ifndef USE_GC
    CALL(virtual_string)->free(printing_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_print_string_pointer, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 printing_ptr = CALL(virtual_string)->load("hello, world!");
    u64 comma_ptr = CALL(virtual_string)->load(",");
    u64 substring_index_ptr = CALL(virtual_string)->offset(printing_ptr, comma_ptr);
    CALL(os)->putc(substring_index_ptr);
#ifndef USE_GC
    CALL(virtual_string)->free(printing_ptr);
    CALL(virtual_string)->free(substring_index_ptr);
    CALL(virtual_string)->free(comma_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_print_string_pointer_copy, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    const char* substring_expected = " world!";
    u64 printing_ptr = CALL(virtual_string)->load("hello, world!");
    u64 comma_ptr = CALL(virtual_string)->load(",");
    u64 substring_index_ptr = CALL(virtual_string)->offset(printing_ptr, comma_ptr);
    u64 substring_ptr = CALL(virtual_string)->copy(substring_index_ptr);

    char* substring_actual = CALL(virtual_string)->unsafe(substring_ptr);
    RX_ASSERT(strcmp(substring_expected, substring_actual) == 0);

#ifndef USE_GC
    CALL(virtual_string)->free(printing_ptr);
    CALL(virtual_string)->free(comma_ptr);
    CALL(virtual_string)->free(substring_index_ptr);
    CALL(virtual_string)->free(substring_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_size, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr = CALL(virtual_stack)->alloc();
    u64 size_actual = CALL(virtual_string)->size(list_ptr);
    u64 size_expected = 0;
    RX_ASSERT(size_expected == size_actual);
#ifndef USE_GC
    CALL(virtual_stack)->free(list_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_unsafe, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr = CALL(virtual_stack)->alloc();
    char* ptr_actual = CALL(virtual_string)->unsafe(list_ptr);
    char* ptr_expected = 0;
    RX_ASSERT(ptr_expected == ptr_actual);
#ifndef USE_GC
    CALL(virtual_stack)->free(list_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_offset, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 string_ptr = CALL(virtual_string)->load("himem.sys");
    u64 list_ptr = CALL(virtual_stack)->alloc();
    u64 size_actual = CALL(virtual_string)->offset(string_ptr, list_ptr);
    u64 size_expected = 0;
    RX_ASSERT(size_expected == size_actual);
#ifndef USE_GC
    CALL(virtual_stack)->free(list_ptr);
    CALL(virtual_string)->free(string_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, teststring_pointer_unsafe, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 printing_ptr = CALL(virtual_string)->load("hello, world!");
    u64 comma_ptr = CALL(virtual_string)->load(",");
    u64 substring_index_ptr = CALL(virtual_string)->offset(printing_ptr, comma_ptr);

    char* expected_value = CALL(virtual_string)->unsafe(substring_index_ptr);
    const char* actual_value = " world!";
    RX_ASSERT(strcmp(expected_value, actual_value) == 0);

    CALL(virtual_string)->free(substring_index_ptr);
    CALL(os)->putc(substring_index_ptr);
#ifndef USE_GC
    CALL(virtual_string)->free(printing_ptr);
    CALL(virtual_string)->free(comma_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_size, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 printing_ptr = CALL(virtual_string)->load("hello, world!");
    u64 comma_ptr = CALL(virtual_string)->load(",");
    u64 substring_index_ptr = CALL(virtual_string)->offset(printing_ptr, comma_ptr);
    u64 substring_ptr = CALL(virtual_string)->copy(substring_index_ptr);

    u64 size_expected = strlen(" world!"); /* adds one 0 to termination byte */
    u64 size_actual = CALL(virtual_string)->size(substring_ptr);
    RX_ASSERT(size_expected == size_actual);

#ifndef USE_GC
    CALL(virtual_string)->free(printing_ptr);
    CALL(virtual_string)->free(comma_ptr);
    CALL(virtual_string)->free(substring_index_ptr);
    CALL(virtual_string)->free(substring_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_offset_subsearch, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 printing_ptr = CALL(virtual_string)->load("hello, world!");
    u64 comma_ptr = CALL(virtual_string)->load(",");
    u64 w_ptr = CALL(virtual_string)->load("w");
    u64 substring_index_ptr1 = CALL(virtual_string)->offset(printing_ptr, comma_ptr);
    u64 substring_index_ptr2 = CALL(virtual_string)->offset(substring_index_ptr1, w_ptr);
    u64 substring_ptr = CALL(virtual_string)->copy(substring_index_ptr2);

    u64 size_expected = strlen("orld!"); /* adds one 0 to termination byte */
    u64 size_actual = CALL(virtual_string)->size(substring_ptr);
    RX_ASSERT(size_expected == size_actual);

#ifndef USE_GC
    CALL(virtual_string)->free(printing_ptr);
    CALL(virtual_string)->free(comma_ptr);
    CALL(virtual_string)->free(substring_index_ptr1);
    CALL(virtual_string)->free(substring_index_ptr2);
    CALL(virtual_string)->free(substring_ptr);
    CALL(virtual_string)->free(w_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_print_string_pointer_free, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 printing_ptr = CALL(virtual_string)->load("hello, world!");
    u64 comma_ptr = CALL(virtual_string)->load(",");
    u64 substring_index_ptr = CALL(virtual_string)->offset(printing_ptr, comma_ptr);
    CALL(virtual_string)->free(substring_index_ptr);
    CALL(os)->putc(substring_index_ptr);
#ifndef USE_GC
    CALL(virtual_string)->free(printing_ptr);
    CALL(virtual_string)->free(comma_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_offset_strcat, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 name_ptr = CALL(virtual_string)->load("name");
    u64 at_ptr = CALL(virtual_string)->load("@");
    CALL(virtual_string)->strcat(name_ptr, at_ptr);

    u64 path_ptr1 = CALL(virtual_string)->load("name@domain.org");
    u64 path_ptr2 = CALL(virtual_string)->copy(at_ptr);

    u64 domain_name = CALL(virtual_string)->offset(path_ptr1, path_ptr2);
    CALL(virtual_string)->strcat(name_ptr, domain_name);

    char* domain = CALL(virtual_string)->unsafe(path_ptr1);
    char* name = CALL(virtual_string)->unsafe(name_ptr);
    u64 domain_len = strlen(domain);
    u64 name_len = strlen(name);
    RX_ASSERT(domain_len > 0);
    RX_ASSERT(name_len > 0);
    RX_ASSERT(domain_len == name_len);
    RX_ASSERT(strcmp(domain, name) == 0);

#ifndef USE_GC
    CALL(virtual_string)->free(domain_name);
    CALL(virtual_string)->free(name_ptr);
    CALL(virtual_string)->free(path_ptr1);
    CALL(virtual_string)->free(path_ptr2);
    CALL(virtual_string)->free(name_ptr);
    CALL(virtual_string)->free(at_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_file_read_invalid_type, .fixture = test_fixture) {
    CALL(pointer)->init(8);
#ifndef USE_GC
    u64 list_ptr = CALL(virtual_stack)->alloc();
    u64 data_ptr = CALL(virtual_file)->data(list_ptr);
    CALL(virtual_data)->free(data_ptr);
    CALL(virtual_file)->free(list_ptr);
    CALL(virtual_stack)->free(list_ptr);
#else
    u64 list_ptr = CALL(virtual_stack)->alloc();
    CALL(virtual_file)->data(list_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_load_open_match_last_unsafe_free, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 file_path_ptr = CALL(os)->getcwd();
    u64 file_name_ptr = CALL(virtual_string)->load("/all_english_words.txt//");
    CALL(virtual_string)->strcat(file_path_ptr, file_name_ptr);
    u64 pattern_ptr = CALL(virtual_string)->load("//");
    u64 last_match_ptr = CALL(virtual_string)->offset(file_path_ptr, pattern_ptr);
    CALL(virtual_string)->free(pattern_ptr);
    CALL(virtual_string)->put_char(last_match_ptr, '\0');
    CALL(virtual_string)->free(last_match_ptr);
    CALL(virtual_string)->strcat(file_path_ptr, file_name_ptr);
    CALL(virtual_string)->free(file_name_ptr);
    u64 mode_ptr = CALL(virtual_string)->load("rb");
    u64 f_ptr = CALL(virtual_file)->alloc(file_path_ptr, mode_ptr);
    CALL(virtual_string)->free(file_path_ptr);
    CALL(virtual_string)->free(mode_ptr);
    u64 data_ptr = CALL(virtual_file)->data(f_ptr);
    CALL(virtual_file)->free(f_ptr);
    CALL(os)->putc(data_ptr);
#ifndef USE_GC
    CALL(virtual_data)->free(data_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_load_open_match_last_unsafe_free_unsuppported_calls, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 ptr1 = CALL(virtual_string)->load("qwerty//u");
    u64 ptr2 = CALL(virtual_string)->load("asdfghjkl");
    u64 pattern_ptr = CALL(virtual_string)->load("/u");
    u64 last_match_ptr = CALL(virtual_string)->offset(ptr1, pattern_ptr);
    CALL(virtual_string)->strcat(last_match_ptr, ptr2);
    CALL(virtual_string)->free(pattern_ptr);
    CALL(virtual_string)->put_char(last_match_ptr, '\0');
#ifndef USE_GC
    CALL(virtual_string)->free(ptr1);
    CALL(virtual_string)->free(ptr2);
    CALL(virtual_string)->free(last_match_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_user, .fixture = test_fixture) {
    CALL(pointer)->init(8);
#ifndef USE_GC
    u64 ptr1 = CALL(virtual_user)->alloc();
    CALL(virtual_user)->free(ptr1);
#else
    CALL(virtual_user)->alloc();
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_load_open_file_unsafe_hashtable, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 file_path_ptr = CALL(os)->getcwd();
    u64 file_name_ptr = CALL(virtual_string)->load("/all_english_words.txt");
    CALL(virtual_string)->strcat(file_path_ptr, file_name_ptr);
    CALL(virtual_string)->free(file_name_ptr);
    u64 mode_ptr = CALL(virtual_string)->load("rb");
    u64 f_ptr = CALL(virtual_file)->alloc(file_path_ptr, mode_ptr);
    if (f_ptr != 0) {
        u64 data_ptr = CALL(virtual_file)->data(f_ptr);
        u64 list_ptr = CALL(virtual_stack)->alloc();
        CALL(virtual_file)->free(f_ptr);
        u8* file_data = CALL(virtual_data)->unsafe(data_ptr);
        for (int i = 0; i < 100; i++) {
            u8* tmp = file_data;
            while (*tmp != 0 && *tmp != '\n') {
                tmp++;
            }
            *tmp++ = '\0';
            u64 string_ptr = CALL(virtual_string)->load((char*)file_data);
            CALL(virtual_stack)->push(list_ptr, string_ptr);
            CALL(os)->putc(string_ptr);
            file_data = tmp;
        }
        CALL(virtual_stack)->free(list_ptr);
        CALL(virtual_data)->free(data_ptr);
    }
#ifndef USE_GC
    CALL(virtual_string)->free(mode_ptr);
    CALL(virtual_string)->free(file_name_ptr);
    CALL(virtual_string)->free(file_path_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

RX_TEST_CASE(tests_v1, test_load_load_match_last, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 str_ptr = CALL(virtual_string)->load("Hello, world!");
    u64 ch_ptr = CALL(virtual_string)->load("z");
    u64 last_match_ptr = CALL(virtual_string)->offset(str_ptr, ch_ptr);
    RX_ASSERT(last_match_ptr == 0);
#ifndef USE_GC
    CALL(virtual_string)->free(last_match_ptr);
    CALL(virtual_string)->free(str_ptr);
    CALL(virtual_string)->free(ch_ptr);
#endif
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* internal */
static void parse_text(u64 text_string_ptr);

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_init, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_load, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    CALL(virtual_string)->load("asdf");
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_init_string_load_9, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    CALL(virtual_string)->load("0");
    CALL(virtual_string)->load("1");
    CALL(virtual_string)->load("2");
    CALL(virtual_string)->load("3");
    CALL(virtual_string)->load("4");
    CALL(virtual_string)->load("5");
    CALL(virtual_string)->load("6");
    CALL(virtual_string)->load("7");
    CALL(virtual_string)->load("8");
    CALL(virtual_string)->load("9");
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_strings, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 text_string_ptr0 = CALL(virtual_string)->load("a");
    u64 text_string_ptr1 = CALL(virtual_string)->load("a\nb");
    u64 text_string_ptr2 = CALL(virtual_string)->load("ab\nabc\n");
    u64 text_string_ptr3 = CALL(virtual_string)->load("adadadsadsadad\ndad\nadsaddasaddad\nsad\n");
    u64 text_string_ptr4 = CALL(virtual_string)->load("ab\ndad\nabcd\nbcd\n");
    u64 text_string_ptr5 = CALL(virtual_string)->load("ab\nc\nabc\nbcd\n");
    u64 text_string_ptr6 = CALL(virtual_string)->load("abc\nabcd\nbcde\nabc\n");
    u64 text_string_ptr7 = CALL(virtual_string)->load("abc\n\n");
    parse_text(0);
    parse_text(text_string_ptr0);
    parse_text(text_string_ptr1);
    parse_text(text_string_ptr2);
    parse_text(text_string_ptr3);
    parse_text(text_string_ptr4);
    parse_text(text_string_ptr5);
    parse_text(text_string_ptr6);
    parse_text(text_string_ptr7);
    CALL(virtual_string)->free(text_string_ptr0);
    CALL(virtual_string)->free(text_string_ptr1);
    CALL(virtual_string)->free(text_string_ptr2);
    CALL(virtual_string)->free(text_string_ptr3);
    CALL(virtual_string)->free(text_string_ptr4);
    CALL(virtual_string)->free(text_string_ptr5);
    CALL(virtual_string)->free(text_string_ptr6);
    CALL(virtual_string)->free(text_string_ptr7);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_pointer_free, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    CALL(virtual_string_pointer)->free(0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_pointer_lessthan_free, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("b");
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("abba");
    u64 quantum_str_ptr2 = CALL(virtual_string)->offset(quantum_str_ptr1, pattern_ptr);
    u64 data_ptr = CALL(virtual_string)->lessthan(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(data_ptr != 0);
    CALL(virtual_string_pointer)->free(data_ptr);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_0_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 error_ptr = CALL(virtual_string)->lessthan(0, 0);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_0_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 error_ptr = CALL(virtual_string)->lessthan(0, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_1_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr = CALL(virtual_string)->load("");
    ASSERT_DEBUG(quantum_str_ptr == 0);
    u64 error_ptr = CALL(virtual_string)->lessthan(1, 0);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_1_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->lessthan(1, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_a_b, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("b");
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("abba");
    u64 quantum_str_ptr2 = CALL(virtual_string)->offset(quantum_str_ptr1, pattern_ptr);
    u64 data_ptr = CALL(virtual_string)->lessthan(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(data_ptr != 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_b_a, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("b");
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("abba");
    u64 quantum_str_ptr2 = CALL(virtual_string)->offset(quantum_str_ptr1, pattern_ptr);
    u64 error_ptr = CALL(virtual_string)->lessthan(quantum_str_ptr2, quantum_str_ptr1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_1_2, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->lessthan(1, 2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_1_3, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->lessthan(1, 2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_2_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->lessthan(2, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_a_string_free, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    CALL(virtual_string)->free(quantum_str_ptr2);
    u64 error_ptr = CALL(virtual_string)->lessthan(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_string_free_b, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    CALL(virtual_string)->free(quantum_str_ptr1);
    u64 error_ptr = CALL(virtual_string)->lessthan(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_string_list, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    u64 list_ptr2 = CALL(virtual_stack)->alloc();
    u64 error_ptr = CALL(virtual_string)->lessthan(quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_list_string, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr1 = CALL(virtual_stack)->alloc();
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("a");
    u64 error_ptr = CALL(virtual_string)->lessthan(list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_0_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 error_ptr = CALL(virtual_string)->greaterthan(0, 0);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_0_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 error_ptr = CALL(virtual_string)->greaterthan(0, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_1_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr = CALL(virtual_string)->load("");
    ASSERT_DEBUG(quantum_str_ptr == 0);
    u64 error_ptr = CALL(virtual_string)->greaterthan(1, 0);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_1_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->greaterthan(1, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_a_b, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("b");
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("abba");
    u64 quantum_str_ptr2 = CALL(virtual_string)->offset(quantum_str_ptr1, pattern_ptr);
    u64 error_ptr = CALL(virtual_string)->greaterthan(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_b_a, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("b");
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("abba");
    u64 quantum_str_ptr2 = CALL(virtual_string)->offset(quantum_str_ptr1, pattern_ptr);
    u64 data_ptr = CALL(virtual_string)->greaterthan(quantum_str_ptr2, quantum_str_ptr1);
    RX_ASSERT(data_ptr != 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_1_2, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->greaterthan(1, 2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_1_3, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->greaterthan(1, 3);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_2_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->greaterthan(2, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_a_string_free, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    CALL(virtual_string)->free(quantum_str_ptr2);
    u64 error_ptr = CALL(virtual_string)->greaterthan(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_string_free_b, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    CALL(virtual_string)->free(quantum_str_ptr1);
    u64 error_ptr = CALL(virtual_string)->greaterthan(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_string_list, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 list_ptr2 = CALL(virtual_stack)->alloc();
    u64 error_ptr = CALL(virtual_string)->greaterthan(quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_list_string, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr1 = CALL(virtual_stack)->alloc();
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("a");
    u64 error_ptr = CALL(virtual_string)->greaterthan(list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_0_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 error_ptr = CALL(virtual_string)->equals(0, 0);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_0_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 error_ptr = CALL(virtual_string)->equals(0, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_1_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr = CALL(virtual_string)->load("");
    ASSERT_DEBUG(quantum_str_ptr == 0);
    u64 error_ptr = CALL(virtual_string)->equals(1, 0);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_1_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->equals(1, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_a_b, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("b");
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("abba");
    u64 quantum_str_ptr2 = CALL(virtual_string)->offset(quantum_str_ptr1, pattern_ptr);
    u64 error_ptr = CALL(virtual_string)->equals(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_b_a, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("b");
    u64 source_ptr = CALL(virtual_string)->load("abba");
    u64 quantum_str_ptr1 = CALL(virtual_string)->offset(source_ptr, pattern_ptr);
    u64 quantum_str_ptr2 = CALL(virtual_string)->offset(source_ptr, pattern_ptr);
    u64 data_ptr = CALL(virtual_string)->equals(quantum_str_ptr2, quantum_str_ptr1);
    RX_ASSERT(data_ptr != 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_1_2, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->equals(1, 2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_1_3, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->equals(1, 3);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_2_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->equals(2, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_a_string_free, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    CALL(virtual_string)->free(quantum_str_ptr2);
    u64 error_ptr = CALL(virtual_string)->equals(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_string_free_b, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    CALL(virtual_string)->free(quantum_str_ptr1);
    u64 error_ptr = CALL(virtual_string)->equals(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_string_list, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    u64 list_ptr2 = CALL(virtual_stack)->alloc();
    u64 error_ptr = CALL(virtual_string)->equals(quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_list_string, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr1 = CALL(virtual_stack)->alloc();
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("a");
    u64 error_ptr = CALL(virtual_string)->equals(list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_0_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 error_ptr = CALL(virtual_string)->compare(0, 0);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_0_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 error_ptr = CALL(virtual_string)->compare(0, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_1_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr = CALL(virtual_string)->load("");
    ASSERT_DEBUG(quantum_str_ptr == 0);
    u64 error_ptr = CALL(virtual_string)->compare(1, 0);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_1_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->compare(1, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_a_b, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("b");
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("abba");
    u64 quantum_str_ptr2 = CALL(virtual_string)->offset(quantum_str_ptr1, pattern_ptr);
    u64 data_ptr = CALL(virtual_string)->compare(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(data_ptr != 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_b_a, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("b");
    u64 source_ptr = CALL(virtual_string)->load("abba");
    u64 quantum_str_ptr1 = CALL(virtual_string)->offset(source_ptr, pattern_ptr);
    u64 quantum_str_ptr2 = CALL(virtual_string)->offset(source_ptr, pattern_ptr);
    u64 data_ptr = CALL(virtual_string)->compare(quantum_str_ptr2, quantum_str_ptr1);
    RX_ASSERT(data_ptr != 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_1_2, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->compare(1, 2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_1_3, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->compare(1, 3);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_2_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->compare(2, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_a_string_free, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    CALL(virtual_string)->free(quantum_str_ptr2);
    u64 error_ptr = CALL(virtual_string)->compare(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_string_free_b, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    CALL(virtual_string)->free(quantum_str_ptr1);
    u64 error_ptr = CALL(virtual_string)->compare(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_string_list, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    u64 list_ptr2 = CALL(virtual_stack)->alloc();
    u64 error_ptr = CALL(virtual_string)->compare(quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_list_string, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr1 = CALL(virtual_stack)->alloc();
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("a");
    u64 error_ptr = CALL(virtual_string)->compare(list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_0_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 error_ptr = CALL(virtual_string)->strcmp(0, 0);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_0_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 error_ptr = CALL(virtual_string)->strcmp(0, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_1_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr = CALL(virtual_string)->load("");
    ASSERT_DEBUG(quantum_str_ptr == 0);
    u64 error_ptr = CALL(virtual_string)->strcmp(1, 0);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_1_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->strcmp(1, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_a_b, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("b");
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("abba");
    u64 quantum_str_ptr2 = CALL(virtual_string)->offset(quantum_str_ptr1, pattern_ptr);
    u64 data_ptr = CALL(virtual_string)->strcmp(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(data_ptr != 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_b_a, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("b");
    u64 source_ptr = CALL(virtual_string)->load("abba");
    u64 quantum_str_ptr1 = CALL(virtual_string)->offset(source_ptr, pattern_ptr);
    u64 quantum_str_ptr2 = CALL(virtual_string)->offset(source_ptr, pattern_ptr);
    u64 data_ptr = CALL(virtual_string)->strcmp(quantum_str_ptr2, quantum_str_ptr1);
    RX_ASSERT(data_ptr != 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_1_2, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->strcmp(1, 2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}
/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_1_3, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->strcmp(1, 3);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_2_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->strcmp(2, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_a_string_free, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    CALL(virtual_string)->free(quantum_str_ptr2);
    u64 error_ptr = CALL(virtual_string)->strcmp(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_string_free_b, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    CALL(virtual_string)->free(quantum_str_ptr1);
    u64 error_ptr = CALL(virtual_string)->strcmp(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_string_list, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    u64 list_ptr2 = CALL(virtual_stack)->alloc();
    u64 error_ptr = CALL(virtual_string)->strcmp(quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_list_string, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr1 = CALL(virtual_stack)->alloc();
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("a");
    u64 error_ptr = CALL(virtual_string)->strcmp(list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_0_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 error_ptr = CALL(virtual_string)->move_right(0, 0);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_0_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 error_ptr = CALL(virtual_string)->move_right(0, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_1_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr = CALL(virtual_string)->load("");
    ASSERT_DEBUG(quantum_str_ptr == 0);
    u64 error_ptr = CALL(virtual_string)->move_right(1, 0);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_1_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->move_right(1, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_a_b, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("abba");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 error_ptr = CALL(virtual_string)->move_right(quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_b_a, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("a");
    u64 source_ptr = CALL(virtual_string)->load("abba");
    u64 quantum_str_ptr1 = CALL(virtual_string)->offset(source_ptr, pattern_ptr);
    u64 error_ptr = CALL(virtual_string)->move_right(quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_a_b_2, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("b");
    u64 source_ptr = CALL(virtual_string)->load("abba");
    u64 quantum_str_ptr1 = CALL(virtual_string)->offset(source_ptr, pattern_ptr);
    u64 data_ptr = CALL(virtual_string)->move_right(quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_a_b_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("a");
    u64 source_ptr = CALL(virtual_string)->load("abba");
    u64 quantum_str_ptr1 = CALL(virtual_string)->offset(source_ptr, pattern_ptr);
    u64 error_ptr = CALL(virtual_string)->move_right(quantum_str_ptr1, 1);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(quantum_str_ptr1), "") == 0);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_a_b_free, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("b");
    u64 source_ptr = CALL(virtual_string)->load("abba");
    u64 quantum_str_ptr1 = CALL(virtual_string)->offset(source_ptr, pattern_ptr);
    CALL(virtual_string)->free(source_ptr);
    u64 error_ptr = CALL(virtual_string)->move_right(quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_1_2, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->move_right(1, 2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_1_3, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->move_right(1, 3);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_2_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->move_right(2, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_a_string_free, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    CALL(virtual_string)->free(quantum_str_ptr2);
    u64 error_ptr = CALL(virtual_string)->move_right(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_string_free_b, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    CALL(virtual_string)->free(quantum_str_ptr1);
    u64 error_ptr = CALL(virtual_string)->move_right(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_string_list, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    u64 list_ptr2 = CALL(virtual_stack)->alloc();
    u64 error_ptr = CALL(virtual_string)->move_right(quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_list_string, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr1 = CALL(virtual_stack)->alloc();
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("a");
    u64 error_ptr = CALL(virtual_string)->move_right(list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_0_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 error_ptr = CALL(virtual_string)->right(0, 0);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_0_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 error_ptr = CALL(virtual_string)->right(0, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_1_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr = CALL(virtual_string)->load("");
    ASSERT_DEBUG(quantum_str_ptr == 0);
    u64 error_ptr = CALL(virtual_string)->right(1, 0);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_1_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 data_ptr = CALL(virtual_string)->right(quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(data_ptr), "") == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_a_b, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("abba");
    u64 error_ptr = CALL(virtual_string)->right(quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr != 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_b_a, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("a");
    u64 source_ptr = CALL(virtual_string)->load("abba");
    u64 quantum_str_ptr1 = CALL(virtual_string)->offset(source_ptr, pattern_ptr);
    u64 error_ptr = CALL(virtual_string)->right(quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_a_b_2, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("b");
    u64 source_ptr = CALL(virtual_string)->load("abba");
    u64 quantum_str_ptr1 = CALL(virtual_string)->offset(source_ptr, pattern_ptr);
    u64 data_ptr = CALL(virtual_string)->right(quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_a_b_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("a");
    u64 source_ptr = CALL(virtual_string)->load("abba");
    u64 quantum_str_ptr1 = CALL(virtual_string)->offset(source_ptr, pattern_ptr);
    u64 error_ptr = CALL(virtual_string)->right(quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(quantum_str_ptr1), "") == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_a_b_free, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("b");
    u64 source_ptr = CALL(virtual_string)->load("abba");
    u64 quantum_str_ptr1 = CALL(virtual_string)->offset(source_ptr, pattern_ptr);
    CALL(virtual_string)->free(source_ptr);
    u64 error_ptr = CALL(virtual_string)->right(quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_1_2, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->right(1, 2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_1_3, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->right(1, 3);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_2_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("");
    u64 error_ptr = CALL(virtual_string)->right(quantum_str_ptr2, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_a_string_free, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    CALL(virtual_string)->free(quantum_str_ptr2);
    u64 error_ptr = CALL(virtual_string)->right(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_string_free_b, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    CALL(virtual_string)->free(quantum_str_ptr1);
    u64 error_ptr = CALL(virtual_string)->right(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_string_list, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 list_ptr2 = CALL(virtual_stack)->alloc();
    u64 error_ptr = CALL(virtual_string)->right(quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_list_string, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr1 = CALL(virtual_stack)->alloc();
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("a");
    u64 error_ptr = CALL(virtual_string)->right(list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_0_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 error_ptr = CALL(virtual_string)->move_left(0, 0);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_0_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 error_ptr = CALL(virtual_string)->move_left(0, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_1_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr = CALL(virtual_string)->load("");
    ASSERT_DEBUG(quantum_str_ptr == 0);
    u64 error_ptr = CALL(virtual_string)->move_left(1, 0);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_1_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->move_left(1, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_a_b, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("b");
    u64 source_ptr = CALL(virtual_string)->load("abba");
    u64 quantum_str_ptr1 = CALL(virtual_string)->offset(source_ptr, pattern_ptr);
    u64 error_ptr = CALL(virtual_string)->move_left(quantum_str_ptr1, 4);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_b_a, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("a");
    u64 source_ptr = CALL(virtual_string)->load("abba");
    u64 quantum_str_ptr1 = CALL(virtual_string)->offset(source_ptr, pattern_ptr);
    u64 data_ptr = CALL(virtual_string)->move_left(quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_a_b_2, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("b");
    u64 source_ptr = CALL(virtual_string)->load("abba");
    u64 quantum_str_ptr1 = CALL(virtual_string)->offset(source_ptr, pattern_ptr);
    u64 data_ptr = CALL(virtual_string)->move_left(quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_a_b_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("a");
    u64 source_ptr = CALL(virtual_string)->load("abba");
    u64 quantum_str_ptr1 = CALL(virtual_string)->offset(source_ptr, pattern_ptr);
    u64 data_ptr = CALL(virtual_string)->move_left(quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(quantum_str_ptr1), "a") == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_a_b_free, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("b");
    u64 source_ptr = CALL(virtual_string)->load("abba");
    u64 quantum_str_ptr1 = CALL(virtual_string)->offset(source_ptr, pattern_ptr);
    CALL(virtual_string)->free(source_ptr);
    u64 error_ptr = CALL(virtual_string)->move_left(quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_1_2, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->move_left(1, 2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}
/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_1_3, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->move_left(1, 3);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_2_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->move_left(2, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_a_string_free, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    CALL(virtual_string)->free(quantum_str_ptr2);
    u64 error_ptr = CALL(virtual_string)->move_left(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_string_free_b, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    CALL(virtual_string)->free(quantum_str_ptr1);
    u64 error_ptr = CALL(virtual_string)->move_left(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_string_list, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    u64 list_ptr2 = CALL(virtual_stack)->alloc();
    u64 error_ptr = CALL(virtual_string)->move_left(quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_list_string, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr1 = CALL(virtual_stack)->alloc();
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("a");
    u64 error_ptr = CALL(virtual_string)->move_left(list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_0_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 error_ptr = CALL(virtual_string)->left_strncpy(0, 0);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_0_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 error_ptr = CALL(virtual_string)->left_strncpy(0, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_1_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr = CALL(virtual_string)->load("");
    ASSERT_DEBUG(quantum_str_ptr == 0);
    u64 error_ptr = CALL(virtual_string)->left_strncpy(1, 0);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_1_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->left_strncpy(1, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_a_b, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("b");
    u64 source_ptr = CALL(virtual_string)->load("abba");
    u64 quantum_str_ptr1 = CALL(virtual_string)->offset(source_ptr, pattern_ptr);
    u64 error_ptr = CALL(virtual_string)->left_strncpy(quantum_str_ptr1, 4);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_b_a, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("c");
    u64 source_ptr = CALL(virtual_string)->load("abbc");
    u64 quantum_str_ptr1 = CALL(virtual_string)->offset(source_ptr, pattern_ptr);
    u64 data_ptr = CALL(virtual_string)->left_strncpy(quantum_str_ptr1, 3);
    RX_ASSERT(quantum_str_ptr1 != 0);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(quantum_str_ptr1), "") == 0);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(data_ptr), "bbc") == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_a_b_2, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("b");
    u64 source_ptr = CALL(virtual_string)->load("abba");
    u64 quantum_str_ptr1 = CALL(virtual_string)->offset(source_ptr, pattern_ptr);
    u64 data_ptr = CALL(virtual_string)->left_strncpy(quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_a_b_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("a");
    u64 source_ptr = CALL(virtual_string)->load("abba");
    u64 quantum_str_ptr1 = CALL(virtual_string)->offset(source_ptr, pattern_ptr);
    u64 data_ptr = CALL(virtual_string)->left_strncpy(quantum_str_ptr1, 1);
    RX_ASSERT(quantum_str_ptr1 != 0);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(quantum_str_ptr1), "") == 0);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(data_ptr), "a") == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_a_b_free, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("b");
    u64 source_ptr = CALL(virtual_string)->load("abba");
    u64 quantum_str_ptr1 = CALL(virtual_string)->offset(source_ptr, pattern_ptr);
    CALL(virtual_string)->free(source_ptr);
    u64 error_ptr = CALL(virtual_string)->left_strncpy(quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_1_2, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->left_strncpy(1, 2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}
/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_1_3, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->left_strncpy(1, 3);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_2_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->left_strncpy(2, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_a_string_free, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    CALL(virtual_string)->free(quantum_str_ptr2);
    u64 error_ptr = CALL(virtual_string)->left_strncpy(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_string_free_b, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    CALL(virtual_string)->free(quantum_str_ptr1);
    u64 error_ptr = CALL(virtual_string)->left_strncpy(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_string_list, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 list_ptr2 = CALL(virtual_stack)->alloc();
    u64 error_ptr = CALL(virtual_string)->left_strncpy(quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_list_string, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr1 = CALL(virtual_stack)->alloc();
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->left_strncpy(list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_0_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 error_ptr = CALL(virtual_string)->left(0, 0);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_0_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 error_ptr = CALL(virtual_string)->left(0, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_1_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr = CALL(virtual_string)->load("");
    ASSERT_DEBUG(quantum_str_ptr == 0);
    u64 error_ptr = CALL(virtual_string)->left(1, 0);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_1_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->left(1, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_a_b, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("b");
    u64 source_ptr = CALL(virtual_string)->load("abba");
    u64 quantum_str_ptr1 = CALL(virtual_string)->offset(source_ptr, pattern_ptr);
    u64 error_ptr = CALL(virtual_string)->left(quantum_str_ptr1, 4);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_b_a, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("c");
    u64 source_ptr = CALL(virtual_string)->load("abbc");
    u64 quantum_str_ptr1 = CALL(virtual_string)->offset(source_ptr, pattern_ptr);
    u64 data_ptr = CALL(virtual_string)->left(quantum_str_ptr1, 3);
    RX_ASSERT(quantum_str_ptr1 != 0);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(quantum_str_ptr1), "") == 0);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(data_ptr), "bbc") == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_a_b_2, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("b");
    u64 source_ptr = CALL(virtual_string)->load("abba");
    u64 quantum_str_ptr1 = CALL(virtual_string)->offset(source_ptr, pattern_ptr);
    u64 data_ptr = CALL(virtual_string)->left(quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_a_b_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("a");
    u64 source_ptr = CALL(virtual_string)->load("abba");
    u64 quantum_str_ptr1 = CALL(virtual_string)->offset(source_ptr, pattern_ptr);
    u64 data_ptr = CALL(virtual_string)->left(quantum_str_ptr1, 1);
    RX_ASSERT(quantum_str_ptr1 != 0);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(quantum_str_ptr1), "") == 0);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(data_ptr), "a") == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_a_b_free, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("b");
    u64 source_ptr = CALL(virtual_string)->load("abba");
    u64 quantum_str_ptr1 = CALL(virtual_string)->offset(source_ptr, pattern_ptr);
    CALL(virtual_string)->free(source_ptr);
    u64 error_ptr = CALL(virtual_string)->left(quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_1_2, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->left(1, 2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}
/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_1_3, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->left(1, 3);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_2_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->left(2, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_a_string_free, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    CALL(virtual_string)->free(quantum_str_ptr2);
    u64 error_ptr = CALL(virtual_string)->left(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(virtual_string)->free(quantum_str_ptr2);
    CALL(pointer)->gc();
    CALL(virtual_string)->free(quantum_str_ptr2);
    CALL(pointer)->destroy();
    CALL(virtual_string)->free(quantum_str_ptr2);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_string_free_b, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    CALL(virtual_string)->free(quantum_str_ptr1);
    u64 error_ptr = CALL(virtual_string)->left(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_string_list, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    u64 list_ptr2 = CALL(virtual_stack)->alloc();
    u64 error_ptr = CALL(virtual_string)->left(quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_list_string, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr1 = CALL(virtual_stack)->alloc();
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("a");
    u64 error_ptr = CALL(virtual_string)->left(list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_0_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 error_ptr = CALL(virtual_string)->strncpy(0, 0);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_0_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 error_ptr = CALL(virtual_string)->strncpy(0, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_1_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr = CALL(virtual_string)->load("");
    ASSERT_DEBUG(quantum_str_ptr == 0);
    u64 error_ptr = CALL(virtual_string)->strncpy(1, 0);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_1_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->strncpy(1, 1);
    RX_ASSERT(error_ptr == 3);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_a_b, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("b");
    u64 source_ptr = CALL(virtual_string)->load("abba");
    u64 quantum_str_ptr1 = CALL(virtual_string)->offset(source_ptr, pattern_ptr);
    u64 error_ptr = CALL(virtual_string)->strncpy(quantum_str_ptr1, 2);
    RX_ASSERT(error_ptr == 0);
    u64 data_ptr = CALL(virtual_string)->strncpy(quantum_str_ptr1, 1);
    u64 str_ptr1 = CALL(virtual_string)->offset(source_ptr, data_ptr);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(str_ptr1 != 0);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(str_ptr1), "") == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_b_a, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("b");
    u64 source_ptr = CALL(virtual_string)->load("abcd");
    u64 quantum_str_ptr1 = CALL(virtual_string)->offset(source_ptr, pattern_ptr);
    u64 data_ptr = CALL(virtual_string)->strncpy(quantum_str_ptr1, 2);
    RX_ASSERT(data_ptr != 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_a_b_2, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("b");
    u64 source_ptr = CALL(virtual_string)->load("abba");
    u64 quantum_str_ptr1 = CALL(virtual_string)->offset(source_ptr, pattern_ptr);
    u64 data_ptr = CALL(virtual_string)->strncpy(quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_a_b_0, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("a");
    u64 source_ptr = CALL(virtual_string)->load("abba");
    u64 quantum_str_ptr1 = CALL(virtual_string)->offset(source_ptr, pattern_ptr);
    u64 data_ptr = CALL(virtual_string)->strncpy(quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr == 0);
    RX_ASSERT(quantum_str_ptr1 != 0);
    RX_ASSERT(strcmp(CALL(virtual_string)->unsafe(quantum_str_ptr1), "") == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_a_b_free, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("b");
    u64 source_ptr = CALL(virtual_string)->load("abba");
    u64 quantum_str_ptr1 = CALL(virtual_string)->offset(source_ptr, pattern_ptr);
    CALL(virtual_string)->free(source_ptr);
    u64 error_ptr = CALL(virtual_string)->strncpy(quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_a_b_2_overflow, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 pattern_ptr = CALL(virtual_string)->load("h");
    u64 source_ptr = CALL(virtual_string)->load("abcdefgh");
    u64 quantum_str_ptr1 = CALL(virtual_string)->offset(source_ptr, pattern_ptr);
    u64 error_ptr = CALL(virtual_string)->strncpy(quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_1_2, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->strncpy(1, 2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_1_3, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->strncpy(1, 3);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_2_1, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 data_ptr = CALL(virtual_string)->strncpy(2, 1);
    RX_ASSERT(data_ptr != 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_a_string_free, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    CALL(virtual_string)->free(quantum_str_ptr2);
    u64 error_ptr = CALL(virtual_string)->strncpy(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_string_free_b, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("b");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    CALL(virtual_string)->free(quantum_str_ptr1);
    u64 error_ptr = CALL(virtual_string)->strncpy(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_string_list, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 quantum_str_ptr1 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr1 != 0);
    u64 list_ptr2 = CALL(virtual_stack)->alloc();
    u64 error_ptr = CALL(virtual_string)->strncpy(quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_list_string, .fixture = test_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr1 = CALL(virtual_stack)->alloc();
    u64 quantum_str_ptr2 = CALL(virtual_string)->load("a");
    ASSERT_DEBUG(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(virtual_string)->strncpy(list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

static void parse_text(u64 text_string_ptr) {
    u64 gc_ptr = CALL(virtual_stack)->alloc();
    u64 text_size = CALL(virtual_string)->size(text_string_ptr);
    if (text_string_ptr == 0) {
        return;
    }
    u64 list_ptr = CALL(virtual_stack)->alloc();
    char* text = CALL(virtual_string)->unsafe(text_string_ptr);
    char* tmp = text;
    while (*tmp != 0 && text_size > 0) {
        while (*tmp != 0 && *tmp != '\n' && text_size > 0) {
            tmp++;
            text_size--;
        }
        if (text_size == 0) {
            CALL(virtual_stack)->free(list_ptr);
            return;
        }
        *tmp++ = '\0';
        text_size--;
        u64 string_ptr = CALL(virtual_string)->load(text);
        CALL(virtual_stack)->push(list_ptr, string_ptr);
        text = tmp;
    }
    u64 data_ptr = 0;
    u64 list_data_ptr = CALL(virtual_stack)->alloc();
    while ((data_ptr = CALL(virtual_stack)->pop(list_ptr)) != 0) {
        CALL(virtual_stack)->push(list_data_ptr, data_ptr);
    }
    CALL(virtual_stack)->free(list_ptr);
    CALL(virtual_stack)->push(gc_ptr, list_data_ptr);
    u64 quit = 0;
    while (quit == 0) {
        u64 string_ptr = CALL(virtual_stack)->pop(list_data_ptr);
        if (CALL(virtual_string)->size(string_ptr) == 0) {
            quit = 1;
            continue;
        }
        CALL(os)->putc(string_ptr);
        u64 pattern_ptr = CALL(virtual_stack)->pop(list_data_ptr);
        if (CALL(virtual_string)->size(pattern_ptr) == 0) {
            quit = 1;
            continue;
        }
        CALL(os)->putc(pattern_ptr);
        u64 size = CALL(virtual_string)->size(pattern_ptr);
        u64 string_pointer_ptr = 0;
        u64 current_ptr = string_ptr;
        while ((string_pointer_ptr = CALL(virtual_string)->strchr(current_ptr, pattern_ptr)) != 0) {
            u64 match_ptr = CALL(virtual_string)->match(string_pointer_ptr, pattern_ptr);
            if (match_ptr == 0) {
                break;
            }
            if (CALL(virtual_string)->lessthan(string_pointer_ptr, match_ptr)) {
                u64 match_start_ptr = CALL(virtual_string)->left(match_ptr, size);
                u64 str_ncpy = CALL(virtual_string)->strncpy(match_start_ptr, size);
                u64 distance = CALL(virtual_string)->lessthan(string_ptr, match_start_ptr);
                if (distance > 0) {
                    u64 i = 0;
                    while (i++ < distance) {
                        printf(" ");
                    }
                }
                printf("%s[%lld]\n", CALL(virtual_string)->unsafe(str_ncpy), distance);
                CALL(virtual_string_pointer)->free(match_start_ptr);
                CALL(virtual_string_pointer)->free(str_ncpy);
            }
            CALL(virtual_string_pointer)->free(string_pointer_ptr);
            current_ptr = match_ptr;
        }
    }
    CALL(virtual_stack)->release(gc_ptr);
    CALL(virtual_stack)->free(gc_ptr);
}

static int run(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- rexo unit test code\n");
#endif
    /* Execute the main function that runs the test cases found. */
    int result = rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;
    return result;
}

const pointer_test_suite PRIVATE_API(pointer_test_suite_definitions) = {
    .run = run
};
