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
#include "common/alloc.h"
#include "list-micro/data.h"
#include "list/v1/list_v1.h"
#include "playground/brain/brain.h"
#include "playground/hashtable/v1/hashtable_v1.h"
#include "pointer/types/data/v1/data_v1.h"
#include "pointer/types/file/v1/file_v1.h"
#include "pointer/types/list/v1/list_v1.h"
#include "pointer/types/object/v1/object_v1.h"
#include "pointer/types/os/v1/os_v1.h"
#include "pointer/types/string/v1/string_v1.h"
#include "pointer/types/string_pointer/v1/string_pointer_v1.h"
#include "pointer/types/user/v1/user_v1.h"
#include "pointer/v1/pointer_v1.h"

#include "../../tests/src/test.h"

#define DEFAULT_SIZE 0x100

/* definition */
extern const struct test_suite list_alloc_test_suite_definition;
extern const struct test_suite list_micro_test_suite_definition;
extern const struct test_suite list_test_suite_definition;
extern const struct test_suite vm_v1_test_suite_definition;
extern const struct test_suite pointer_test_suite_definition;

/* definition */
static const struct test_suite* list_alloc_tests = &list_alloc_test_suite_definition;
static const struct test_suite* list_micro_tests = &list_micro_test_suite_definition;
static const struct test_suite* list_tests = &list_test_suite_definition;
static const struct test_suite* vm_v1_tests = &vm_v1_test_suite_definition;
static const struct test_suite* pointer_tests = &pointer_test_suite_definition;

/* definition */
extern const struct vm_methods vm_methods_definition;

/* definition */
extern const struct pointer_methods pointer_methods_definition;
extern const struct list_methods list_methods_definition;
extern const struct file_methods file_methods_definition;
extern const struct string_methods string_methods_definition;
extern const struct user_methods user_methods_definition;
extern const struct data_methods data_methods_definition;
extern const struct object_methods object_methods_definition;
extern const struct os_methods os_methods_definition;
extern const struct string_pointer_methods string_pointer_methods_definition;

/* definition */
const struct pointer_methods* pointer = &pointer_methods_definition;
const struct list_methods* list = &list_methods_definition;
const struct file_methods* file = &file_methods_definition;
const struct string_methods* string = &string_methods_definition;
const struct user_methods* user = &user_methods_definition;
const struct data_methods* data = &data_methods_definition;
const struct object_methods* object = &object_methods_definition;
const struct os_methods* os = &os_methods_definition;
const struct string_pointer_methods* string_pointer = &string_pointer_methods_definition;

typedef struct test_data {
}* TEST_DATA;

RX_SET_UP(test_set_up) {
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

/* test init */
RX_TEST_CASE(main_brain1_tests, test_print_0, .fixture = test_fixture) {
    pointer->init(8);
    os->putc(0);
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_load_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 empty_ptr = string->load("\0");
    RX_ASSERT(empty_ptr == 0);
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_load_null, .fixture = test_fixture) {
    pointer->init(8);
    u64 empty_ptr = string->load(0);
    RX_ASSERT(empty_ptr == 0);
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_load_empty, .fixture = test_fixture) {
    pointer->init(8);
    u64 empty_ptr = string->load("");
    RX_ASSERT(empty_ptr == 0);
#ifndef USE_GC
    string->free(empty_ptr);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_print_load_empty, .fixture = test_fixture) {
    pointer->init(8);
    u64 empty_ptr = string->load("");
    os->putc(empty_ptr);
    RX_ASSERT(empty_ptr == 0);
#ifndef USE_GC
    string->free(empty_ptr);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_load_copy, .fixture = test_fixture) {
    pointer->init(8);
    u64 char_ptr = string->load("/");
    u64 copy_ptr = string->copy(char_ptr);
    RX_ASSERT(strcmp(string->unsafe(char_ptr), "/") == 0);
    RX_ASSERT(strcmp(string->unsafe(copy_ptr), "/") == 0);
    RX_ASSERT(char_ptr != copy_ptr);
#ifndef USE_GC
    string->free(char_ptr);
    string->free(copy_ptr);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_load_push_peek_pop, .fixture = test_fixture) {
    pointer->init(8);
    u64 char_ptr = string->load("/");
    RX_ASSERT(strcmp(string->unsafe(char_ptr), "/") == 0);
#ifndef USE_GC
    string->free(char_ptr);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

RX_TEST_CASE(main_brain1_tests, test_load_free_free, .fixture = test_fixture) {
    pointer->init(8);
    u64 char_ptr = string->load("/");
    RX_ASSERT(strcmp(string->unsafe(char_ptr), "/") == 0);
#ifndef USE_GC
    string->free(char_ptr);
    string->free(char_ptr);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_strcat_load_alloc, .fixture = test_fixture) {
    pointer->init(8);
    u64 char_ptr = string->load("/");
    u64 pattern_ptr = string->load("\0");
    string->strcat(pattern_ptr, char_ptr);
    RX_ASSERT(pattern_ptr == 0);
    RX_ASSERT(strcmp(string->unsafe(char_ptr), "/") == 0);
#ifndef USE_GC
    string->free(char_ptr);
    string->free(pattern_ptr);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_string_size, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_pointer_size_0, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_pointer_size_empty, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_pointer_size_object, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_object_0, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_object_alloc_0, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_object_load_0, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_object_load_string_0, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_object_load_string, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_object_load_string_unsafe_alloc_0, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_object_load_string_unsafe_string, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_object_load_string_unsafe_0, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_object_load_string_unsafe, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_strcat_alloc_alloc, .fixture = test_fixture) {
    pointer->init(8);
    u64 char_ptr = string->load("\0");
    u64 pattern_ptr = string->load("\0");
    string->strcat(pattern_ptr, char_ptr);
    RX_ASSERT(char_ptr == 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    string->free(char_ptr);
    string->free(pattern_ptr);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_string_offset_ptr_diff, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr1 = string->load("192.168.0.111");
    u64 string_ptr2 = string->load("192.168.0.12");
    u64 last_matched_ptr1 = string->offset(string_ptr1, string_ptr2);
    RX_ASSERT(last_matched_ptr1 == 0);
#ifndef USE_GC
    string->free(last_matched_ptr1);
    string->free(string_ptr1);
    string->free(string_ptr2);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_string_offset_ptr_less, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr1 = string->load("192.168.0.11");
    u64 string_ptr2 = string->load("8.0.112");
    u64 last_matched_ptr1 = string->offset(string_ptr1, string_ptr2);
    RX_ASSERT(last_matched_ptr1 == 0);
#ifndef USE_GC
    string->free(last_matched_ptr1);
    string->free(string_ptr1);
    string->free(string_ptr2);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_string_offset_ptr_offset, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr1 = string->load("192.168.0.11");
    u64 string_ptr2 = string->load("8.0.12");
    u64 last_matched_ptr1 = string->offset(string_ptr1, string_ptr2);
    RX_ASSERT(last_matched_ptr1 == 0);
#ifndef USE_GC
    string->free(last_matched_ptr1);
    string->free(string_ptr1);
    string->free(string_ptr2);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_string_offset_ptr_offset_found, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr1 = string->load("192.168.0.111");
    u64 string_ptr2 = string->load("8.0.");
    u64 last_matched_ptr1 = string->offset(string_ptr1, string_ptr2);
    RX_ASSERT(strcmp(string->unsafe(last_matched_ptr1), "111") == 0);
#ifndef USE_GC
    string->free(last_matched_ptr1);
    string->free(string_ptr1);
    string->free(string_ptr2);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_string_pointer_free_list, .fixture = test_fixture) {
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
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_string_pointer_free, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_string_offset_list, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    u64 string_ptr = string->load("hello");
    u64 last_matched_ptr = string->offset(list_ptr, string_ptr);
    RX_ASSERT(last_matched_ptr == 0);
#ifndef USE_GC
    string->free(string_ptr);
    list->free(list_ptr);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_string_pointer_match, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_string_pointer_match_miss, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_string_pointer_match_pattern, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_string_pointer_match_patter_0, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_string_pointer_match_patter_none, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_string_pointer_match_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr = string->load("192.168.0.1");
    u64 string_pointer_ptr = string->match(string_ptr, 0);
    RX_ASSERT(string_pointer_ptr == 0);
#ifndef USE_GC
    string->free(string_ptr);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_string_pointer_match_0_0, .fixture = test_fixture) {
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
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_string_pointer_match_0_string, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr = string->load("192.168.0.1");
    u64 string_pointer_ptr = string->match(0, string_ptr);
    RX_ASSERT(string_pointer_ptr == 0);
#ifndef USE_GC
    string->free(string_ptr);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_string_match_list, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    u64 string_ptr = string->load("hello");
    u64 last_matched_ptr = string->match(list_ptr, string_ptr);
    RX_ASSERT(last_matched_ptr == 0);
#ifndef USE_GC
    string->free(string_ptr);
    list->free(list_ptr);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_string_pointer_strchr, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_string_pointer_strchr_miss, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_string_pointer_strchr_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr = string->load("192.168.0.1");
    u64 string_pointer_ptr = string->strchr(string_ptr, 0);
    RX_ASSERT(string_pointer_ptr == 0);
#ifndef USE_GC
    string->free(string_ptr);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_string_pointer_strchr_0_0, .fixture = test_fixture) {
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
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_string_pointer_strchr_0_string, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr = string->load("192.168.0.1");
    u64 string_pointer_ptr = string->strchr(0, string_ptr);
    RX_ASSERT(string_pointer_ptr == 0);
#ifndef USE_GC
    string->free(string_ptr);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_string_strchr_list, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    u64 string_ptr = string->load("hello");
    u64 last_matched_ptr = string->strchr(list_ptr, string_ptr);
    RX_ASSERT(last_matched_ptr == 0);
#ifndef USE_GC
    string->free(string_ptr);
    list->free(list_ptr);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_string_pointer_strrchr, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_string_pointer_strrchr_match_offset, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_string_pointer_strchr_match_offset, .fixture = test_fixture) {
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
#else
    /* memory leak: list_ptr is not freed, use pointer->release(); */
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_string_pointer_strrchr_miss, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_string_pointer_strrchr_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr = string->load("192.168.0.1");
    u64 string_pointer_ptr = string->strrchr(string_ptr, 0);
    RX_ASSERT(string_pointer_ptr == 0);
#ifndef USE_GC
    string->free(string_ptr);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_string_pointer_strrchr_0_0, .fixture = test_fixture) {
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
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_string_pointer_strrchr_0_string, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr = string->load("192.168.0.1");
    u64 string_pointer_ptr = string->strrchr(0, string_ptr);
    RX_ASSERT(string_pointer_ptr == 0);
#ifndef USE_GC
    string->free(string_ptr);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_string_strrchr_list, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    u64 string_ptr = string->load("hello");
    u64 last_matched_ptr = string->strrchr(list_ptr, string_ptr);
    RX_ASSERT(last_matched_ptr == 0);
#ifndef USE_GC
    string->free(string_ptr);
    list->free(list_ptr);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_list_push_0_1, .fixture = test_fixture) {
    pointer->init(8);
    list->push(0, 1);
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_list_push_1_0, .fixture = test_fixture) {
    pointer->init(8);
    list->push(1, 0);
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_list_push_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    list->push(list_ptr, 0);
#ifndef USE_GC
    list->free(list_ptr);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_list_push_list, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    list->push(list_ptr, list_ptr);
#ifndef USE_GC
    list->free(list_ptr);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_list_push_string, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr = string->load("@");
    u64 list_ptr = list->alloc();
    list->push(string_ptr, list_ptr);
#ifndef USE_GC
    list->free(list_ptr);
    string->free(string_ptr);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_list_size_string, .fixture = test_fixture) {
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
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_list_push_size, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr = string->load("@");
    u64 list_ptr = list->alloc();
    list->push(list_ptr, string_ptr);
    u64 value = list->size(list_ptr);
    RX_ASSERT(value != 0);
#ifndef USE_GC
    list->free(list_ptr);
    string->free(string_ptr);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_list_pop_0_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 value_ptr = list->pop(0);
    RX_ASSERT(value_ptr == 0);
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_list_pop_1_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 value_ptr = list->pop(1);
    RX_ASSERT(value_ptr == 0);
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_list_pop_list, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    u64 value_ptr = list->pop(list_ptr);
    RX_ASSERT(value_ptr == 0);
#ifndef USE_GC
    list->free(list_ptr);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_list_pop_string, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr = string->load("@");
    u64 list_ptr = list->alloc();
    list->push(list_ptr, string_ptr);
    u64 value_ptr = list->pop(list_ptr);
    RX_ASSERT(value_ptr != 0);
#ifndef USE_GC
    list->free(list_ptr);
    string->free(string_ptr);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_list_peek_0_1, .fixture = test_fixture) {
    pointer->init(8);
    u64 value_ptr = list->peek(0);
    RX_ASSERT(value_ptr == 0);
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_list_peek_1_0, .fixture = test_fixture) {
    pointer->init(8);
    u64 value_ptr = list->peek(1);
    RX_ASSERT(value_ptr == 0);
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_list_peek_list, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    u64 value_ptr = list->peek(list_ptr);
    RX_ASSERT(value_ptr == 0);
#ifndef USE_GC
    list->free(list_ptr);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_list_peek_string, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr = string->load("@");
    u64 list_ptr = list->alloc();
    list->push(list_ptr, string_ptr);
    u64 value_ptr = list->peek(list_ptr);
    RX_ASSERT(value_ptr != 0);
#ifndef USE_GC
    list->free(list_ptr);
    string->free(string_ptr);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_strcpy, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_strcat_load_alloc_copy, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_strcat_alloc_load, .fixture = test_fixture) {
    pointer->init(8);
    u64 zero_ptr = string->load("\0");
    u64 char_ptr = string->load("/");
    string->strcat(zero_ptr, char_ptr);
    RX_ASSERT(zero_ptr == 0);
    RX_ASSERT(strcmp(string->unsafe(char_ptr), "/") == 0);
#ifndef USE_GC
    string->free(zero_ptr);
    string->free(char_ptr);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_strcat_load_alloc_alloc, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_strcat_load_load, .fixture = test_fixture) {
    pointer->init(8);
    u64 char_ptr = string->load("/");
    u64 pattern_ptr = string->load("*");
    string->strcat(pattern_ptr, char_ptr);
    RX_ASSERT(strcmp(string->unsafe(pattern_ptr), "*/") == 0);
    RX_ASSERT(strcmp(string->unsafe(char_ptr), "/") == 0);
#ifndef USE_GC
    string->free(pattern_ptr);
    string->free(char_ptr);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_strcpy_load_alloc, .fixture = test_fixture) {
    pointer->init(8);
    u64 pattern_ptr = string->load("/");
    u64 empty_ptr = string->load("\0");
    string->strcpy(empty_ptr, pattern_ptr);
    RX_ASSERT(empty_ptr == 0);
    RX_ASSERT(strcmp(string->unsafe(pattern_ptr), "/") == 0);
#ifndef USE_GC
    string->free(empty_ptr);
    string->free(pattern_ptr);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_strcpy_load_load, .fixture = test_fixture) {
    pointer->init(8);
    u64 char_ptr = string->load("/input.txt");
    u64 pattern_ptr = string->load("*");
    string->strcpy(pattern_ptr, char_ptr);
    RX_ASSERT(strcmp(string->unsafe(pattern_ptr), "/input.txt") == 0);
    RX_ASSERT(strcmp(string->unsafe(char_ptr), "/input.txt") == 0);
#ifndef USE_GC
    string->free(pattern_ptr);
    string->free(char_ptr);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_load_open_file_close_file, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_offset_strcpy, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_offset_strcpy_free, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_print_free, .fixture = test_fixture) {
    pointer->init(8);
    u64 printing_ptr = string->load("hello, world!");
    string->free(printing_ptr);
    os->putc(printing_ptr);
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_print, .fixture = test_fixture) {
    pointer->init(8);
    u64 printing_ptr = string->load("hello, world!");
    os->putc(printing_ptr);
#ifndef USE_GC
    string->free(printing_ptr);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_print_string_pointer, .fixture = test_fixture) {
    pointer->init(8);
    u64 printing_ptr = string->load("hello, world!");
    u64 comma_ptr = string->load(",");
    u64 substring_index_ptr = string->offset(printing_ptr, comma_ptr);
    os->putc(substring_index_ptr);
#ifndef USE_GC
    string->free(printing_ptr);
    string->free(substring_index_ptr);
    string->free(comma_ptr);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_print_string_pointer_copy, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_list_size, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    u64 size_actual = string->size(list_ptr);
    u64 size_expected = 0;
    RX_ASSERT(size_expected == size_actual);
#ifndef USE_GC
    list->free(list_ptr);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_list_unsafe, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    char* ptr_actual = string->unsafe(list_ptr);
    char* ptr_expected = 0;
    RX_ASSERT(ptr_expected == ptr_actual);
#ifndef USE_GC
    list->free(list_ptr);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_list_offset, .fixture = test_fixture) {
    pointer->init(8);
    u64 string_ptr = string->load("himem.sys");
    u64 list_ptr = list->alloc();
    u64 size_actual = string->offset(string_ptr, list_ptr);
    u64 size_expected = 0;
    RX_ASSERT(size_expected == size_actual);
#ifndef USE_GC
    list->free(list_ptr);
    string->free(string_ptr);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, teststring_pointer_unsafe, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_string_pointer_size, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_string_offset_subsearch, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_print_string_pointer_free, .fixture = test_fixture) {
    pointer->init(8);
    u64 printing_ptr = string->load("hello, world!");
    u64 comma_ptr = string->load(",");
    u64 substring_index_ptr = string->offset(printing_ptr, comma_ptr);
    string->free(substring_index_ptr);
    os->putc(substring_index_ptr);
#ifndef USE_GC
    string->free(printing_ptr);
    string->free(comma_ptr);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_offset_strcat, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_file_read_invalid_type, .fixture = test_fixture) {
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
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_load_open_match_last_unsafe_free, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_load_open_match_last_unsafe_free_unsuppported_calls, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_user, .fixture = test_fixture) {
    pointer->init(8);
#ifndef USE_GC
    u64 ptr1 = user->alloc();
    user->free(ptr1);
#else
    user->alloc();
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(main_brain1_tests, test_load_open_file_unsafe_hashtable, .fixture = test_fixture) {
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
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

RX_TEST_CASE(main_brain1_tests, test_load_load_match_last, .fixture = test_fixture) {
    pointer->init(8);
    u64 str_ptr = string->load("Hello, world!");
    u64 ch_ptr = string->load("z");
    u64 last_match_ptr = string->offset(str_ptr, ch_ptr);
    RX_ASSERT(last_match_ptr == 0);
#ifndef USE_GC
    string->free(last_match_ptr);
    string->free(str_ptr);
    string->free(ch_ptr);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
}

u64 _list_ptr;

extern inline void source1(void) {
    u64 file_path_ptr = list->pop(_list_ptr);
    u64 file_name_ptr = string->load("input.txt");
    u64 pattern_ptr = string->load("/");
    u64 path_ptr = string->load("/");
    u64 last_match_ptr = string->offset(file_path_ptr, pattern_ptr);
    string->free(pattern_ptr);
    string->put_char(last_match_ptr, '\0');
    string->strcpy(path_ptr, file_path_ptr);
    string->free(last_match_ptr);
    string->strcat(file_path_ptr, file_name_ptr);
    string->free(file_name_ptr);

    char* path = string->unsafe(path_ptr);
    u64 path_len = strlen(path);
    u64 input_len = strlen("input.txt");
    RX_ASSERT(path_len > 0);
    RX_ASSERT(input_len > 0);
    char* buf = calloc(1, path_len + input_len + 1);
    strcpy(buf, path); /* NOLINT */
    strcat(buf, "input.txt"); /* NOLINT */
    char* file_path = string->unsafe(file_path_ptr);
    RX_ASSERT(strlen(file_path) == strlen(buf));
    RX_ASSERT(strcmp(file_path, buf) == 0);
    free(buf);

    string->free(path_ptr);
    u64 mode_ptr = string->load("rb");
    u64 f_ptr = file->alloc(file_path_ptr, mode_ptr);
    string->free(file_path_ptr);
    string->free(mode_ptr);
    u64 data_ptr = file->data(f_ptr);
    file->free(f_ptr);
    os->putc(data_ptr);
    data->free(data_ptr);
}

extern inline void source2(void) {
    u64 file_path_ptr = os->getcwd();
    u64 file_name_ptr = string->load("/all_english_words.txt");
    string->strcat(file_path_ptr, file_name_ptr);
    string->free(file_name_ptr);
    u64 mode_ptr = string->load("rb");
    u64 f_ptr = file->alloc(file_path_ptr, mode_ptr);
    if (f_ptr != 0) {
        u64 data_ptr = file->data(f_ptr);
        u64 size = data->size(data_ptr);
        if (size > 100) {
            size = 100;
        }
        u64 list_ptr = list->alloc();
        file->free(f_ptr);
        u8* file_data = data->unsafe(data_ptr);
        for (u64 i = 0; i < size; i++) {
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
}

int main(int argc, char** argv) {
#ifdef USE_MEMORY_DEBUG_INFO
    global_statistics();
#endif
    CLEAN(argc)
    TEST_RUN(alloc, list_alloc_tests);
    TEST_RUN(micro, list_micro_tests);
    TEST_RUN(tests, list_tests);
    TEST_RUN(vm_v1, vm_v1_tests);
    TEST_RUN(pointer_v1, pointer_tests);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- acceptance test code\n");
#endif
    pointer->init(DEFAULT_SIZE);
    _list_ptr = list->alloc();
    u64 argv_ptr = string->load(argv[0]);
    list->push(_list_ptr, argv_ptr);
    source1();
    source2();
    list->free(_list_ptr);
#ifndef USE_GC
    string->free(argv_ptr);
#else
    pointer->gc();
#endif
    pointer->release();
    pointer->destroy();
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- rexo unit test code\n");
#endif

    /* Execute the main function that runs the test cases found. */
    int result = rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;
#ifdef USE_MEMORY_DEBUG_INFO
    global_statistics();
#endif
    return alloc | tests | micro | vm_v1 | pointer_v1 | result;
}
