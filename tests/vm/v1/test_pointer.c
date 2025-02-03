/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   February 3, 2025 at 10:17:20 PM GMT+3
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

#include "test_pointer.h"

#include "vm/v1/os/os_v1.h"
#include "vm/v1/pointer/pointer_v1.h"
#include "vm/v1/types/stack/stack_v1.h"
#include "vm/v1/vm_v1.h"

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
    os_v1->putc(0);
}

/* test init */
RX_TEST_CASE(tests_v1, test_load_0, .fixture = test_fixture) {
    u64 empty_ptr = type_string_v1->load("\0");
    RX_ASSERT(empty_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_v1, test_load_null_ptr, .fixture = test_fixture) {
    u64 null_ptr = 0;
#ifndef USE_GC
    pointer_v1->free(null_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_v1, test_load_null, .fixture = test_fixture) {
    u64 empty_ptr = type_string_v1->load(0);
    RX_ASSERT(empty_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_v1, test_load_empty, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 empty_ptr = type_string_v1->load("");
    RX_ASSERT(empty_ptr == 0);
#ifndef USE_GC
    type_string_v1->free(empty_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_print_load_empty, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 empty_ptr = type_string_v1->load("");
    os_v1->putc(empty_ptr);
    RX_ASSERT(empty_ptr == 0);
#ifndef USE_GC
    type_string_v1->free(empty_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_load_copy, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 char_ptr = type_string_v1->load("/");
    u64 copy_ptr = type_string_v1->copy(char_ptr);
    RX_ASSERT(char_ptr != 0);
    RX_ASSERT(copy_ptr != 0);
    RX_ASSERT(char_ptr != copy_ptr);
    RX_ASSERT(strcmp(type_string_v1->unsafe(char_ptr), "/") == 0);
    RX_ASSERT(strcmp(type_string_v1->unsafe(copy_ptr), "/") == 0);
#ifndef USE_GC
    type_string_v1->free(char_ptr);
    type_string_v1->free(copy_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_load_push_peek_pop, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 char_ptr = type_string_v1->load("/");
    RX_ASSERT(strcmp(type_string_v1->unsafe(char_ptr), "/") == 0);
#ifndef USE_GC
    type_string_v1->free(char_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

RX_TEST_CASE(tests_v1, test_load_free_free, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 char_ptr = type_string_v1->load("/");
    RX_ASSERT(strcmp(type_string_v1->unsafe(char_ptr), "/") == 0);
#ifndef USE_GC
    type_string_v1->free(char_ptr);
    type_string_v1->free(char_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_strcat_load_alloc, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 char_ptr = type_string_v1->load("/");
    u64 pattern_ptr = type_string_v1->load("\0");
    type_string_v1->strcat(pattern_ptr, char_ptr);
    RX_ASSERT(pattern_ptr == 0);
    RX_ASSERT(strcmp(type_string_v1->unsafe(char_ptr), "/") == 0);
#ifndef USE_GC
    type_string_v1->free(char_ptr);
    type_string_v1->free(pattern_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_size, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 char_ptr = type_string_v1->load("/");
    u64 pattern_ptr = type_string_v1->load("\0");
    u64 string_size = type_data_v1->size(pattern_ptr);
    RX_ASSERT(string_size == 0);
    RX_ASSERT(pattern_ptr == 0);
    RX_ASSERT(strcmp(type_string_v1->unsafe(char_ptr), "/") == 0);
#ifndef USE_GC
    type_string_v1->free(char_ptr);
    type_string_v1->free(pattern_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_data_size, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 data_ptr = type_data_v1->alloc(2);
    u64 data_size = type_data_v1->size(data_ptr);
    RX_ASSERT(data_size == 2);
#ifndef USE_GC
    type_data_v1->free(data_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_pointer_size_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 char_ptr = type_string_v1->load("/");
    u64 pattern_ptr = type_string_v1->load("\0");
    u64 string_size = type_string_v1->size(0);
    RX_ASSERT(string_size == 0);
    RX_ASSERT(pattern_ptr == 0);
    RX_ASSERT(strcmp(type_string_v1->unsafe(char_ptr), "/") == 0);
#ifndef USE_GC
    type_string_v1->free(char_ptr);
    type_string_v1->free(pattern_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_pointer_size_empty, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 char_ptr = type_string_v1->load(" ");
    u64 pattern_ptr = type_string_v1->load("\0");
    u64 string_size = type_string_v1->size(char_ptr);
    RX_ASSERT(string_size == 1);
    RX_ASSERT(pattern_ptr == 0);
    RX_ASSERT(strcmp(type_string_v1->unsafe(char_ptr), " ") == 0);
#ifndef USE_GC
    type_string_v1->free(char_ptr);
    type_string_v1->free(pattern_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_pointer_size_object, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 object_ptr = type_object_v1->alloc(1);
    u64 pattern_ptr = type_string_v1->load("\0");
    u64 object_size = type_object_v1->size(object_ptr);
    RX_ASSERT(object_size == 1);
    RX_ASSERT(object_ptr != 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    type_string_v1->free(object_ptr);
    type_string_v1->free(pattern_ptr);
    type_object_v1->free(object_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_object_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 object_ptr = type_object_v1->alloc(0);
    u64 pattern_ptr = type_string_v1->load("\0");
    u64 object_size = type_object_v1->size(object_ptr);
    RX_ASSERT(object_size == 0);
    RX_ASSERT(object_ptr == 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    type_string_v1->free(object_ptr);
    type_string_v1->free(pattern_ptr);
    type_object_v1->free(object_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_object_alloc_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 object_ptr = type_object_v1->alloc(0);
    u64 pattern_ptr = type_string_v1->load("\0");
    u64 object_size = type_object_v1->size(object_ptr);
    RX_ASSERT(object_size == 0);
    RX_ASSERT(object_ptr == 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    type_string_v1->free(object_ptr);
    type_string_v1->free(pattern_ptr);
    type_object_v1->free(object_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_object_load_free, .fixture = test_fixture) {
    pointer_v1->init(8);
    const char* test_data = "Hello, world!";
    const void* ptr = (const void*)test_data;
    u64 object_ptr = type_object_v1->load(ptr, strlen(ptr));
    type_object_v1->free(object_ptr);
#ifndef USE_GC
    type_string_v1->free(object_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_object_free_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 object_ptr = 0;
    type_object_v1->free(object_ptr);
#ifndef USE_GC
    type_string_v1->free(object_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_object_load_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 object_ptr = type_object_v1->load(0, 0x01234567);
    u64 pattern_ptr = type_string_v1->load("\0");
    u64 object_size = type_object_v1->size(object_ptr);
    RX_ASSERT(object_size == 0);
    RX_ASSERT(object_ptr == 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    type_string_v1->free(object_ptr);
    type_string_v1->free(pattern_ptr);
    type_object_v1->free(object_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_object_load_string_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    const char* test_data = "Hello, world!";
    const void* ptr = (const void*)test_data;
    u64 object_ptr = type_object_v1->load(ptr, 0);
    u64 pattern_ptr = type_string_v1->load("\0");
    u64 object_size = type_object_v1->size(object_ptr);
    RX_ASSERT(object_size == 0);
    RX_ASSERT(object_ptr == 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    type_string_v1->free(object_ptr);
    type_string_v1->free(pattern_ptr);
    type_object_v1->free(object_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_object_load_string, .fixture = test_fixture) {
    pointer_v1->init(8);
    const char* test_data = "Hello, world!";
    const void* ptr = (const void*)test_data;
    u64 object_ptr = type_object_v1->load(ptr, strlen(ptr));
    u64 pattern_ptr = type_string_v1->load("\0");
    u64 object_size = type_object_v1->size(object_ptr);
    RX_ASSERT(object_size == strlen(ptr));
    RX_ASSERT(object_ptr != 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    type_string_v1->free(object_ptr);
    type_string_v1->free(pattern_ptr);
    type_object_v1->free(object_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_object_load_string_unsafe_alloc_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 object_ptr = type_object_v1->alloc(0);
    u64 pattern_ptr = type_string_v1->load("\0");
    u64 object_size = type_object_v1->size(object_ptr);
    const char* ch = (char*)type_object_v1->unsafe(object_ptr);
    RX_ASSERT(ch == 0);
    RX_ASSERT(object_size == 0);
    RX_ASSERT(object_ptr == 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    type_string_v1->free(object_ptr);
    type_string_v1->free(pattern_ptr);
    type_object_v1->free(object_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_object_load_string_unsafe_string, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 object_ptr = type_object_v1->alloc(0);
    u64 pattern_ptr = type_string_v1->load("\0");
    u64 object_size = type_object_v1->size(object_ptr);
    const char* ch = (char*)type_object_v1->unsafe(pattern_ptr);
    RX_ASSERT(ch == 0);
    RX_ASSERT(object_size == 0);
    RX_ASSERT(object_ptr == 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    type_string_v1->free(object_ptr);
    type_string_v1->free(pattern_ptr);
    type_object_v1->free(object_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_object_load_string_unsafe_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    const char* test_data = "Hello, world!";
    const void* ptr = (const void*)test_data;
    u64 object_ptr = type_object_v1->load(ptr, strlen(ptr));
    u64 pattern_ptr = type_string_v1->load("\0");
    u64 object_size = type_object_v1->size(object_ptr);
    const char* ch = (char*)type_object_v1->unsafe(0);
    RX_ASSERT(ch == 0);
    RX_ASSERT(object_size == strlen(ptr));
    RX_ASSERT(object_ptr != 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    type_string_v1->free(object_ptr);
    type_string_v1->free(pattern_ptr);
    type_object_v1->free(object_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_object_load_string_unsafe, .fixture = test_fixture) {
    pointer_v1->init(8);
    const char* test_data = "Hello, world!";
    const void* ptr = (const void*)test_data;
    u64 object_ptr = type_object_v1->load(ptr, strlen(ptr));
    u64 pattern_ptr = type_string_v1->load("\0");
    u64 object_size = type_object_v1->size(object_ptr);
    char* ch = (char*)type_object_v1->unsafe(object_ptr);
    *ch = 'Z';
    RX_ASSERT(object_size == strlen(ptr));
    RX_ASSERT(object_ptr != 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    type_string_v1->free(object_ptr);
    type_string_v1->free(pattern_ptr);
    type_object_v1->free(object_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_strcat_alloc_alloc, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 char_ptr = type_string_v1->load("\0");
    u64 pattern_ptr = type_string_v1->load("\0");
    type_string_v1->strcat(pattern_ptr, char_ptr);
    RX_ASSERT(char_ptr == 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    type_string_v1->free(char_ptr);
    type_string_v1->free(pattern_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_offset_ptr_diff, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 string_ptr1 = type_string_v1->load("192.168.0.111");
    u64 string_ptr2 = type_string_v1->load("192.168.0.12");
    u64 last_matched_ptr1 = type_string_v1->offset(string_ptr1, string_ptr2);
    RX_ASSERT(last_matched_ptr1 == 0);
#ifndef USE_GC
    type_string_v1->free(last_matched_ptr1);
    type_string_v1->free(string_ptr1);
    type_string_v1->free(string_ptr2);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_offset_ptr_less, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 string_ptr1 = type_string_v1->load("192.168.0.11");
    u64 string_ptr2 = type_string_v1->load("8.0.112");
    u64 last_matched_ptr1 = type_string_v1->offset(string_ptr1, string_ptr2);
    RX_ASSERT(last_matched_ptr1 == 0);
#ifndef USE_GC
    type_string_v1->free(last_matched_ptr1);
    type_string_v1->free(string_ptr1);
    type_string_v1->free(string_ptr2);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_offset_ptr_offset, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 string_ptr1 = type_string_v1->load("192.168.0.11");
    u64 string_ptr2 = type_string_v1->load("8.0.12");
    u64 last_matched_ptr1 = type_string_v1->offset(string_ptr1, string_ptr2);
    RX_ASSERT(last_matched_ptr1 == 0);
#ifndef USE_GC
    type_string_v1->free(last_matched_ptr1);
    type_string_v1->free(string_ptr1);
    type_string_v1->free(string_ptr2);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_offset_ptr_offset_found, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 string_ptr1 = type_string_v1->load("192.168.0.111");
    u64 string_ptr2 = type_string_v1->load("8.0.");
    u64 last_matched_ptr1 = type_string_v1->offset(string_ptr1, string_ptr2);
    RX_ASSERT(strcmp(type_string_v1->unsafe(last_matched_ptr1), "111") == 0);
#ifndef USE_GC
    type_string_v1->free(last_matched_ptr1);
    type_string_v1->free(string_ptr1);
    type_string_v1->free(string_ptr2);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_free_list, .fixture = test_fixture) {
    pointer_v1->init(8);
#ifndef USE_GC
    u64 list_ptr = stack_v1->alloc();
    u64 string_ptr = type_string_v1->load("hello");
    u64 e_ptr = type_string_v1->load("e");
    u64 string_pointer_ptr = type_string_v1->offset(string_ptr, e_ptr);
    type_string_v1->free(list_ptr);
    type_string_v1->free(string_pointer_ptr);
    type_string_v1->free(string_ptr);
    type_string_v1->free(e_ptr);
    stack_v1->free(list_ptr);
#else
    stack_v1->alloc();
    u64 string_ptr = type_string_v1->load("hello");
    u64 e_ptr = type_string_v1->load("e");
    type_string_v1->offset(string_ptr, e_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_free, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr = stack_v1->alloc();
    u64 string_ptr = type_string_v1->load("hello");
    u64 e_ptr = type_string_v1->load("e");
    u64 string_pointer_ptr = type_string_v1->offset(string_ptr, e_ptr);
    stack_v1->push(list_ptr, string_pointer_ptr);
    stack_v1->push(list_ptr, string_ptr);
#ifndef USE_GC
    type_string_v1->free(e_ptr);
    stack_v1->free(list_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_offset_list, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr = stack_v1->alloc();
    u64 string_ptr = type_string_v1->load("hello");
    u64 last_matched_ptr = type_string_v1->offset(list_ptr, string_ptr);
    RX_ASSERT(last_matched_ptr == 0);
#ifndef USE_GC
    type_string_v1->free(string_ptr);
    stack_v1->free(list_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_match, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr = stack_v1->alloc();
    u64 string_ptr = type_string_v1->load("192.192.0.1");
    u64 dot_ptr = type_string_v1->load("192.");
    u64 string_pointer_ptr1 = type_string_v1->match(string_ptr, dot_ptr);
    u64 string_pointer_ptr2 = type_string_v1->match(string_pointer_ptr1, dot_ptr);
    RX_ASSERT(strcmp(type_string_v1->unsafe(string_pointer_ptr1), "192.0.1") == 0);
    RX_ASSERT(strcmp(type_string_v1->unsafe(string_pointer_ptr2), "0.1") == 0);
    stack_v1->push(list_ptr, string_pointer_ptr1);
    stack_v1->push(list_ptr, string_pointer_ptr2);
    stack_v1->push(list_ptr, string_ptr);
#ifndef USE_GC
    type_string_v1->free(dot_ptr);
    stack_v1->free(list_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_match_miss, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr = stack_v1->alloc();
    u64 string_ptr = type_string_v1->load("192.168.0.1");
    u64 dot_ptr = type_string_v1->load(":");
    u64 string_pointer_ptr1 = type_string_v1->match(string_ptr, dot_ptr);
    RX_ASSERT(string_pointer_ptr1 == 0);
    stack_v1->push(list_ptr, string_pointer_ptr1);
    stack_v1->push(list_ptr, string_ptr);
#ifndef USE_GC
    type_string_v1->free(dot_ptr);
    stack_v1->free(list_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_match_pattern, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr = stack_v1->alloc();
    u64 string_ptr = type_string_v1->load("192.168.0.112");
    u64 dot_ptr = type_string_v1->load("192.168.0.");
    u64 string_pointer_ptr1 = type_string_v1->match(string_ptr, dot_ptr);
    RX_ASSERT(strcmp(type_string_v1->unsafe(string_pointer_ptr1), "112") == 0);
    stack_v1->push(list_ptr, string_pointer_ptr1);
    stack_v1->push(list_ptr, string_ptr);
#ifndef USE_GC
    type_string_v1->free(dot_ptr);
    stack_v1->free(list_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_match_pattern_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr = stack_v1->alloc();
    u64 string_ptr = type_string_v1->load("192.168.0.112");
    u64 dot_ptr = type_string_v1->load("193");
    u64 string_pointer_ptr1 = type_string_v1->match(string_ptr, dot_ptr);
    RX_ASSERT(string_pointer_ptr1 == 0);
    stack_v1->push(list_ptr, string_pointer_ptr1);
    stack_v1->push(list_ptr, string_ptr);
#ifndef USE_GC
    type_string_v1->free(dot_ptr);
    stack_v1->free(list_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_match_pattern_none, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr = stack_v1->alloc();
    u64 string_ptr = type_string_v1->load("192.168.0.11");
    u64 dot_ptr = type_string_v1->load("192.168.0.111");
    u64 string_pointer_ptr1 = type_string_v1->match(string_ptr, dot_ptr);
    RX_ASSERT(string_pointer_ptr1 == 0);
    stack_v1->push(list_ptr, string_pointer_ptr1);
    stack_v1->push(list_ptr, string_ptr);
#ifndef USE_GC
    type_string_v1->free(dot_ptr);
    stack_v1->free(list_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_release_string, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 string_ptr = type_string_v1->load("192.168.0.11");
    stack_v1->release(string_ptr);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_match_pattern_load_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 string_ptr = type_string_v1->load("192.168.0.1");
    u64 dot_ptr = type_string_v1->load("57.34.7.2");
    u64 error_ptr = type_string_v1->match(string_ptr, dot_ptr);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_offset_pattern_load_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 string_ptr = type_string_v1->load("192.168.0.1");
    u64 dot_ptr = type_string_v1->load("57.34.7.2");
    u64 error_ptr = type_string_v1->offset(string_ptr, dot_ptr);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_match_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 string_ptr = type_string_v1->load("192.168.0.1");
    u64 string_pointer_ptr = type_string_v1->match(string_ptr, 0);
    RX_ASSERT(string_pointer_ptr == 0);
#ifndef USE_GC
    type_string_v1->free(string_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_match_0_0, .fixture = test_fixture) {
    pointer_v1->init(8);
#ifndef USE_GC
    u64 string_ptr = type_string_v1->load("192.168.0.1");
    u64 string_pointer_ptr = type_string_v1->match(0, 0);
    RX_ASSERT(string_pointer_ptr == 0);
    type_string_v1->free(string_ptr);
#else
    type_string_v1->load("192.168.0.1");
    u64 string_pointer_ptr = type_string_v1->match(0, 0);
    RX_ASSERT(string_pointer_ptr == 0);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_match_0_string, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 string_ptr = type_string_v1->load("192.168.0.1");
    u64 string_pointer_ptr = type_string_v1->match(0, string_ptr);
    RX_ASSERT(string_pointer_ptr == 0);
#ifndef USE_GC
    type_string_v1->free(string_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_match_list, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr = stack_v1->alloc();
    u64 string_ptr = type_string_v1->load("hello");
    u64 last_matched_ptr = type_string_v1->match(list_ptr, string_ptr);
    RX_ASSERT(last_matched_ptr == 0);
#ifndef USE_GC
    type_string_v1->free(string_ptr);
    stack_v1->free(list_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_strchr, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr = stack_v1->alloc();
    u64 string_ptr = type_string_v1->load("192.168.0.1");
    u64 dot_ptr = type_string_v1->load(".");
    u64 string_pointer_ptr1 = type_string_v1->strchr(string_ptr, dot_ptr);
    u64 string_pointer_ptr2 = type_string_v1->match(string_pointer_ptr1, dot_ptr);
    RX_ASSERT(strcmp(type_string_v1->unsafe(string_pointer_ptr1), ".168.0.1") == 0);
    RX_ASSERT(strcmp(type_string_v1->unsafe(string_pointer_ptr2), "168.0.1") == 0);
    stack_v1->push(list_ptr, string_pointer_ptr1);
    stack_v1->push(list_ptr, string_pointer_ptr2);
    stack_v1->push(list_ptr, string_ptr);
#ifndef USE_GC
    type_string_v1->free(dot_ptr);
    stack_v1->free(list_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_strchr_miss, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr = stack_v1->alloc();
    u64 string_ptr = type_string_v1->load("192.168.0.1");
    u64 dot_ptr = type_string_v1->load(":");
    u64 string_pointer_ptr1 = type_string_v1->strchr(string_ptr, dot_ptr);
    RX_ASSERT(string_pointer_ptr1 == 0);
    stack_v1->push(list_ptr, string_pointer_ptr1);
    stack_v1->push(list_ptr, string_ptr);
#ifndef USE_GC
    type_string_v1->free(dot_ptr);
    stack_v1->free(list_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_strchr_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 string_ptr = type_string_v1->load("192.168.0.1");
    u64 string_pointer_ptr = type_string_v1->strchr(string_ptr, 0);
    RX_ASSERT(string_pointer_ptr == 0);
#ifndef USE_GC
    type_string_v1->free(string_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_strchr_0_0, .fixture = test_fixture) {
    pointer_v1->init(8);
#ifndef USE_GC
    u64 string_ptr = type_string_v1->load("192.168.0.1");
    u64 string_pointer_ptr = type_string_v1->strchr(0, 0);
    RX_ASSERT(string_pointer_ptr == 0);
    type_string_v1->free(string_ptr);
#else
    type_string_v1->load("192.168.0.1");
    u64 string_pointer_ptr = type_string_v1->strchr(0, 0);
    RX_ASSERT(string_pointer_ptr == 0);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_strchr_0_string, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 string_ptr = type_string_v1->load("192.168.0.1");
    u64 string_pointer_ptr = type_string_v1->strchr(0, string_ptr);
    RX_ASSERT(string_pointer_ptr == 0);
#ifndef USE_GC
    type_string_v1->free(string_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_strchr_list, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr = stack_v1->alloc();
    u64 string_ptr = type_string_v1->load("hello");
    u64 last_matched_ptr = type_string_v1->strchr(list_ptr, string_ptr);
    RX_ASSERT(last_matched_ptr == 0);
#ifndef USE_GC
    type_string_v1->free(string_ptr);
    stack_v1->free(list_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_strrchr, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 string_ptr = type_string_v1->load("192.168.0.1");
    u64 dot_ptr = type_string_v1->load(".");
    u64 string_pointer_ptr1 = type_string_v1->strrchr(string_ptr, dot_ptr);
    u64 string_pointer_ptr2 = type_string_v1->match(string_pointer_ptr1, dot_ptr);
    RX_ASSERT(strcmp(type_string_v1->unsafe(string_pointer_ptr1), ".1") == 0);
    RX_ASSERT(strcmp(type_string_v1->unsafe(string_pointer_ptr2), "1") == 0);
#ifndef USE_GC
    type_string_v1->free(string_pointer_ptr1);
    type_string_v1->free(string_pointer_ptr2);
    type_string_v1->free(string_ptr);
    type_string_v1->free(dot_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_strrchr_match_offset, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 string_ptr = type_string_v1->load("192.168.0.111");
    u64 dot_ptr = type_string_v1->load(".");
    u64 string_pointer_ptr1 = type_string_v1->strrchr(string_ptr, dot_ptr);
    u64 string_pointer_ptr2 = type_string_v1->match(string_pointer_ptr1, dot_ptr);
    u64 string_pointer_ptr3 = type_string_v1->offset(string_ptr, dot_ptr);
    RX_ASSERT(strcmp(type_string_v1->unsafe(string_pointer_ptr1), ".111") == 0);
    RX_ASSERT(strcmp(type_string_v1->unsafe(string_pointer_ptr2), "111") == 0);
    RX_ASSERT(strcmp(type_string_v1->unsafe(string_pointer_ptr3), "111") == 0);
#ifndef USE_GC
    type_string_v1->free(string_pointer_ptr1);
    type_string_v1->free(string_pointer_ptr2);
    type_string_v1->free(string_pointer_ptr3);
    type_string_v1->free(string_ptr);
    type_string_v1->free(dot_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_peekn_0_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr = stack_v1->alloc();
    u64 error_ptr = stack_v1->peekn(0, 0);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_peekn_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr = stack_v1->alloc();
    u64 error_ptr = stack_v1->peekn(list_ptr, 0);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_peekn_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr1 = stack_v1->alloc();
    u64 string_ptr1 = type_string_v1->load("192.168.0.1");
    stack_v1->push(list_ptr1, string_ptr1);
    u64 list_ptr2 = stack_v1->peekn(list_ptr1, 1);
    RX_ASSERT(list_ptr2 != 0);
    RX_ASSERT(stack_v1->size(list_ptr2) != 0);
    u64 string_ptr2 = stack_v1->peek(list_ptr2);
    RX_ASSERT(string_ptr2 != 0);
    RX_ASSERT(string_ptr1 == string_ptr2);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_peekn_2, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 string_ptr = type_string_v1->load("192.168.0.1");
    u64 list_ptr = stack_v1->alloc();
    stack_v1->push(list_ptr, string_ptr);
    u64 error_ptr = stack_v1->peekn(list_ptr, 2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_popn_0_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr = stack_v1->alloc();
    u64 error_ptr = stack_v1->popn(0, 0);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_popn_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr = stack_v1->alloc();
    u64 error_ptr = stack_v1->popn(list_ptr, 0);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_popn_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr1 = stack_v1->alloc();
    u64 string_ptr1 = type_string_v1->load("192.168.0.1");
    stack_v1->push(list_ptr1, string_ptr1);
    u64 list_ptr2 = stack_v1->popn(list_ptr1, 1);
    RX_ASSERT(list_ptr2 != 0);
    RX_ASSERT(stack_v1->size(list_ptr2) != 0);
    u64 string_ptr2 = stack_v1->peek(list_ptr2);
    RX_ASSERT(string_ptr2 != 0);
    RX_ASSERT(string_ptr1 == string_ptr2);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_popn_2, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 string_ptr = type_string_v1->load("192.168.0.1");
    u64 list_ptr = stack_v1->alloc();
    stack_v1->push(list_ptr, string_ptr);
    u64 error_ptr = stack_v1->popn(list_ptr, 2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_strchr_match_offset, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr = stack_v1->alloc();
    u64 list_match_ptr = stack_v1->alloc();
    u64 string_ptr = type_string_v1->load("a.bc.bb.ba.a");
    u64 pattern_ptr = type_string_v1->load(".b");
    u64 string_pointer_ptr = 0;
    u64 match_ptr = string_ptr;
    u64 current_ptr = match_ptr;
    while ((string_pointer_ptr = type_string_v1->strchr(current_ptr, pattern_ptr)) != 0) {
        stack_v1->push(list_ptr, string_pointer_ptr);
        match_ptr = type_string_v1->match(string_pointer_ptr, pattern_ptr);
        stack_v1->push(list_match_ptr, match_ptr);
        current_ptr = match_ptr;
    }
    u64 size = stack_v1->size(list_ptr);
    RX_ASSERT(size == 4);
    u64 string_ptr1 = stack_v1->pop(list_ptr);
    u64 string_ptr2 = stack_v1->pop(list_ptr);
    u64 string_ptr3 = stack_v1->pop(list_ptr);
    u64 string_ptr4 = stack_v1->pop(list_ptr);
    RX_ASSERT(strcmp(type_string_v1->unsafe(string_ptr1), ".a") == 0);
    RX_ASSERT(strcmp(type_string_v1->unsafe(string_ptr2), ".ba.a") == 0);
    RX_ASSERT(strcmp(type_string_v1->unsafe(string_ptr3), ".bb.ba.a") == 0);
    RX_ASSERT(strcmp(type_string_v1->unsafe(string_ptr4), ".bc.bb.ba.a") == 0);
    RX_ASSERT(stack_v1->size(list_ptr) == 0);
    u64 match_size = stack_v1->size(list_match_ptr);
    RX_ASSERT(match_size == 3);
    u64 string_match_ptr1 = stack_v1->pop(list_match_ptr);
    u64 string_match_ptr2 = stack_v1->pop(list_match_ptr);
    u64 string_match_ptr3 = stack_v1->pop(list_match_ptr);
    RX_ASSERT(strcmp(type_string_v1->unsafe(string_match_ptr1), "a.a") == 0);
    RX_ASSERT(strcmp(type_string_v1->unsafe(string_match_ptr2), "b.ba.a") == 0);
    RX_ASSERT(strcmp(type_string_v1->unsafe(string_match_ptr3), "c.bb.ba.a") == 0);
    RX_ASSERT(stack_v1->size(list_match_ptr) == 0);
#ifndef USE_GC
    type_string_v1->free(string_ptr1);
    type_string_v1->free(string_ptr2);
    type_string_v1->free(string_ptr3);
    type_string_v1->free(string_ptr4);
    type_string_v1->free(string_match_ptr1);
    type_string_v1->free(string_match_ptr2);
    type_string_v1->free(string_match_ptr3);
    type_string_v1->free(string_ptr);
    type_string_v1->free(pattern_ptr);
    stack_v1->free(list_match_ptr);
    stack_v1->free(list_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_strrchr_miss, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr = stack_v1->alloc();
    u64 string_ptr = type_string_v1->load("192.168.0.1");
    u64 dot_ptr = type_string_v1->load(":");
    u64 string_pointer_ptr1 = type_string_v1->strrchr(string_ptr, dot_ptr);
    RX_ASSERT(string_pointer_ptr1 == 0);
    stack_v1->push(list_ptr, string_pointer_ptr1);
    stack_v1->push(list_ptr, string_ptr);
#ifndef USE_GC
    type_string_v1->free(dot_ptr);
    stack_v1->free(list_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_strrchr_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 string_ptr = type_string_v1->load("192.168.0.1");
    u64 string_pointer_ptr = type_string_v1->strrchr(string_ptr, 0);
    RX_ASSERT(string_pointer_ptr == 0);
#ifndef USE_GC
    type_string_v1->free(string_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_strrchr_0_0, .fixture = test_fixture) {
    pointer_v1->init(8);
#ifndef USE_GC
    u64 string_ptr = type_string_v1->load("192.168.0.1");
    u64 string_pointer_ptr = type_string_v1->strrchr(0, 0);
    RX_ASSERT(string_pointer_ptr == 0);
    type_string_v1->free(string_ptr);
#else
    type_string_v1->load("192.168.0.1");
    u64 string_pointer_ptr = type_string_v1->strrchr(0, 0);
    RX_ASSERT(string_pointer_ptr == 0);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_strrchr_0_string, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 string_ptr = type_string_v1->load("192.168.0.1");
    u64 string_pointer_ptr = type_string_v1->strrchr(0, string_ptr);
    RX_ASSERT(string_pointer_ptr == 0);
#ifndef USE_GC
    type_string_v1->free(string_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_strrchr_list, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr = stack_v1->alloc();
    u64 string_ptr = type_string_v1->load("hello");
    u64 last_matched_ptr = type_string_v1->strrchr(list_ptr, string_ptr);
    RX_ASSERT(last_matched_ptr == 0);
#ifndef USE_GC
    type_string_v1->free(string_ptr);
    stack_v1->free(list_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_push_0_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    stack_v1->push(0, 1);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_push_1_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    stack_v1->push(1, 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_push_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr = stack_v1->alloc();
    stack_v1->push(list_ptr, 0);
#ifndef USE_GC
    stack_v1->free(list_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_push_list, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr = stack_v1->alloc();
    stack_v1->push(list_ptr, list_ptr);
#ifndef USE_GC
    stack_v1->free(list_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_push_string, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 string_ptr = type_string_v1->load("@");
    u64 list_ptr = stack_v1->alloc();
    stack_v1->push(string_ptr, list_ptr);
#ifndef USE_GC
    stack_v1->free(list_ptr);
    type_string_v1->free(string_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_size_string, .fixture = test_fixture) {
    pointer_v1->init(8);
#ifndef USE_GC
    u64 string_ptr = type_string_v1->load("@");
    u64 list_ptr = stack_v1->alloc();
    u64 value = stack_v1->size(string_ptr);
    RX_ASSERT(value == 0);
    stack_v1->free(list_ptr);
    type_string_v1->free(string_ptr);
#else
    u64 string_ptr = type_string_v1->load("@");
    stack_v1->alloc();
    u64 value = stack_v1->size(string_ptr);
    RX_ASSERT(value == 0);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_push_size, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 string_ptr = type_string_v1->load("@");
    u64 list_ptr = stack_v1->alloc();
    stack_v1->push(list_ptr, string_ptr);
    u64 value = stack_v1->size(list_ptr);
    RX_ASSERT(value != 0);
#ifndef USE_GC
    stack_v1->free(list_ptr);
    type_string_v1->free(string_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_pop_0_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 value_ptr = stack_v1->pop(0);
    RX_ASSERT(value_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_pop_1_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 value_ptr = stack_v1->pop(1);
    RX_ASSERT(value_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_pop_list, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr = stack_v1->alloc();
    u64 value_ptr = stack_v1->pop(list_ptr);
    RX_ASSERT(value_ptr == 0);
#ifndef USE_GC
    stack_v1->free(list_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_pop_string, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 string_ptr = type_string_v1->load("@");
    u64 list_ptr = stack_v1->alloc();
    stack_v1->push(list_ptr, string_ptr);
    u64 value_ptr = stack_v1->pop(list_ptr);
    RX_ASSERT(value_ptr != 0);
#ifndef USE_GC
    stack_v1->free(list_ptr);
    type_string_v1->free(string_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_peek_0_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 value_ptr = stack_v1->peek(0);
    RX_ASSERT(value_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_peek_1_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 value_ptr = stack_v1->peek(1);
    RX_ASSERT(value_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_peek_list, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr = stack_v1->alloc();
    u64 value_ptr = stack_v1->peek(list_ptr);
    RX_ASSERT(value_ptr == 0);
#ifndef USE_GC
    stack_v1->free(list_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_peek_string, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 string_ptr = type_string_v1->load("@");
    u64 list_ptr = stack_v1->alloc();
    stack_v1->push(list_ptr, string_ptr);
    u64 value_ptr = stack_v1->peek(list_ptr);
    RX_ASSERT(value_ptr != 0);
#ifndef USE_GC
    stack_v1->free(list_ptr);
    type_string_v1->free(string_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_strcpy, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 path_ptr1 = type_string_v1->load("/");
    u64 path_ptr2 = type_string_v1->load("@");
    u64 path_copy_ptr = type_string_v1->copy(path_ptr1);
    type_string_v1->strcat(path_copy_ptr, path_ptr2);

    char* path1 = type_string_v1->unsafe(path_ptr1);
    char* path2 = type_string_v1->unsafe(path_ptr2);
    u64 path1_len = strlen(path1);
    u64 path2_len = strlen(path2);
    RX_ASSERT(path1_len > 0);
    RX_ASSERT(path2_len > 0);
    char* buf = calloc(1, path1_len + path2_len + 1);
    strcpy(buf, path1); /* NOLINT */
    strcat(buf, path2); /* NOLINT */
    char* path_copy = type_string_v1->unsafe(path_copy_ptr);
    RX_ASSERT(strlen(path_copy) == strlen(buf));
    RX_ASSERT(strcmp(path_copy, buf) == 0);
    free(buf);
#ifndef USE_GC
    type_string_v1->free(path_ptr1);
    type_string_v1->free(path_ptr2);
    type_string_v1->free(path_copy_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_strcat_load_alloc_copy, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 string_ptr = type_string_v1->load("/all_english_words.txt");
    u64 zero_ptr = type_string_v1->load("\0");
    u64 data_ptr = type_data_v1->alloc(1);
    u64 list_ptr = stack_v1->alloc();
    u64 empty_ptr = type_string_v1->load("\0");

    u64 null_ptr = 0;
    u64 none_ptr = 0xfffffffff;

    u64 data_ptr1 = type_string_v1->copy(null_ptr);
    u64 data_ptr2 = type_string_v1->copy(zero_ptr);
    u64 data_ptr3 = type_string_v1->copy(string_ptr);
    u64 data_ptr4 = type_string_v1->copy(none_ptr);

    os_v1->putc(string_ptr);
    os_v1->putc(zero_ptr);
    os_v1->putc(data_ptr);
    os_v1->putc(list_ptr);
    os_v1->putc(empty_ptr);
    os_v1->putc(null_ptr);
    os_v1->putc(none_ptr);

    type_string_v1->put_char(string_ptr, 'a');
    type_string_v1->put_char(zero_ptr, 'a');
    type_string_v1->put_char(data_ptr, 'a');
    type_string_v1->put_char(list_ptr, 'a');
    type_string_v1->put_char(empty_ptr, 'a');
    type_string_v1->put_char(null_ptr, 'a');
    type_string_v1->put_char(none_ptr, 'a');

    type_string_v1->put_char(string_ptr, '\0');
    type_string_v1->put_char(zero_ptr, '\0');
    type_string_v1->put_char(data_ptr, '\0');
    type_string_v1->put_char(list_ptr, '\0');
    type_string_v1->put_char(empty_ptr, '\0');
    type_string_v1->put_char(null_ptr, '\0');
    type_string_v1->put_char(none_ptr, '\0');

    type_string_v1->put_char(string_ptr, '/');
    type_string_v1->strcpy(string_ptr, string_ptr);
    type_string_v1->strcpy(string_ptr, zero_ptr);
    type_string_v1->strcpy(zero_ptr, string_ptr);
    type_string_v1->strcpy(data_ptr, null_ptr);
    type_string_v1->strcpy(list_ptr, null_ptr);
    type_string_v1->strcpy(string_ptr, data_ptr);
    type_string_v1->strcpy(string_ptr, empty_ptr);
    type_string_v1->strcpy(data_ptr, null_ptr);
    type_string_v1->strcpy(list_ptr, string_ptr);
    type_string_v1->strcpy(data_ptr, null_ptr);
    type_string_v1->strcpy(list_ptr, null_ptr);
    type_string_v1->strcpy(string_ptr, data_ptr);
    type_string_v1->strcpy(data_ptr, null_ptr);
    type_string_v1->strcpy(list_ptr, string_ptr);
    type_string_v1->strcpy(null_ptr, null_ptr);
    type_string_v1->strcpy(string_ptr, null_ptr);
    type_string_v1->strcpy(string_ptr, none_ptr);
    type_string_v1->strcpy(none_ptr, string_ptr);

    type_string_v1->strcat(string_ptr, string_ptr);
    type_string_v1->strcat(string_ptr, zero_ptr);
    type_string_v1->strcat(zero_ptr, string_ptr);
    type_string_v1->strcat(data_ptr, null_ptr);
    type_string_v1->strcat(list_ptr, null_ptr);
    type_string_v1->strcat(string_ptr, data_ptr);
    type_string_v1->strcat(string_ptr, empty_ptr);
    type_string_v1->strcat(data_ptr, null_ptr);
    type_string_v1->strcat(list_ptr, string_ptr);
    type_string_v1->strcat(data_ptr, null_ptr);
    type_string_v1->strcat(list_ptr, null_ptr);
    type_string_v1->strcat(string_ptr, data_ptr);
    type_string_v1->strcat(data_ptr, null_ptr);
    type_string_v1->strcat(list_ptr, string_ptr);
    type_string_v1->strcat(null_ptr, null_ptr);
    type_string_v1->strcat(string_ptr, null_ptr);
    type_string_v1->strcat(string_ptr, none_ptr);
    type_string_v1->strcat(none_ptr, string_ptr);

    u64 last_matched_ptr1 = type_string_v1->offset(string_ptr, string_ptr);
    type_string_v1->offset(data_ptr, null_ptr);
    type_string_v1->offset(list_ptr, null_ptr);
    type_string_v1->offset(string_ptr, data_ptr);
    type_string_v1->offset(string_ptr, empty_ptr);
    type_string_v1->offset(data_ptr, null_ptr);
    type_string_v1->offset(list_ptr, string_ptr);
    type_string_v1->offset(data_ptr, null_ptr);
    type_string_v1->offset(list_ptr, null_ptr);
    type_string_v1->offset(string_ptr, data_ptr);
    type_string_v1->offset(data_ptr, null_ptr);
    type_string_v1->offset(list_ptr, string_ptr);
    type_string_v1->offset(null_ptr, null_ptr);
    type_string_v1->offset(string_ptr, null_ptr);

    u64 last_matched_ptr2 = type_string_v1->offset(string_ptr, string_ptr);
    type_string_v1->offset(string_ptr, zero_ptr);
    type_string_v1->offset(data_ptr, zero_ptr);
    type_string_v1->offset(list_ptr, zero_ptr);
    type_string_v1->offset(string_ptr, data_ptr);
    type_string_v1->offset(zero_ptr, zero_ptr);
    type_string_v1->offset(zero_ptr, data_ptr);
    type_string_v1->offset(zero_ptr, list_ptr);
    type_string_v1->offset(zero_ptr, string_ptr);
    type_string_v1->offset(empty_ptr, empty_ptr);
    type_string_v1->offset(empty_ptr, string_ptr);
    type_string_v1->offset(empty_ptr, zero_ptr);

    type_string_v1->offset(null_ptr, null_ptr);
    type_string_v1->offset(null_ptr, empty_ptr);
    type_string_v1->offset(null_ptr, string_ptr);
    type_string_v1->offset(null_ptr, data_ptr);
    type_string_v1->offset(null_ptr, list_ptr);
    type_string_v1->offset(empty_ptr, null_ptr);
    type_string_v1->offset(empty_ptr, empty_ptr);
    type_string_v1->offset(empty_ptr, string_ptr);
    type_string_v1->offset(empty_ptr, data_ptr);
    type_string_v1->offset(empty_ptr, list_ptr);
    type_string_v1->offset(string_ptr, null_ptr);
    type_string_v1->offset(string_ptr, empty_ptr);

    u64 last_matched_ptr3 = type_string_v1->offset(string_ptr, string_ptr);
    type_string_v1->offset(string_ptr, data_ptr);
    type_string_v1->offset(string_ptr, list_ptr);
    type_string_v1->offset(data_ptr, null_ptr);
    type_string_v1->offset(data_ptr, empty_ptr);
    type_string_v1->offset(data_ptr, string_ptr);
    type_string_v1->offset(data_ptr, data_ptr);
    type_string_v1->offset(data_ptr, list_ptr);
    type_string_v1->offset(list_ptr, null_ptr);
    type_string_v1->offset(list_ptr, empty_ptr);
    type_string_v1->offset(list_ptr, string_ptr);
    type_string_v1->offset(list_ptr, data_ptr);
    type_string_v1->offset(list_ptr, list_ptr);
    type_string_v1->offset(string_ptr, none_ptr);
    type_string_v1->offset(none_ptr, string_ptr);

    type_string_v1->unsafe(null_ptr);
    type_string_v1->unsafe(empty_ptr);
    type_string_v1->unsafe(string_ptr);
    type_string_v1->unsafe(data_ptr);
    type_string_v1->unsafe(list_ptr);
    type_string_v1->unsafe(none_ptr);

    type_file_v1->alloc(zero_ptr, zero_ptr);
    type_file_v1->alloc(string_ptr, zero_ptr);
    type_file_v1->alloc(data_ptr, zero_ptr);
    type_file_v1->alloc(list_ptr, zero_ptr);
    type_file_v1->alloc(zero_ptr, string_ptr);
    type_file_v1->alloc(zero_ptr, data_ptr);
    type_file_v1->alloc(zero_ptr, list_ptr);
    type_file_v1->alloc(string_ptr, string_ptr);
    type_file_v1->alloc(data_ptr, null_ptr);
    type_file_v1->alloc(list_ptr, null_ptr);
    type_file_v1->alloc(string_ptr, data_ptr);
    type_file_v1->alloc(string_ptr, empty_ptr);
    type_file_v1->alloc(data_ptr, null_ptr);
    type_file_v1->alloc(list_ptr, string_ptr);
    type_file_v1->alloc(data_ptr, null_ptr);
    type_file_v1->alloc(list_ptr, null_ptr);
    type_file_v1->alloc(string_ptr, data_ptr);
    type_file_v1->alloc(data_ptr, null_ptr);
    type_file_v1->alloc(list_ptr, string_ptr);
    type_file_v1->alloc(null_ptr, null_ptr);
    type_file_v1->alloc(string_ptr, null_ptr);
    type_file_v1->alloc(string_ptr, none_ptr);
    type_file_v1->alloc(none_ptr, string_ptr);

    const u64* data_unsafe_ptr1 = type_data_v1->unsafe(empty_ptr);
    const u64* data_unsafe_ptr2 = type_data_v1->unsafe(null_ptr);
    const u64* data_unsafe_ptr3 = type_data_v1->unsafe(string_ptr);
    const u64* data_unsafe_ptr4 = type_data_v1->unsafe(list_ptr);
    const u64* data_unsafe_ptr5 = type_data_v1->unsafe(none_ptr);

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

    u64 undefined_ptr = type_string_v1->copy(list_ptr);

    RX_ASSERT(undefined_ptr == 0);
    RX_ASSERT(last_matched_ptr1 != 0);
    RX_ASSERT(last_matched_ptr2 != 0);
    RX_ASSERT(last_matched_ptr3 != 0);
#ifndef USE_GC
    type_string_v1->free(last_matched_ptr1);
    type_string_v1->free(last_matched_ptr2);
    type_string_v1->free(last_matched_ptr3);
    type_string_v1->free(last_matched_ptr2);
    type_string_v1->free(last_matched_ptr3);
    type_string_v1->free(null_ptr);
    type_string_v1->free(zero_ptr);
    type_string_v1->free(string_ptr);
    type_string_v1->free(data_ptr3);
    type_string_v1->free(list_ptr);
    type_string_v1->free(empty_ptr);
    type_string_v1->free(data_ptr);
    type_string_v1->free(string_ptr);
    type_string_v1->free(data_ptr3);
    type_string_v1->free(empty_ptr);
    type_string_v1->free(data_ptr);
    type_data_v1->free(zero_ptr);
    type_data_v1->free(data_ptr);
    type_data_v1->free(data_ptr3);
    type_data_v1->free(empty_ptr);
    type_data_v1->free(null_ptr);
    type_data_v1->free(data_ptr);
    type_data_v1->free(list_ptr);
    type_data_v1->free(data_ptr);
    type_data_v1->free(data_ptr3);
    type_data_v1->free(none_ptr);
    stack_v1->free(zero_ptr);
    stack_v1->free(data_ptr);
    stack_v1->free(empty_ptr);
    stack_v1->free(null_ptr);
    stack_v1->free(data_ptr);
    stack_v1->free(list_ptr);
    stack_v1->free(data_ptr);
    stack_v1->free(list_ptr);
    stack_v1->free(none_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_strcat_alloc_load, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 zero_ptr = type_string_v1->load("\0");
    u64 char_ptr = type_string_v1->load("/");
    type_string_v1->strcat(zero_ptr, char_ptr);
    RX_ASSERT(zero_ptr == 0);
    RX_ASSERT(strcmp(type_string_v1->unsafe(char_ptr), "/") == 0);
#ifndef USE_GC
    type_string_v1->free(zero_ptr);
    type_string_v1->free(char_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_strcat_load_alloc_alloc, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 char_ptr = type_string_v1->load("/");
    u64 pattern_ptr = type_string_v1->load("\0");
    u64 empty_ptr = type_string_v1->load("\0");
    type_string_v1->strcat(pattern_ptr, char_ptr);
    type_string_v1->strcpy(empty_ptr, pattern_ptr);
    RX_ASSERT(pattern_ptr == 0);
    RX_ASSERT(empty_ptr == 0);
    RX_ASSERT(strcmp(type_string_v1->unsafe(char_ptr), "/") == 0);
#ifndef USE_GC
    type_string_v1->free(empty_ptr);
    type_string_v1->free(pattern_ptr);
    type_string_v1->free(char_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_strcat_load_load, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 char_ptr = type_string_v1->load("/");
    u64 pattern_ptr = type_string_v1->load("*");
    type_string_v1->strcat(pattern_ptr, char_ptr);
    RX_ASSERT(strcmp(type_string_v1->unsafe(pattern_ptr), "*/") == 0);
    RX_ASSERT(strcmp(type_string_v1->unsafe(char_ptr), "/") == 0);
#ifndef USE_GC
    type_string_v1->free(pattern_ptr);
    type_string_v1->free(char_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_strcpy_load_alloc, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("/");
    u64 empty_ptr = type_string_v1->load("\0");
    type_string_v1->strcpy(empty_ptr, pattern_ptr);
    RX_ASSERT(empty_ptr == 0);
    RX_ASSERT(strcmp(type_string_v1->unsafe(pattern_ptr), "/") == 0);
#ifndef USE_GC
    type_string_v1->free(empty_ptr);
    type_string_v1->free(pattern_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_strcpy_load_load, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 char_ptr = type_string_v1->load("/input.txt");
    u64 pattern_ptr = type_string_v1->load("*");
    type_string_v1->strcpy(pattern_ptr, char_ptr);
    RX_ASSERT(strcmp(type_string_v1->unsafe(pattern_ptr), "/input.txt") == 0);
    RX_ASSERT(strcmp(type_string_v1->unsafe(char_ptr), "/input.txt") == 0);
#ifndef USE_GC
    type_string_v1->free(pattern_ptr);
    type_string_v1->free(char_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_os_getenv, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 ui_mode_ptr = type_string_v1->load("UI_MODE");
    u64 file_path_ptr = os_v1->getenv(ui_mode_ptr);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_os_getenv_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 file_path_ptr = os_v1->getenv(0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_os_getenv_list, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr = stack_v1->alloc();
    u64 file_path_ptr = os_v1->getenv(list_ptr);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_load_open_file_close_file, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 file_path_ptr = os_v1->getcwd();
    u64 file_name_ptr = type_string_v1->load("/nonexistent.txt");
    type_string_v1->strcat(file_path_ptr, file_name_ptr);
    type_string_v1->free(file_name_ptr);
    u64 mode_ptr = type_string_v1->load("rb");
    u64 f_ptr = type_file_v1->alloc(file_path_ptr, mode_ptr);
    u64 data_ptr = type_file_v1->data(f_ptr);
    RX_ASSERT(f_ptr == 0);
    RX_ASSERT(data_ptr == 0);
#ifndef USE_GC
    type_file_v1->free(f_ptr);
    type_data_v1->free(data_ptr);
    type_string_v1->free(mode_ptr);
    type_string_v1->free(file_name_ptr);
    type_string_v1->free(file_path_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_offset_strcpy, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 name_ptr = type_string_v1->load("name");
    u64 at_ptr = type_string_v1->load("@");
    u64 domain_ptr = type_string_v1->load("domain.org");
    type_string_v1->strcat(name_ptr, at_ptr);
    type_string_v1->strcat(name_ptr, domain_ptr);

    u64 path_ptr1 = type_string_v1->load("name@domain.org");
    u64 path_ptr2 = type_string_v1->copy(at_ptr);

    u64 domain_name = type_string_v1->offset(path_ptr1, path_ptr2);
    type_string_v1->strcpy(name_ptr, domain_name);

    char* domain = type_string_v1->unsafe(domain_ptr);
    char* name = type_string_v1->unsafe(name_ptr);
    RX_ASSERT(strcmp(domain, "domain.org") == 0);
    RX_ASSERT(strcmp(name, "domain.org") == 0);

#ifndef USE_GC
    type_string_v1->free(domain_name);
    type_string_v1->free(name_ptr);
    type_string_v1->free(path_ptr1);
    type_string_v1->free(path_ptr2);
    type_string_v1->free(name_ptr);
    type_string_v1->free(domain_ptr);
    type_string_v1->free(at_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_offset_strcpy_free, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 name_ptr = type_string_v1->load("name");
    u64 at_ptr = type_string_v1->load("@");
    u64 domain_ptr = type_string_v1->load("domain.org");
    type_string_v1->strcat(name_ptr, at_ptr);
    type_string_v1->strcat(name_ptr, domain_ptr);

    u64 path_ptr1 = type_string_v1->load("name@domain.org");
    u64 path_ptr2 = type_string_v1->copy(at_ptr);

    u64 domain_name = type_string_v1->offset(path_ptr1, path_ptr2);
    type_string_v1->free(path_ptr1);
    type_string_v1->strcpy(name_ptr, domain_name);

    char* domain = type_string_v1->unsafe(domain_ptr);
    char* name = type_string_v1->unsafe(name_ptr);
    RX_ASSERT(strcmp(domain, "domain.org") == 0);
    RX_ASSERT(strcmp(name, "name@domain.org") == 0);

#ifndef USE_GC
    type_string_v1->free(domain_name);
    type_string_v1->free(name_ptr);
    type_string_v1->free(path_ptr2);
    type_string_v1->free(name_ptr);
    type_string_v1->free(domain_ptr);
    type_string_v1->free(at_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_print_free, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 printing_ptr = type_string_v1->load("hello, world!");
    type_string_v1->free(printing_ptr);
    os_v1->putc(printing_ptr);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_print, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 printing_ptr = type_string_v1->load("hello, world!");
    os_v1->putc(printing_ptr);
#ifndef USE_GC
    type_string_v1->free(printing_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_print_string_pointer, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 printing_ptr = type_string_v1->load("hello, world!");
    u64 comma_ptr = type_string_v1->load(",");
    u64 substring_index_ptr = type_string_v1->offset(printing_ptr, comma_ptr);
    os_v1->putc(substring_index_ptr);
#ifndef USE_GC
    type_string_v1->free(printing_ptr);
    type_string_v1->free(substring_index_ptr);
    type_string_v1->free(comma_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_print_string_pointer_copy, .fixture = test_fixture) {
    pointer_v1->init(8);
    const char* substring_expected = " world!";
    u64 printing_ptr = type_string_v1->load("hello, world!");
    u64 comma_ptr = type_string_v1->load(",");
    u64 substring_index_ptr = type_string_v1->offset(printing_ptr, comma_ptr);
    u64 substring_ptr = type_string_v1->copy(substring_index_ptr);

    char* substring_actual = type_string_v1->unsafe(substring_ptr);
    RX_ASSERT(strcmp(substring_expected, substring_actual) == 0);

#ifndef USE_GC
    type_string_v1->free(printing_ptr);
    type_string_v1->free(comma_ptr);
    type_string_v1->free(substring_index_ptr);
    type_string_v1->free(substring_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_size, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr = stack_v1->alloc();
    u64 size_actual = type_string_v1->size(list_ptr);
    u64 size_expected = 0;
    RX_ASSERT(size_expected == size_actual);
#ifndef USE_GC
    stack_v1->free(list_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_unsafe, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr = stack_v1->alloc();
    char* ptr_actual = type_string_v1->unsafe(list_ptr);
    char* ptr_expected = 0;
    RX_ASSERT(ptr_expected == ptr_actual);
#ifndef USE_GC
    stack_v1->free(list_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_offset, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 string_ptr = type_string_v1->load("himem.sys");
    u64 list_ptr = stack_v1->alloc();
    u64 size_actual = type_string_v1->offset(string_ptr, list_ptr);
    u64 size_expected = 0;
    RX_ASSERT(size_expected == size_actual);
#ifndef USE_GC
    stack_v1->free(list_ptr);
    type_string_v1->free(string_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, teststring_pointer_unsafe, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 printing_ptr = type_string_v1->load("hello, world!");
    u64 comma_ptr = type_string_v1->load(",");
    u64 substring_index_ptr = type_string_v1->offset(printing_ptr, comma_ptr);

    char* expected_value = type_string_v1->unsafe(substring_index_ptr);
    const char* actual_value = " world!";
    RX_ASSERT(strcmp(expected_value, actual_value) == 0);

    type_string_v1->free(substring_index_ptr);
    os_v1->putc(substring_index_ptr);
#ifndef USE_GC
    type_string_v1->free(printing_ptr);
    type_string_v1->free(comma_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_size, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 printing_ptr = type_string_v1->load("hello, world!");
    u64 comma_ptr = type_string_v1->load(",");
    u64 substring_index_ptr = type_string_v1->offset(printing_ptr, comma_ptr);
    u64 substring_ptr = type_string_v1->copy(substring_index_ptr);

    u64 size_expected = strlen(" world!"); /* adds one 0 to termination byte */
    u64 size_actual = type_string_v1->size(substring_ptr);
    RX_ASSERT(size_expected == size_actual);

#ifndef USE_GC
    type_string_v1->free(printing_ptr);
    type_string_v1->free(comma_ptr);
    type_string_v1->free(substring_index_ptr);
    type_string_v1->free(substring_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_offset_subsearch, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 printing_ptr = type_string_v1->load("hello, world!");
    u64 comma_ptr = type_string_v1->load(",");
    u64 w_ptr = type_string_v1->load("w");
    u64 substring_index_ptr1 = type_string_v1->offset(printing_ptr, comma_ptr);
    u64 substring_index_ptr2 = type_string_v1->offset(substring_index_ptr1, w_ptr);
    u64 substring_ptr = type_string_v1->copy(substring_index_ptr2);

    u64 size_expected = strlen("orld!"); /* adds one 0 to termination byte */
    u64 size_actual = type_string_v1->size(substring_ptr);
    RX_ASSERT(size_expected == size_actual);

#ifndef USE_GC
    type_string_v1->free(printing_ptr);
    type_string_v1->free(comma_ptr);
    type_string_v1->free(substring_index_ptr1);
    type_string_v1->free(substring_index_ptr2);
    type_string_v1->free(substring_ptr);
    type_string_v1->free(w_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_print_string_pointer_free, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 printing_ptr = type_string_v1->load("hello, world!");
    u64 comma_ptr = type_string_v1->load(",");
    u64 substring_index_ptr = type_string_v1->offset(printing_ptr, comma_ptr);
    type_string_v1->free(substring_index_ptr);
    os_v1->putc(substring_index_ptr);
#ifndef USE_GC
    type_string_v1->free(printing_ptr);
    type_string_v1->free(comma_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_offset_strcat, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 name_ptr = type_string_v1->load("name");
    u64 at_ptr = type_string_v1->load("@");
    type_string_v1->strcat(name_ptr, at_ptr);

    u64 path_ptr1 = type_string_v1->load("name@domain.org");
    u64 path_ptr2 = type_string_v1->copy(at_ptr);

    u64 domain_name = type_string_v1->offset(path_ptr1, path_ptr2);
    type_string_v1->strcat(name_ptr, domain_name);

    char* domain = type_string_v1->unsafe(path_ptr1);
    char* name = type_string_v1->unsafe(name_ptr);
    u64 domain_len = strlen(domain);
    u64 name_len = strlen(name);
    RX_ASSERT(domain_len > 0);
    RX_ASSERT(name_len > 0);
    RX_ASSERT(domain_len == name_len);
    RX_ASSERT(strcmp(domain, name) == 0);

#ifndef USE_GC
    type_string_v1->free(domain_name);
    type_string_v1->free(name_ptr);
    type_string_v1->free(path_ptr1);
    type_string_v1->free(path_ptr2);
    type_string_v1->free(name_ptr);
    type_string_v1->free(at_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_file_read_invalid_type, .fixture = test_fixture) {
    pointer_v1->init(8);
#ifndef USE_GC
    u64 list_ptr = stack_v1->alloc();
    u64 data_ptr = type_file_v1->data(list_ptr);
    type_data_v1->free(data_ptr);
    type_file_v1->free(list_ptr);
    stack_v1->free(list_ptr);
#else
    u64 list_ptr = stack_v1->alloc();
    type_file_v1->data(list_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_load_open_match_last_unsafe_free, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 file_path_ptr = os_v1->getcwd();
    u64 file_name_ptr = type_string_v1->load("/all_english_words.txt//");
    type_string_v1->strcat(file_path_ptr, file_name_ptr);
    u64 pattern_ptr = type_string_v1->load("//");
    u64 last_match_ptr = type_string_v1->offset(file_path_ptr, pattern_ptr);
    type_string_v1->free(pattern_ptr);
    type_string_v1->put_char(last_match_ptr, '\0');
    type_string_v1->free(last_match_ptr);
    type_string_v1->strcat(file_path_ptr, file_name_ptr);
    type_string_v1->free(file_name_ptr);
    u64 mode_ptr = type_string_v1->load("rb");
    u64 f_ptr = type_file_v1->alloc(file_path_ptr, mode_ptr);
    type_string_v1->free(file_path_ptr);
    type_string_v1->free(mode_ptr);
    u64 data_ptr = type_file_v1->data(f_ptr);
    type_file_v1->free(f_ptr);
    os_v1->putc(data_ptr);
#ifndef USE_GC
    type_data_v1->free(data_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_load_open_match_last_unsafe_free_unsuppported_calls, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 ptr1 = type_string_v1->load("qwerty//u");
    u64 ptr2 = type_string_v1->load("asdfghjkl");
    u64 pattern_ptr = type_string_v1->load("/u");
    u64 last_match_ptr = type_string_v1->offset(ptr1, pattern_ptr);
    type_string_v1->strcat(last_match_ptr, ptr2);
    type_string_v1->free(pattern_ptr);
    type_string_v1->put_char(last_match_ptr, '\0');
#ifndef USE_GC
    type_string_v1->free(ptr1);
    type_string_v1->free(ptr2);
    type_string_v1->free(last_match_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_user, .fixture = test_fixture) {
    pointer_v1->init(8);
#ifndef USE_GC
    u64 ptr1 = type_user_v1->alloc();
    type_user_v1->free(ptr1);
#else
    type_user_v1->alloc();
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_load_open_file_unsafe_hashtable, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 file_path_ptr = os_v1->getcwd();
    u64 file_name_ptr = type_string_v1->load("/all_english_words.txt");
    type_string_v1->strcat(file_path_ptr, file_name_ptr);
    type_string_v1->free(file_name_ptr);
    u64 mode_ptr = type_string_v1->load("rb");
    u64 f_ptr = type_file_v1->alloc(file_path_ptr, mode_ptr);
    if (f_ptr != 0) {
        u64 data_ptr = type_file_v1->data(f_ptr);
        u64 list_ptr = stack_v1->alloc();
        type_file_v1->free(f_ptr);
        u8* file_data = type_data_v1->unsafe(data_ptr);
        for (int i = 0; i < 100; i++) {
            u8* tmp = file_data;
            while (*tmp != 0 && *tmp != '\n') {
                tmp++;
            }
            *tmp++ = '\0';
            u64 string_ptr = type_string_v1->load((char*)file_data);
            stack_v1->push(list_ptr, string_ptr);
            os_v1->putc(string_ptr);
            file_data = tmp;
        }
        stack_v1->free(list_ptr);
        type_data_v1->free(data_ptr);
    }
#ifndef USE_GC
    type_string_v1->free(mode_ptr);
    type_string_v1->free(file_name_ptr);
    type_string_v1->free(file_path_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

RX_TEST_CASE(tests_v1, test_load_load_match_last, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 str_ptr = type_string_v1->load("Hello, world!");
    u64 ch_ptr = type_string_v1->load("z");
    u64 last_match_ptr = type_string_v1->offset(str_ptr, ch_ptr);
    RX_ASSERT(last_match_ptr == 0);
#ifndef USE_GC
    type_string_v1->free(last_match_ptr);
    type_string_v1->free(str_ptr);
    type_string_v1->free(ch_ptr);
#endif
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* internal */
static void parse_text(u64 text_string_ptr);

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_init, .fixture = test_fixture) {
    pointer_v1->init(8);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_load, .fixture = test_fixture) {
    pointer_v1->init(8);
    type_string_v1->load("asdf");
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_init_string_load_9, .fixture = test_fixture) {
    pointer_v1->init(8);
    type_string_v1->load("0");
    type_string_v1->load("1");
    type_string_v1->load("2");
    type_string_v1->load("3");
    type_string_v1->load("4");
    type_string_v1->load("5");
    type_string_v1->load("6");
    type_string_v1->load("7");
    type_string_v1->load("8");
    type_string_v1->load("9");
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_strings, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 text_string_ptr0 = type_string_v1->load("a");
    u64 text_string_ptr1 = type_string_v1->load("a\nb");
    u64 text_string_ptr2 = type_string_v1->load("ab\nabc\n");
    u64 text_string_ptr3 = type_string_v1->load("adadadsadsadad\ndad\nadsaddasaddad\nsad\n");
    u64 text_string_ptr4 = type_string_v1->load("ab\ndad\nabcd\nbcd\n");
    u64 text_string_ptr5 = type_string_v1->load("ab\nc\nabc\nbcd\n");
    u64 text_string_ptr6 = type_string_v1->load("abc\nabcd\nbcde\nabc\n");
    u64 text_string_ptr7 = type_string_v1->load("abc\n\n");
    parse_text(0);
    parse_text(text_string_ptr0);
    parse_text(text_string_ptr1);
    parse_text(text_string_ptr2);
    parse_text(text_string_ptr3);
    parse_text(text_string_ptr4);
    parse_text(text_string_ptr5);
    parse_text(text_string_ptr6);
    parse_text(text_string_ptr7);
    type_string_v1->free(text_string_ptr0);
    type_string_v1->free(text_string_ptr1);
    type_string_v1->free(text_string_ptr2);
    type_string_v1->free(text_string_ptr3);
    type_string_v1->free(text_string_ptr4);
    type_string_v1->free(text_string_ptr5);
    type_string_v1->free(text_string_ptr6);
    type_string_v1->free(text_string_ptr7);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_pointer_free, .fixture = test_fixture) {
    pointer_v1->init(8);
    type_string_pointer_v1->free(0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_pointer_lessthan_free, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("b");
    u64 quantum_str_ptr1 = type_string_v1->load("abba");
    u64 quantum_str_ptr2 = type_string_v1->offset(quantum_str_ptr1, pattern_ptr);
    u64 data_ptr = type_string_v1->lessthan(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(data_ptr != 0);
    type_string_pointer_v1->free(data_ptr);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_0_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 error_ptr = type_string_v1->lessthan(0, 0);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_0_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 error_ptr = type_string_v1->lessthan(0, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_1_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr = type_string_v1->load("");
    u64 error_ptr = type_string_v1->lessthan(1, 0);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_1_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->lessthan(1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_a_b, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("b");
    u64 quantum_str_ptr1 = type_string_v1->load("abba");
    u64 quantum_str_ptr2 = type_string_v1->offset(quantum_str_ptr1, pattern_ptr);
    u64 data_ptr = type_string_v1->lessthan(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(data_ptr != 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_b_a, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("b");
    u64 quantum_str_ptr1 = type_string_v1->load("abba");
    u64 quantum_str_ptr2 = type_string_v1->offset(quantum_str_ptr1, pattern_ptr);
    u64 error_ptr = type_string_v1->lessthan(quantum_str_ptr2, quantum_str_ptr1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_1_2, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->lessthan(1, 2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_1_3, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->lessthan(1, 2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_2_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->lessthan(2, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_a_string_free, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    type_string_v1->free(quantum_str_ptr2);
    u64 error_ptr = type_string_v1->lessthan(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_string_free_b, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    type_string_v1->free(quantum_str_ptr1);
    u64 error_ptr = type_string_v1->lessthan(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_string_list, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 list_ptr2 = stack_v1->alloc();
    u64 error_ptr = type_string_v1->lessthan(quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_list_string, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr1 = stack_v1->alloc();
    u64 quantum_str_ptr2 = type_string_v1->load("a");
    u64 error_ptr = type_string_v1->lessthan(list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_0_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 error_ptr = type_string_v1->greaterthan(0, 0);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_0_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 error_ptr = type_string_v1->greaterthan(0, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_1_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr = type_string_v1->load("");
    u64 error_ptr = type_string_v1->greaterthan(1, 0);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_1_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->greaterthan(1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_a_b, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("b");
    u64 quantum_str_ptr1 = type_string_v1->load("abba");
    u64 quantum_str_ptr2 = type_string_v1->offset(quantum_str_ptr1, pattern_ptr);
    u64 error_ptr = type_string_v1->greaterthan(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_b_a, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("b");
    u64 quantum_str_ptr1 = type_string_v1->load("abba");
    u64 quantum_str_ptr2 = type_string_v1->offset(quantum_str_ptr1, pattern_ptr);
    u64 data_ptr = type_string_v1->greaterthan(quantum_str_ptr2, quantum_str_ptr1);
    RX_ASSERT(data_ptr != 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_1_2, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->greaterthan(1, 2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_1_3, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->greaterthan(1, 3);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_2_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->greaterthan(2, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_a_string_free, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    type_string_v1->free(quantum_str_ptr2);
    u64 error_ptr = type_string_v1->greaterthan(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_string_free_b, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    type_string_v1->free(quantum_str_ptr1);
    u64 error_ptr = type_string_v1->greaterthan(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_string_list, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 list_ptr2 = stack_v1->alloc();
    u64 error_ptr = type_string_v1->greaterthan(quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_list_string, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr1 = stack_v1->alloc();
    u64 quantum_str_ptr2 = type_string_v1->load("a");
    u64 error_ptr = type_string_v1->greaterthan(list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_0_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 error_ptr = type_string_v1->equals(0, 0);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_0_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 error_ptr = type_string_v1->equals(0, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_1_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr = type_string_v1->load("");
    u64 error_ptr = type_string_v1->equals(1, 0);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_1_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->equals(1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_a_b, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("b");
    u64 quantum_str_ptr1 = type_string_v1->load("abba");
    u64 quantum_str_ptr2 = type_string_v1->offset(quantum_str_ptr1, pattern_ptr);
    u64 error_ptr = type_string_v1->equals(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_b_a, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("b");
    u64 source_ptr = type_string_v1->load("abba");
    u64 quantum_str_ptr1 = type_string_v1->offset(source_ptr, pattern_ptr);
    u64 quantum_str_ptr2 = type_string_v1->offset(source_ptr, pattern_ptr);
    u64 data_ptr = type_string_v1->equals(quantum_str_ptr2, quantum_str_ptr1);
    RX_ASSERT(data_ptr != 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_1_2, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->equals(1, 2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_1_3, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->equals(1, 3);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_2_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->equals(2, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_a_string_free, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    type_string_v1->free(quantum_str_ptr2);
    u64 error_ptr = type_string_v1->equals(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_string_free_b, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    type_string_v1->free(quantum_str_ptr1);
    u64 error_ptr = type_string_v1->equals(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_string_list, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 list_ptr2 = stack_v1->alloc();
    u64 error_ptr = type_string_v1->equals(quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_list_string, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr1 = stack_v1->alloc();
    u64 quantum_str_ptr2 = type_string_v1->load("a");
    u64 error_ptr = type_string_v1->equals(list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_0_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 error_ptr = type_string_v1->compare(0, 0);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_0_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 error_ptr = type_string_v1->compare(0, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_1_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr = type_string_v1->load("");
    u64 error_ptr = type_string_v1->compare(1, 0);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_1_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->compare(1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_a_b, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("b");
    u64 quantum_str_ptr1 = type_string_v1->load("abba");
    u64 quantum_str_ptr2 = type_string_v1->offset(quantum_str_ptr1, pattern_ptr);
    u64 data_ptr = type_string_v1->compare(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(data_ptr != 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_b_a, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("b");
    u64 source_ptr = type_string_v1->load("abba");
    u64 quantum_str_ptr1 = type_string_v1->offset(source_ptr, pattern_ptr);
    u64 quantum_str_ptr2 = type_string_v1->offset(source_ptr, pattern_ptr);
    u64 data_ptr = type_string_v1->compare(quantum_str_ptr2, quantum_str_ptr1);
    RX_ASSERT(data_ptr != 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_1_2, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->compare(1, 2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_1_3, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->compare(1, 3);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_2_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->compare(2, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_a_string_free, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    type_string_v1->free(quantum_str_ptr2);
    u64 error_ptr = type_string_v1->compare(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_string_free_b, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    type_string_v1->free(quantum_str_ptr1);
    u64 error_ptr = type_string_v1->compare(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_string_list, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 list_ptr2 = stack_v1->alloc();
    u64 error_ptr = type_string_v1->compare(quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_list_string, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr1 = stack_v1->alloc();
    u64 quantum_str_ptr2 = type_string_v1->load("a");
    u64 error_ptr = type_string_v1->compare(list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_0_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 error_ptr = type_string_v1->strcmp(0, 0);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_0_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 error_ptr = type_string_v1->strcmp(0, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_1_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr = type_string_v1->load("");
    u64 error_ptr = type_string_v1->strcmp(1, 0);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_1_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->strcmp(1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_a_b, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("b");
    u64 quantum_str_ptr1 = type_string_v1->load("abba");
    u64 quantum_str_ptr2 = type_string_v1->offset(quantum_str_ptr1, pattern_ptr);
    u64 data_ptr = type_string_v1->strcmp(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(data_ptr != 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_b_a, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("b");
    u64 source_ptr = type_string_v1->load("abba");
    u64 quantum_str_ptr1 = type_string_v1->offset(source_ptr, pattern_ptr);
    u64 quantum_str_ptr2 = type_string_v1->offset(source_ptr, pattern_ptr);
    u64 data_ptr = type_string_v1->strcmp(quantum_str_ptr2, quantum_str_ptr1);
    RX_ASSERT(data_ptr != 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_1_2, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->strcmp(1, 2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}
/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_1_3, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->strcmp(1, 3);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_2_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->strcmp(2, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_a_string_free, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    type_string_v1->free(quantum_str_ptr2);
    u64 error_ptr = type_string_v1->strcmp(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_string_free_b, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    type_string_v1->free(quantum_str_ptr1);
    u64 error_ptr = type_string_v1->strcmp(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_string_list, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 list_ptr2 = stack_v1->alloc();
    u64 error_ptr = type_string_v1->strcmp(quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_list_string, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr1 = stack_v1->alloc();
    u64 quantum_str_ptr2 = type_string_v1->load("a");
    u64 error_ptr = type_string_v1->strcmp(list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_0_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 error_ptr = type_string_v1->move_right(0, 0);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_0_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 error_ptr = type_string_v1->move_right(0, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_1_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr = type_string_v1->load("");
    u64 error_ptr = type_string_v1->move_right(1, 0);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_1_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->move_right(1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_a_b, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("abba");
    u64 error_ptr = type_string_v1->move_right(quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_b_a, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("a");
    u64 source_ptr = type_string_v1->load("abba");
    u64 quantum_str_ptr1 = type_string_v1->offset(source_ptr, pattern_ptr);
    u64 error_ptr = type_string_v1->move_right(quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_a_b_2, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("b");
    u64 source_ptr = type_string_v1->load("abba");
    u64 quantum_str_ptr1 = type_string_v1->offset(source_ptr, pattern_ptr);
    u64 data_ptr = type_string_v1->move_right(quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_a_b_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("a");
    u64 source_ptr = type_string_v1->load("abba");
    u64 quantum_str_ptr1 = type_string_v1->offset(source_ptr, pattern_ptr);
    u64 error_ptr = type_string_v1->move_right(quantum_str_ptr1, 1);
    RX_ASSERT(strcmp(type_string_v1->unsafe(quantum_str_ptr1), "") == 0);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_a_b_free, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("b");
    u64 source_ptr = type_string_v1->load("abba");
    u64 quantum_str_ptr1 = type_string_v1->offset(source_ptr, pattern_ptr);
    type_string_v1->free(source_ptr);
    u64 error_ptr = type_string_v1->move_right(quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_1_2, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->move_right(1, 2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_1_3, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->move_right(1, 3);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_2_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->move_right(2, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_a_string_free, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    type_string_v1->free(quantum_str_ptr2);
    u64 error_ptr = type_string_v1->move_right(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_string_free_b, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    type_string_v1->free(quantum_str_ptr1);
    u64 error_ptr = type_string_v1->move_right(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_string_list, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 list_ptr2 = stack_v1->alloc();
    u64 error_ptr = type_string_v1->move_right(quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_list_string, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr1 = stack_v1->alloc();
    u64 quantum_str_ptr2 = type_string_v1->load("a");
    u64 error_ptr = type_string_v1->move_right(list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_0_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 error_ptr = type_string_v1->right(0, 0);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_0_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 error_ptr = type_string_v1->right(0, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_1_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr = type_string_v1->load("");
    u64 error_ptr = type_string_v1->right(1, 0);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_1_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 data_ptr = type_string_v1->right(quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(strcmp(type_string_v1->unsafe(data_ptr), "") == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_a_b, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("abba");
    u64 error_ptr = type_string_v1->right(quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr != 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_b_a, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("a");
    u64 source_ptr = type_string_v1->load("abba");
    u64 quantum_str_ptr1 = type_string_v1->offset(source_ptr, pattern_ptr);
    u64 error_ptr = type_string_v1->right(quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_a_b_2, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("b");
    u64 source_ptr = type_string_v1->load("abba");
    u64 quantum_str_ptr1 = type_string_v1->offset(source_ptr, pattern_ptr);
    u64 data_ptr = type_string_v1->right(quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_a_b_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("a");
    u64 source_ptr = type_string_v1->load("abba");
    u64 quantum_str_ptr1 = type_string_v1->offset(source_ptr, pattern_ptr);
    u64 error_ptr = type_string_v1->right(quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
    RX_ASSERT(strcmp(type_string_v1->unsafe(quantum_str_ptr1), "") == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_a_b_free, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("b");
    u64 source_ptr = type_string_v1->load("abba");
    u64 quantum_str_ptr1 = type_string_v1->offset(source_ptr, pattern_ptr);
    type_string_v1->free(source_ptr);
    u64 error_ptr = type_string_v1->right(quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_1_2, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->right(1, 2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_1_3, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->right(1, 3);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_2_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("");
    u64 error_ptr = type_string_v1->right(quantum_str_ptr2, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_a_string_free, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    type_string_v1->free(quantum_str_ptr2);
    u64 error_ptr = type_string_v1->right(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_string_free_b, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    type_string_v1->free(quantum_str_ptr1);
    u64 error_ptr = type_string_v1->right(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_string_list, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 list_ptr2 = stack_v1->alloc();
    u64 error_ptr = type_string_v1->right(quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_list_string, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr1 = stack_v1->alloc();
    u64 quantum_str_ptr2 = type_string_v1->load("a");
    u64 error_ptr = type_string_v1->right(list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_0_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 error_ptr = type_string_v1->move_left(0, 0);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_0_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 error_ptr = type_string_v1->move_left(0, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_1_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr = type_string_v1->load("");
    u64 error_ptr = type_string_v1->move_left(1, 0);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_1_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->move_left(1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_a_b, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("b");
    u64 source_ptr = type_string_v1->load("abba");
    u64 quantum_str_ptr1 = type_string_v1->offset(source_ptr, pattern_ptr);
    u64 error_ptr = type_string_v1->move_left(quantum_str_ptr1, 4);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_b_a, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("a");
    u64 source_ptr = type_string_v1->load("abba");
    u64 quantum_str_ptr1 = type_string_v1->offset(source_ptr, pattern_ptr);
    u64 data_ptr = type_string_v1->move_left(quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_a_b_2, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("b");
    u64 source_ptr = type_string_v1->load("abba");
    u64 quantum_str_ptr1 = type_string_v1->offset(source_ptr, pattern_ptr);
    u64 data_ptr = type_string_v1->move_left(quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_a_b_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("a");
    u64 source_ptr = type_string_v1->load("abba");
    u64 quantum_str_ptr1 = type_string_v1->offset(source_ptr, pattern_ptr);
    u64 data_ptr = type_string_v1->move_left(quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(strcmp(type_string_v1->unsafe(quantum_str_ptr1), "a") == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_a_b_free, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("b");
    u64 source_ptr = type_string_v1->load("abba");
    u64 quantum_str_ptr1 = type_string_v1->offset(source_ptr, pattern_ptr);
    type_string_v1->free(source_ptr);
    u64 error_ptr = type_string_v1->move_left(quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_1_2, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->move_left(1, 2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}
/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_1_3, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->move_left(1, 3);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_2_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->move_left(2, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_a_string_free, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    type_string_v1->free(quantum_str_ptr2);
    u64 error_ptr = type_string_v1->move_left(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_string_free_b, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    type_string_v1->free(quantum_str_ptr1);
    u64 error_ptr = type_string_v1->move_left(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_string_list, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 list_ptr2 = stack_v1->alloc();
    u64 error_ptr = type_string_v1->move_left(quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_list_string, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr1 = stack_v1->alloc();
    u64 quantum_str_ptr2 = type_string_v1->load("a");
    u64 error_ptr = type_string_v1->move_left(list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_0_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 error_ptr = type_string_v1->left_strncpy(0, 0);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_0_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 error_ptr = type_string_v1->left_strncpy(0, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_1_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr = type_string_v1->load("");
    u64 error_ptr = type_string_v1->left_strncpy(1, 0);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_1_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->left_strncpy(1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_a_b, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("b");
    u64 source_ptr = type_string_v1->load("abba");
    u64 quantum_str_ptr1 = type_string_v1->offset(source_ptr, pattern_ptr);
    u64 error_ptr = type_string_v1->left_strncpy(quantum_str_ptr1, 4);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_b_a, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("c");
    u64 source_ptr = type_string_v1->load("abbc");
    u64 quantum_str_ptr1 = type_string_v1->offset(source_ptr, pattern_ptr);
    u64 data_ptr = type_string_v1->left_strncpy(quantum_str_ptr1, 3);
    RX_ASSERT(quantum_str_ptr1 != 0);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(strcmp(type_string_v1->unsafe(quantum_str_ptr1), "") == 0);
    RX_ASSERT(strcmp(type_string_v1->unsafe(data_ptr), "bbc") == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_a_b_2, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("b");
    u64 source_ptr = type_string_v1->load("abba");
    u64 quantum_str_ptr1 = type_string_v1->offset(source_ptr, pattern_ptr);
    u64 data_ptr = type_string_v1->left_strncpy(quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_a_b_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("a");
    u64 source_ptr = type_string_v1->load("abba");
    u64 quantum_str_ptr1 = type_string_v1->offset(source_ptr, pattern_ptr);
    u64 data_ptr = type_string_v1->left_strncpy(quantum_str_ptr1, 1);
    RX_ASSERT(quantum_str_ptr1 != 0);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(strcmp(type_string_v1->unsafe(quantum_str_ptr1), "") == 0);
    RX_ASSERT(strcmp(type_string_v1->unsafe(data_ptr), "a") == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_a_b_free, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("b");
    u64 source_ptr = type_string_v1->load("abba");
    u64 quantum_str_ptr1 = type_string_v1->offset(source_ptr, pattern_ptr);
    type_string_v1->free(source_ptr);
    u64 error_ptr = type_string_v1->left_strncpy(quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_1_2, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->left_strncpy(1, 2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}
/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_1_3, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->left_strncpy(1, 3);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_2_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->left_strncpy(2, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_a_string_free, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    type_string_v1->free(quantum_str_ptr2);
    u64 error_ptr = type_string_v1->left_strncpy(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_string_free_b, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    type_string_v1->free(quantum_str_ptr1);
    u64 error_ptr = type_string_v1->left_strncpy(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_string_list, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 list_ptr2 = stack_v1->alloc();
    u64 error_ptr = type_string_v1->left_strncpy(quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_list_string, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr1 = stack_v1->alloc();
    u64 quantum_str_ptr2 = type_string_v1->load("a");
    u64 error_ptr = type_string_v1->left_strncpy(list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_0_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 error_ptr = type_string_v1->left(0, 0);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_0_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 error_ptr = type_string_v1->left(0, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_1_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr = type_string_v1->load("");
    u64 error_ptr = type_string_v1->left(1, 0);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_1_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->left(1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_a_b, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("b");
    u64 source_ptr = type_string_v1->load("abba");
    u64 quantum_str_ptr1 = type_string_v1->offset(source_ptr, pattern_ptr);
    u64 error_ptr = type_string_v1->left(quantum_str_ptr1, 4);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_b_a, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("c");
    u64 source_ptr = type_string_v1->load("abbc");
    u64 quantum_str_ptr1 = type_string_v1->offset(source_ptr, pattern_ptr);
    u64 data_ptr = type_string_v1->left(quantum_str_ptr1, 3);
    RX_ASSERT(quantum_str_ptr1 != 0);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(strcmp(type_string_v1->unsafe(quantum_str_ptr1), "") == 0);
    RX_ASSERT(strcmp(type_string_v1->unsafe(data_ptr), "bbc") == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_a_b_2, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("b");
    u64 source_ptr = type_string_v1->load("abba");
    u64 quantum_str_ptr1 = type_string_v1->offset(source_ptr, pattern_ptr);
    u64 data_ptr = type_string_v1->left(quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_a_b_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("a");
    u64 source_ptr = type_string_v1->load("abba");
    u64 quantum_str_ptr1 = type_string_v1->offset(source_ptr, pattern_ptr);
    u64 data_ptr = type_string_v1->left(quantum_str_ptr1, 1);
    RX_ASSERT(quantum_str_ptr1 != 0);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(strcmp(type_string_v1->unsafe(quantum_str_ptr1), "") == 0);
    RX_ASSERT(strcmp(type_string_v1->unsafe(data_ptr), "a") == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_a_b_free, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("b");
    u64 source_ptr = type_string_v1->load("abba");
    u64 quantum_str_ptr1 = type_string_v1->offset(source_ptr, pattern_ptr);
    type_string_v1->free(source_ptr);
    u64 error_ptr = type_string_v1->left(quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_1_2, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->left(1, 2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}
/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_1_3, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->left(1, 3);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_2_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->left(2, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_a_string_free, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    type_string_v1->free(quantum_str_ptr2);
    u64 error_ptr = type_string_v1->left(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_string_free_b, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    type_string_v1->free(quantum_str_ptr1);
    u64 error_ptr = type_string_v1->left(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_string_list, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 list_ptr2 = stack_v1->alloc();
    u64 error_ptr = type_string_v1->left(quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_list_string, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr1 = stack_v1->alloc();
    u64 quantum_str_ptr2 = type_string_v1->load("a");
    u64 error_ptr = type_string_v1->left(list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_0_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 error_ptr = type_string_v1->strncpy(0, 0);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_0_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 error_ptr = type_string_v1->strncpy(0, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_1_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr = type_string_v1->load("");
    u64 error_ptr = type_string_v1->strncpy(1, 0);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_1_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->strncpy(1, 1);
    RX_ASSERT(error_ptr == 3);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_a_b, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("b");
    u64 source_ptr = type_string_v1->load("abba");
    u64 quantum_str_ptr1 = type_string_v1->offset(source_ptr, pattern_ptr);
    u64 error_ptr = type_string_v1->strncpy(quantum_str_ptr1, 2);
    RX_ASSERT(error_ptr == 0);
    u64 data_ptr = type_string_v1->strncpy(quantum_str_ptr1, 1);
    u64 str_ptr1 = type_string_v1->offset(source_ptr, data_ptr);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(str_ptr1 != 0);
    RX_ASSERT(strcmp(type_string_v1->unsafe(str_ptr1), "") == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_b_a, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("b");
    u64 source_ptr = type_string_v1->load("abcd");
    u64 quantum_str_ptr1 = type_string_v1->offset(source_ptr, pattern_ptr);
    u64 data_ptr = type_string_v1->strncpy(quantum_str_ptr1, 2);
    RX_ASSERT(data_ptr != 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_a_b_2, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("b");
    u64 source_ptr = type_string_v1->load("abba");
    u64 quantum_str_ptr1 = type_string_v1->offset(source_ptr, pattern_ptr);
    u64 data_ptr = type_string_v1->strncpy(quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_a_b_0, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("a");
    u64 source_ptr = type_string_v1->load("abba");
    u64 quantum_str_ptr1 = type_string_v1->offset(source_ptr, pattern_ptr);
    u64 data_ptr = type_string_v1->strncpy(quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr == 0);
    RX_ASSERT(quantum_str_ptr1 != 0);
    RX_ASSERT(strcmp(type_string_v1->unsafe(quantum_str_ptr1), "") == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_a_b_free, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("b");
    u64 source_ptr = type_string_v1->load("abba");
    u64 quantum_str_ptr1 = type_string_v1->offset(source_ptr, pattern_ptr);
    type_string_v1->free(source_ptr);
    u64 error_ptr = type_string_v1->strncpy(quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_a_b_2_overflow, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 pattern_ptr = type_string_v1->load("h");
    u64 source_ptr = type_string_v1->load("abcdefgh");
    u64 quantum_str_ptr1 = type_string_v1->offset(source_ptr, pattern_ptr);
    u64 error_ptr = type_string_v1->strncpy(quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_1_2, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->strncpy(1, 2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_1_3, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 error_ptr = type_string_v1->strncpy(1, 3);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_2_1, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    u64 data_ptr = type_string_v1->strncpy(2, 1);
    RX_ASSERT(data_ptr != 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_a_string_free, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    type_string_v1->free(quantum_str_ptr2);
    u64 error_ptr = type_string_v1->strncpy(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_string_free_b, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 quantum_str_ptr2 = type_string_v1->load("b");
    type_string_v1->free(quantum_str_ptr1);
    u64 error_ptr = type_string_v1->strncpy(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_string_list, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 quantum_str_ptr1 = type_string_v1->load("a");
    u64 list_ptr2 = stack_v1->alloc();
    u64 error_ptr = type_string_v1->strncpy(quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_list_string, .fixture = test_fixture) {
    pointer_v1->init(8);
    u64 list_ptr1 = stack_v1->alloc();
    u64 quantum_str_ptr2 = type_string_v1->load("a");
    u64 error_ptr = type_string_v1->strncpy(list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer_v1->gc();
    pointer_v1->destroy();
}

static void parse_text(u64 text_string_ptr) {
    u64 gc_ptr = stack_v1->alloc();
    u64 text_size = type_string_v1->size(text_string_ptr);
    if (text_string_ptr == 0) {
        return;
    }
    u64 list_ptr = stack_v1->alloc();
    char* text = type_string_v1->unsafe(text_string_ptr);
    char* tmp = text;
    while (*tmp != 0 && text_size > 0) {
        while (*tmp != 0 && *tmp != '\n' && text_size > 0) {
            tmp++;
            text_size--;
        }
        if (text_size == 0) {
            stack_v1->free(list_ptr);
            return;
        }
        *tmp++ = '\0';
        text_size--;
        u64 string_ptr = type_string_v1->load(text);
        stack_v1->push(list_ptr, string_ptr);
        text = tmp;
    }
    u64 data_ptr = 0;
    u64 list_data_ptr = stack_v1->alloc();
    while ((data_ptr = stack_v1->pop(list_ptr)) != 0) {
        stack_v1->push(list_data_ptr, data_ptr);
    }
    stack_v1->free(list_ptr);
    stack_v1->push(gc_ptr, list_data_ptr);
    u64 quit = 0;
    while (quit == 0) {
        u64 string_ptr = stack_v1->pop(list_data_ptr);
        if (type_string_v1->size(string_ptr) == 0) {
            quit = 1;
            continue;
        }
        os_v1->putc(string_ptr);
        u64 pattern_ptr = stack_v1->pop(list_data_ptr);
        if (type_string_v1->size(pattern_ptr) == 0) {
            quit = 1;
            continue;
        }
        os_v1->putc(pattern_ptr);
        u64 size = type_string_v1->size(pattern_ptr);
        u64 string_pointer_ptr = 0;
        u64 current_ptr = string_ptr;
        while ((string_pointer_ptr = type_string_v1->strchr(current_ptr, pattern_ptr)) != 0) {
            u64 match_ptr = type_string_v1->match(string_pointer_ptr, pattern_ptr);
            if (match_ptr == 0) {
                break;
            }
            if (type_string_v1->lessthan(string_pointer_ptr, match_ptr)) {
                u64 match_start_ptr = type_string_v1->left(match_ptr, size);
                u64 str_ncpy = type_string_v1->strncpy(match_start_ptr, size);
                u64 distance = type_string_v1->lessthan(string_ptr, match_start_ptr);
                if (distance > 0) {
                    u64 i = 0;
                    while (i++ < distance) {
                        printf(" ");
                    }
                }
                printf("%s[%lld]\n", type_string_v1->unsafe(str_ncpy), distance);
                type_string_pointer_v1->free(match_start_ptr);
                type_string_pointer_v1->free(str_ncpy);
            }
            type_string_pointer_v1->free(string_pointer_ptr);
            current_ptr = match_ptr;
        }
    }
    stack_v1->release(gc_ptr);
    stack_v1->free(gc_ptr);
}

static int run(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- rexo unit test code\n");
#endif
    /* Execute the main function that runs the test cases found. */
    int result = rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;
    return result;
}

const pointer_test_suite PRIVATE_API(pointer_test_suite_definition) = {
    .run = run
};
