/*
 *
 * MIT License
 *
 * Copyright (c) 2023 Artur Mustafin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */
/* #define RXP_DEBUG_TESTS */
#include "../tests/src/test.h"

#include "test_pointer.h"

#include "common/memory.h"
#include "list-micro/data.h"
#include "list/v1/list_v1.h"

#include "playground/brain/brain.h"
#include "playground/hashtable/v1/hashtable_v1.h"

#include "pointer/types/data/v1/data_v1.h"
#include "pointer/types/file/v1/file_v1.h"
#include "pointer/types/list/v1/list_v1.h"
#include "pointer/types/object/v1/object_v1.h"
#include "pointer/types/string/v1/string_v1.h"
#include "pointer/types/string_pointer/v1/string_pointer_v1.h"
#include "pointer/types/user/v1/user_v1.h"
#include "pointer/v1/pointer_v1.h"

#include "pointer/os/v1/os_v1.h"

#define DEFAULT_SIZE 0x100

/* definition */
extern const struct test_suite list_alloc_test_suite_definition;
extern const struct test_suite list_micro_test_suite_definition;
extern const struct test_suite list_test_suite_definition;
extern const struct test_suite vm_v1_test_suite_definition;

/* definition */
static const struct test_suite* list_alloc_tests = &list_alloc_test_suite_definition;
static const struct test_suite* list_micro_tests = &list_micro_test_suite_definition;
static const struct test_suite* list_tests = &list_test_suite_definition;
static const struct test_suite* vm_v1_tests = &vm_v1_test_suite_definition;

/* definition */
extern const struct virtual_methods virtual_methods_definition;

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
    os->putc(0);
}

/* test init */
RX_TEST_CASE(tests_v1, test_load_0, .fixture = test_fixture) {
    u64 empty_ptr = string->load("\0");
    RX_ASSERT(empty_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_v1, test_load_null_ptr, .fixture = test_fixture) {
    u64 null_ptr = 0;
#ifndef USE_GC
    pointer->free(null_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_v1, test_load_null, .fixture = test_fixture) {
    u64 empty_ptr = string->load(0);
    RX_ASSERT(empty_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_v1, test_load_empty, .fixture = test_fixture) {
    pointer->init(8);
    u64 empty_ptr = string->load("");
    RX_ASSERT(empty_ptr == 0);
#ifndef USE_GC
    string->free(empty_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_print_load_empty, .fixture = test_fixture) {
    pointer->init(8);
    u64 empty_ptr = string->load("");
    os->putc(empty_ptr);
    RX_ASSERT(empty_ptr == 0);
#ifndef USE_GC
    string->free(empty_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_load_copy, .fixture = test_fixture) {
    pointer->init(8);
    u64 char_ptr = string->load("/");
    u64 copy_ptr = string->copy(char_ptr);
    RX_ASSERT(char_ptr != 0);
    RX_ASSERT(copy_ptr != 0);
    RX_ASSERT(char_ptr != copy_ptr);
    RX_ASSERT(strcmp(string->unsafe(char_ptr), "/") == 0);
    RX_ASSERT(strcmp(string->unsafe(copy_ptr), "/") == 0);
#ifndef USE_GC
    string->free(char_ptr);
    string->free(copy_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_load_push_peek_pop, .fixture = test_fixture) {
    pointer->init(8);
    u64 char_ptr = string->load("/");
    RX_ASSERT(strcmp(string->unsafe(char_ptr), "/") == 0);
#ifndef USE_GC
    string->free(char_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

RX_TEST_CASE(tests_v1, test_load_free_free, .fixture = test_fixture) {
    pointer->init(8);
    u64 char_ptr = string->load("/");
    RX_ASSERT(strcmp(string->unsafe(char_ptr), "/") == 0);
#ifndef USE_GC
    string->free(char_ptr);
    string->free(char_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_strcat_load_alloc, .fixture = test_fixture) {
    pointer->init(8);
    u64 char_ptr = string->load("/");
    u64 pattern_ptr = string->load("\0");
    string->strcat(pattern_ptr, char_ptr);
    RX_ASSERT(pattern_ptr == 0);
    RX_ASSERT(strcmp(string->unsafe(char_ptr), "/") == 0);
#ifndef USE_GC
    string->free(char_ptr);
    string->free(pattern_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_size, .fixture = test_fixture) {
    pointer->init(8);
    u64 char_ptr = string->load("/");
    u64 pattern_ptr = string->load("\0");
    u64 string_size = data->size(pattern_ptr);
    RX_ASSERT(string_size == 0);
    RX_ASSERT(pattern_ptr == 0);
    RX_ASSERT(strcmp(string->unsafe(char_ptr), "/") == 0);
#ifndef USE_GC
    string->free(char_ptr);
    string->free(pattern_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_pointer_size_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 char_ptr = string->load("/");
    u64 pattern_ptr = string->load("\0");
    u64 string_size = string->size(0);
    RX_ASSERT(string_size == 0);
    RX_ASSERT(pattern_ptr == 0);
    RX_ASSERT(strcmp(string->unsafe(char_ptr), "/") == 0);
#ifndef USE_GC
    string->free(char_ptr);
    string->free(pattern_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_pointer_size_empty, .fixture = test_fixture) {
    pointer->init(8);
    u64 char_ptr = string->load(" ");
    u64 pattern_ptr = string->load("\0");
    u64 string_size = string->size(char_ptr);
    RX_ASSERT(string_size == 1);
    RX_ASSERT(pattern_ptr == 0);
    RX_ASSERT(strcmp(string->unsafe(char_ptr), " ") == 0);
#ifndef USE_GC
    string->free(char_ptr);
    string->free(pattern_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_pointer_size_object, .fixture = test_fixture) {
    pointer->init(8);
    u64 object_ptr = object->alloc(1);
    u64 pattern_ptr = string->load("\0");
    u64 object_size = object->size(object_ptr);
    RX_ASSERT(object_size == 1);
    RX_ASSERT(object_ptr != 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    string->free(object_ptr);
    string->free(pattern_ptr);
    object->free(object_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_object_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 object_ptr = object->alloc(0);
    u64 pattern_ptr = string->load("\0");
    u64 object_size = object->size(object_ptr);
    RX_ASSERT(object_size == 0);
    RX_ASSERT(object_ptr == 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    string->free(object_ptr);
    string->free(pattern_ptr);
    object->free(object_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_object_alloc_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 object_ptr = object->alloc(0);
    u64 pattern_ptr = string->load("\0");
    u64 object_size = object->size(object_ptr);
    RX_ASSERT(object_size == 0);
    RX_ASSERT(object_ptr == 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    string->free(object_ptr);
    string->free(pattern_ptr);
    object->free(object_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_object_load_free, .fixture = test_fixture) {
    pointer->init(8);
    const char* test_data = "Hello, world!";
    const void* ptr = (const void*)test_data;
    u64 object_ptr = object->load(ptr, strlen(ptr));
    object->free(object_ptr);
#ifndef USE_GC
    string->free(object_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_object_free_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 object_ptr = 0;
    object->free(object_ptr);
#ifndef USE_GC
    string->free(object_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_object_load_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 object_ptr = object->load(0, 0x01234567);
    u64 pattern_ptr = string->load("\0");
    u64 object_size = object->size(object_ptr);
    RX_ASSERT(object_size == 0);
    RX_ASSERT(object_ptr == 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    string->free(object_ptr);
    string->free(pattern_ptr);
    object->free(object_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_object_load_string_0, .fixture = test_fixture) {
    pointer->init(8);
    const char* test_data = "Hello, world!";
    const void* ptr = (const void*)test_data;
    u64 object_ptr = object->load(ptr, 0);
    u64 pattern_ptr = string->load("\0");
    u64 object_size = object->size(object_ptr);
    RX_ASSERT(object_size == 0);
    RX_ASSERT(object_ptr == 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    string->free(object_ptr);
    string->free(pattern_ptr);
    object->free(object_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_object_load_string, .fixture = test_fixture) {
    pointer->init(8);
    const char* test_data = "Hello, world!";
    const void* ptr = (const void*)test_data;
    u64 object_ptr = object->load(ptr, strlen(ptr));
    u64 pattern_ptr = string->load("\0");
    u64 object_size = object->size(object_ptr);
    RX_ASSERT(object_size == strlen(ptr));
    RX_ASSERT(object_ptr != 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    string->free(object_ptr);
    string->free(pattern_ptr);
    object->free(object_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_object_load_string_unsafe_alloc_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 object_ptr = object->alloc(0);
    u64 pattern_ptr = string->load("\0");
    u64 object_size = object->size(object_ptr);
    const char* ch = (char*)object->unsafe(object_ptr);
    RX_ASSERT(ch == 0);
    RX_ASSERT(object_size == 0);
    RX_ASSERT(object_ptr == 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    string->free(object_ptr);
    string->free(pattern_ptr);
    object->free(object_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_object_load_string_unsafe_string, .fixture = test_fixture) {
    pointer->init(8);
    u64 object_ptr = object->alloc(0);
    u64 pattern_ptr = string->load("\0");
    u64 object_size = object->size(object_ptr);
    const char* ch = (char*)object->unsafe(pattern_ptr);
    RX_ASSERT(ch == 0);
    RX_ASSERT(object_size == 0);
    RX_ASSERT(object_ptr == 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    string->free(object_ptr);
    string->free(pattern_ptr);
    object->free(object_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_object_load_string_unsafe_0, .fixture = test_fixture) {
    pointer->init(8);
    const char* test_data = "Hello, world!";
    const void* ptr = (const void*)test_data;
    u64 object_ptr = object->load(ptr, strlen(ptr));
    u64 pattern_ptr = string->load("\0");
    u64 object_size = object->size(object_ptr);
    const char* ch = (char*)object->unsafe(0);
    RX_ASSERT(ch == 0);
    RX_ASSERT(object_size == strlen(ptr));
    RX_ASSERT(object_ptr != 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    string->free(object_ptr);
    string->free(pattern_ptr);
    object->free(object_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_object_load_string_unsafe, .fixture = test_fixture) {
    pointer->init(8);
    const char* test_data = "Hello, world!";
    const void* ptr = (const void*)test_data;
    u64 object_ptr = object->load(ptr, strlen(ptr));
    u64 pattern_ptr = string->load("\0");
    u64 object_size = object->size(object_ptr);
    char* ch = (char*)object->unsafe(object_ptr);
    *ch = 'Z';
    RX_ASSERT(object_size == strlen(ptr));
    RX_ASSERT(object_ptr != 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    string->free(object_ptr);
    string->free(pattern_ptr);
    object->free(object_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_strcat_alloc_alloc, .fixture = test_fixture) {
    pointer->init(8);
    u64 char_ptr = string->load("\0");
    u64 pattern_ptr = string->load("\0");
    string->strcat(pattern_ptr, char_ptr);
    RX_ASSERT(char_ptr == 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    string->free(char_ptr);
    string->free(pattern_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_offset_ptr_diff, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr1 = string->load("192.168.0.111");
    u64 string_ptr2 = string->load("192.168.0.12");
    u64 last_matched_ptr1 = string->offset(string_ptr1, string_ptr2);
    RX_ASSERT(last_matched_ptr1 == 0);
#ifndef USE_GC
    string->free(last_matched_ptr1);
    string->free(string_ptr1);
    string->free(string_ptr2);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_offset_ptr_less, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr1 = string->load("192.168.0.11");
    u64 string_ptr2 = string->load("8.0.112");
    u64 last_matched_ptr1 = string->offset(string_ptr1, string_ptr2);
    RX_ASSERT(last_matched_ptr1 == 0);
#ifndef USE_GC
    string->free(last_matched_ptr1);
    string->free(string_ptr1);
    string->free(string_ptr2);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_offset_ptr_offset, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr1 = string->load("192.168.0.11");
    u64 string_ptr2 = string->load("8.0.12");
    u64 last_matched_ptr1 = string->offset(string_ptr1, string_ptr2);
    RX_ASSERT(last_matched_ptr1 == 0);
#ifndef USE_GC
    string->free(last_matched_ptr1);
    string->free(string_ptr1);
    string->free(string_ptr2);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_offset_ptr_offset_found, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr1 = string->load("192.168.0.111");
    u64 string_ptr2 = string->load("8.0.");
    u64 last_matched_ptr1 = string->offset(string_ptr1, string_ptr2);
    RX_ASSERT(strcmp(string->unsafe(last_matched_ptr1), "111") == 0);
#ifndef USE_GC
    string->free(last_matched_ptr1);
    string->free(string_ptr1);
    string->free(string_ptr2);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_free_list, .fixture = test_fixture) {
    pointer->init(8);
#ifndef USE_GC
    u64 list_ptr = list->alloc();
    u64 string_ptr = string->load("hello");
    u64 e_ptr = string->load("e");
    u64 string_pointer_ptr = string->offset(string_ptr, e_ptr);
    string->free(list_ptr);
    string->free(string_pointer_ptr);
    string->free(string_ptr);
    string->free(e_ptr);
    list->free(list_ptr);
#else
    list->alloc();
    u64 string_ptr = string->load("hello");
    u64 e_ptr = string->load("e");
    string->offset(string_ptr, e_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_free, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    u64 string_ptr = string->load("hello");
    u64 e_ptr = string->load("e");
    u64 string_pointer_ptr = string->offset(string_ptr, e_ptr);
    list->push(list_ptr, string_pointer_ptr);
    list->push(list_ptr, string_ptr);
#ifndef USE_GC
    string->free(e_ptr);
    list->free(list_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_offset_list, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    u64 string_ptr = string->load("hello");
    u64 last_matched_ptr = string->offset(list_ptr, string_ptr);
    RX_ASSERT(last_matched_ptr == 0);
#ifndef USE_GC
    string->free(string_ptr);
    list->free(list_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_match, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    u64 string_ptr = string->load("192.192.0.1");
    u64 dot_ptr = string->load("192.");
    u64 string_pointer_ptr1 = string->match(string_ptr, dot_ptr);
    u64 string_pointer_ptr2 = string->match(string_pointer_ptr1, dot_ptr);
    RX_ASSERT(strcmp(string->unsafe(string_pointer_ptr1), "192.0.1") == 0);
    RX_ASSERT(strcmp(string->unsafe(string_pointer_ptr2), "0.1") == 0);
    list->push(list_ptr, string_pointer_ptr1);
    list->push(list_ptr, string_pointer_ptr2);
    list->push(list_ptr, string_ptr);
#ifndef USE_GC
    string->free(dot_ptr);
    list->free(list_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_match_miss, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    u64 string_ptr = string->load("192.168.0.1");
    u64 dot_ptr = string->load(":");
    u64 string_pointer_ptr1 = string->match(string_ptr, dot_ptr);
    RX_ASSERT(string_pointer_ptr1 == 0);
    list->push(list_ptr, string_pointer_ptr1);
    list->push(list_ptr, string_ptr);
#ifndef USE_GC
    string->free(dot_ptr);
    list->free(list_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_match_pattern, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    u64 string_ptr = string->load("192.168.0.112");
    u64 dot_ptr = string->load("192.168.0.");
    u64 string_pointer_ptr1 = string->match(string_ptr, dot_ptr);
    RX_ASSERT(strcmp(string->unsafe(string_pointer_ptr1), "112") == 0);
    list->push(list_ptr, string_pointer_ptr1);
    list->push(list_ptr, string_ptr);
#ifndef USE_GC
    string->free(dot_ptr);
    list->free(list_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_match_pattern_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    u64 string_ptr = string->load("192.168.0.112");
    u64 dot_ptr = string->load("193");
    u64 string_pointer_ptr1 = string->match(string_ptr, dot_ptr);
    RX_ASSERT(string_pointer_ptr1 == 0);
    list->push(list_ptr, string_pointer_ptr1);
    list->push(list_ptr, string_ptr);
#ifndef USE_GC
    string->free(dot_ptr);
    list->free(list_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_match_pattern_none, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    u64 string_ptr = string->load("192.168.0.11");
    u64 dot_ptr = string->load("192.168.0.111");
    u64 string_pointer_ptr1 = string->match(string_ptr, dot_ptr);
    RX_ASSERT(string_pointer_ptr1 == 0);
    list->push(list_ptr, string_pointer_ptr1);
    list->push(list_ptr, string_ptr);
#ifndef USE_GC
    string->free(dot_ptr);
    list->free(list_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_release_string, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr = string->load("192.168.0.11");
    list->release(string_ptr);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_match_pattern_load_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr = string->load("192.168.0.1");
    u64 dot_ptr = string->load("57.34.7.2");
    u64 error_ptr = string->match(string_ptr, dot_ptr);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_offset_pattern_load_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr = string->load("192.168.0.1");
    u64 dot_ptr = string->load("57.34.7.2");
    u64 error_ptr = string->offset(string_ptr, dot_ptr);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_match_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr = string->load("192.168.0.1");
    u64 string_pointer_ptr = string->match(string_ptr, 0);
    RX_ASSERT(string_pointer_ptr == 0);
#ifndef USE_GC
    string->free(string_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_match_0_0, .fixture = test_fixture) {
    pointer->init(8);
#ifndef USE_GC
    u64 string_ptr = string->load("192.168.0.1");
    u64 string_pointer_ptr = string->match(0, 0);
    RX_ASSERT(string_pointer_ptr == 0);
    string->free(string_ptr);
#else
    string->load("192.168.0.1");
    u64 string_pointer_ptr = string->match(0, 0);
    RX_ASSERT(string_pointer_ptr == 0);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_match_0_string, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr = string->load("192.168.0.1");
    u64 string_pointer_ptr = string->match(0, string_ptr);
    RX_ASSERT(string_pointer_ptr == 0);
#ifndef USE_GC
    string->free(string_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_match_list, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    u64 string_ptr = string->load("hello");
    u64 last_matched_ptr = string->match(list_ptr, string_ptr);
    RX_ASSERT(last_matched_ptr == 0);
#ifndef USE_GC
    string->free(string_ptr);
    list->free(list_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_strchr, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    u64 string_ptr = string->load("192.168.0.1");
    u64 dot_ptr = string->load(".");
    u64 string_pointer_ptr1 = string->strchr(string_ptr, dot_ptr);
    u64 string_pointer_ptr2 = string->match(string_pointer_ptr1, dot_ptr);
    RX_ASSERT(strcmp(string->unsafe(string_pointer_ptr1), ".168.0.1") == 0);
    RX_ASSERT(strcmp(string->unsafe(string_pointer_ptr2), "168.0.1") == 0);
    list->push(list_ptr, string_pointer_ptr1);
    list->push(list_ptr, string_pointer_ptr2);
    list->push(list_ptr, string_ptr);
#ifndef USE_GC
    string->free(dot_ptr);
    list->free(list_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_strchr_miss, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    u64 string_ptr = string->load("192.168.0.1");
    u64 dot_ptr = string->load(":");
    u64 string_pointer_ptr1 = string->strchr(string_ptr, dot_ptr);
    RX_ASSERT(string_pointer_ptr1 == 0);
    list->push(list_ptr, string_pointer_ptr1);
    list->push(list_ptr, string_ptr);
#ifndef USE_GC
    string->free(dot_ptr);
    list->free(list_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_strchr_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr = string->load("192.168.0.1");
    u64 string_pointer_ptr = string->strchr(string_ptr, 0);
    RX_ASSERT(string_pointer_ptr == 0);
#ifndef USE_GC
    string->free(string_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_strchr_0_0, .fixture = test_fixture) {
    pointer->init(8);
#ifndef USE_GC
    u64 string_ptr = string->load("192.168.0.1");
    u64 string_pointer_ptr = string->strchr(0, 0);
    RX_ASSERT(string_pointer_ptr == 0);
    string->free(string_ptr);
#else
    string->load("192.168.0.1");
    u64 string_pointer_ptr = string->strchr(0, 0);
    RX_ASSERT(string_pointer_ptr == 0);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_strchr_0_string, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr = string->load("192.168.0.1");
    u64 string_pointer_ptr = string->strchr(0, string_ptr);
    RX_ASSERT(string_pointer_ptr == 0);
#ifndef USE_GC
    string->free(string_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_strchr_list, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    u64 string_ptr = string->load("hello");
    u64 last_matched_ptr = string->strchr(list_ptr, string_ptr);
    RX_ASSERT(last_matched_ptr == 0);
#ifndef USE_GC
    string->free(string_ptr);
    list->free(list_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_strrchr, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr = string->load("192.168.0.1");
    u64 dot_ptr = string->load(".");
    u64 string_pointer_ptr1 = string->strrchr(string_ptr, dot_ptr);
    u64 string_pointer_ptr2 = string->match(string_pointer_ptr1, dot_ptr);
    RX_ASSERT(strcmp(string->unsafe(string_pointer_ptr1), ".1") == 0);
    RX_ASSERT(strcmp(string->unsafe(string_pointer_ptr2), "1") == 0);
#ifndef USE_GC
    string->free(string_pointer_ptr1);
    string->free(string_pointer_ptr2);
    string->free(string_ptr);
    string->free(dot_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_strrchr_match_offset, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr = string->load("192.168.0.111");
    u64 dot_ptr = string->load(".");
    u64 string_pointer_ptr1 = string->strrchr(string_ptr, dot_ptr);
    u64 string_pointer_ptr2 = string->match(string_pointer_ptr1, dot_ptr);
    u64 string_pointer_ptr3 = string->offset(string_ptr, dot_ptr);
    RX_ASSERT(strcmp(string->unsafe(string_pointer_ptr1), ".111") == 0);
    RX_ASSERT(strcmp(string->unsafe(string_pointer_ptr2), "111") == 0);
    RX_ASSERT(strcmp(string->unsafe(string_pointer_ptr3), "111") == 0);
#ifndef USE_GC
    string->free(string_pointer_ptr1);
    string->free(string_pointer_ptr2);
    string->free(string_pointer_ptr3);
    string->free(string_ptr);
    string->free(dot_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_peekn_0_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    u64 error_ptr = list->peekn(0, 0);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_peekn_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    u64 error_ptr = list->peekn(list_ptr, 0);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_peekn_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr1 = list->alloc();
    u64 string_ptr1 = string->load("192.168.0.1");
    list->push(list_ptr1, string_ptr1);
    u64 list_ptr2 = list->peekn(list_ptr1, 1);
    RX_ASSERT(list_ptr2 != 0);
    RX_ASSERT(list->size(list_ptr2) != 0);
    u64 string_ptr2 = list->peek(list_ptr2);
    RX_ASSERT(string_ptr2 != 0);
    RX_ASSERT(string_ptr1 == string_ptr2);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_peekn_2, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr = string->load("192.168.0.1");
    u64 list_ptr = list->alloc();
    list->push(list_ptr, string_ptr);
    u64 error_ptr = list->peekn(list_ptr, 2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_popn_0_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    u64 error_ptr = list->popn(0, 0);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_popn_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    u64 error_ptr = list->popn(list_ptr, 0);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_popn_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr1 = list->alloc();
    u64 string_ptr1 = string->load("192.168.0.1");
    list->push(list_ptr1, string_ptr1);
    u64 list_ptr2 = list->popn(list_ptr1, 1);
    RX_ASSERT(list_ptr2 != 0);
    RX_ASSERT(list->size(list_ptr2) != 0);
    u64 string_ptr2 = list->peek(list_ptr2);
    RX_ASSERT(string_ptr2 != 0);
    RX_ASSERT(string_ptr1 == string_ptr2);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_popn_2, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr = string->load("192.168.0.1");
    u64 list_ptr = list->alloc();
    list->push(list_ptr, string_ptr);
    u64 error_ptr = list->popn(list_ptr, 2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_strchr_match_offset, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    u64 list_match_ptr = list->alloc();
    u64 string_ptr = string->load("a.bc.bb.ba.a");
    u64 pattern_ptr = string->load(".b");
    u64 string_pointer_ptr = 0;
    u64 match_ptr = string_ptr;
    u64 current_ptr = match_ptr;
    while ((string_pointer_ptr = string->strchr(current_ptr, pattern_ptr)) != 0) {
        list->push(list_ptr, string_pointer_ptr);
        match_ptr = string->match(string_pointer_ptr, pattern_ptr);
        list->push(list_match_ptr, match_ptr);
        current_ptr = match_ptr;
    }
    u64 size = list->size(list_ptr);
    RX_ASSERT(size == 4);
    u64 string_ptr1 = list->pop(list_ptr);
    u64 string_ptr2 = list->pop(list_ptr);
    u64 string_ptr3 = list->pop(list_ptr);
    u64 string_ptr4 = list->pop(list_ptr);
    RX_ASSERT(strcmp(string->unsafe(string_ptr1), ".a") == 0);
    RX_ASSERT(strcmp(string->unsafe(string_ptr2), ".ba.a") == 0);
    RX_ASSERT(strcmp(string->unsafe(string_ptr3), ".bb.ba.a") == 0);
    RX_ASSERT(strcmp(string->unsafe(string_ptr4), ".bc.bb.ba.a") == 0);
    RX_ASSERT(list->size(list_ptr) == 0);
    u64 match_size = list->size(list_match_ptr);
    RX_ASSERT(match_size == 3);
    u64 string_match_ptr1 = list->pop(list_match_ptr);
    u64 string_match_ptr2 = list->pop(list_match_ptr);
    u64 string_match_ptr3 = list->pop(list_match_ptr);
    RX_ASSERT(strcmp(string->unsafe(string_match_ptr1), "a.a") == 0);
    RX_ASSERT(strcmp(string->unsafe(string_match_ptr2), "b.ba.a") == 0);
    RX_ASSERT(strcmp(string->unsafe(string_match_ptr3), "c.bb.ba.a") == 0);
    RX_ASSERT(list->size(list_match_ptr) == 0);
#ifndef USE_GC
    string->free(string_ptr1);
    string->free(string_ptr2);
    string->free(string_ptr3);
    string->free(string_ptr4);
    string->free(string_match_ptr1);
    string->free(string_match_ptr2);
    string->free(string_match_ptr3);
    string->free(string_ptr);
    string->free(pattern_ptr);
    list->free(list_match_ptr);
    list->free(list_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_strrchr_miss, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    u64 string_ptr = string->load("192.168.0.1");
    u64 dot_ptr = string->load(":");
    u64 string_pointer_ptr1 = string->strrchr(string_ptr, dot_ptr);
    RX_ASSERT(string_pointer_ptr1 == 0);
    list->push(list_ptr, string_pointer_ptr1);
    list->push(list_ptr, string_ptr);
#ifndef USE_GC
    string->free(dot_ptr);
    list->free(list_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_strrchr_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr = string->load("192.168.0.1");
    u64 string_pointer_ptr = string->strrchr(string_ptr, 0);
    RX_ASSERT(string_pointer_ptr == 0);
#ifndef USE_GC
    string->free(string_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_strrchr_0_0, .fixture = test_fixture) {
    pointer->init(8);
#ifndef USE_GC
    u64 string_ptr = string->load("192.168.0.1");
    u64 string_pointer_ptr = string->strrchr(0, 0);
    RX_ASSERT(string_pointer_ptr == 0);
    string->free(string_ptr);
#else
    string->load("192.168.0.1");
    u64 string_pointer_ptr = string->strrchr(0, 0);
    RX_ASSERT(string_pointer_ptr == 0);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_strrchr_0_string, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr = string->load("192.168.0.1");
    u64 string_pointer_ptr = string->strrchr(0, string_ptr);
    RX_ASSERT(string_pointer_ptr == 0);
#ifndef USE_GC
    string->free(string_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_strrchr_list, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    u64 string_ptr = string->load("hello");
    u64 last_matched_ptr = string->strrchr(list_ptr, string_ptr);
    RX_ASSERT(last_matched_ptr == 0);
#ifndef USE_GC
    string->free(string_ptr);
    list->free(list_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_push_0_1, .fixture = test_fixture) {
    pointer->init(8);
    list->push(0, 1);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_push_1_0, .fixture = test_fixture) {
    pointer->init(8);
    list->push(1, 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_push_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    list->push(list_ptr, 0);
#ifndef USE_GC
    list->free(list_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_push_list, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    list->push(list_ptr, list_ptr);
#ifndef USE_GC
    list->free(list_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_push_string, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr = string->load("@");
    u64 list_ptr = list->alloc();
    list->push(string_ptr, list_ptr);
#ifndef USE_GC
    list->free(list_ptr);
    string->free(string_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_size_string, .fixture = test_fixture) {
    pointer->init(8);
#ifndef USE_GC
    u64 string_ptr = string->load("@");
    u64 list_ptr = list->alloc();
    u64 value = list->size(string_ptr);
    RX_ASSERT(value == 0);
    list->free(list_ptr);
    string->free(string_ptr);
#else
    u64 string_ptr = string->load("@");
    list->alloc();
    u64 value = list->size(string_ptr);
    RX_ASSERT(value == 0);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_push_size, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr = string->load("@");
    u64 list_ptr = list->alloc();
    list->push(list_ptr, string_ptr);
    u64 value = list->size(list_ptr);
    RX_ASSERT(value != 0);
#ifndef USE_GC
    list->free(list_ptr);
    string->free(string_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_pop_0_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 value_ptr = list->pop(0);
    RX_ASSERT(value_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_pop_1_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 value_ptr = list->pop(1);
    RX_ASSERT(value_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_pop_list, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    u64 value_ptr = list->pop(list_ptr);
    RX_ASSERT(value_ptr == 0);
#ifndef USE_GC
    list->free(list_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_pop_string, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr = string->load("@");
    u64 list_ptr = list->alloc();
    list->push(list_ptr, string_ptr);
    u64 value_ptr = list->pop(list_ptr);
    RX_ASSERT(value_ptr != 0);
#ifndef USE_GC
    list->free(list_ptr);
    string->free(string_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_peek_0_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 value_ptr = list->peek(0);
    RX_ASSERT(value_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_peek_1_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 value_ptr = list->peek(1);
    RX_ASSERT(value_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_peek_list, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    u64 value_ptr = list->peek(list_ptr);
    RX_ASSERT(value_ptr == 0);
#ifndef USE_GC
    list->free(list_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_peek_string, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr = string->load("@");
    u64 list_ptr = list->alloc();
    list->push(list_ptr, string_ptr);
    u64 value_ptr = list->peek(list_ptr);
    RX_ASSERT(value_ptr != 0);
#ifndef USE_GC
    list->free(list_ptr);
    string->free(string_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_strcpy, .fixture = test_fixture) {
    pointer->init(8);
    u64 path_ptr1 = string->load("/");
    u64 path_ptr2 = string->load("@");
    u64 path_copy_ptr = string->copy(path_ptr1);
    string->strcat(path_copy_ptr, path_ptr2);

    char* path1 = string->unsafe(path_ptr1);
    char* path2 = string->unsafe(path_ptr2);
    u64 path1_len = strlen(path1);
    u64 path2_len = strlen(path2);
    RX_ASSERT(path1_len > 0);
    RX_ASSERT(path2_len > 0);
    char* buf = calloc(1, path1_len + path2_len + 1);
    strcpy(buf, path1); /* NOLINT */
    strcat(buf, path2); /* NOLINT */
    char* path_copy = string->unsafe(path_copy_ptr);
    RX_ASSERT(strlen(path_copy) == strlen(buf));
    RX_ASSERT(strcmp(path_copy, buf) == 0);
    free(buf);
#ifndef USE_GC
    string->free(path_ptr1);
    string->free(path_ptr2);
    string->free(path_copy_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_strcat_load_alloc_copy, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr = string->load("/all_english_words.txt");
    u64 zero_ptr = string->load("\0");
    u64 data_ptr = data->alloc(1);
    u64 list_ptr = list->alloc();
    u64 empty_ptr = string->load("\0");

    u64 null_ptr = 0;
    u64 none_ptr = 0xfffffffff;

    u64 data_ptr1 = string->copy(null_ptr);
    u64 data_ptr2 = string->copy(zero_ptr);
    u64 data_ptr3 = string->copy(string_ptr);
    u64 data_ptr4 = string->copy(none_ptr);

    os->putc(string_ptr);
    os->putc(zero_ptr);
    os->putc(data_ptr);
    os->putc(list_ptr);
    os->putc(empty_ptr);
    os->putc(null_ptr);
    os->putc(none_ptr);

    string->put_char(string_ptr, 'a');
    string->put_char(zero_ptr, 'a');
    string->put_char(data_ptr, 'a');
    string->put_char(list_ptr, 'a');
    string->put_char(empty_ptr, 'a');
    string->put_char(null_ptr, 'a');
    string->put_char(none_ptr, 'a');

    string->put_char(string_ptr, '\0');
    string->put_char(zero_ptr, '\0');
    string->put_char(data_ptr, '\0');
    string->put_char(list_ptr, '\0');
    string->put_char(empty_ptr, '\0');
    string->put_char(null_ptr, '\0');
    string->put_char(none_ptr, '\0');

    string->put_char(string_ptr, '/');
    string->strcpy(string_ptr, string_ptr);
    string->strcpy(string_ptr, zero_ptr);
    string->strcpy(zero_ptr, string_ptr);
    string->strcpy(data_ptr, null_ptr);
    string->strcpy(list_ptr, null_ptr);
    string->strcpy(string_ptr, data_ptr);
    string->strcpy(string_ptr, empty_ptr);
    string->strcpy(data_ptr, null_ptr);
    string->strcpy(list_ptr, string_ptr);
    string->strcpy(data_ptr, null_ptr);
    string->strcpy(list_ptr, null_ptr);
    string->strcpy(string_ptr, data_ptr);
    string->strcpy(data_ptr, null_ptr);
    string->strcpy(list_ptr, string_ptr);
    string->strcpy(null_ptr, null_ptr);
    string->strcpy(string_ptr, null_ptr);
    string->strcpy(string_ptr, none_ptr);
    string->strcpy(none_ptr, string_ptr);

    string->strcat(string_ptr, string_ptr);
    string->strcat(string_ptr, zero_ptr);
    string->strcat(zero_ptr, string_ptr);
    string->strcat(data_ptr, null_ptr);
    string->strcat(list_ptr, null_ptr);
    string->strcat(string_ptr, data_ptr);
    string->strcat(string_ptr, empty_ptr);
    string->strcat(data_ptr, null_ptr);
    string->strcat(list_ptr, string_ptr);
    string->strcat(data_ptr, null_ptr);
    string->strcat(list_ptr, null_ptr);
    string->strcat(string_ptr, data_ptr);
    string->strcat(data_ptr, null_ptr);
    string->strcat(list_ptr, string_ptr);
    string->strcat(null_ptr, null_ptr);
    string->strcat(string_ptr, null_ptr);
    string->strcat(string_ptr, none_ptr);
    string->strcat(none_ptr, string_ptr);

    u64 last_matched_ptr1 = string->offset(string_ptr, string_ptr);
    string->offset(data_ptr, null_ptr);
    string->offset(list_ptr, null_ptr);
    string->offset(string_ptr, data_ptr);
    string->offset(string_ptr, empty_ptr);
    string->offset(data_ptr, null_ptr);
    string->offset(list_ptr, string_ptr);
    string->offset(data_ptr, null_ptr);
    string->offset(list_ptr, null_ptr);
    string->offset(string_ptr, data_ptr);
    string->offset(data_ptr, null_ptr);
    string->offset(list_ptr, string_ptr);
    string->offset(null_ptr, null_ptr);
    string->offset(string_ptr, null_ptr);

    u64 last_matched_ptr2 = string->offset(string_ptr, string_ptr);
    string->offset(string_ptr, zero_ptr);
    string->offset(data_ptr, zero_ptr);
    string->offset(list_ptr, zero_ptr);
    string->offset(string_ptr, data_ptr);
    string->offset(zero_ptr, zero_ptr);
    string->offset(zero_ptr, data_ptr);
    string->offset(zero_ptr, list_ptr);
    string->offset(zero_ptr, string_ptr);
    string->offset(empty_ptr, empty_ptr);
    string->offset(empty_ptr, string_ptr);
    string->offset(empty_ptr, zero_ptr);

    string->offset(null_ptr, null_ptr);
    string->offset(null_ptr, empty_ptr);
    string->offset(null_ptr, string_ptr);
    string->offset(null_ptr, data_ptr);
    string->offset(null_ptr, list_ptr);
    string->offset(empty_ptr, null_ptr);
    string->offset(empty_ptr, empty_ptr);
    string->offset(empty_ptr, string_ptr);
    string->offset(empty_ptr, data_ptr);
    string->offset(empty_ptr, list_ptr);
    string->offset(string_ptr, null_ptr);
    string->offset(string_ptr, empty_ptr);

    u64 last_matched_ptr3 = string->offset(string_ptr, string_ptr);
    string->offset(string_ptr, data_ptr);
    string->offset(string_ptr, list_ptr);
    string->offset(data_ptr, null_ptr);
    string->offset(data_ptr, empty_ptr);
    string->offset(data_ptr, string_ptr);
    string->offset(data_ptr, data_ptr);
    string->offset(data_ptr, list_ptr);
    string->offset(list_ptr, null_ptr);
    string->offset(list_ptr, empty_ptr);
    string->offset(list_ptr, string_ptr);
    string->offset(list_ptr, data_ptr);
    string->offset(list_ptr, list_ptr);
    string->offset(string_ptr, none_ptr);
    string->offset(none_ptr, string_ptr);

    string->unsafe(null_ptr);
    string->unsafe(empty_ptr);
    string->unsafe(string_ptr);
    string->unsafe(data_ptr);
    string->unsafe(list_ptr);
    string->unsafe(none_ptr);

    file->alloc(zero_ptr, zero_ptr);
    file->alloc(string_ptr, zero_ptr);
    file->alloc(data_ptr, zero_ptr);
    file->alloc(list_ptr, zero_ptr);
    file->alloc(zero_ptr, string_ptr);
    file->alloc(zero_ptr, data_ptr);
    file->alloc(zero_ptr, list_ptr);
    file->alloc(string_ptr, string_ptr);
    file->alloc(data_ptr, null_ptr);
    file->alloc(list_ptr, null_ptr);
    file->alloc(string_ptr, data_ptr);
    file->alloc(string_ptr, empty_ptr);
    file->alloc(data_ptr, null_ptr);
    file->alloc(list_ptr, string_ptr);
    file->alloc(data_ptr, null_ptr);
    file->alloc(list_ptr, null_ptr);
    file->alloc(string_ptr, data_ptr);
    file->alloc(data_ptr, null_ptr);
    file->alloc(list_ptr, string_ptr);
    file->alloc(null_ptr, null_ptr);
    file->alloc(string_ptr, null_ptr);
    file->alloc(string_ptr, none_ptr);
    file->alloc(none_ptr, string_ptr);

    const u64* data_unsafe_ptr1 = data->unsafe(empty_ptr);
    const u64* data_unsafe_ptr2 = data->unsafe(null_ptr);
    const u64* data_unsafe_ptr3 = data->unsafe(string_ptr);
    const u64* data_unsafe_ptr4 = data->unsafe(list_ptr);
    const u64* data_unsafe_ptr5 = data->unsafe(none_ptr);

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

    u64 undefined_ptr = string->copy(list_ptr);

    RX_ASSERT(undefined_ptr == 0);
    RX_ASSERT(last_matched_ptr1 != 0);
    RX_ASSERT(last_matched_ptr2 != 0);
    RX_ASSERT(last_matched_ptr3 != 0);
#ifndef USE_GC
    string->free(last_matched_ptr1);
    string->free(last_matched_ptr2);
    string->free(last_matched_ptr3);
    string->free(last_matched_ptr2);
    string->free(last_matched_ptr3);
    string->free(null_ptr);
    string->free(zero_ptr);
    string->free(string_ptr);
    string->free(data_ptr3);
    string->free(list_ptr);
    string->free(empty_ptr);
    string->free(data_ptr);
    string->free(string_ptr);
    string->free(data_ptr3);
    string->free(empty_ptr);
    string->free(data_ptr);
    data->free(zero_ptr);
    data->free(data_ptr);
    data->free(data_ptr3);
    data->free(empty_ptr);
    data->free(null_ptr);
    data->free(data_ptr);
    data->free(list_ptr);
    data->free(data_ptr);
    data->free(data_ptr3);
    data->free(none_ptr);
    list->free(zero_ptr);
    list->free(data_ptr);
    list->free(empty_ptr);
    list->free(null_ptr);
    list->free(data_ptr);
    list->free(list_ptr);
    list->free(data_ptr);
    list->free(list_ptr);
    list->free(none_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_strcat_alloc_load, .fixture = test_fixture) {
    pointer->init(8);
    u64 zero_ptr = string->load("\0");
    u64 char_ptr = string->load("/");
    string->strcat(zero_ptr, char_ptr);
    RX_ASSERT(zero_ptr == 0);
    RX_ASSERT(strcmp(string->unsafe(char_ptr), "/") == 0);
#ifndef USE_GC
    string->free(zero_ptr);
    string->free(char_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_strcat_load_alloc_alloc, .fixture = test_fixture) {
    pointer->init(8);
    u64 char_ptr = string->load("/");
    u64 pattern_ptr = string->load("\0");
    u64 empty_ptr = string->load("\0");
    string->strcat(pattern_ptr, char_ptr);
    string->strcpy(empty_ptr, pattern_ptr);
    RX_ASSERT(pattern_ptr == 0);
    RX_ASSERT(empty_ptr == 0);
    RX_ASSERT(strcmp(string->unsafe(char_ptr), "/") == 0);
#ifndef USE_GC
    string->free(empty_ptr);
    string->free(pattern_ptr);
    string->free(char_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_strcat_load_load, .fixture = test_fixture) {
    pointer->init(8);
    u64 char_ptr = string->load("/");
    u64 pattern_ptr = string->load("*");
    string->strcat(pattern_ptr, char_ptr);
    RX_ASSERT(strcmp(string->unsafe(pattern_ptr), "*/") == 0);
    RX_ASSERT(strcmp(string->unsafe(char_ptr), "/") == 0);
#ifndef USE_GC
    string->free(pattern_ptr);
    string->free(char_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_strcpy_load_alloc, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("/");
    u64 empty_ptr = string->load("\0");
    string->strcpy(empty_ptr, pattern_ptr);
    RX_ASSERT(empty_ptr == 0);
    RX_ASSERT(strcmp(string->unsafe(pattern_ptr), "/") == 0);
#ifndef USE_GC
    string->free(empty_ptr);
    string->free(pattern_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_strcpy_load_load, .fixture = test_fixture) {
    pointer->init(8);
    u64 char_ptr = string->load("/input.txt");
    u64 pattern_ptr = string->load("*");
    string->strcpy(pattern_ptr, char_ptr);
    RX_ASSERT(strcmp(string->unsafe(pattern_ptr), "/input.txt") == 0);
    RX_ASSERT(strcmp(string->unsafe(char_ptr), "/input.txt") == 0);
#ifndef USE_GC
    string->free(pattern_ptr);
    string->free(char_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_os_getenv, .fixture = test_fixture) {
    pointer->init(8);
    u64 ui_mode_ptr = string->load("UI_MODE");
    u64 file_path_ptr = os->getenv(ui_mode_ptr);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_os_getenv_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 file_path_ptr = os->getenv(0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_os_getenv_list, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    u64 file_path_ptr = os->getenv(list_ptr);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_load_open_file_close_file, .fixture = test_fixture) {
    pointer->init(8);
    u64 file_path_ptr = os->getcwd();
    u64 file_name_ptr = string->load("/nonexistent.txt");
    string->strcat(file_path_ptr, file_name_ptr);
    string->free(file_name_ptr);
    u64 mode_ptr = string->load("rb");
    u64 f_ptr = file->alloc(file_path_ptr, mode_ptr);
    u64 data_ptr = file->data(f_ptr);
    RX_ASSERT(f_ptr == 0);
    RX_ASSERT(data_ptr == 0);
#ifndef USE_GC
    file->free(f_ptr);
    data->free(data_ptr);
    string->free(mode_ptr);
    string->free(file_name_ptr);
    string->free(file_path_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_offset_strcpy, .fixture = test_fixture) {
    pointer->init(8);
    u64 name_ptr = string->load("name");
    u64 at_ptr = string->load("@");
    u64 domain_ptr = string->load("domain.org");
    string->strcat(name_ptr, at_ptr);
    string->strcat(name_ptr, domain_ptr);

    u64 path_ptr1 = string->load("name@domain.org");
    u64 path_ptr2 = string->copy(at_ptr);

    u64 domain_name = string->offset(path_ptr1, path_ptr2);
    string->strcpy(name_ptr, domain_name);

    char* domain = string->unsafe(domain_ptr);
    char* name = string->unsafe(name_ptr);
    RX_ASSERT(strcmp(domain, "domain.org") == 0);
    RX_ASSERT(strcmp(name, "domain.org") == 0);

#ifndef USE_GC
    string->free(domain_name);
    string->free(name_ptr);
    string->free(path_ptr1);
    string->free(path_ptr2);
    string->free(name_ptr);
    string->free(domain_ptr);
    string->free(at_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_offset_strcpy_free, .fixture = test_fixture) {
    pointer->init(8);
    u64 name_ptr = string->load("name");
    u64 at_ptr = string->load("@");
    u64 domain_ptr = string->load("domain.org");
    string->strcat(name_ptr, at_ptr);
    string->strcat(name_ptr, domain_ptr);

    u64 path_ptr1 = string->load("name@domain.org");
    u64 path_ptr2 = string->copy(at_ptr);

    u64 domain_name = string->offset(path_ptr1, path_ptr2);
    string->free(path_ptr1);
    string->strcpy(name_ptr, domain_name);

    char* domain = string->unsafe(domain_ptr);
    char* name = string->unsafe(name_ptr);
    RX_ASSERT(strcmp(domain, "domain.org") == 0);
    RX_ASSERT(strcmp(name, "name@domain.org") == 0);

#ifndef USE_GC
    string->free(domain_name);
    string->free(name_ptr);
    string->free(path_ptr2);
    string->free(name_ptr);
    string->free(domain_ptr);
    string->free(at_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_print_free, .fixture = test_fixture) {
    pointer->init(8);
    u64 printing_ptr = string->load("hello, world!");
    string->free(printing_ptr);
    os->putc(printing_ptr);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_print, .fixture = test_fixture) {
    pointer->init(8);
    u64 printing_ptr = string->load("hello, world!");
    os->putc(printing_ptr);
#ifndef USE_GC
    string->free(printing_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_print_string_pointer, .fixture = test_fixture) {
    pointer->init(8);
    u64 printing_ptr = string->load("hello, world!");
    u64 comma_ptr = string->load(",");
    u64 substring_index_ptr = string->offset(printing_ptr, comma_ptr);
    os->putc(substring_index_ptr);
#ifndef USE_GC
    string->free(printing_ptr);
    string->free(substring_index_ptr);
    string->free(comma_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_print_string_pointer_copy, .fixture = test_fixture) {
    pointer->init(8);
    const char* substring_expected = " world!";
    u64 printing_ptr = string->load("hello, world!");
    u64 comma_ptr = string->load(",");
    u64 substring_index_ptr = string->offset(printing_ptr, comma_ptr);
    u64 substring_ptr = string->copy(substring_index_ptr);

    char* substring_actual = string->unsafe(substring_ptr);
    RX_ASSERT(strcmp(substring_expected, substring_actual) == 0);

#ifndef USE_GC
    string->free(printing_ptr);
    string->free(comma_ptr);
    string->free(substring_index_ptr);
    string->free(substring_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_size, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    u64 size_actual = string->size(list_ptr);
    u64 size_expected = 0;
    RX_ASSERT(size_expected == size_actual);
#ifndef USE_GC
    list->free(list_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_unsafe, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    char* ptr_actual = string->unsafe(list_ptr);
    char* ptr_expected = 0;
    RX_ASSERT(ptr_expected == ptr_actual);
#ifndef USE_GC
    list->free(list_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_list_offset, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr = string->load("himem.sys");
    u64 list_ptr = list->alloc();
    u64 size_actual = string->offset(string_ptr, list_ptr);
    u64 size_expected = 0;
    RX_ASSERT(size_expected == size_actual);
#ifndef USE_GC
    list->free(list_ptr);
    string->free(string_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, teststring_pointer_unsafe, .fixture = test_fixture) {
    pointer->init(8);
    u64 printing_ptr = string->load("hello, world!");
    u64 comma_ptr = string->load(",");
    u64 substring_index_ptr = string->offset(printing_ptr, comma_ptr);

    char* expected_value = string->unsafe(substring_index_ptr);
    const char* actual_value = " world!";
    RX_ASSERT(strcmp(expected_value, actual_value) == 0);

    string->free(substring_index_ptr);
    os->putc(substring_index_ptr);
#ifndef USE_GC
    string->free(printing_ptr);
    string->free(comma_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_pointer_size, .fixture = test_fixture) {
    pointer->init(8);
    u64 printing_ptr = string->load("hello, world!");
    u64 comma_ptr = string->load(",");
    u64 substring_index_ptr = string->offset(printing_ptr, comma_ptr);
    u64 substring_ptr = string->copy(substring_index_ptr);

    u64 size_expected = strlen(" world!"); /* adds one 0 to termination byte */
    u64 size_actual = string->size(substring_ptr);
    RX_ASSERT(size_expected == size_actual);

#ifndef USE_GC
    string->free(printing_ptr);
    string->free(comma_ptr);
    string->free(substring_index_ptr);
    string->free(substring_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_string_offset_subsearch, .fixture = test_fixture) {
    pointer->init(8);
    u64 printing_ptr = string->load("hello, world!");
    u64 comma_ptr = string->load(",");
    u64 w_ptr = string->load("w");
    u64 substring_index_ptr1 = string->offset(printing_ptr, comma_ptr);
    u64 substring_index_ptr2 = string->offset(substring_index_ptr1, w_ptr);
    u64 substring_ptr = string->copy(substring_index_ptr2);

    u64 size_expected = strlen("orld!"); /* adds one 0 to termination byte */
    u64 size_actual = string->size(substring_ptr);
    RX_ASSERT(size_expected == size_actual);

#ifndef USE_GC
    string->free(printing_ptr);
    string->free(comma_ptr);
    string->free(substring_index_ptr1);
    string->free(substring_index_ptr2);
    string->free(substring_ptr);
    string->free(w_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_print_string_pointer_free, .fixture = test_fixture) {
    pointer->init(8);
    u64 printing_ptr = string->load("hello, world!");
    u64 comma_ptr = string->load(",");
    u64 substring_index_ptr = string->offset(printing_ptr, comma_ptr);
    string->free(substring_index_ptr);
    os->putc(substring_index_ptr);
#ifndef USE_GC
    string->free(printing_ptr);
    string->free(comma_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_offset_strcat, .fixture = test_fixture) {
    pointer->init(8);
    u64 name_ptr = string->load("name");
    u64 at_ptr = string->load("@");
    string->strcat(name_ptr, at_ptr);

    u64 path_ptr1 = string->load("name@domain.org");
    u64 path_ptr2 = string->copy(at_ptr);

    u64 domain_name = string->offset(path_ptr1, path_ptr2);
    string->strcat(name_ptr, domain_name);

    char* domain = string->unsafe(path_ptr1);
    char* name = string->unsafe(name_ptr);
    u64 domain_len = strlen(domain);
    u64 name_len = strlen(name);
    RX_ASSERT(domain_len > 0);
    RX_ASSERT(name_len > 0);
    RX_ASSERT(domain_len == name_len);
    RX_ASSERT(strcmp(domain, name) == 0);

#ifndef USE_GC
    string->free(domain_name);
    string->free(name_ptr);
    string->free(path_ptr1);
    string->free(path_ptr2);
    string->free(name_ptr);
    string->free(at_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_file_read_invalid_type, .fixture = test_fixture) {
    pointer->init(8);
#ifndef USE_GC
    u64 list_ptr = list->alloc();
    u64 data_ptr = file->data(list_ptr);
    data->free(data_ptr);
    file->free(list_ptr);
    list->free(list_ptr);
#else
    u64 list_ptr = list->alloc();
    file->data(list_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_load_open_match_last_unsafe_free, .fixture = test_fixture) {
    pointer->init(8);
    u64 file_path_ptr = os->getcwd();
    u64 file_name_ptr = string->load("/all_english_words.txt//");
    string->strcat(file_path_ptr, file_name_ptr);
    u64 pattern_ptr = string->load("//");
    u64 last_match_ptr = string->offset(file_path_ptr, pattern_ptr);
    string->free(pattern_ptr);
    string->put_char(last_match_ptr, '\0');
    string->free(last_match_ptr);
    string->strcat(file_path_ptr, file_name_ptr);
    string->free(file_name_ptr);
    u64 mode_ptr = string->load("rb");
    u64 f_ptr = file->alloc(file_path_ptr, mode_ptr);
    string->free(file_path_ptr);
    string->free(mode_ptr);
    u64 data_ptr = file->data(f_ptr);
    file->free(f_ptr);
    os->putc(data_ptr);
#ifndef USE_GC
    data->free(data_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_load_open_match_last_unsafe_free_unsuppported_calls, .fixture = test_fixture) {
    pointer->init(8);
    u64 ptr1 = string->load("qwerty//u");
    u64 ptr2 = string->load("asdfghjkl");
    u64 pattern_ptr = string->load("/u");
    u64 last_match_ptr = string->offset(ptr1, pattern_ptr);
    string->strcat(last_match_ptr, ptr2);
    string->free(pattern_ptr);
    string->put_char(last_match_ptr, '\0');
#ifndef USE_GC
    string->free(ptr1);
    string->free(ptr2);
    string->free(last_match_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_user, .fixture = test_fixture) {
    pointer->init(8);
#ifndef USE_GC
    u64 ptr1 = user->alloc();
    user->free(ptr1);
#else
    user->alloc();
#endif
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests_v1, test_load_open_file_unsafe_hashtable, .fixture = test_fixture) {
    pointer->init(8);
    u64 file_path_ptr = os->getcwd();
    u64 file_name_ptr = string->load("/all_english_words.txt");
    string->strcat(file_path_ptr, file_name_ptr);
    string->free(file_name_ptr);
    u64 mode_ptr = string->load("rb");
    u64 f_ptr = file->alloc(file_path_ptr, mode_ptr);
    if (f_ptr != 0) {
        u64 data_ptr = file->data(f_ptr);
        u64 list_ptr = list->alloc();
        file->free(f_ptr);
        u8* file_data = data->unsafe(data_ptr);
        for (int i = 0; i < 100; i++) {
            u8* tmp = file_data;
            while (*tmp != 0 && *tmp != '\n') {
                tmp++;
            }
            *tmp++ = '\0';
            u64 string_ptr = string->load((char*)file_data);
            list->push(list_ptr, string_ptr);
            os->putc(string_ptr);
            file_data = tmp;
        }
        list->free(list_ptr);
        data->free(data_ptr);
    }
#ifndef USE_GC
    string->free(mode_ptr);
    string->free(file_name_ptr);
    string->free(file_path_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

RX_TEST_CASE(tests_v1, test_load_load_match_last, .fixture = test_fixture) {
    pointer->init(8);
    u64 str_ptr = string->load("Hello, world!");
    u64 ch_ptr = string->load("z");
    u64 last_match_ptr = string->offset(str_ptr, ch_ptr);
    RX_ASSERT(last_match_ptr == 0);
#ifndef USE_GC
    string->free(last_match_ptr);
    string->free(str_ptr);
    string->free(ch_ptr);
#endif
    pointer->gc();
    pointer->destroy();
}

/* internal */
static void parse_text(u64 text_string_ptr);

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_init, .fixture = test_fixture) {
    pointer->init(8);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_load, .fixture = test_fixture) {
    pointer->init(8);
    string->load("asdf");
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_init_string_load_9, .fixture = test_fixture) {
    pointer->init(8);
    string->load("0");
    string->load("1");
    string->load("2");
    string->load("3");
    string->load("4");
    string->load("5");
    string->load("6");
    string->load("7");
    string->load("8");
    string->load("9");
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_strings, .fixture = test_fixture) {
    pointer->init(8);
    u64 text_string_ptr0 = string->load("a");
    u64 text_string_ptr1 = string->load("a\nb");
    u64 text_string_ptr2 = string->load("ab\nabc\n");
    u64 text_string_ptr3 = string->load("adadadsadsadad\ndad\nadsaddasaddad\nsad\n");
    u64 text_string_ptr4 = string->load("ab\ndad\nabcd\nbcd\n");
    u64 text_string_ptr5 = string->load("ab\nc\nabc\nbcd\n");
    u64 text_string_ptr6 = string->load("abc\nabcd\nbcde\nabc\n");
    u64 text_string_ptr7 = string->load("abc\n\n");
    parse_text(0);
    parse_text(text_string_ptr0);
    parse_text(text_string_ptr1);
    parse_text(text_string_ptr2);
    parse_text(text_string_ptr3);
    parse_text(text_string_ptr4);
    parse_text(text_string_ptr5);
    parse_text(text_string_ptr6);
    parse_text(text_string_ptr7);
    string->free(text_string_ptr0);
    string->free(text_string_ptr1);
    string->free(text_string_ptr2);
    string->free(text_string_ptr3);
    string->free(text_string_ptr4);
    string->free(text_string_ptr5);
    string->free(text_string_ptr6);
    string->free(text_string_ptr7);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_pointer_free, .fixture = test_fixture) {
    pointer->init(8);
    string_pointer->free(0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_pointer_lessthan_free, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("b");
    u64 quantum_str_ptr1 = string->load("abba");
    u64 quantum_str_ptr2 = string->offset(quantum_str_ptr1, pattern_ptr);
    u64 data_ptr = string->lessthan(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(data_ptr != 0);
    string_pointer->free(data_ptr);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_0_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 error_ptr = string->lessthan(0, 0);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_0_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 error_ptr = string->lessthan(0, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_1_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr = string->load("");
    u64 error_ptr = string->lessthan(1, 0);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_1_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->lessthan(1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_a_b, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("b");
    u64 quantum_str_ptr1 = string->load("abba");
    u64 quantum_str_ptr2 = string->offset(quantum_str_ptr1, pattern_ptr);
    u64 data_ptr = string->lessthan(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(data_ptr != 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_b_a, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("b");
    u64 quantum_str_ptr1 = string->load("abba");
    u64 quantum_str_ptr2 = string->offset(quantum_str_ptr1, pattern_ptr);
    u64 error_ptr = string->lessthan(quantum_str_ptr2, quantum_str_ptr1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_1_2, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->lessthan(1, 2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_1_3, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->lessthan(1, 2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_2_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->lessthan(2, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_a_string_free, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    string->free(quantum_str_ptr2);
    u64 error_ptr = string->lessthan(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_string_free_b, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    string->free(quantum_str_ptr1);
    u64 error_ptr = string->lessthan(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_string_list, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 list_ptr2 = list->alloc();
    u64 error_ptr = string->lessthan(quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_list_string, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr1 = list->alloc();
    u64 quantum_str_ptr2 = string->load("a");
    u64 error_ptr = string->lessthan(list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_0_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 error_ptr = string->greaterthan(0, 0);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_0_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 error_ptr = string->greaterthan(0, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_1_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr = string->load("");
    u64 error_ptr = string->greaterthan(1, 0);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_1_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->greaterthan(1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_a_b, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("b");
    u64 quantum_str_ptr1 = string->load("abba");
    u64 quantum_str_ptr2 = string->offset(quantum_str_ptr1, pattern_ptr);
    u64 error_ptr = string->greaterthan(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_b_a, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("b");
    u64 quantum_str_ptr1 = string->load("abba");
    u64 quantum_str_ptr2 = string->offset(quantum_str_ptr1, pattern_ptr);
    u64 data_ptr = string->greaterthan(quantum_str_ptr2, quantum_str_ptr1);
    RX_ASSERT(data_ptr != 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_1_2, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->greaterthan(1, 2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_1_3, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->greaterthan(1, 3);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_2_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->greaterthan(2, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_a_string_free, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    string->free(quantum_str_ptr2);
    u64 error_ptr = string->greaterthan(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_string_free_b, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    string->free(quantum_str_ptr1);
    u64 error_ptr = string->greaterthan(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_string_list, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 list_ptr2 = list->alloc();
    u64 error_ptr = string->greaterthan(quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_list_string, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr1 = list->alloc();
    u64 quantum_str_ptr2 = string->load("a");
    u64 error_ptr = string->greaterthan(list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_0_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 error_ptr = string->equals(0, 0);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_0_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 error_ptr = string->equals(0, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_1_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr = string->load("");
    u64 error_ptr = string->equals(1, 0);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_1_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->equals(1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_a_b, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("b");
    u64 quantum_str_ptr1 = string->load("abba");
    u64 quantum_str_ptr2 = string->offset(quantum_str_ptr1, pattern_ptr);
    u64 error_ptr = string->equals(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_b_a, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("b");
    u64 source_ptr = string->load("abba");
    u64 quantum_str_ptr1 = string->offset(source_ptr, pattern_ptr);
    u64 quantum_str_ptr2 = string->offset(source_ptr, pattern_ptr);
    u64 data_ptr = string->equals(quantum_str_ptr2, quantum_str_ptr1);
    RX_ASSERT(data_ptr != 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_1_2, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->equals(1, 2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_1_3, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->equals(1, 3);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_2_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->equals(2, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_a_string_free, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    string->free(quantum_str_ptr2);
    u64 error_ptr = string->equals(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_string_free_b, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    string->free(quantum_str_ptr1);
    u64 error_ptr = string->equals(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_string_list, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 list_ptr2 = list->alloc();
    u64 error_ptr = string->equals(quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_list_string, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr1 = list->alloc();
    u64 quantum_str_ptr2 = string->load("a");
    u64 error_ptr = string->equals(list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_0_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 error_ptr = string->compare(0, 0);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_0_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 error_ptr = string->compare(0, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_1_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr = string->load("");
    u64 error_ptr = string->compare(1, 0);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_1_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->compare(1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_a_b, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("b");
    u64 quantum_str_ptr1 = string->load("abba");
    u64 quantum_str_ptr2 = string->offset(quantum_str_ptr1, pattern_ptr);
    u64 data_ptr = string->compare(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(data_ptr != 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_b_a, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("b");
    u64 source_ptr = string->load("abba");
    u64 quantum_str_ptr1 = string->offset(source_ptr, pattern_ptr);
    u64 quantum_str_ptr2 = string->offset(source_ptr, pattern_ptr);
    u64 data_ptr = string->compare(quantum_str_ptr2, quantum_str_ptr1);
    RX_ASSERT(data_ptr != 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_1_2, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->compare(1, 2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_1_3, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->compare(1, 3);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_2_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->compare(2, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_a_string_free, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    string->free(quantum_str_ptr2);
    u64 error_ptr = string->compare(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_string_free_b, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    string->free(quantum_str_ptr1);
    u64 error_ptr = string->compare(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_string_list, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 list_ptr2 = list->alloc();
    u64 error_ptr = string->compare(quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_list_string, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr1 = list->alloc();
    u64 quantum_str_ptr2 = string->load("a");
    u64 error_ptr = string->compare(list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_0_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 error_ptr = string->strcmp(0, 0);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_0_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 error_ptr = string->strcmp(0, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_1_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr = string->load("");
    u64 error_ptr = string->strcmp(1, 0);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_1_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->strcmp(1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_a_b, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("b");
    u64 quantum_str_ptr1 = string->load("abba");
    u64 quantum_str_ptr2 = string->offset(quantum_str_ptr1, pattern_ptr);
    u64 data_ptr = string->strcmp(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(data_ptr != 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_b_a, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("b");
    u64 source_ptr = string->load("abba");
    u64 quantum_str_ptr1 = string->offset(source_ptr, pattern_ptr);
    u64 quantum_str_ptr2 = string->offset(source_ptr, pattern_ptr);
    u64 data_ptr = string->strcmp(quantum_str_ptr2, quantum_str_ptr1);
    RX_ASSERT(data_ptr != 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_1_2, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->strcmp(1, 2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}
/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_1_3, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->strcmp(1, 3);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_2_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->strcmp(2, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_a_string_free, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    string->free(quantum_str_ptr2);
    u64 error_ptr = string->strcmp(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_string_free_b, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    string->free(quantum_str_ptr1);
    u64 error_ptr = string->strcmp(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_string_list, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 list_ptr2 = list->alloc();
    u64 error_ptr = string->strcmp(quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_list_string, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr1 = list->alloc();
    u64 quantum_str_ptr2 = string->load("a");
    u64 error_ptr = string->strcmp(list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_0_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 error_ptr = string->move_right(0, 0);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_0_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 error_ptr = string->move_right(0, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_1_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr = string->load("");
    u64 error_ptr = string->move_right(1, 0);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_1_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->move_right(1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_a_b, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("abba");
    u64 error_ptr = string->move_right(quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_b_a, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("a");
    u64 source_ptr = string->load("abba");
    u64 quantum_str_ptr1 = string->offset(source_ptr, pattern_ptr);
    u64 error_ptr = string->move_right(quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_a_b_2, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("b");
    u64 source_ptr = string->load("abba");
    u64 quantum_str_ptr1 = string->offset(source_ptr, pattern_ptr);
    u64 data_ptr = string->move_right(quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_a_b_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("a");
    u64 source_ptr = string->load("abba");
    u64 quantum_str_ptr1 = string->offset(source_ptr, pattern_ptr);
    u64 error_ptr = string->move_right(quantum_str_ptr1, 1);
    RX_ASSERT(strcmp(string->unsafe(quantum_str_ptr1), "") == 0);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_a_b_free, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("b");
    u64 source_ptr = string->load("abba");
    u64 quantum_str_ptr1 = string->offset(source_ptr, pattern_ptr);
    string->free(source_ptr);
    u64 error_ptr = string->move_right(quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_1_2, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->move_right(1, 2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_1_3, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->move_right(1, 3);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_2_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->move_right(2, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_a_string_free, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    string->free(quantum_str_ptr2);
    u64 error_ptr = string->move_right(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_string_free_b, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    string->free(quantum_str_ptr1);
    u64 error_ptr = string->move_right(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_string_list, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 list_ptr2 = list->alloc();
    u64 error_ptr = string->move_right(quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_list_string, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr1 = list->alloc();
    u64 quantum_str_ptr2 = string->load("a");
    u64 error_ptr = string->move_right(list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_0_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 error_ptr = string->right(0, 0);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_0_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 error_ptr = string->right(0, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_1_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr = string->load("");
    u64 error_ptr = string->right(1, 0);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_1_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 data_ptr = string->right(quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(strcmp(string->unsafe(data_ptr), "") == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_a_b, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("abba");
    u64 error_ptr = string->right(quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr != 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_b_a, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("a");
    u64 source_ptr = string->load("abba");
    u64 quantum_str_ptr1 = string->offset(source_ptr, pattern_ptr);
    u64 error_ptr = string->right(quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_a_b_2, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("b");
    u64 source_ptr = string->load("abba");
    u64 quantum_str_ptr1 = string->offset(source_ptr, pattern_ptr);
    u64 data_ptr = string->right(quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_a_b_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("a");
    u64 source_ptr = string->load("abba");
    u64 quantum_str_ptr1 = string->offset(source_ptr, pattern_ptr);
    u64 error_ptr = string->right(quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
    RX_ASSERT(strcmp(string->unsafe(quantum_str_ptr1), "") == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_a_b_free, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("b");
    u64 source_ptr = string->load("abba");
    u64 quantum_str_ptr1 = string->offset(source_ptr, pattern_ptr);
    string->free(source_ptr);
    u64 error_ptr = string->right(quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_1_2, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->right(1, 2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_1_3, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->right(1, 3);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_2_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("");
    u64 error_ptr = string->right(quantum_str_ptr2, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_a_string_free, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    string->free(quantum_str_ptr2);
    u64 error_ptr = string->right(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_string_free_b, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    string->free(quantum_str_ptr1);
    u64 error_ptr = string->right(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_string_list, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 list_ptr2 = list->alloc();
    u64 error_ptr = string->right(quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_list_string, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr1 = list->alloc();
    u64 quantum_str_ptr2 = string->load("a");
    u64 error_ptr = string->right(list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_0_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 error_ptr = string->move_left(0, 0);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_0_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 error_ptr = string->move_left(0, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_1_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr = string->load("");
    u64 error_ptr = string->move_left(1, 0);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_1_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->move_left(1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_a_b, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("b");
    u64 source_ptr = string->load("abba");
    u64 quantum_str_ptr1 = string->offset(source_ptr, pattern_ptr);
    u64 error_ptr = string->move_left(quantum_str_ptr1, 4);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_b_a, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("a");
    u64 source_ptr = string->load("abba");
    u64 quantum_str_ptr1 = string->offset(source_ptr, pattern_ptr);
    u64 data_ptr = string->move_left(quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_a_b_2, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("b");
    u64 source_ptr = string->load("abba");
    u64 quantum_str_ptr1 = string->offset(source_ptr, pattern_ptr);
    u64 data_ptr = string->move_left(quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_a_b_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("a");
    u64 source_ptr = string->load("abba");
    u64 quantum_str_ptr1 = string->offset(source_ptr, pattern_ptr);
    u64 data_ptr = string->move_left(quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(strcmp(string->unsafe(quantum_str_ptr1), "a") == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_a_b_free, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("b");
    u64 source_ptr = string->load("abba");
    u64 quantum_str_ptr1 = string->offset(source_ptr, pattern_ptr);
    string->free(source_ptr);
    u64 error_ptr = string->move_left(quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_1_2, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->move_left(1, 2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}
/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_1_3, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->move_left(1, 3);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_2_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->move_left(2, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_a_string_free, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    string->free(quantum_str_ptr2);
    u64 error_ptr = string->move_left(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_string_free_b, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    string->free(quantum_str_ptr1);
    u64 error_ptr = string->move_left(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_string_list, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 list_ptr2 = list->alloc();
    u64 error_ptr = string->move_left(quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_list_string, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr1 = list->alloc();
    u64 quantum_str_ptr2 = string->load("a");
    u64 error_ptr = string->move_left(list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_0_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 error_ptr = string->left_strncpy(0, 0);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_0_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 error_ptr = string->left_strncpy(0, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_1_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr = string->load("");
    u64 error_ptr = string->left_strncpy(1, 0);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_1_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->left_strncpy(1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_a_b, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("b");
    u64 source_ptr = string->load("abba");
    u64 quantum_str_ptr1 = string->offset(source_ptr, pattern_ptr);
    u64 error_ptr = string->left_strncpy(quantum_str_ptr1, 4);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_b_a, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("c");
    u64 source_ptr = string->load("abbc");
    u64 quantum_str_ptr1 = string->offset(source_ptr, pattern_ptr);
    u64 data_ptr = string->left_strncpy(quantum_str_ptr1, 3);
    RX_ASSERT(quantum_str_ptr1 != 0);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(strcmp(string->unsafe(quantum_str_ptr1), "") == 0);
    RX_ASSERT(strcmp(string->unsafe(data_ptr), "bbc") == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_a_b_2, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("b");
    u64 source_ptr = string->load("abba");
    u64 quantum_str_ptr1 = string->offset(source_ptr, pattern_ptr);
    u64 data_ptr = string->left_strncpy(quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_a_b_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("a");
    u64 source_ptr = string->load("abba");
    u64 quantum_str_ptr1 = string->offset(source_ptr, pattern_ptr);
    u64 data_ptr = string->left_strncpy(quantum_str_ptr1, 1);
    RX_ASSERT(quantum_str_ptr1 != 0);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(strcmp(string->unsafe(quantum_str_ptr1), "") == 0);
    RX_ASSERT(strcmp(string->unsafe(data_ptr), "a") == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_a_b_free, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("b");
    u64 source_ptr = string->load("abba");
    u64 quantum_str_ptr1 = string->offset(source_ptr, pattern_ptr);
    string->free(source_ptr);
    u64 error_ptr = string->left_strncpy(quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_1_2, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->left_strncpy(1, 2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}
/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_1_3, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->left_strncpy(1, 3);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_2_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->left_strncpy(2, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_a_string_free, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    string->free(quantum_str_ptr2);
    u64 error_ptr = string->left_strncpy(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_string_free_b, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    string->free(quantum_str_ptr1);
    u64 error_ptr = string->left_strncpy(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_string_list, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 list_ptr2 = list->alloc();
    u64 error_ptr = string->left_strncpy(quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_list_string, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr1 = list->alloc();
    u64 quantum_str_ptr2 = string->load("a");
    u64 error_ptr = string->left_strncpy(list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_0_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 error_ptr = string->left(0, 0);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_0_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 error_ptr = string->left(0, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_1_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr = string->load("");
    u64 error_ptr = string->left(1, 0);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_1_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->left(1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_a_b, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("b");
    u64 source_ptr = string->load("abba");
    u64 quantum_str_ptr1 = string->offset(source_ptr, pattern_ptr);
    u64 error_ptr = string->left(quantum_str_ptr1, 4);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_b_a, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("c");
    u64 source_ptr = string->load("abbc");
    u64 quantum_str_ptr1 = string->offset(source_ptr, pattern_ptr);
    u64 data_ptr = string->left(quantum_str_ptr1, 3);
    RX_ASSERT(quantum_str_ptr1 != 0);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(strcmp(string->unsafe(quantum_str_ptr1), "") == 0);
    RX_ASSERT(strcmp(string->unsafe(data_ptr), "bbc") == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_a_b_2, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("b");
    u64 source_ptr = string->load("abba");
    u64 quantum_str_ptr1 = string->offset(source_ptr, pattern_ptr);
    u64 data_ptr = string->left(quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_a_b_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("a");
    u64 source_ptr = string->load("abba");
    u64 quantum_str_ptr1 = string->offset(source_ptr, pattern_ptr);
    u64 data_ptr = string->left(quantum_str_ptr1, 1);
    RX_ASSERT(quantum_str_ptr1 != 0);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(strcmp(string->unsafe(quantum_str_ptr1), "") == 0);
    RX_ASSERT(strcmp(string->unsafe(data_ptr), "a") == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_a_b_free, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("b");
    u64 source_ptr = string->load("abba");
    u64 quantum_str_ptr1 = string->offset(source_ptr, pattern_ptr);
    string->free(source_ptr);
    u64 error_ptr = string->left(quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_1_2, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->left(1, 2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}
/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_1_3, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->left(1, 3);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_2_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->left(2, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_a_string_free, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    string->free(quantum_str_ptr2);
    u64 error_ptr = string->left(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_string_free_b, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    string->free(quantum_str_ptr1);
    u64 error_ptr = string->left(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_string_list, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 list_ptr2 = list->alloc();
    u64 error_ptr = string->left(quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_list_string, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr1 = list->alloc();
    u64 quantum_str_ptr2 = string->load("a");
    u64 error_ptr = string->left(list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_0_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 error_ptr = string->strncpy(0, 0);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_0_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 error_ptr = string->strncpy(0, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_1_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr = string->load("");
    u64 error_ptr = string->strncpy(1, 0);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_1_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->strncpy(1, 1);
    RX_ASSERT(error_ptr == 3);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_a_b, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("b");
    u64 source_ptr = string->load("abba");
    u64 quantum_str_ptr1 = string->offset(source_ptr, pattern_ptr);
    u64 error_ptr = string->strncpy(quantum_str_ptr1, 2);
    RX_ASSERT(error_ptr == 0);
    u64 data_ptr = string->strncpy(quantum_str_ptr1, 1);
    u64 str_ptr1 = string->offset(source_ptr, data_ptr);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(str_ptr1 != 0);
    RX_ASSERT(strcmp(string->unsafe(str_ptr1), "") == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_b_a, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("b");
    u64 source_ptr = string->load("abcd");
    u64 quantum_str_ptr1 = string->offset(source_ptr, pattern_ptr);
    u64 data_ptr = string->strncpy(quantum_str_ptr1, 2);
    RX_ASSERT(data_ptr != 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_a_b_2, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("b");
    u64 source_ptr = string->load("abba");
    u64 quantum_str_ptr1 = string->offset(source_ptr, pattern_ptr);
    u64 data_ptr = string->strncpy(quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_a_b_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("a");
    u64 source_ptr = string->load("abba");
    u64 quantum_str_ptr1 = string->offset(source_ptr, pattern_ptr);
    u64 data_ptr = string->strncpy(quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr == 0);
    RX_ASSERT(quantum_str_ptr1 != 0);
    RX_ASSERT(strcmp(string->unsafe(quantum_str_ptr1), "") == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_a_b_free, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("b");
    u64 source_ptr = string->load("abba");
    u64 quantum_str_ptr1 = string->offset(source_ptr, pattern_ptr);
    string->free(source_ptr);
    u64 error_ptr = string->strncpy(quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_a_b_2_overflow, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("h");
    u64 source_ptr = string->load("abcdefgh");
    u64 quantum_str_ptr1 = string->offset(source_ptr, pattern_ptr);
    u64 error_ptr = string->strncpy(quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_1_2, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->strncpy(1, 2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_1_3, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 error_ptr = string->strncpy(1, 3);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_2_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    u64 data_ptr = string->strncpy(2, 1);
    RX_ASSERT(data_ptr != 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_a_string_free, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    string->free(quantum_str_ptr2);
    u64 error_ptr = string->strncpy(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_string_free_b, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 quantum_str_ptr2 = string->load("b");
    string->free(quantum_str_ptr1);
    u64 error_ptr = string->strncpy(quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_string_list, .fixture = test_fixture) {
    pointer->init(8);
    u64 quantum_str_ptr1 = string->load("a");
    u64 list_ptr2 = list->alloc();
    u64 error_ptr = string->strncpy(quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_list_string, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr1 = list->alloc();
    u64 quantum_str_ptr2 = string->load("a");
    u64 error_ptr = string->strncpy(list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    pointer->gc();
    pointer->destroy();
}

static void parse_text(u64 text_string_ptr) {
    u64 gc_ptr = list->alloc();
    u64 text_size = string->size(text_string_ptr);
    if (text_string_ptr == 0) {
        return;
    }
    u64 list_ptr = list->alloc();
    char* text = string->unsafe(text_string_ptr);
    char* tmp = text;
    while (*tmp != 0 && text_size > 0) {
        while (*tmp != 0 && *tmp != '\n' && text_size > 0) {
            tmp++;
            text_size--;
        }
        if (text_size == 0) {
            list->free(list_ptr);
            return;
        }
        *tmp++ = '\0';
        text_size--;
        u64 string_ptr = string->load(text);
        list->push(list_ptr, string_ptr);
        text = tmp;
    }
    u64 data_ptr = 0;
    u64 list_data_ptr = list->alloc();
    while ((data_ptr = list->pop(list_ptr)) != 0) {
        list->push(list_data_ptr, data_ptr);
    }
    list->free(list_ptr);
    list->push(gc_ptr, list_data_ptr);
    u64 quit = 0;
    while (quit == 0) {
        u64 string_ptr = list->pop(list_data_ptr);
        if (string->size(string_ptr) == 0) {
            quit = 1;
            continue;
        }
        os->putc(string_ptr);
        u64 pattern_ptr = list->pop(list_data_ptr);
        if (string->size(pattern_ptr) == 0) {
            quit = 1;
            continue;
        }
        os->putc(pattern_ptr);
        u64 size = string->size(pattern_ptr);
        u64 string_pointer_ptr = 0;
        u64 current_ptr = string_ptr;
        while ((string_pointer_ptr = string->strchr(current_ptr, pattern_ptr)) != 0) {
            u64 match_ptr = string->match(string_pointer_ptr, pattern_ptr);
            if (match_ptr == 0) {
                break;
            }
            if (string->lessthan(string_pointer_ptr, match_ptr)) {
                u64 match_start_ptr = string->left(match_ptr, size);
                u64 str_ncpy = string->strncpy(match_start_ptr, size);
                u64 distance = string->lessthan(string_ptr, match_start_ptr);
                if (distance > 0) {
                    u64 i = 0;
                    while (i++ < distance) {
                        printf(" ");
                    }
                }
                printf("%s[%lld]\n", string->unsafe(str_ncpy), distance);
                string_pointer->free(match_start_ptr);
                string_pointer->free(str_ncpy);
            }
            string_pointer->free(string_pointer_ptr);
            current_ptr = match_ptr;
        }
    }
    list->release(gc_ptr);
    list->free(gc_ptr);
}

static int run(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- rexo unit test code\n");
#endif
    /* Execute the main function that runs the test cases found. */
    int result = rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;
    return result;
}

const struct test_suite pointer_test_suite_definition = {
    .run = run
};
