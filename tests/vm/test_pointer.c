/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 7, 2025 at 6:12:35 PM GMT+3
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

#include "system/error/error_v1.h"
#include "system/hashtable/hashtable_v1.h"
#include "system/memory/memory_v1.h"
#include "system/os/os_v1.h"

#include "virtual/env/env_v1.h"
#include "virtual/pointer/pointer_v1.h"
#include "virtual/types/data/data_v1.h"
#include "virtual/types/file/file_v1.h"
#include "virtual/types/object/object_v1.h"
#include "virtual/types/stack/stack_v1.h"
#include "virtual/types/string/string_v1.h"
#include "virtual/types/string_pointer/string_pointer_v1.h"
#include "virtual/types/user/user_v1.h"
#include "virtual/virtual/virtual_v1.h"
#include "virtual/vm/vm_v1.h"

#include "internal/pointer_type_v1.h"

#define USING_TESTS
#include "test.h"

/* definition */
CSYS_EXPORT extern const system_memory_methods* PRIVATE_API(memory);
CSYS_EXPORT extern const system_os_methods* PRIVATE_API(os);
CSYS_EXPORT extern const system_hashtable_methods* PRIVATE_API(hashtable);

/* definition */
CVM_EXPORT extern const virtual_vm_methods* PRIVATE_API(vm);
CVM_EXPORT extern const virtual_type_methods* PRIVATE_API(type);
CVM_EXPORT extern const virtual_methods* PRIVATE_API(virtual);
CVM_EXPORT extern const virtual_system_methods* PRIVATE_API(system);
CVM_EXPORT extern const virtual_pointer_methods* PRIVATE_API(pointer);
CVM_EXPORT extern const virtual_env_methods* PRIVATE_API(env);
CVM_EXPORT extern const virtual_data_methods* PRIVATE_API(data);
CVM_EXPORT extern const virtual_file_methods* PRIVATE_API(file);
CVM_EXPORT extern const virtual_object_methods* PRIVATE_API(object);
CVM_EXPORT extern const virtual_stack_methods* PRIVATE_API(stack);
CVM_EXPORT extern const virtual_string_methods* PRIVATE_API(string);
CVM_EXPORT extern const virtual_string_pointer_methods* PRIVATE_API(string_pointer);
CVM_EXPORT extern const virtual_user_methods* PRIVATE_API(user);

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    const_vm_ptr ctx;
}* TEST_DATA;

/*api*/
static const virtual_methods* virtual_methods_ptr;

/* mocks */
static const_pointer_ptr mock_virtual_read_zero(const_vm_ptr cvm, u64 address);

static const virtual_env_methods* temp_api;

/* implementation */
static const_pointer_ptr mock_virtual_read_zero(const_vm_ptr cvm, u64 address) {
    return NULL_PTR;
}

RX_SET_UP(test_set_up) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = CALL(vm)->init(8);
    rx->ctx = cvm;
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    CALL(vm)->gc(cvm);
    CALL(vm)->destroy(cvm);
}

RX_SET_UP(test_set_up_pointer_init) {
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down_pointer_destroy) {
    /* nothing to cleanup */
}

/* internal */
static void parse_text(const_vm_ptr cvm, u64 text_string_ptr);

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

/* Define the fixture. */
RX_FIXTURE(test_fixture_pointer, TEST_DATA, .set_up = test_set_up_pointer_init, .tear_down = test_tear_down_pointer_destroy);

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_print_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    CALL(env)->puts(cvm, 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_load_0, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    u64 empty_ptr = CALL(string)->load(cvm, "\0");
    RX_ASSERT(empty_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_load_null_ptr, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 null_ptr = 0;
    CALL(vm)->release(cvm, null_ptr);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_load_null, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 empty_ptr = CALL(string)->load(cvm, 0);
    RX_ASSERT(empty_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_load_empty, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 empty_ptr = CALL(string)->load(cvm, "");
    RX_ASSERT(empty_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, empty_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_load_gc, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 string_ptr = CALL(string)->load(cvm, "hello, world!");
    RX_ASSERT(string_ptr != 0);
#ifndef USE_GC
    CALL(string)->free(cvm, string_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_print_load_empty, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 empty_ptr = CALL(string)->load(cvm, "");
    CALL(env)->puts(cvm, empty_ptr);
    RX_ASSERT(empty_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, empty_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_load_copy, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 char_ptr = CALL(string)->load(cvm, "/");
    u64 copy_ptr = CALL(string)->copy(cvm, char_ptr);
    RX_ASSERT(char_ptr != 0);
    RX_ASSERT(copy_ptr != 0);
    RX_ASSERT(char_ptr != copy_ptr);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, char_ptr), "/") == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, copy_ptr), "/") == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, char_ptr);
    CALL(string)->free(cvm, copy_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_load_copy_free_gc_destroy_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 char_ptr = CALL(string)->load(cvm, "/");
    RX_ASSERT(char_ptr != 0);
#ifndef USE_GC
    CALL(string)->free(cvm, char_ptr);
#endif
    CALL(string)->free(cvm, char_ptr);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_load_copy_free_gc_free_destroy, .fixture = test_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    u64 char_ptr = CALL(string)->load(cvm, "/");
    RX_ASSERT(char_ptr != 0);
#ifndef USE_GC
    CALL(string)->free(cvm, char_ptr);
#endif
    CALL(vm)->gc(cvm);
    CALL(string)->free(cvm, char_ptr);
    CALL(vm)->destroy(cvm);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_load_copy_free_free_gc_destroy, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 char_ptr = CALL(string)->load(cvm, "/");
    RX_ASSERT(char_ptr != 0);
#ifndef USE_GC
    CALL(string)->free(cvm, char_ptr);
    CALL(string)->free(cvm, char_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_load_copy_free_gc_destroy_destroy, .fixture = test_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    u64 char_ptr = CALL(string)->load(cvm, "/");
    RX_ASSERT(char_ptr != 0);
#ifndef USE_GC
    CALL(string)->free(cvm, char_ptr);
#endif
    CALL(vm)->gc(cvm);
    CALL(vm)->destroy(cvm);
    CALL(vm)->destroy(cvm);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_init_init_load_free_gc_destroy, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 char_ptr = CALL(string)->load(cvm, "/");
    RX_ASSERT(char_ptr != 0);
#ifndef USE_GC
    CALL(string)->free(cvm, char_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_init_load_free_gc_gc_destroy, .fixture = test_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    u64 char_ptr = CALL(string)->load(cvm, "/");
    RX_ASSERT(char_ptr != 0);
#ifndef USE_GC
    CALL(string)->free(cvm, char_ptr);
#endif
    CALL(vm)->gc(cvm);
    CALL(vm)->gc(cvm);
    CALL(vm)->destroy(cvm);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_init_load_free_destroy_gc, .fixture = test_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    u64 char_ptr = CALL(string)->load(cvm, "/");
    RX_ASSERT(char_ptr != 0);
    CALL(string)->free(cvm, char_ptr);
    CALL(vm)->destroy(cvm);
    CALL(vm)->gc(cvm);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_load_copy_virtual_read_ptr_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    virtual_methods mock_virtual_methods_definitions;
    /*api */
    memcpy(&mock_virtual_methods_definitions, PRIVATE_API(virtual), sizeof(virtual_methods)); /* NOLINT: sizeof(virtual_methods*) */
    /* setup mocks */
    mock_virtual_methods_definitions.read = mock_virtual_read_zero;
    /* setup api endpoint */
    virtual_methods* mock_virtual_methods = &mock_virtual_methods_definitions;
    /* backup api calls */
    memcpy(&virtual_methods_ptr, &PRIVATE_API(virtual), sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    /* init */
    u64 char_ptr = CALL(string)->load(cvm, "/");
    /* prepare to mock api calls */
    memcpy(&PRIVATE_API(virtual), &mock_virtual_methods, sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    /* virtual_string->free fails in virtual->read call */
    u64 copy_ptr = CALL(string)->copy(cvm, char_ptr);
    /* restore api calls */
    memcpy(&PRIVATE_API(virtual), &virtual_methods_ptr, sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    RX_ASSERT(char_ptr != 0);
    RX_ASSERT(copy_ptr == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, char_ptr), "/") == 0);
    CALL(string)->free(cvm, char_ptr);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_load_copy_pointer_virtual_read_ptr_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    virtual_methods mock_virtual_methods_definitions;
    /*api */
    memcpy(&mock_virtual_methods_definitions, PRIVATE_API(virtual), sizeof(virtual_methods)); /* NOLINT: sizeof(virtual_methods*) */
    /* setup mocks */
    mock_virtual_methods_definitions.read = mock_virtual_read_zero;
    /* setup api endpoint */
    virtual_methods* mock_virtual_methods = &mock_virtual_methods_definitions;
    /* backup api calls */
    memcpy(&virtual_methods_ptr, &PRIVATE_API(virtual), sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    /* prepare to mock api calls */
    memcpy(&PRIVATE_API(virtual), &mock_virtual_methods, sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    u64 address = CALL(virtual)->alloc(cvm, 2, TYPE_STRING_POINTER);
    u64 move_ptr = CALL(string)->move_left(cvm, address, 1);
    RX_ASSERT(move_ptr == 0);
    /* restore api calls */
    memcpy(&PRIVATE_API(virtual), &virtual_methods_ptr, sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    CALL(pointer)->free(cvm, address);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_load_copy_virtual_read_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    virtual_methods mock_virtual_methods_definitions;
    /*api */
    memcpy(&mock_virtual_methods_definitions, PRIVATE_API(virtual), sizeof(virtual_methods)); /* NOLINT: sizeof(virtual_methods*) */
    /* setup mocks */
    mock_virtual_methods_definitions.read = mock_virtual_read_zero;
    /* setup api endpoint */
    virtual_methods* mock_virtual_methods = &mock_virtual_methods_definitions;
    /* backup api calls */
    memcpy(&virtual_methods_ptr, &PRIVATE_API(virtual), sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    /* init */
    u64 char_ptr = CALL(string)->load(cvm, "/");
    /* prepare to mock api calls */
    memcpy(&PRIVATE_API(virtual), &mock_virtual_methods, sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    /* virtual_string->free fails in virtual->read call */
    u64 copy_ptr = CALL(string)->copy(cvm, char_ptr);
    /* restore api calls */
    memcpy(&PRIVATE_API(virtual), &virtual_methods_ptr, sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    RX_ASSERT(char_ptr != 0);
    RX_ASSERT(copy_ptr == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, char_ptr), "/") == 0);
    CALL(string)->free(cvm, char_ptr);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_load_push_peek_pop, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 char_ptr = CALL(string)->load(cvm, "/");
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, char_ptr), "/") == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, char_ptr);
#endif
}

RX_TEST_CASE(tests_pointer_v1, test_load_free_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 char_ptr = CALL(string)->load(cvm, "/");
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, char_ptr), "/") == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, char_ptr);
    CALL(string)->free(cvm, char_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_strcat_load_alloc, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 char_ptr = CALL(string)->load(cvm, "/");
    u64 pattern_ptr = CALL(string)->load(cvm, "\0");
    CALL(string)->strcat(cvm, pattern_ptr, char_ptr);
    RX_ASSERT(pattern_ptr == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, char_ptr), "/") == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, char_ptr);
    CALL(string)->free(cvm, pattern_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_size, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 char_ptr = CALL(string)->load(cvm, "/");
    u64 pattern_ptr = CALL(string)->load(cvm, "\0");
    u64 string_size = CALL(data)->size(cvm, pattern_ptr);
    RX_ASSERT(string_size == 0);
    RX_ASSERT(pattern_ptr == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, char_ptr), "/") == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, char_ptr);
    CALL(string)->free(cvm, pattern_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_data_size_error_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 string_size = CALL(data)->size(cvm, 1);
    RX_ASSERT(string_size == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_data_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 char_ptr = CALL(string)->load(cvm, "/");
    CALL(error)->clear();
    CALL(data)->free(cvm, char_ptr);
    u64 error_count = CALL(error)->count();
    RX_ASSERT(error_count != 0);
#ifndef USE_GC
    CALL(string)->free(cvm, char_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_file_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 char_ptr = CALL(string)->load(cvm, "/");
    CALL(error)->clear();
    CALL(file)->free(cvm, char_ptr);
    u64 error_count = CALL(error)->count();
    RX_ASSERT(error_count != 0);
#ifndef USE_GC
    CALL(string)->free(cvm, char_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_data_size, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 data_ptr = CALL(data)->alloc(cvm, 2);
    u64 data_size = CALL(data)->size(cvm, data_ptr);
    RX_ASSERT(data_size == 2);
#ifndef USE_GC
    CALL(data)->free(cvm, data_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_pointer_size_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 char_ptr = CALL(string)->load(cvm, "/");
    u64 pattern_ptr = CALL(string)->load(cvm, "\0");
    u64 string_size = CALL(string)->size(cvm, 0);
    RX_ASSERT(string_size == 0);
    RX_ASSERT(pattern_ptr == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, char_ptr), "/") == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, char_ptr);
    CALL(string)->free(cvm, pattern_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_pointer_size_empty, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 char_ptr = CALL(string)->load(cvm, " ");
    u64 pattern_ptr = CALL(string)->load(cvm, "\0");
    u64 string_size = CALL(string)->size(cvm, char_ptr);
    RX_ASSERT(string_size == 1);
    RX_ASSERT(pattern_ptr == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, char_ptr), " ") == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, char_ptr);
    CALL(string)->free(cvm, pattern_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_pointer_size_object, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 object_ptr = CALL(object)->alloc(cvm, 1);
    u64 pattern_ptr = CALL(string)->load(cvm, "\0");
    u64 object_size = CALL(object)->size(cvm, object_ptr);
    RX_ASSERT(object_size == 1);
    RX_ASSERT(object_ptr != 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, pattern_ptr);
    CALL(object)->free(cvm, object_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_object_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 object_ptr = CALL(object)->alloc(cvm, 0);
    u64 pattern_ptr = CALL(string)->load(cvm, "\0");
    u64 object_size = CALL(object)->size(cvm, object_ptr);
    RX_ASSERT(object_size == 0);
    RX_ASSERT(object_ptr == 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, pattern_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_object_size_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 object_size = CALL(object)->size(cvm, 1);
    RX_ASSERT(object_size == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_object_alloc_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 object_ptr = CALL(object)->alloc(cvm, 0);
    u64 pattern_ptr = CALL(string)->load(cvm, "\0");
    u64 object_size = CALL(object)->size(cvm, object_ptr);
    RX_ASSERT(object_size == 0);
    RX_ASSERT(object_ptr == 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, pattern_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_object_load_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    const char* test_data = "Hello, world!";
    const_void_ptr ptr = (const_void_ptr)test_data;
    u64 object_ptr = CALL(object)->load(cvm, ptr, strlen(ptr));
    CALL(object)->free(cvm, object_ptr);
#ifndef USE_GC
    CALL(string)->free(cvm, object_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_object_free_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 object_ptr = 0;
    CALL(object)->free(cvm, object_ptr);
#ifndef USE_GC
    CALL(string)->free(cvm, object_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_object_load_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 object_ptr = CALL(object)->load(cvm, 0, 0x01234567);
    u64 pattern_ptr = CALL(string)->load(cvm, "\0");
    u64 object_size = CALL(object)->size(cvm, object_ptr);
    RX_ASSERT(object_size == 0);
    RX_ASSERT(object_ptr == 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, pattern_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_object_load_string_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    const char* test_data = "Hello, world!";
    const_void_ptr ptr = (const_void_ptr)test_data;
    u64 object_ptr = CALL(object)->load(cvm, ptr, 0);
    u64 pattern_ptr = CALL(string)->load(cvm, "\0");
    u64 object_size = CALL(object)->size(cvm, object_ptr);
    RX_ASSERT(object_size == 0);
    RX_ASSERT(object_ptr == 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, pattern_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_object_load_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    const char* test_data = "Hello, world!";
    const_void_ptr ptr = (const_void_ptr)test_data;
    u64 object_ptr = CALL(object)->load(cvm, ptr, strlen(ptr));
    u64 pattern_ptr = CALL(string)->load(cvm, "\0");
    u64 object_size = CALL(object)->size(cvm, object_ptr);
    RX_ASSERT(object_size == strlen(ptr));
    RX_ASSERT(object_ptr != 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, pattern_ptr);
    CALL(object)->free(cvm, object_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_object_load_string_unsafe_alloc_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 object_ptr = CALL(object)->alloc(cvm, 0);
    u64 pattern_ptr = CALL(string)->load(cvm, "\0");
    u64 object_size = CALL(object)->size(cvm, object_ptr);
    const char* ch = (char*)CALL(object)->unsafe(cvm, object_ptr);
    RX_ASSERT(ch == 0);
    RX_ASSERT(object_size == 0);
    RX_ASSERT(object_ptr == 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, pattern_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_object_load_string_unsafe_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 object_ptr = CALL(object)->alloc(cvm, 0);
    u64 pattern_ptr = CALL(string)->load(cvm, "\0");
    u64 object_size = CALL(object)->size(cvm, object_ptr);
    const char* ch = (char*)CALL(object)->unsafe(cvm, pattern_ptr);
    RX_ASSERT(ch == 0);
    RX_ASSERT(object_size == 0);
    RX_ASSERT(object_ptr == 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, pattern_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_object_load_string_unsafe_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    const char* test_data = "Hello, world!";
    const_void_ptr ptr = (const_void_ptr)test_data;
    u64 object_ptr = CALL(object)->load(cvm, ptr, strlen(ptr));
    u64 pattern_ptr = CALL(string)->load(cvm, "\0");
    u64 object_size = CALL(object)->size(cvm, object_ptr);
    const char* ch = (char*)CALL(object)->unsafe(cvm, 0);
    RX_ASSERT(ch == 0);
    RX_ASSERT(object_size == strlen(ptr));
    RX_ASSERT(object_ptr != 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, object_ptr);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(object)->free(cvm, object_ptr);
#endif
    CALL(vm)->gc(cvm);
    CALL(object)->free(cvm, object_ptr);
    CALL(vm)->destroy(cvm);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_object_load_string_unsafe, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    const char* test_data = "Hello, world!";
    const_void_ptr ptr = (const_void_ptr)test_data;
    u64 object_ptr = CALL(object)->load(cvm, ptr, strlen(ptr));
    u64 pattern_ptr = CALL(string)->load(cvm, "\0");
    u64 object_size = CALL(object)->size(cvm, object_ptr);
    char* ch = (char*)CALL(object)->unsafe(cvm, object_ptr);
    *ch = 'Z';
    RX_ASSERT(object_size == strlen(ptr));
    RX_ASSERT(object_ptr != 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, object_ptr);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(object)->free(cvm, object_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_object_load_string_unsafe_error, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    char* ch = (char*)CALL(object)->unsafe(cvm, 42);
    RX_ASSERT(ch == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_strcat_alloc_alloc, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 char_ptr = CALL(string)->load(cvm, "\0");
    u64 pattern_ptr = CALL(string)->load(cvm, "\0");
    CALL(string)->strcat(cvm, pattern_ptr, char_ptr);
    RX_ASSERT(char_ptr == 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, char_ptr);
    CALL(string)->free(cvm, pattern_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_offset_ptr_diff, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 string_ptr1 = CALL(string)->load(cvm, "192.168.0.111");
    u64 string_ptr2 = CALL(string)->load(cvm, "192.168.0.12");
    u64 last_matched_ptr1 = CALL(string)->offset(cvm, string_ptr1, string_ptr2);
    RX_ASSERT(last_matched_ptr1 == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, last_matched_ptr1);
    CALL(string)->free(cvm, string_ptr1);
    CALL(string)->free(cvm, string_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_offset_ptr_less, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 string_ptr1 = CALL(string)->load(cvm, "192.168.0.11");
    u64 string_ptr2 = CALL(string)->load(cvm, "8.0.112");
    u64 last_matched_ptr1 = CALL(string)->offset(cvm, string_ptr1, string_ptr2);
    RX_ASSERT(last_matched_ptr1 == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, last_matched_ptr1);
    CALL(string)->free(cvm, string_ptr1);
    CALL(string)->free(cvm, string_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_offset_ptr_offset, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 string_ptr1 = CALL(string)->load(cvm, "192.168.0.11");
    u64 string_ptr2 = CALL(string)->load(cvm, "8.0.12");
    u64 last_matched_ptr1 = CALL(string)->offset(cvm, string_ptr1, string_ptr2);
    RX_ASSERT(last_matched_ptr1 == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, last_matched_ptr1);
    CALL(string)->free(cvm, string_ptr1);
    CALL(string)->free(cvm, string_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_offset_ptr_offset_found, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 string_ptr1 = CALL(string)->load(cvm, "192.168.0.111");
    u64 string_ptr2 = CALL(string)->load(cvm, "8.0.");
    u64 last_matched_ptr1 = CALL(string)->offset(cvm, string_ptr1, string_ptr2);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, last_matched_ptr1), "111") == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, string_ptr2);
    CALL(string_pointer)->free(cvm, last_matched_ptr1);
    CALL(string)->free(cvm, string_ptr1);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_free_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
#ifndef USE_GC
    u64 list_ptr = CALL(stack)->alloc(cvm);
    u64 string_ptr = CALL(string)->load(cvm, "hello");
    u64 e_ptr = CALL(string)->load(cvm, "e");
    u64 string_pointer_ptr = CALL(string)->offset(cvm, string_ptr, e_ptr);
    RX_ASSERT(strcmp("llo", CALL(string)->unsafe(cvm, string_pointer_ptr)) == 0);
    CALL(string)->free(cvm, list_ptr);
    CALL(string_pointer)->free(cvm, string_pointer_ptr);
    CALL(string)->free(cvm, string_ptr);
    CALL(string)->free(cvm, e_ptr);
    CALL(stack)->free(cvm, list_ptr);
#else
    CALL(stack)->alloc(cvm);
    u64 string_ptr = CALL(string)->load(cvm, "hello");
    u64 e_ptr = CALL(string)->load(cvm, "e");
    CALL(string)->offset(cvm, string_ptr, e_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(cvm);
    u64 string_ptr = CALL(string)->load(cvm, "hello");
    u64 e_ptr = CALL(string)->load(cvm, "e");
    u64 string_pointer_ptr = CALL(string)->offset(cvm, string_ptr, e_ptr);
    CALL(stack)->push(cvm, list_ptr, string_pointer_ptr);
    CALL(stack)->push(cvm, list_ptr, string_ptr);
#ifndef USE_GC
    CALL(string)->free(cvm, e_ptr);
    CALL(stack)->free(cvm, list_ptr);
    CALL(string_pointer)->free(cvm, string_pointer_ptr);
    CALL(string)->free(cvm, string_ptr);
    CALL(stack)->free(cvm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_stack_pointer_alloc_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(cvm);
    u64 string_ptr = CALL(string)->load(cvm, "hello");
    u64 e_ptr = CALL(string)->load(cvm, "e");
    u64 string_pointer_ptr = CALL(string)->offset(cvm, string_ptr, e_ptr);
    char* dot = (char*)CALL(memory)->alloc(1);
    CALL(os)->memcpy(dot, ".", 1);
    u64 string_from_memory_alloc_ptr = CALL(pointer)->alloc(cvm, dot, 1, TYPE_STRING);
    CALL(stack)->push(cvm, list_ptr, string_pointer_ptr);
    CALL(stack)->push(cvm, list_ptr, string_from_memory_alloc_ptr);
    CALL(stack)->push(cvm, list_ptr, string_ptr);
#ifndef USE_GC
    CALL(string)->free(cvm, e_ptr);
    CALL(string_pointer)->free(cvm, string_pointer_ptr);
    CALL(string)->free(cvm, string_ptr);
    CALL(pointer)->free(cvm, string_pointer_ptr);
    CALL(stack)->free(cvm, list_ptr);
#endif
    CALL(pointer)->free(cvm, string_from_memory_alloc_ptr);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_offset_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(cvm);
    u64 string_ptr = CALL(string)->load(cvm, "hello");
    u64 last_matched_ptr = CALL(string)->offset(cvm, list_ptr, string_ptr);
    RX_ASSERT(last_matched_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, string_ptr);
    CALL(stack)->free(cvm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_match, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(cvm);
    u64 string_ptr = CALL(string)->load(cvm, "192.192.0.1");
    u64 dot_ptr = CALL(string)->load(cvm, "192.");
    u64 string_pointer_ptr1 = CALL(string)->match(cvm, string_ptr, dot_ptr);
    u64 string_pointer_ptr2 = CALL(string)->match(cvm, string_pointer_ptr1, dot_ptr);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, string_pointer_ptr1), "192.0.1") == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, string_pointer_ptr2), "0.1") == 0);
    CALL(stack)->push(cvm, list_ptr, string_pointer_ptr1);
    CALL(stack)->push(cvm, list_ptr, string_pointer_ptr2);
    CALL(stack)->push(cvm, list_ptr, string_ptr);
#ifndef USE_GC
    CALL(string)->free(cvm, dot_ptr);
    CALL(stack)->free(cvm, list_ptr);
    CALL(string_pointer)->free(cvm, string_pointer_ptr1);
    CALL(string_pointer)->free(cvm, string_pointer_ptr2);
    CALL(string)->free(cvm, string_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_match_miss, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(cvm);
    u64 string_ptr = CALL(string)->load(cvm, "192.168.0.1");
    u64 dot_ptr = CALL(string)->load(cvm, ":");
    u64 string_pointer_ptr1 = CALL(string)->match(cvm, string_ptr, dot_ptr);
    RX_ASSERT(string_pointer_ptr1 == 0);
    CALL(stack)->push(cvm, list_ptr, string_pointer_ptr1);
    CALL(stack)->push(cvm, list_ptr, string_ptr);
#ifndef USE_GC
    CALL(string)->free(cvm, dot_ptr);
    CALL(stack)->free(cvm, list_ptr);
    CALL(string_pointer)->free(cvm, string_pointer_ptr1);
    CALL(string)->free(cvm, string_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_match_pattern, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(cvm);
    u64 string_ptr = CALL(string)->load(cvm, "192.168.0.112");
    u64 dot_ptr = CALL(string)->load(cvm, "192.168.0.");
    u64 string_pointer_ptr1 = CALL(string)->match(cvm, string_ptr, dot_ptr);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, string_pointer_ptr1), "112") == 0);
    CALL(stack)->push(cvm, list_ptr, string_pointer_ptr1);
    CALL(stack)->push(cvm, list_ptr, string_ptr);
#ifndef USE_GC
    CALL(string)->free(cvm, dot_ptr);
    CALL(stack)->free(cvm, list_ptr);
    CALL(string_pointer)->free(cvm, string_pointer_ptr1);
    CALL(string)->free(cvm, string_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_match_pattern_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(cvm);
    u64 string_ptr = CALL(string)->load(cvm, "192.168.0.112");
    u64 dot_ptr = CALL(string)->load(cvm, "193");
    u64 string_pointer_ptr1 = CALL(string)->match(cvm, string_ptr, dot_ptr);
    RX_ASSERT(string_pointer_ptr1 == 0);
    CALL(stack)->push(cvm, list_ptr, string_pointer_ptr1);
    CALL(stack)->push(cvm, list_ptr, string_ptr);
#ifndef USE_GC
    CALL(string)->free(cvm, dot_ptr);
    CALL(stack)->free(cvm, list_ptr);
    CALL(string_pointer)->free(cvm, string_pointer_ptr1);
    CALL(string)->free(cvm, string_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_match_pattern_none, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(cvm);
    u64 string_ptr = CALL(string)->load(cvm, "192.168.0.11");
    u64 dot_ptr = CALL(string)->load(cvm, "192.168.0.111");
    u64 string_pointer_ptr1 = CALL(string)->match(cvm, string_ptr, dot_ptr);
    RX_ASSERT(string_pointer_ptr1 == 0);
    CALL(stack)->push(cvm, list_ptr, string_pointer_ptr1);
    CALL(stack)->push(cvm, list_ptr, string_ptr);
#ifndef USE_GC
    CALL(string)->free(cvm, dot_ptr);
    CALL(stack)->free(cvm, list_ptr);
    CALL(string_pointer)->free(cvm, string_pointer_ptr1);
    CALL(string)->free(cvm, string_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_release_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 stack_data_ptr = CALL(stack)->alloc(cvm);
    CALL(stack)->release(cvm, stack_data_ptr);
#ifndef USE_GC
    CALL(string)->free(cvm, stack_data_ptr);
    CALL(stack)->free(cvm, stack_data_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_release, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 string_ptr = CALL(string)->load(cvm, "192.168.0.11");
    CALL(stack)->release(cvm, string_ptr);
#ifndef USE_GC
    CALL(string)->free(cvm, string_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_release_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    CALL(stack)->release(cvm, 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_free_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 string_ptr = CALL(string)->load(cvm, "192.168.0.11");
    CALL(stack)->free(cvm, string_ptr);
#ifndef USE_GC
    CALL(string)->free(cvm, string_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_free_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    CALL(stack)->free(cvm, 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_data_free_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    CALL(data)->free(cvm, 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_match_pattern_load_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 string_ptr = CALL(string)->load(cvm, "192.168.0.1");
    u64 dot_ptr = CALL(string)->load(cvm, "57.34.7.2");
    u64 error_ptr = CALL(string)->match(cvm, string_ptr, dot_ptr);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, string_ptr);
    CALL(string)->free(cvm, dot_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_offset_pattern_load_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 string_ptr = CALL(string)->load(cvm, "192.168.0.1");
    u64 dot_ptr = CALL(string)->load(cvm, "57.34.7.2");
    u64 error_ptr = CALL(string)->offset(cvm, string_ptr, dot_ptr);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, string_ptr);
    CALL(string)->free(cvm, dot_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_match_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 string_ptr = CALL(string)->load(cvm, "192.168.0.1");
    u64 string_pointer_ptr = CALL(string)->match(cvm, string_ptr, 0);
    RX_ASSERT(string_pointer_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, string_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_match_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
#ifndef USE_GC
    u64 string_ptr = CALL(string)->load(cvm, "192.168.0.1");
    u64 string_pointer_ptr = CALL(string)->match(cvm, 0, 0);
    RX_ASSERT(string_pointer_ptr == 0);
    CALL(string)->free(cvm, string_ptr);
#else
    CALL(string)->load(cvm, "192.168.0.1");
    u64 string_pointer_ptr = CALL(string)->match(cvm, 0, 0);
    RX_ASSERT(string_pointer_ptr == 0);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_match_0_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 string_ptr = CALL(string)->load(cvm, "192.168.0.1");
    u64 string_pointer_ptr = CALL(string)->match(cvm, 0, string_ptr);
    RX_ASSERT(string_pointer_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, string_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_match_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(cvm);
    u64 string_ptr = CALL(string)->load(cvm, "hello");
    u64 last_matched_ptr = CALL(string)->match(cvm, list_ptr, string_ptr);
    RX_ASSERT(last_matched_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, string_ptr);
    CALL(stack)->free(cvm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_match_list_error, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(cvm);
    u64 last_matched_ptr = CALL(string)->match(cvm, list_ptr, 0);
    RX_ASSERT(last_matched_ptr == 0);
#ifndef USE_GC
    CALL(stack)->free(cvm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_match_list_error_42, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(cvm);
    u64 last_matched_ptr = CALL(string)->match(cvm, list_ptr, 42);
    RX_ASSERT(last_matched_ptr == 0);
#ifndef USE_GC
    CALL(stack)->free(cvm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_strchr, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(cvm);
    u64 string_ptr = CALL(string)->load(cvm, "a.b.c/2");
    u64 dot_ptr = CALL(string)->load(cvm, ".");
    u64 string_pointer_ptr1 = CALL(string)->strchr(cvm, string_ptr, dot_ptr);
    u64 string_pointer_ptr2 = CALL(string)->match(cvm, string_pointer_ptr1, dot_ptr);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, string_pointer_ptr1), ".b.c/2") == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, string_pointer_ptr2), "b.c/2") == 0);
    CALL(stack)->push(cvm, list_ptr, string_pointer_ptr1);
    CALL(stack)->push(cvm, list_ptr, string_pointer_ptr2);
    CALL(stack)->push(cvm, list_ptr, string_ptr);
#ifndef USE_GC
    CALL(string)->free(cvm, dot_ptr);
    CALL(stack)->free(cvm, list_ptr);
    CALL(string_pointer)->free(cvm, string_pointer_ptr1);
    CALL(string_pointer)->free(cvm, string_pointer_ptr2);
    CALL(string)->free(cvm, string_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_strchr_miss, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(cvm);
    u64 string_ptr = CALL(string)->load(cvm, "192.168.0.1");
    u64 dot_ptr = CALL(string)->load(cvm, ":");
    u64 string_pointer_ptr1 = CALL(string)->strchr(cvm, string_ptr, dot_ptr);
    RX_ASSERT(string_pointer_ptr1 == 0);
    CALL(stack)->push(cvm, list_ptr, string_pointer_ptr1);
    CALL(stack)->push(cvm, list_ptr, string_ptr);
#ifndef USE_GC
    CALL(string)->free(cvm, dot_ptr);
    CALL(stack)->free(cvm, list_ptr);
    CALL(string_pointer)->free(cvm, string_pointer_ptr1);
    CALL(string)->free(cvm, string_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_strchr_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 string_ptr = CALL(string)->load(cvm, "192.168.0.1");
    u64 string_pointer_ptr = CALL(string)->strchr(cvm, string_ptr, 0);
    RX_ASSERT(string_pointer_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, string_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_strchr_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
#ifndef USE_GC
    u64 string_ptr = CALL(string)->load(cvm, "192.168.0.1");
    u64 string_pointer_ptr = CALL(string)->strchr(cvm, 0, 0);
    RX_ASSERT(string_pointer_ptr == 0);
    CALL(string)->free(cvm, string_ptr);
#else
    CALL(string)->load(cvm, "192.168.0.1");
    u64 string_pointer_ptr = CALL(string)->strchr(cvm, 0, 0);
    RX_ASSERT(string_pointer_ptr == 0);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_strchr_0_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 string_ptr = CALL(string)->load(cvm, "192.168.0.1");
    u64 string_pointer_ptr = CALL(string)->strchr(cvm, 0, string_ptr);
    RX_ASSERT(string_pointer_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, string_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_strchr_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(cvm);
    u64 string_ptr = CALL(string)->load(cvm, "hello");
    u64 last_matched_ptr = CALL(string)->strchr(cvm, list_ptr, string_ptr);
    RX_ASSERT(last_matched_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, string_ptr);
    CALL(stack)->free(cvm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_strrchr, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 string_ptr = CALL(string)->load(cvm, "192.168.0.1");
    u64 dot_ptr = CALL(string)->load(cvm, ".");
    u64 string_pointer_ptr1 = CALL(string)->strrchr(cvm, string_ptr, dot_ptr);
    u64 string_pointer_ptr2 = CALL(string)->match(cvm, string_pointer_ptr1, dot_ptr);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, string_pointer_ptr1), ".1") == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, string_pointer_ptr2), "1") == 0);
#ifndef USE_GC
    CALL(string_pointer)->free(cvm, string_pointer_ptr1);
    CALL(string_pointer)->free(cvm, string_pointer_ptr2);
    CALL(string)->free(cvm, string_ptr);
    CALL(string)->free(cvm, dot_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_strrchr_ptr_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 string_ptr = CALL(string)->load(cvm, "192.168.0.1");
    char* dot = (char*)CALL(memory)->alloc(1);
    CALL(os)->memcpy(dot, ".", 1);
    u64 dot_ptr = CALL(pointer)->alloc(cvm, dot, 1, TYPE_STRING_POINTER);
    u64 string_pointer_ptr1 = CALL(string)->strrchr(cvm, string_ptr, dot_ptr);
    u64 string_pointer_ptr2 = CALL(string)->match(cvm, string_pointer_ptr1, dot_ptr);
    RX_ASSERT(string_pointer_ptr1 == 0);
    RX_ASSERT(string_pointer_ptr2 == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, string_ptr);
#endif
    CALL(pointer)->free(cvm, dot_ptr);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_strchr_ptr_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 string_ptr = CALL(string)->load(cvm, "192.168.0.1");
    char* dot = (char*)CALL(memory)->alloc(1);
    CALL(os)->memcpy(dot, ".", 1);
    u64 dot_ptr = CALL(pointer)->alloc(cvm, dot, 1, TYPE_STRING_POINTER);
    u64 string_pointer_ptr1 = CALL(string)->strchr(cvm, string_ptr, dot_ptr);
    u64 string_pointer_ptr2 = CALL(string)->match(cvm, string_pointer_ptr1, dot_ptr);
    RX_ASSERT(string_pointer_ptr1 == 0);
    RX_ASSERT(string_pointer_ptr2 == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, string_ptr);
#endif
    CALL(pointer)->free(cvm, dot_ptr);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_strrchr_match_offset, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 string_ptr = CALL(string)->load(cvm, "192.168.0.111");
    u64 dot_ptr = CALL(string)->load(cvm, ".");
    u64 string_pointer_ptr1 = CALL(string)->strrchr(cvm, string_ptr, dot_ptr);
    u64 string_pointer_ptr2 = CALL(string)->match(cvm, string_pointer_ptr1, dot_ptr);
    u64 string_pointer_ptr3 = CALL(string)->offset(cvm, string_ptr, dot_ptr);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, string_pointer_ptr1), ".111") == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, string_pointer_ptr2), "111") == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, string_pointer_ptr3), "111") == 0);
#ifndef USE_GC
    CALL(string_pointer)->free(cvm, string_pointer_ptr1);
    CALL(string_pointer)->free(cvm, string_pointer_ptr2);
    CALL(string_pointer)->free(cvm, string_pointer_ptr3);
    CALL(string)->free(cvm, string_ptr);
    CALL(string)->free(cvm, dot_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_peekn_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(cvm);
    RX_ASSERT(list_ptr != 0);
    u64 error_ptr = CALL(stack)->peekn(cvm, 0, 0);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(stack)->free(cvm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_peekn_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(cvm);
    u64 error_ptr = CALL(stack)->peekn(cvm, list_ptr, 0);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(stack)->free(cvm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_peekn_error_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    char* dot = (char*)CALL(memory)->alloc(1);
    CALL(os)->memcpy(dot, ".", 1);
    u64 list_ptr = CALL(pointer)->alloc(cvm, dot, 1, TYPE_STRING);
    u64 error_ptr = CALL(stack)->peekn(cvm, list_ptr, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->free(cvm, list_ptr);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_peekn_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr1 = CALL(stack)->alloc(cvm);
    u64 string_ptr1 = CALL(string)->load(cvm, "192.168.0.1");
    CALL(stack)->push(cvm, list_ptr1, string_ptr1);
    u64 list_ptr2 = CALL(stack)->peekn(cvm, list_ptr1, 1);
    RX_ASSERT(list_ptr2 != 0);
    RX_ASSERT(CALL(stack)->size(cvm, list_ptr2) != 0);
    u64 string_ptr2 = CALL(stack)->peek(cvm, list_ptr2);
    RX_ASSERT(string_ptr2 != 0);
    RX_ASSERT(string_ptr1 == string_ptr2);
#ifndef USE_GC
    CALL(string)->free(cvm, string_ptr1);
    CALL(string)->free(cvm, string_ptr2);
    CALL(stack)->free(cvm, list_ptr2);
    CALL(stack)->free(cvm, list_ptr1);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_peekn_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 string_ptr = CALL(string)->load(cvm, "192.168.0.1");
    u64 list_ptr = CALL(stack)->alloc(cvm);
    CALL(stack)->push(cvm, list_ptr, string_ptr);
    u64 error_ptr = CALL(stack)->peekn(cvm, list_ptr, 2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, string_ptr);
    CALL(stack)->free(cvm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_0_peekn, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(cvm);
    u64 error_ptr = CALL(stack)->peekn(cvm, list_ptr, 2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(stack)->free(cvm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_0_popn, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(cvm);
    u64 error_ptr = CALL(stack)->popn(cvm, list_ptr, 2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(stack)->free(cvm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_popn_error_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    char* dot = (char*)CALL(memory)->alloc(1);
    CALL(os)->memcpy(dot, ".", 1);
    u64 list_ptr = CALL(pointer)->alloc(cvm, dot, 1, TYPE_STRING);
    u64 error_ptr = CALL(stack)->popn(cvm, list_ptr, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(pointer)->free(cvm, list_ptr);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_popn_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(cvm);
    RX_ASSERT(list_ptr != 0);
    u64 error_ptr = CALL(stack)->popn(cvm, 0, 0);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(stack)->free(cvm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_popn_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(cvm);
    u64 error_ptr = CALL(stack)->popn(cvm, list_ptr, 0);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(stack)->free(cvm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_popn_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr1 = CALL(stack)->alloc(cvm);
    u64 string_ptr1 = CALL(string)->load(cvm, "192.168.0.1");
    CALL(stack)->push(cvm, list_ptr1, string_ptr1);
    u64 list_ptr2 = CALL(stack)->popn(cvm, list_ptr1, 1);
    RX_ASSERT(list_ptr2 != 0);
    RX_ASSERT(CALL(stack)->size(cvm, list_ptr2) != 0);
    u64 string_ptr2 = CALL(stack)->peek(cvm, list_ptr2);
    RX_ASSERT(string_ptr2 != 0);
    RX_ASSERT(string_ptr1 == string_ptr2);
#ifndef USE_GC
    CALL(string)->free(cvm, string_ptr1);
    CALL(string)->free(cvm, string_ptr2);
    CALL(stack)->free(cvm, list_ptr2);
    CALL(stack)->free(cvm, list_ptr1);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_popn_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 string_ptr = CALL(string)->load(cvm, "192.168.0.1");
    u64 list_ptr = CALL(stack)->alloc(cvm);
    CALL(stack)->push(cvm, list_ptr, string_ptr);
    u64 error_ptr = CALL(stack)->popn(cvm, list_ptr, 2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, string_ptr);
    CALL(stack)->free(cvm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_strrchr_miss, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(cvm);
    u64 string_ptr = CALL(string)->load(cvm, "192.168.0.1");
    u64 dot_ptr = CALL(string)->load(cvm, ":");
    u64 string_pointer_ptr1 = CALL(string)->strrchr(cvm, string_ptr, dot_ptr);
    RX_ASSERT(string_pointer_ptr1 == 0);
    CALL(stack)->push(cvm, list_ptr, string_pointer_ptr1);
    CALL(stack)->push(cvm, list_ptr, string_ptr);
#ifndef USE_GC
    CALL(string)->free(cvm, dot_ptr);
    CALL(stack)->free(cvm, list_ptr);
    CALL(string)->free(cvm, string_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_strrchr_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 string_ptr = CALL(string)->load(cvm, "192.168.0.1");
    u64 string_pointer_ptr = CALL(string)->strrchr(cvm, string_ptr, 0);
    RX_ASSERT(string_pointer_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, string_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_strrchr_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
#ifndef USE_GC
    u64 string_ptr = CALL(string)->load(cvm, "192.168.0.1");
    u64 string_pointer_ptr = CALL(string)->strrchr(cvm, 0, 0);
    RX_ASSERT(string_pointer_ptr == 0);
    CALL(string)->free(cvm, string_ptr);
#else
    CALL(string)->load(cvm, "192.168.0.1");
    u64 string_pointer_ptr = CALL(string)->strrchr(cvm, 0, 0);
    RX_ASSERT(string_pointer_ptr == 0);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_strrchr_0_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 string_ptr = CALL(string)->load(cvm, "192.168.0.1");
    u64 string_pointer_ptr = CALL(string)->strrchr(cvm, 0, string_ptr);
    RX_ASSERT(string_pointer_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, string_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_strrchr_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(cvm);
    u64 string_ptr = CALL(string)->load(cvm, "hello");
    u64 last_matched_ptr = CALL(string)->strrchr(cvm, list_ptr, string_ptr);
    RX_ASSERT(last_matched_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, string_ptr);
    CALL(stack)->free(cvm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_push_0_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    CALL(stack)->push(cvm, 0, 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_push_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    CALL(stack)->push(cvm, 1, 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_push_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(cvm);
    CALL(stack)->push(cvm, list_ptr, 0);
#ifndef USE_GC
    CALL(stack)->free(cvm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_push_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(cvm);
    CALL(stack)->push(cvm, list_ptr, list_ptr);
#ifndef USE_GC
    CALL(stack)->free(cvm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_push_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 string_ptr = CALL(string)->load(cvm, "@");
    u64 list_ptr = CALL(stack)->alloc(cvm);
    CALL(stack)->push(cvm, string_ptr, list_ptr);
#ifndef USE_GC
    CALL(stack)->free(cvm, list_ptr);
    CALL(string)->free(cvm, string_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_size_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
#ifndef USE_GC
    u64 string_ptr = CALL(string)->load(cvm, "@");
    u64 list_ptr = CALL(stack)->alloc(cvm);
    u64 value = CALL(stack)->size(cvm, string_ptr);
    RX_ASSERT(value == 0);
    CALL(stack)->free(cvm, list_ptr);
    CALL(string)->free(cvm, string_ptr);
#else
    u64 string_ptr = CALL(string)->load(cvm, "@");
    CALL(stack)->alloc(cvm);
    u64 value = CALL(stack)->size(cvm, string_ptr);
    RX_ASSERT(value == 0);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_stack_size_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 value = CALL(stack)->size(cvm, 0);
    RX_ASSERT(value == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_push_size, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 string_ptr = CALL(string)->load(cvm, "@");
    u64 list_ptr = CALL(stack)->alloc(cvm);
    CALL(stack)->push(cvm, list_ptr, string_ptr);
    u64 value = CALL(stack)->size(cvm, list_ptr);
    RX_ASSERT(value != 0);
#ifndef USE_GC
    CALL(stack)->free(cvm, list_ptr);
    CALL(string)->free(cvm, string_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_pop_0_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 value_ptr = CALL(stack)->pop(cvm, 0);
    RX_ASSERT(value_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_pop_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 value_ptr = CALL(stack)->pop(cvm, 1);
    RX_ASSERT(value_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_pop_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(cvm);
    u64 value_ptr = CALL(stack)->pop(cvm, list_ptr);
    RX_ASSERT(value_ptr == 0);
#ifndef USE_GC
    CALL(stack)->free(cvm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_pop_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 string_ptr = CALL(string)->load(cvm, "@");
    u64 list_ptr = CALL(stack)->alloc(cvm);
    CALL(stack)->push(cvm, list_ptr, string_ptr);
    u64 value_ptr = CALL(stack)->pop(cvm, list_ptr);
    RX_ASSERT(value_ptr != 0);
#ifndef USE_GC
    CALL(stack)->free(cvm, list_ptr);
    CALL(string)->free(cvm, string_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_peek_0_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 value_ptr = CALL(stack)->peek(cvm, 0);
    RX_ASSERT(value_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_peek_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 value_ptr = CALL(stack)->peek(cvm, 1);
    RX_ASSERT(value_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_peek_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(cvm);
    u64 value_ptr = CALL(stack)->peek(cvm, list_ptr);
    RX_ASSERT(value_ptr == 0);
#ifndef USE_GC
    CALL(stack)->free(cvm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_peek_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 string_ptr = CALL(string)->load(cvm, "@");
    u64 list_ptr = CALL(stack)->alloc(cvm);
    CALL(stack)->push(cvm, list_ptr, string_ptr);
    u64 value_ptr = CALL(stack)->peek(cvm, list_ptr);
    RX_ASSERT(value_ptr != 0);
#ifndef USE_GC
    CALL(stack)->free(cvm, list_ptr);
    CALL(string)->free(cvm, string_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_strcpy, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 path_ptr1 = CALL(string)->load(cvm, "/");
    u64 path_ptr2 = CALL(string)->load(cvm, "@");
    u64 path_copy_ptr = CALL(string)->copy(cvm, path_ptr1);
    CALL(string)->strcat(cvm, path_copy_ptr, path_ptr2);

    char* path1 = CALL(string)->unsafe(cvm, path_ptr1);
    char* path2 = CALL(string)->unsafe(cvm, path_ptr2);
    u64 path1_len = strlen(path1);
    u64 path2_len = strlen(path2);
    RX_ASSERT(path1_len > 0);
    RX_ASSERT(path2_len > 0);
    char* buf = CALL(memory)->alloc(path1_len + path2_len + 1);
    strcpy(buf, path1); /* NOLINT */
    strcat(buf, path2); /* NOLINT */
    char* path_copy = CALL(string)->unsafe(cvm, path_copy_ptr);
    RX_ASSERT(strlen(path_copy) == strlen(buf));
    RX_ASSERT(strcmp(path_copy, buf) == 0);
    CALL(memory)->free(buf, path1_len + path2_len + 1);
#ifndef USE_GC
    CALL(string)->free(cvm, path_ptr1);
    CALL(string)->free(cvm, path_ptr2);
    CALL(string)->free(cvm, path_copy_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_strcat_load_put_char, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    const char* ch = "hello, world!";
    u64 string_ptr = CALL(pointer)->copy(cvm, ch, 2, 0, TYPE_STRING);
    CALL(string)->put_char(cvm, string_ptr, 'a');
    const char* actual = CALL(string)->unsafe(cvm, string_ptr);
    RX_ASSERT(actual[0] == 'a');
    RX_ASSERT(actual[1] == 'e');
    CALL(vm)->release(cvm, string_ptr);
    CALL(string)->free(cvm, string_ptr);
}

RX_TEST_CASE(tests_pointer_v1, test_strcat_load_put_char_offset, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    const char* ch = "hello, world!";
    u64 string_ptr = CALL(pointer)->copy(cvm, ch, 2, 14, TYPE_STRING);
    CALL(string)->put_char(cvm, string_ptr, 'a');
    const char* actual = CALL(string)->unsafe(cvm, string_ptr);
    RX_ASSERT(actual[0] == 'a');
    RX_ASSERT(actual[1] == 'e');
    CALL(vm)->release(cvm, string_ptr);
    CALL(string)->free(cvm, string_ptr);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_strcat_load_alloc_copy, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 string_ptr = CALL(string)->load(cvm, "/all_english_words.txt");
    u64 zero_ptr = CALL(string)->load(cvm, "\0");
    u64 data_ptr = CALL(data)->alloc(cvm, 1);
    CALL(data)->free(cvm, data_ptr);
    u64 list_ptr = CALL(stack)->alloc(cvm);
    u64 empty_ptr = CALL(string)->load(cvm, "\0");

    u64 null_ptr = 0;
    u64 none_ptr = 0xfffffffff;

    u64 data_ptr1 = CALL(string)->copy(cvm, null_ptr);
    u64 data_ptr2 = CALL(string)->copy(cvm, zero_ptr);
    u64 data_ptr3 = CALL(string)->copy(cvm, string_ptr);
    u64 data_ptr4 = CALL(string)->copy(cvm, none_ptr);

    CALL(env)->puts(cvm, string_ptr);
    CALL(env)->puts(cvm, zero_ptr);
    CALL(env)->puts(cvm, data_ptr);
    CALL(env)->puts(cvm, list_ptr);
    CALL(env)->puts(cvm, empty_ptr);
    CALL(env)->puts(cvm, null_ptr);
    CALL(env)->puts(cvm, none_ptr);

    CALL(string)->put_char(cvm, string_ptr, 'a');
    CALL(string)->put_char(cvm, zero_ptr, 'a');
    CALL(string)->put_char(cvm, data_ptr, 'a');
    CALL(string)->put_char(cvm, list_ptr, 'a');
    CALL(string)->put_char(cvm, empty_ptr, 'a');
    CALL(string)->put_char(cvm, null_ptr, 'a');
    CALL(string)->put_char(cvm, none_ptr, 'a');

    CALL(string)->put_char(cvm, string_ptr, '\0');
    CALL(string)->put_char(cvm, zero_ptr, '\0');
    CALL(string)->put_char(cvm, data_ptr, '\0');
    CALL(string)->put_char(cvm, list_ptr, '\0');
    CALL(string)->put_char(cvm, empty_ptr, '\0');
    CALL(string)->put_char(cvm, null_ptr, '\0');
    CALL(string)->put_char(cvm, none_ptr, '\0');

    CALL(string)->put_char(cvm, string_ptr, '/');
    CALL(string)->strcpy(cvm, string_ptr, string_ptr);
    CALL(string)->strcpy(cvm, string_ptr, zero_ptr);
    CALL(string)->strcpy(cvm, zero_ptr, string_ptr);
    CALL(string)->strcpy(cvm, data_ptr, null_ptr);
    CALL(string)->strcpy(cvm, list_ptr, null_ptr);
    CALL(string)->strcpy(cvm, string_ptr, data_ptr);
    CALL(string)->strcpy(cvm, string_ptr, empty_ptr);
    CALL(string)->strcpy(cvm, data_ptr, null_ptr);
    CALL(string)->strcpy(cvm, list_ptr, string_ptr);
    CALL(string)->strcpy(cvm, data_ptr, null_ptr);
    CALL(string)->strcpy(cvm, list_ptr, null_ptr);
    CALL(string)->strcpy(cvm, string_ptr, data_ptr);
    CALL(string)->strcpy(cvm, data_ptr, null_ptr);
    CALL(string)->strcpy(cvm, list_ptr, string_ptr);
    CALL(string)->strcpy(cvm, null_ptr, null_ptr);
    CALL(string)->strcpy(cvm, string_ptr, null_ptr);
    CALL(string)->strcpy(cvm, string_ptr, none_ptr);
    CALL(string)->strcpy(cvm, none_ptr, string_ptr);

    CALL(string)->strcat(cvm, string_ptr, string_ptr);
    CALL(string)->strcat(cvm, string_ptr, zero_ptr);
    CALL(string)->strcat(cvm, zero_ptr, string_ptr);
    CALL(string)->strcat(cvm, data_ptr, null_ptr);
    CALL(string)->strcat(cvm, list_ptr, null_ptr);
    CALL(string)->strcat(cvm, string_ptr, data_ptr);
    CALL(string)->strcat(cvm, string_ptr, empty_ptr);
    CALL(string)->strcat(cvm, data_ptr, null_ptr);
    CALL(string)->strcat(cvm, list_ptr, string_ptr);
    CALL(string)->strcat(cvm, data_ptr, null_ptr);
    CALL(string)->strcat(cvm, list_ptr, null_ptr);
    CALL(string)->strcat(cvm, string_ptr, data_ptr);
    CALL(string)->strcat(cvm, data_ptr, null_ptr);
    CALL(string)->strcat(cvm, list_ptr, string_ptr);
    CALL(string)->strcat(cvm, null_ptr, null_ptr);
    CALL(string)->strcat(cvm, string_ptr, null_ptr);
    CALL(string)->strcat(cvm, string_ptr, none_ptr);
    CALL(string)->strcat(cvm, none_ptr, string_ptr);

    u64 last_matched_ptr1 = CALL(string)->offset(cvm, string_ptr, string_ptr);
    CALL(string)->offset(cvm, data_ptr, null_ptr);
    CALL(string)->offset(cvm, list_ptr, null_ptr);
    CALL(string)->offset(cvm, string_ptr, data_ptr);
    CALL(string)->offset(cvm, string_ptr, empty_ptr);
    CALL(string)->offset(cvm, data_ptr, null_ptr);
    CALL(string)->offset(cvm, list_ptr, string_ptr);
    CALL(string)->offset(cvm, data_ptr, null_ptr);
    CALL(string)->offset(cvm, list_ptr, null_ptr);
    CALL(string)->offset(cvm, string_ptr, data_ptr);
    CALL(string)->offset(cvm, data_ptr, null_ptr);
    CALL(string)->offset(cvm, list_ptr, string_ptr);
    CALL(string)->offset(cvm, null_ptr, null_ptr);
    CALL(string)->offset(cvm, string_ptr, null_ptr);

    u64 last_matched_ptr2 = CALL(string)->offset(cvm, string_ptr, string_ptr);
    CALL(string)->offset(cvm, string_ptr, zero_ptr);
    CALL(string)->offset(cvm, data_ptr, zero_ptr);
    CALL(string)->offset(cvm, list_ptr, zero_ptr);
    CALL(string)->offset(cvm, string_ptr, data_ptr);
    CALL(string)->offset(cvm, zero_ptr, zero_ptr);
    CALL(string)->offset(cvm, zero_ptr, data_ptr);
    CALL(string)->offset(cvm, zero_ptr, list_ptr);
    CALL(string)->offset(cvm, zero_ptr, string_ptr);
    CALL(string)->offset(cvm, empty_ptr, empty_ptr);
    CALL(string)->offset(cvm, empty_ptr, string_ptr);
    CALL(string)->offset(cvm, empty_ptr, zero_ptr);

    CALL(string)->offset(cvm, null_ptr, null_ptr);
    CALL(string)->offset(cvm, null_ptr, empty_ptr);
    CALL(string)->offset(cvm, null_ptr, string_ptr);
    CALL(string)->offset(cvm, null_ptr, data_ptr);
    CALL(string)->offset(cvm, null_ptr, list_ptr);
    CALL(string)->offset(cvm, empty_ptr, null_ptr);
    CALL(string)->offset(cvm, empty_ptr, empty_ptr);
    CALL(string)->offset(cvm, empty_ptr, string_ptr);
    CALL(string)->offset(cvm, empty_ptr, data_ptr);
    CALL(string)->offset(cvm, empty_ptr, list_ptr);
    CALL(string)->offset(cvm, string_ptr, null_ptr);
    CALL(string)->offset(cvm, string_ptr, empty_ptr);

    u64 last_matched_ptr3 = CALL(string)->offset(cvm, string_ptr, string_ptr);
    CALL(string)->offset(cvm, string_ptr, data_ptr);
    CALL(string)->offset(cvm, string_ptr, list_ptr);
    CALL(string)->offset(cvm, data_ptr, null_ptr);
    CALL(string)->offset(cvm, data_ptr, empty_ptr);
    CALL(string)->offset(cvm, data_ptr, string_ptr);
    CALL(string)->offset(cvm, data_ptr, data_ptr);
    CALL(string)->offset(cvm, data_ptr, list_ptr);
    CALL(string)->offset(cvm, list_ptr, null_ptr);
    CALL(string)->offset(cvm, list_ptr, empty_ptr);
    CALL(string)->offset(cvm, list_ptr, string_ptr);
    CALL(string)->offset(cvm, list_ptr, data_ptr);
    CALL(string)->offset(cvm, list_ptr, list_ptr);
    CALL(string)->offset(cvm, string_ptr, none_ptr);
    CALL(string)->offset(cvm, none_ptr, string_ptr);

    CALL(string)->unsafe(cvm, null_ptr);
    CALL(string)->unsafe(cvm, empty_ptr);
    CALL(string)->unsafe(cvm, string_ptr);
    CALL(string)->unsafe(cvm, data_ptr);
    CALL(string)->unsafe(cvm, list_ptr);
    CALL(string)->unsafe(cvm, none_ptr);

    CALL(file)->alloc(cvm, zero_ptr, zero_ptr);
    CALL(file)->alloc(cvm, string_ptr, zero_ptr);
    CALL(file)->alloc(cvm, data_ptr, zero_ptr);
    CALL(file)->alloc(cvm, list_ptr, zero_ptr);
    CALL(file)->alloc(cvm, zero_ptr, string_ptr);
    CALL(file)->alloc(cvm, zero_ptr, data_ptr);
    CALL(file)->alloc(cvm, zero_ptr, list_ptr);
    CALL(file)->alloc(cvm, string_ptr, string_ptr);
    CALL(file)->alloc(cvm, data_ptr, null_ptr);
    CALL(file)->alloc(cvm, list_ptr, null_ptr);
    CALL(file)->alloc(cvm, string_ptr, data_ptr);
    CALL(file)->alloc(cvm, string_ptr, empty_ptr);
    CALL(file)->alloc(cvm, data_ptr, null_ptr);
    CALL(file)->alloc(cvm, list_ptr, string_ptr);
    CALL(file)->alloc(cvm, data_ptr, null_ptr);
    CALL(file)->alloc(cvm, list_ptr, null_ptr);
    CALL(file)->alloc(cvm, string_ptr, data_ptr);
    CALL(file)->alloc(cvm, data_ptr, null_ptr);
    CALL(file)->alloc(cvm, list_ptr, string_ptr);
    CALL(file)->alloc(cvm, null_ptr, null_ptr);
    CALL(file)->alloc(cvm, string_ptr, null_ptr);
    CALL(file)->alloc(cvm, string_ptr, none_ptr);
    CALL(file)->alloc(cvm, none_ptr, string_ptr);

    const u64* data_unsafe_ptr1 = CALL(data)->unsafe(cvm, empty_ptr);
    const u64* data_unsafe_ptr2 = CALL(data)->unsafe(cvm, null_ptr);
    const u64* data_unsafe_ptr3 = CALL(data)->unsafe(cvm, string_ptr);
    const u64* data_unsafe_ptr4 = CALL(data)->unsafe(cvm, list_ptr);
    const u64* data_unsafe_ptr5 = CALL(data)->unsafe(cvm, none_ptr);

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

    u64 undefined_ptr = CALL(string)->copy(cvm, list_ptr);

    RX_ASSERT(undefined_ptr == 0);
    RX_ASSERT(last_matched_ptr1 != 0);
    RX_ASSERT(last_matched_ptr2 != 0);
    RX_ASSERT(last_matched_ptr3 != 0);
#ifndef USE_GC
    CALL(string)->free(cvm, last_matched_ptr1);
    CALL(string)->free(cvm, last_matched_ptr2);
    CALL(string)->free(cvm, last_matched_ptr3);
    CALL(string)->free(cvm, last_matched_ptr2);
    CALL(string)->free(cvm, last_matched_ptr3);
    CALL(string)->free(cvm, null_ptr);
    CALL(string)->free(cvm, zero_ptr);
    CALL(string)->free(cvm, string_ptr);
    CALL(string)->free(cvm, data_ptr3);
    CALL(string)->free(cvm, list_ptr);
    CALL(string)->free(cvm, empty_ptr);
    CALL(string)->free(cvm, data_ptr);
    CALL(string)->free(cvm, string_ptr);
    CALL(string)->free(cvm, data_ptr3);
    CALL(string)->free(cvm, empty_ptr);
    CALL(string)->free(cvm, data_ptr);
    CALL(data)->free(cvm, zero_ptr);
    CALL(data)->free(cvm, data_ptr);
    CALL(data)->free(cvm, data_ptr3);
    CALL(data)->free(cvm, empty_ptr);
    CALL(data)->free(cvm, null_ptr);
    CALL(data)->free(cvm, data_ptr);
    CALL(data)->free(cvm, list_ptr);
    CALL(data)->free(cvm, data_ptr);
    CALL(data)->free(cvm, data_ptr3);
    CALL(data)->free(cvm, none_ptr);
    CALL(stack)->free(cvm, zero_ptr);
    CALL(stack)->free(cvm, data_ptr);
    CALL(stack)->free(cvm, empty_ptr);
    CALL(stack)->free(cvm, null_ptr);
    CALL(stack)->free(cvm, data_ptr);
    CALL(stack)->free(cvm, list_ptr);
    CALL(stack)->free(cvm, data_ptr);
    CALL(stack)->free(cvm, list_ptr);
    CALL(stack)->free(cvm, none_ptr);
    CALL(string_pointer)->free(cvm, last_matched_ptr1);
    CALL(string_pointer)->free(cvm, last_matched_ptr2);
    CALL(string_pointer)->free(cvm, last_matched_ptr3);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_strcat_alloc_load, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 zero_ptr = CALL(string)->load(cvm, "\0");
    u64 char_ptr = CALL(string)->load(cvm, "/");
    CALL(string)->strcat(cvm, zero_ptr, char_ptr);
    RX_ASSERT(zero_ptr == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, char_ptr), "/") == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, zero_ptr);
    CALL(string)->free(cvm, char_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_strcat_load_alloc_alloc, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 char_ptr = CALL(string)->load(cvm, "/");
    u64 pattern_ptr = CALL(string)->load(cvm, "\0");
    u64 empty_ptr = CALL(string)->load(cvm, "\0");
    CALL(string)->strcat(cvm, pattern_ptr, char_ptr);
    CALL(string)->strcpy(cvm, empty_ptr, pattern_ptr);
    RX_ASSERT(pattern_ptr == 0);
    RX_ASSERT(empty_ptr == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, char_ptr), "/") == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, empty_ptr);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string)->free(cvm, char_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_strcat_load_load, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 char_ptr = CALL(string)->load(cvm, "/");
    u64 pattern_ptr = CALL(string)->load(cvm, "*");
    CALL(string)->strcat(cvm, pattern_ptr, char_ptr);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, pattern_ptr), "*/") == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, char_ptr), "/") == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string)->free(cvm, char_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_strcpy_load_alloc, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "/");
    u64 empty_ptr = CALL(string)->load(cvm, "\0");
    CALL(string)->strcpy(cvm, empty_ptr, pattern_ptr);
    RX_ASSERT(empty_ptr == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, pattern_ptr), "/") == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, empty_ptr);
    CALL(string)->free(cvm, pattern_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_strcpy_load_load, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 char_ptr = CALL(string)->load(cvm, "/input.txt");
    u64 pattern_ptr = CALL(string)->load(cvm, "*");
    CALL(string)->strcpy(cvm, pattern_ptr, char_ptr);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, pattern_ptr), "/input.txt") == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, char_ptr), "/input.txt") == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string)->free(cvm, char_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_env_getenv, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 ui_mode_ptr = CALL(string)->load(cvm, "ASDASD_UI_MODE");
    u64 file_path_ptr = CALL(env)->getenv(cvm, ui_mode_ptr);
    RX_ASSERT(file_path_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, ui_mode_ptr);
#endif
}

/* ---------------------------------------------------------------------- */
/* BEGIN SECTION: zero memory checks / uninitialized calls                */
/* ---------------------------------------------------------------------- */

/************/
/* data API */
/************/

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_data_alloc, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 value = CALL(data)->alloc(cvm, 0);
    RX_ASSERT(value == 0);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_data_alloc_0, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    u64 value = CALL(data)->alloc(cvm, 0);
    RX_ASSERT(value == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_data_free, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(data)->free(cvm, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_data_size, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    u64 value = CALL(data)->size(cvm, 0);
    RX_ASSERT(value == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_data_unsafe, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    void_ptr value = CALL(data)->unsafe(cvm, 0);
    RX_ASSERT(value == 0);
}

/************/
/* file API */
/************/

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_file_alloc, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    u64 value = CALL(file)->alloc(cvm, 0, 0);
    RX_ASSERT(value == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_load_file_alloc, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 file_path_ptr = CALL(string)->load(cvm, "data");
    u64 mode_ptr = CALL(string)->load(cvm, "rb");
    const_pointer_ptr const_ptr = CALL(virtual)->read(cvm, file_path_ptr);
    safe_void_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr->data;
    u8* data_ptr = safe_ptr.ptr;
    char* file_data = (char*)data_ptr;
    file_data[0] = 0;
    RX_ASSERT(data_ptr != 0);
    u64 value = CALL(file)->alloc(cvm, file_path_ptr, mode_ptr);
    RX_ASSERT(value == 0);
    CALL(string)->free(cvm, file_path_ptr);
    CALL(string)->free(cvm, mode_ptr);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_file_free, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(file)->free(cvm, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_file_data, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    u64 value = CALL(file)->data(cvm, 0);
    RX_ASSERT(value == 0);
}

/**************/
/* object API */
/**************/

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_object_alloc, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    u64 value = CALL(object)->alloc(cvm, 0);
    RX_ASSERT(value == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_object_free, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(object)->free(cvm, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_object_unsafe, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    void_ptr value = CALL(object)->unsafe(cvm, 0);
    RX_ASSERT(value == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_object_load, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    u64 value = CALL(object)->load(cvm, 0, 0);
    RX_ASSERT(value == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_object_size, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    u64 value = CALL(object)->size(cvm, 0);
    RX_ASSERT(value == 0);
}

/*************/
/* stack API */
/*************/

RX_TEST_CASE(tests_pointer_v1, test_vm_stack_alloc, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    u64 value = CALL(stack)->alloc(cvm);
    RX_ASSERT(value == 0);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_stack_free, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(stack)->free(cvm, 0);
    RX_ASSERT(0 != 1);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_stack_peek, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    u64 value = CALL(stack)->peek(cvm, 0);
    RX_ASSERT(value == 0);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_stack_peekn, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    u64 value = CALL(stack)->peekn(cvm, 0, 0);
    RX_ASSERT(value == 0);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_stack_pop, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    u64 value = CALL(stack)->pop(cvm, 0);
    RX_ASSERT(value == 0);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_stack_popn, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    u64 value = CALL(stack)->popn(cvm, 0, 0);
    RX_ASSERT(value == 0);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_stack_push, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(stack)->push(cvm, 0, 0);
    RX_ASSERT(0 != 1);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_stack_size, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    u64 value = CALL(stack)->size(cvm, 0);
    RX_ASSERT(value == 0);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_stack_release, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(stack)->release(cvm, 0);
    RX_ASSERT(0 != 1);
}

/**************/
/* string API */
/**************/

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_free, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(string)->free(cvm, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_copy, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(string)->copy(cvm, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_strcpy, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(string)->strcpy(cvm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_strcat, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(string)->strcat(cvm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_strrchr, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(string)->strrchr(cvm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_strchr, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(string)->strchr(cvm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_match, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(string)->match(cvm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_offset, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(string)->offset(cvm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_load, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(string)->load(cvm, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_put_char, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(string)->put_char(cvm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_unsafe, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(string)->unsafe(cvm, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_size, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    CALL(string)->size(cvm, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_size_0, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(string)->size(cvm, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_lessthan, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(string)->lessthan(cvm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_greaterthan, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(string)->greaterthan(cvm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_equals, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(string)->equals(cvm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_compare, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(string)->compare(cvm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_left, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(string)->left(cvm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_strncpy, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(string)->strncpy(cvm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_left_strncpy, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(string)->left_strncpy(cvm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_right, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(string)->right(cvm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_move_left, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(string)->move_left(cvm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_move_right, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(string)->move_right(cvm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_strcmp, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(string)->strcmp(cvm, 0, 0);
    RX_ASSERT(0 != 1);
}

/**********************/
/* string_pointer API */
/**********************/

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_pointer, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(string_pointer)->free(cvm, 0);
    RX_ASSERT(0 != 1);
}

/************/
/* user API */
/************/

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_user_alloc, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    u64 value = CALL(user)->alloc(cvm);
    RX_ASSERT(value == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_user_free, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(user)->free(cvm, 0);
    RX_ASSERT(0 != 1);
}

/**********/
/* env API */
/**********/

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_env_getenv, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    u64 value = CALL(env)->getenv(cvm, 0);
    RX_ASSERT(value == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_env_getcwd, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    u64 value = CALL(env)->getcwd(cvm);
    RX_ASSERT(value == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_env_putc, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(env)->puts(cvm, 0);
    RX_ASSERT(0 != 1);
}

/***************/
/* pointer API */
/***************/

RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_read_data_0_0, .fixture = test_fixture_pointer) {
    const_vm_ptr cvm = 0;
    const_pointer_ptr data_ptr = CALL(pointer)->read(cvm, 0, 0);
    RX_ASSERT(data_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_read_data_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    const_pointer_ptr data_ptr = CALL(pointer)->read(cvm, 1, 0);
    RX_ASSERT(data_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_read_data_error_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    const_pointer_ptr data_ptr = CALL(pointer)->read(cvm, 0, 0);
    RX_ASSERT(data_ptr == 0);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_alloc, .fixture = test_fixture_pointer) {
    const_vm_ptr cvm = 0;
    void_ptr data_ptr = 0;
    u64 address = CALL(pointer)->alloc(cvm, &data_ptr, 0, TYPE_STRING);
    RX_ASSERT(address == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_alloc_safe_0_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 address = CALL(pointer)->alloc(cvm, 0, 0, 0);
    RX_ASSERT(address == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_alloc_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    void_ptr data_ptr = 0;
    u64 address = CALL(pointer)->alloc(cvm, &data_ptr, 0, 0);
    RX_ASSERT(address == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_alloc_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    void_ptr data_ptr = 0;
    u64 address = CALL(pointer)->alloc(cvm, &data_ptr, 1, 0);
    RX_ASSERT(address == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_alloc_0_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    void_ptr data_ptr;
    u64 address = CALL(pointer)->alloc(cvm, &data_ptr, 0, 1);
    RX_ASSERT(address == 0);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_alloc_safe, .fixture = test_fixture_pointer) {
    const_vm_ptr cvm = 0;
    void_ptr data_ptr = 0;
    u64 address = CALL(pointer)->copy(cvm, &data_ptr, 0, 0, TYPE_STRING);
    RX_ASSERT(address == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_alloc_safe_0_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 address = CALL(pointer)->copy(cvm, 0, 0, 0, 0);
    RX_ASSERT(address == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_alloc_safe_0_0, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    void_ptr data_ptr = 0;
    u64 address = CALL(pointer)->copy(cvm, &data_ptr, 0, 0, 0);
    RX_ASSERT(address == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_alloc_safe_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    void_ptr data_ptr = 0;
    u64 address = CALL(pointer)->copy(cvm, &data_ptr, 1, 0, 0);
    RX_ASSERT(address == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_virtual_pointer_0_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    void_ptr data_ptr;
    u64 address = CALL(pointer)->copy(cvm, &data_ptr, 0, 0, 1);
    RX_ASSERT(address == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_release_data_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 result = CALL(pointer)->free(cvm, 0);
    RX_ASSERT(result == FALSE);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_release_data_error_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 result = CALL(pointer)->free(cvm, 1);
    RX_ASSERT(result == FALSE);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_copy_ch_2_1_02, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    const char* ch = "hello, world!";
    u64 address1 = CALL(pointer)->copy(cvm, ch, strlen(ch), 0, TYPE_OBJECT);
    u64 address2 = CALL(pointer)->copy(cvm, ch, 13, 1, TYPE_OBJECT);
    RX_ASSERT(address1 != 0);
    RX_ASSERT(address2 != 0);
    const_pointer_ptr const_ptr1 = CALL(pointer)->read(cvm, address1, TYPE_OBJECT);
    const_pointer_ptr const_ptr2 = CALL(pointer)->read(cvm, address2, TYPE_OBJECT);
    RX_ASSERT(const_ptr1 != 0);
    RX_ASSERT(const_ptr2 != 0);
    const char* data1 = const_ptr1->data;
    const char* data2 = const_ptr2->data;
    RX_ASSERT(data1 != 0);
    RX_ASSERT(data2[1] == 0);
#ifndef USE_GC
    CALL(object)->free(cvm, address1);
    CALL(object)->free(cvm, address2);
#endif
}

RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_alloc_safe, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    CALL(pointer)->copy(cvm, 0, 0, 0, 0);
    RX_ASSERT(0 != 1);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_alloc_safe_0_0_0_0, .fixture = test_fixture_pointer) {
    const_vm_ptr cvm = 0;
    CALL(pointer)->copy(cvm, 0, 0, 0, 0);
    RX_ASSERT(0 != 1);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_alloc_safe_ch_0_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    const char* ch = "hello, world!";
    CALL(pointer)->copy(cvm, ch, 0, 0, 0);
    RX_ASSERT(0 != 1);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_alloc_safe_ch_1_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    const char* ch = "hello, world!";
    u64 result = CALL(pointer)->copy(cvm, ch, 1, 0, 0);
    RX_ASSERT(result == 0);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_alloc_safe_ch_1_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    const char* ch = "hello, world!";
    u64 result = CALL(pointer)->copy(cvm, ch, 1, 1, 0);
    RX_ASSERT(result == 0);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_alloc_safe_ch_1_2_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    const char* ch = "hello, world!";
    u64 result = CALL(pointer)->copy(cvm, ch, 1, 2, 0);
    RX_ASSERT(result == 0);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_free, .fixture = test_fixture_pointer) {
    const_vm_ptr cvm = 0;
    CALL(vm)->release(cvm, 0);
    RX_ASSERT(0 != 1);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_release_type, .fixture = test_fixture_pointer) {
    const_vm_ptr cvm = 0;
    CALL(pointer)->free(cvm, 0);
    RX_ASSERT(0 != 1);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_alloc_safe_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    CALL(pointer)->copy(cvm, 0, 0, 0, 0);
    RX_ASSERT(0 != 1);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_release_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    CALL(vm)->release(cvm, 0);
    RX_ASSERT(0 != 1);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_read_safe_alloc_safe_size_offset, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    const char* src = "hello, world!";
    u64 address = CALL(string)->load(cvm, src);
    const_pointer_ptr const_ptr = CALL(virtual)->read(cvm, address);
    safe_void_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr->data;
    u8* data_ptr = safe_ptr.ptr;
    char* string_data = (char*)data_ptr;
    RX_ASSERT(string_data != 0);
#ifndef USE_GC
    CALL(string)->free(cvm, address);
#endif
}

/***************/
/* virtual API */
/***************/

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_virtual_init, .fixture = test_fixture) {
    const_vm_ptr cvm = CALL(system)->init(0);
    RX_ASSERT(cvm != 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_virtual_destroy, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(system)->destroy(cvm);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_virtual_alloc_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    CALL(virtual)->alloc(cvm, 1, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_virtual_alloc, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(virtual)->alloc(cvm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_virtual_free, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(virtual)->free(cvm, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_virtual_read, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(virtual)->type(cvm, 0);
    RX_ASSERT(0 != 1);
}
/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_virtual_read_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    CALL(virtual)->type(cvm, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_virtual_read_type, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(virtual)->read(cvm, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_virtual_read_type_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    CALL(virtual)->read(cvm, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_virtual_read_type_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    CALL(virtual)->read(cvm, 1);
    RX_ASSERT(0 != 1);
}

/* ---------------------------------------------------------------------- */
/* END SECTION: zero memory checks / uninitialized calls                  */
/* ---------------------------------------------------------------------- */

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_env_getenv_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 file_path_ptr = CALL(env)->getenv(cvm, 0);
    RX_ASSERT(file_path_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_env_getenv_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(cvm);
    u64 file_path_ptr = CALL(env)->getenv(cvm, list_ptr);
    RX_ASSERT(file_path_ptr == 0);
#ifndef USE_GC
    CALL(stack)->free(cvm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_env_getenv_string_0_byte, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 string_ptr = CALL(string)->load(cvm, "hello, world");
    CALL(string)->put_char(cvm, string_ptr, 0);
    u64 file_path_ptr = CALL(env)->getenv(cvm, string_ptr);
    RX_ASSERT(file_path_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, string_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_load_open_file_close_file, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 file_path_ptr = CALL(env)->getcwd(cvm);
    u64 file_name_ptr = CALL(string)->load(cvm, "data/file.txt");
    CALL(string)->strcat(cvm, file_path_ptr, file_name_ptr);
    CALL(string)->free(cvm, file_name_ptr);
    u64 mode_ptr = CALL(string)->load(cvm, "rb");
    u64 f_ptr = CALL(file)->alloc(cvm, file_path_ptr, mode_ptr);
    u64 data_ptr = CALL(file)->data(cvm, f_ptr);
    RX_ASSERT(f_ptr == 0);
    RX_ASSERT(data_ptr == 0);
#ifndef USE_GC
    CALL(file)->free(cvm, f_ptr);
    CALL(data)->free(cvm, data_ptr);
    CALL(string)->free(cvm, mode_ptr);
    CALL(string)->free(cvm, file_name_ptr);
    CALL(string)->free(cvm, file_path_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_offset_strcpy, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 name_ptr = CALL(string)->load(cvm, "name");
    u64 at_ptr = CALL(string)->load(cvm, "@");
    u64 domain_ptr = CALL(string)->load(cvm, "domain.org");
    CALL(string)->strcat(cvm, name_ptr, at_ptr);
    CALL(string)->strcat(cvm, name_ptr, domain_ptr);

    u64 path_ptr1 = CALL(string)->load(cvm, "name@domain.org");
    u64 path_ptr2 = CALL(string)->copy(cvm, at_ptr);

    u64 domain_name = CALL(string)->offset(cvm, path_ptr1, path_ptr2);
    CALL(string)->strcpy(cvm, name_ptr, domain_name);

    char* domain = CALL(string)->unsafe(cvm, domain_ptr);
    char* name = CALL(string)->unsafe(cvm, name_ptr);
    RX_ASSERT(strcmp(domain, "domain.org") == 0);
    RX_ASSERT(strcmp(name, "domain.org") == 0);

#ifndef USE_GC
    CALL(string)->free(cvm, domain_name);
    CALL(string)->free(cvm, name_ptr);
    CALL(string)->free(cvm, path_ptr1);
    CALL(string)->free(cvm, path_ptr2);
    CALL(string)->free(cvm, name_ptr);
    CALL(string)->free(cvm, domain_ptr);
    CALL(string)->free(cvm, at_ptr);
    CALL(string_pointer)->free(cvm, domain_name);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_offset_strcpy_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 name_ptr = CALL(string)->load(cvm, "name");
    u64 at_ptr = CALL(string)->load(cvm, "@");
    u64 domain_ptr = CALL(string)->load(cvm, "domain.org");
    CALL(string)->strcat(cvm, name_ptr, at_ptr);
    CALL(string)->strcat(cvm, name_ptr, domain_ptr);

    u64 path_ptr1 = CALL(string)->load(cvm, "name@domain.org");
    u64 path_ptr2 = CALL(string)->copy(cvm, at_ptr);

    u64 domain_name = CALL(string)->offset(cvm, path_ptr1, path_ptr2);
    CALL(string)->free(cvm, path_ptr1);
    CALL(string)->strcpy(cvm, name_ptr, domain_name);

    char* domain = CALL(string)->unsafe(cvm, domain_ptr);
    char* name = CALL(string)->unsafe(cvm, name_ptr);
    RX_ASSERT(strcmp(domain, "domain.org") == 0);
    RX_ASSERT(strcmp(name, "name@domain.org") == 0);

#ifndef USE_GC
    CALL(string)->free(cvm, domain_name);
    CALL(string)->free(cvm, name_ptr);
    CALL(string)->free(cvm, path_ptr2);
    CALL(string)->free(cvm, name_ptr);
    CALL(string)->free(cvm, domain_ptr);
    CALL(string)->free(cvm, at_ptr);
    CALL(string_pointer)->free(cvm, domain_name);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_print_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 printing_ptr = CALL(string)->load(cvm, "hello, world!");
    CALL(string)->free(cvm, printing_ptr);
    CALL(env)->puts(cvm, printing_ptr);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_sting_free_0, .fixture = test_fixture) {
    virtual_methods mock_virtual_methods_definitions;
    /*api */
    memcpy(&mock_virtual_methods_definitions, PRIVATE_API(virtual), sizeof(virtual_methods)); /* NOLINT: sizeof(virtual_methods*) */
    /* setup mocks */
    mock_virtual_methods_definitions.read = mock_virtual_read_zero;
    /* setup api endpoint */
    virtual_methods* mock_virtual_methods = &mock_virtual_methods_definitions;
    /* backup api calls */
    memcpy(&virtual_methods_ptr, &PRIVATE_API(virtual), sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    /* init */
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    /* prepare to mock api calls */
    memcpy(&PRIVATE_API(virtual), &mock_virtual_methods, sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    /* virtual_string->free fails in virtual->read call */
    CALL(string)->free(cvm, 0);
    /* restore api calls */
    memcpy(&PRIVATE_API(virtual), &virtual_methods_ptr, sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    /* cleanup */
    CALL(vm)->gc(cvm);
    /* destroy */
    CALL(vm)->destroy(cvm);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_sting_free_ptr_0, .fixture = test_fixture) {
    virtual_methods mock_virtual_methods_definitions;
    /*api */
    memcpy(&mock_virtual_methods_definitions, PRIVATE_API(virtual), sizeof(virtual_methods)); /* NOLINT: sizeof(virtual_methods*) */
    /* setup mocks */
    mock_virtual_methods_definitions.read = mock_virtual_read_zero;
    /* setup api endpoint */
    virtual_methods* mock_virtual_methods = &mock_virtual_methods_definitions;
    /* backup api calls */
    memcpy(&virtual_methods_ptr, &PRIVATE_API(virtual), sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    /* init */
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    /* prepare to mock api calls */
    memcpy(&PRIVATE_API(virtual), &mock_virtual_methods, sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    /* virtual_string->free fails in virtual->read call */
    CALL(string)->free(cvm, 0);
    /* restore api calls */
    memcpy(&PRIVATE_API(virtual), &virtual_methods_ptr, sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_print_string_pointer_virtual_read_type, .fixture = test_fixture) {
    virtual_methods mock_virtual_methods_definitions;
    /*api */
    memcpy(&mock_virtual_methods_definitions, PRIVATE_API(virtual), sizeof(virtual_methods)); /* NOLINT: sizeof(virtual_methods*) */
    /* setup mocks */
    /* setup api endpoint */
    virtual_methods* mock_virtual_methods = &mock_virtual_methods_definitions;
    /* backup api calls */
    memcpy(&virtual_methods_ptr, &PRIVATE_API(virtual), sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    /* init */
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 printing_ptr = CALL(string)->load(cvm, "hello, world!");
    u64 comma_ptr = CALL(string)->load(cvm, ",");
    u64 substring_index_ptr = CALL(string)->offset(cvm, printing_ptr, comma_ptr);
    CALL(env)->puts(cvm, substring_index_ptr);
    CALL(string)->free(cvm, printing_ptr);
    /* prepare to mock api calls */
    memcpy(&PRIVATE_API(virtual), &mock_virtual_methods, sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    CALL(string)->free(cvm, substring_index_ptr);
    /* restore api calls */
    memcpy(&PRIVATE_API(virtual), &virtual_methods_ptr, sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    CALL(string)->free(cvm, comma_ptr);
#ifndef USE_GC
    CALL(string_pointer)->free(cvm, substring_index_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_free_string_pointer_virtual_read_type, .fixture = test_fixture) {
    virtual_methods mock_virtual_methods_definitions;
    /*api */
    memcpy(&mock_virtual_methods_definitions, PRIVATE_API(virtual), sizeof(virtual_methods)); /* NOLINT: sizeof(virtual_methods*) */
    /* setup mocks */
    /* setup api endpoint */
    virtual_methods* mock_virtual_methods = &mock_virtual_methods_definitions;
    /* backup api calls */
    memcpy(&virtual_methods_ptr, &PRIVATE_API(virtual), sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    /* init */
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 printing_ptr = CALL(string)->load(cvm, "hello, world!");
    u64 comma_ptr = CALL(string)->load(cvm, ",");
    u64 space_ptr = CALL(string)->load(cvm, " ");
    u64 exclamation_ptr = CALL(string)->load(cvm, "!");
    u64 substring_index_ptr = CALL(string)->offset(cvm, printing_ptr, comma_ptr);
    u64 substring_space_ptr = CALL(string)->offset(cvm, substring_index_ptr, space_ptr);
    u64 substring_exclamation_ptr = CALL(string)->offset(cvm, substring_space_ptr, exclamation_ptr);
    CALL(env)->puts(cvm, substring_index_ptr);
    /* prepare to mock api calls */
    memcpy(&PRIVATE_API(virtual), &mock_virtual_methods, sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    CALL(string_pointer)->free(cvm, substring_exclamation_ptr);
    CALL(string)->free(cvm, substring_space_ptr);
    CALL(string_pointer)->free(cvm, substring_index_ptr);
    CALL(string)->free(cvm, printing_ptr);
    /* restore api calls */
    memcpy(&PRIVATE_API(virtual), &virtual_methods_ptr, sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    CALL(string)->free(cvm, comma_ptr);
    CALL(string)->free(cvm, comma_ptr);
    CALL(string)->free(cvm, space_ptr);
    CALL(string)->free(cvm, exclamation_ptr);
#ifndef USE_GC
    CALL(string_pointer)->free(cvm, substring_exclamation_ptr);
    CALL(string_pointer)->free(cvm, substring_space_ptr);
    CALL(string_pointer)->free(cvm, substring_index_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_print, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 printing_ptr = CALL(string)->load(cvm, "hello, world!");
    CALL(env)->puts(cvm, printing_ptr);
#ifndef USE_GC
    CALL(string)->free(cvm, printing_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_print_string_pointer, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 printing_ptr = CALL(string)->load(cvm, "hello, world!");
    u64 comma_ptr = CALL(string)->load(cvm, ",");
    u64 substring_index_ptr = CALL(string)->offset(cvm, printing_ptr, comma_ptr);
    CALL(env)->puts(cvm, substring_index_ptr);
#ifndef USE_GC
    CALL(string)->free(cvm, printing_ptr);
    CALL(string_pointer)->free(cvm, substring_index_ptr);
    CALL(string)->free(cvm, comma_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_print_string_pointer_copy, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    const char* substring_expected = " world!";
    u64 printing_ptr = CALL(string)->load(cvm, "hello, world!");
    u64 comma_ptr = CALL(string)->load(cvm, ",");
    u64 substring_index_ptr = CALL(string)->offset(cvm, printing_ptr, comma_ptr);
    u64 substring_ptr = CALL(string)->copy(cvm, substring_index_ptr);

    char* substring_actual = CALL(string)->unsafe(cvm, substring_ptr);
    RX_ASSERT(strcmp(substring_expected, substring_actual) == 0);

#ifndef USE_GC
    CALL(string)->free(cvm, printing_ptr);
    CALL(string)->free(cvm, comma_ptr);
    CALL(string_pointer)->free(cvm, substring_index_ptr);
    CALL(string)->free(cvm, substring_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_size, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(cvm);
    u64 size_actual = CALL(string)->size(cvm, list_ptr);
    RX_ASSERT(size_actual == 0);
#ifndef USE_GC
    CALL(stack)->free(cvm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_unsafe, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(cvm);
    char* ptr_actual = CALL(string)->unsafe(cvm, list_ptr);
    char* ptr_expected = 0;
    RX_ASSERT(ptr_expected == ptr_actual);
#ifndef USE_GC
    CALL(stack)->free(cvm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_offset, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 string_ptr = CALL(string)->load(cvm, "himem.sys");
    u64 list_ptr = CALL(stack)->alloc(cvm);
    u64 size_actual = CALL(string)->offset(cvm, string_ptr, list_ptr);
    u64 size_expected = 0;
    RX_ASSERT(size_expected == size_actual);
#ifndef USE_GC
    CALL(stack)->free(cvm, list_ptr);
    CALL(string)->free(cvm, string_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, teststring_pointer_unsafe, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 printing_ptr = CALL(string)->load(cvm, "hello, world!");
    u64 comma_ptr = CALL(string)->load(cvm, ",");
    u64 substring_index_ptr = CALL(string)->offset(cvm, printing_ptr, comma_ptr);

    char* expected_value = CALL(string)->unsafe(cvm, substring_index_ptr);
    const char* actual_value = " world!";
    RX_ASSERT(strcmp(expected_value, actual_value) == 0);

    CALL(string)->free(cvm, substring_index_ptr);
    CALL(env)->puts(cvm, substring_index_ptr);
#ifndef USE_GC
    CALL(string_pointer)->free(cvm, substring_index_ptr);
    CALL(string)->free(cvm, printing_ptr);
    CALL(string)->free(cvm, comma_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_size, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 printing_ptr = CALL(string)->load(cvm, "hello, world!");
    u64 comma_ptr = CALL(string)->load(cvm, ",");
    u64 substring_index_ptr = CALL(string)->offset(cvm, printing_ptr, comma_ptr);
    u64 substring_ptr = CALL(string)->copy(cvm, substring_index_ptr);

    u64 size_expected = strlen(" world!"); /* adds one 0 to termination byte */
    u64 size_actual = CALL(string)->size(cvm, substring_ptr);
    RX_ASSERT(size_expected == size_actual);

#ifndef USE_GC
    CALL(string)->free(cvm, printing_ptr);
    CALL(string)->free(cvm, comma_ptr);
    CALL(string_pointer)->free(cvm, substring_index_ptr);
    CALL(string)->free(cvm, substring_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_offset_subsearch, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 printing_ptr = CALL(string)->load(cvm, "hello, world!");
    u64 comma_ptr = CALL(string)->load(cvm, ",");
    u64 w_ptr = CALL(string)->load(cvm, "w");
    u64 substring_index_ptr1 = CALL(string)->offset(cvm, printing_ptr, comma_ptr);
    u64 substring_index_ptr2 = CALL(string)->offset(cvm, substring_index_ptr1, w_ptr);
    u64 substring_ptr = CALL(string)->copy(cvm, substring_index_ptr2);

    u64 size_expected = strlen("orld!"); /* adds one 0 to termination byte */
    u64 size_actual = CALL(string)->size(cvm, substring_ptr);
    RX_ASSERT(size_expected == size_actual);

#ifndef USE_GC
    CALL(string)->free(cvm, printing_ptr);
    CALL(string)->free(cvm, comma_ptr);
    CALL(string_pointer)->free(cvm, substring_index_ptr1);
    CALL(string_pointer)->free(cvm, substring_index_ptr2);
    CALL(string)->free(cvm, substring_ptr);
    CALL(string)->free(cvm, w_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_print_string_pointer_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 printing_ptr = CALL(string)->load(cvm, "hello, world!");
    u64 comma_ptr = CALL(string)->load(cvm, ",");
    u64 substring_index_ptr = CALL(string)->offset(cvm, printing_ptr, comma_ptr);
    CALL(env)->puts(cvm, substring_index_ptr);
#ifndef USE_GC
    CALL(string_pointer)->free(cvm, substring_index_ptr);
    CALL(string)->free(cvm, printing_ptr);
    CALL(string)->free(cvm, comma_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_offset_strcat, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 name_ptr = CALL(string)->load(cvm, "name");
    u64 at_ptr = CALL(string)->load(cvm, "@");
    CALL(string)->strcat(cvm, name_ptr, at_ptr);

    u64 path_ptr1 = CALL(string)->load(cvm, "name@domain.org");
    u64 path_ptr2 = CALL(string)->copy(cvm, at_ptr);

    u64 domain_name = CALL(string)->offset(cvm, path_ptr1, path_ptr2);
    CALL(string)->strcat(cvm, name_ptr, domain_name);

    char* domain = CALL(string)->unsafe(cvm, path_ptr1);
    char* name = CALL(string)->unsafe(cvm, name_ptr);
    u64 domain_len = strlen(domain);
    u64 name_len = strlen(name);
    RX_ASSERT(domain_len > 0);
    RX_ASSERT(name_len > 0);
    RX_ASSERT(domain_len == name_len);
    RX_ASSERT(strcmp(domain, name) == 0);

#ifndef USE_GC
    CALL(string_pointer)->free(cvm, domain_name);
    CALL(string)->free(cvm, path_ptr1);
    CALL(string)->free(cvm, path_ptr2);
    CALL(string)->free(cvm, at_ptr);
    CALL(string)->free(cvm, name_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_file_read_invalid_type, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
#ifndef USE_GC
    u64 list_ptr = CALL(stack)->alloc(cvm);
    u64 data_ptr = CALL(file)->data(cvm, list_ptr);
    CALL(data)->free(cvm, data_ptr);
    CALL(file)->free(cvm, list_ptr);
    CALL(stack)->free(cvm, list_ptr);
#else
    u64 list_ptr = CALL(stack)->alloc(cvm);
    CALL(file)->data(cvm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_file_read_error_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 data_ptr = CALL(file)->data(cvm, 0);
    RX_ASSERT(data_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_load_open_match_last_unsafe_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 file_path_ptr = CALL(env)->getcwd(cvm);
    u64 file_name_ptr = CALL(string)->load(cvm, "/all_english_words.txt/");
    CALL(string)->strcat(cvm, file_path_ptr, file_name_ptr);
    u64 pattern_ptr = CALL(string)->load(cvm, "//");
    u64 last_match_ptr = CALL(string)->offset(cvm, file_path_ptr, pattern_ptr);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string)->put_char(cvm, last_match_ptr, '\0');
    CALL(string)->free(cvm, last_match_ptr);
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
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_load_open_match_last_unsafe_free_unsuppported_calls, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 ptr1 = CALL(string)->load(cvm, "qwerty//u");
    u64 ptr2 = CALL(string)->load(cvm, "asdfghjkl");
    u64 pattern_ptr = CALL(string)->load(cvm, "/u");
    u64 last_match_ptr = CALL(string)->offset(cvm, ptr1, pattern_ptr);
    CALL(string)->strcat(cvm, last_match_ptr, ptr2);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string)->put_char(cvm, last_match_ptr, '\0');
#ifndef USE_GC
    CALL(string)->free(cvm, ptr1);
    CALL(string)->free(cvm, ptr2);
    CALL(string_pointer)->free(cvm, last_match_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_user, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
#ifndef USE_GC
    u64 ptr1 = CALL(user)->alloc(cvm);
    CALL(user)->free(cvm, ptr1);
#else
    CALL(user)->alloc(cvm);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_user_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 ptr1 = CALL(user)->alloc(cvm);
    CALL(user)->free(cvm, ptr1);
    CALL(user)->free(cvm, ptr1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_user_free_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    CALL(user)->free(cvm, 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_type_register_user_type, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    type_methods_definitions_type user_definition = {
        .type_id = TYPE_USER
    };
    CALL(type)->register_user_type(cvm, &user_definition);
    CALL(error)->clear();
    /* type->register_user_type fails in register_user_types */
    CALL(type)->register_user_type(cvm, &user_definition);
    RX_ASSERT(CALL(error)->count() == 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_load_open_file_unsafe_hashtable, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 file_path_ptr = CALL(env)->getcwd(cvm);
    u64 file_name_ptr = CALL(string)->load(cvm, "/data/all_english_words.txt");
    CALL(string)->strcat(cvm, file_path_ptr, file_name_ptr);
    CALL(string)->free(cvm, file_name_ptr);
    u64 mode_ptr = CALL(string)->load(cvm, "rb");
    u64 f_ptr = CALL(file)->alloc(cvm, file_path_ptr, mode_ptr);
    if (f_ptr != 0) {
        u64 data_ptr = CALL(file)->data(cvm, f_ptr);
        u64 list_ptr = CALL(stack)->alloc(cvm);
        CALL(file)->free(cvm, f_ptr);
        u8* file_data = CALL(data)->unsafe(cvm, data_ptr);
        for (int i = 0; i < 100; i++) {
            u8* tmp = file_data;
            while (*tmp != 0 && *tmp != '\n') {
                tmp++;
            }
            *tmp++ = '\0';
            u64 string_ptr = CALL(string)->load(cvm, (char*)file_data);
            CALL(stack)->push(cvm, list_ptr, string_ptr);
            CALL(env)->puts(cvm, string_ptr);
            file_data = tmp;
        }
#ifndef USE_GC
        u64 string_ptr = 0;
        while ((string_ptr = CALL(stack)->pop(cvm, list_ptr)) != 0) {
            CALL(string)->free(cvm, string_ptr);
        }
        CALL(stack)->free(cvm, list_ptr);
        CALL(data)->free(cvm, data_ptr);
#endif
    }
#ifndef USE_GC
    CALL(string)->free(cvm, mode_ptr);
    CALL(string)->free(cvm, file_name_ptr);
    CALL(string)->free(cvm, file_path_ptr);
#endif
}

RX_TEST_CASE(tests_pointer_v1, test_load_load_match_last, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 str_ptr = CALL(string)->load(cvm, "Hello, world!");
    u64 ch_ptr = CALL(string)->load(cvm, "z");
    u64 last_match_ptr = CALL(string)->offset(cvm, str_ptr, ch_ptr);
    RX_ASSERT(last_match_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, last_match_ptr);
    CALL(string)->free(cvm, str_ptr);
    CALL(string)->free(cvm, ch_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_load, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 address = CALL(string)->load(cvm, "asdf");
#ifndef USE_GC
    CALL(string)->free(cvm, address);
#endif
    RX_ASSERT(address != 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_init_string_load_9, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 ids[] = {
        CALL(string)->load(cvm, "0"),
        CALL(string)->load(cvm, "1"),
        CALL(string)->load(cvm, "2"),
        CALL(string)->load(cvm, "3"),
        CALL(string)->load(cvm, "4"),
        CALL(string)->load(cvm, "5"),
        CALL(string)->load(cvm, "6"),
        CALL(string)->load(cvm, "7"),
        CALL(string)->load(cvm, "8"),
        CALL(string)->load(cvm, "9")
    };
    const char* expected = "0123456789";
    for (int i = 0; i < 10; i++) {
        u64 string_ptr = ids[i];
        char* string = CALL(string)->unsafe(cvm, string_ptr);
        RX_ASSERT(string[0] == expected[i]);
    }
    for (int i = 0; i < 10; i++) {
        CALL(string)->free(cvm, ids[i]);
    }
}

/* test init */
RX_TEST_CASE(pointer_tests, test_vm_dump_0, .fixture = test_fixture_pointer) {
    CALL(error)->clear();
    CALL(vm)->dump(0);
    u64 error_count = CALL(error)->count();
    RX_ASSERT(error_count != 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_vm_dump_stack_0, .fixture = test_fixture_pointer) {
    CALL(error)->clear();
    CALL(vm)->dump_stack(0, 0);
    u64 error_count = CALL(error)->count();
    RX_ASSERT(error_count != 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_vm_dump_ref_0, .fixture = test_fixture_pointer) {
    CALL(error)->clear();
    CALL(vm)->dump_ref(0);
    u64 error_count = CALL(error)->count();
    RX_ASSERT(error_count != 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_vm_dump_ref_stack_0, .fixture = test_fixture_pointer) {
    CALL(error)->clear();
    CALL(vm)->dump_ref_stack(0, 0);
    u64 error_count = CALL(error)->count();
    RX_ASSERT(error_count != 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_strings, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 text_string_ptr0 = CALL(string)->load(cvm, "a");
    u64 text_string_ptr1 = CALL(string)->load(cvm, "a\nb");
    u64 text_string_ptr2 = CALL(string)->load(cvm, "ab\nabc\n");
    u64 text_string_ptr3 = CALL(string)->load(cvm, "adadadsadsadad\ndad\nadsaddasaddad\nsad\n");
    u64 text_string_ptr4 = CALL(string)->load(cvm, "ab\ndad\nabcd\nbcd\n");
    u64 text_string_ptr5 = CALL(string)->load(cvm, "ab\nc\nabc\nbcd\n");
    u64 text_string_ptr6 = CALL(string)->load(cvm, "abc\nabcd\nbcde\nabc\n");
    u64 text_string_ptr7 = CALL(string)->load(cvm, "abc\n\n");
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(vm)->dump(cvm);
    CALL(vm)->dump_ref(cvm);
#endif
    parse_text(cvm, 0);
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(vm)->dump(cvm);
    CALL(vm)->dump_ref(cvm);
#endif
    parse_text(cvm, text_string_ptr0);
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(vm)->dump(cvm);
    CALL(vm)->dump_ref(cvm);
#endif
    parse_text(cvm, text_string_ptr1);
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(vm)->dump(cvm);
    CALL(vm)->dump_ref(cvm);
#endif
    parse_text(cvm, text_string_ptr2);
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(vm)->dump(cvm);
    CALL(vm)->dump_ref(cvm);
#endif
    parse_text(cvm, text_string_ptr3);
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(vm)->dump(cvm);
    CALL(vm)->dump_ref(cvm);
#endif
    parse_text(cvm, text_string_ptr4);
    const_vm_ptr debug_cvm = CALL(vm)->init(8);
    u64 debug_text_string_ptr5 = CALL(string)->copy(cvm, text_string_ptr5);
    parse_text(debug_cvm, debug_text_string_ptr5);
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(vm)->dump(debug_cvm);
    CALL(vm)->dump_ref(debug_cvm);
#endif
    CALL(string)->free(cvm, debug_text_string_ptr5);
    CALL(vm)->destroy(debug_cvm);
    const_vm_ptr debug_cvm_6 = CALL(vm)->init(8);
    parse_text(debug_cvm_6, text_string_ptr6);
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(vm)->dump(debug_cvm_6);
    CALL(vm)->dump_ref(debug_cvm_6);
#endif
    CALL(vm)->destroy(debug_cvm_6);
    parse_text(cvm, text_string_ptr7);
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(vm)->dump(cvm);
    CALL(vm)->dump_ref(cvm);
#endif
    CALL(string)->free(cvm, text_string_ptr0);
    CALL(string)->free(cvm, text_string_ptr1);
    CALL(string)->free(cvm, text_string_ptr2);
    CALL(string)->free(cvm, text_string_ptr3);
    CALL(string)->free(cvm, text_string_ptr4);
    CALL(string)->free(cvm, text_string_ptr5);
    CALL(string)->free(cvm, text_string_ptr6);
    CALL(string)->free(cvm, text_string_ptr7);
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(vm)->dump(cvm);
    CALL(vm)->dump_ref(cvm);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_pointer_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    CALL(string_pointer)->free(cvm, 1);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_pointer_free_error_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    CALL(string_pointer)->free(cvm, 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_pointer_lessthan_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "b");
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "abba");
    u64 quantum_str_ptr2 = CALL(string)->offset(cvm, quantum_str_ptr1, pattern_ptr);
    u64 data_ptr = CALL(string)->lessthan(cvm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(data_ptr != 0);
#ifndef USE_GC
    CALL(string_pointer)->free(cvm, data_ptr);
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string_pointer)->free(cvm, quantum_str_ptr2);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string_pointer)->free(cvm, data_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 error_ptr = CALL(string)->lessthan(cvm, 0, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_0_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 error_ptr = CALL(string)->lessthan(cvm, 0, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr = CALL(string)->load(cvm, "");
    RX_ASSERT(quantum_str_ptr == 0);
    u64 error_ptr = CALL(string)->lessthan(cvm, 1, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_1_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->lessthan(cvm, 1, 1);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_a_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "b");
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "abba");
    u64 quantum_str_ptr2 = CALL(string)->offset(cvm, quantum_str_ptr1, pattern_ptr);
    u64 data_ptr = CALL(string)->lessthan(cvm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(data_ptr != 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string_pointer)->free(cvm, quantum_str_ptr2);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string_pointer)->free(cvm, data_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_b_a, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "b");
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "abba");
    u64 quantum_str_ptr2 = CALL(string)->offset(cvm, quantum_str_ptr1, pattern_ptr);
    u64 error_ptr = CALL(string)->lessthan(cvm, quantum_str_ptr2, quantum_str_ptr1);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string_pointer)->free(cvm, quantum_str_ptr2);
    CALL(string)->free(cvm, pattern_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_1_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->lessthan(cvm, 1, 2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_1_3, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->lessthan(cvm, 1, 2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_2_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->lessthan(cvm, 2, 1);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_a_string_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    CALL(string)->free(cvm, quantum_str_ptr2);
    u64 error_ptr = CALL(string)->lessthan(cvm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_string_free_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    CALL(string)->free(cvm, quantum_str_ptr1);
    u64 error_ptr = CALL(string)->lessthan(cvm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_string_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    u64 list_ptr2 = CALL(stack)->alloc(cvm);
    u64 error_ptr = CALL(string)->lessthan(cvm, quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(stack)->free(cvm, list_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_list_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr1 = CALL(stack)->alloc(cvm);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "a");
    u64 error_ptr = CALL(string)->lessthan(cvm, list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(string)->free(cvm, quantum_str_ptr2);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr2);
    CALL(stack)->free(cvm, list_ptr1);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 error_ptr = CALL(string)->greaterthan(cvm, 0, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_0_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 error_ptr = CALL(string)->greaterthan(cvm, 0, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr = CALL(string)->load(cvm, "");
    RX_ASSERT(quantum_str_ptr == 0);
    u64 error_ptr = CALL(string)->greaterthan(cvm, 1, 0);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_1_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->greaterthan(cvm, 1, 1);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_a_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "b");
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "abba");
    u64 quantum_str_ptr2 = CALL(string)->offset(cvm, quantum_str_ptr1, pattern_ptr);
    u64 error_ptr = CALL(string)->greaterthan(cvm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string_pointer)->free(cvm, quantum_str_ptr2);
    CALL(string)->free(cvm, pattern_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_b_a, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "b");
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "abba");
    u64 quantum_str_ptr2 = CALL(string)->offset(cvm, quantum_str_ptr1, pattern_ptr);
    u64 data_ptr = CALL(string)->greaterthan(cvm, quantum_str_ptr2, quantum_str_ptr1);
    RX_ASSERT(data_ptr != 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string_pointer)->free(cvm, quantum_str_ptr2);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string_pointer)->free(cvm, data_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_1_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->greaterthan(cvm, 1, 2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_1_3, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->greaterthan(cvm, 1, 3);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_2_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->greaterthan(cvm, 2, 1);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_a_string_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    CALL(string)->free(cvm, quantum_str_ptr2);
    u64 error_ptr = CALL(string)->greaterthan(cvm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_string_free_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    CALL(string)->free(cvm, quantum_str_ptr1);
    u64 error_ptr = CALL(string)->greaterthan(cvm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_string_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 list_ptr2 = CALL(stack)->alloc(cvm);
    u64 error_ptr = CALL(string)->greaterthan(cvm, quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(stack)->free(cvm, list_ptr2);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_list_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr1 = CALL(stack)->alloc(cvm);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "a");
    u64 error_ptr = CALL(string)->greaterthan(cvm, list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(string)->free(cvm, quantum_str_ptr2);
#ifndef USE_GC
    CALL(stack)->free(cvm, list_ptr1);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 error_ptr = CALL(string)->equals(cvm, 0, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_0_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 error_ptr = CALL(string)->equals(cvm, 0, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr = CALL(string)->load(cvm, "");
    RX_ASSERT(quantum_str_ptr == 0);
    u64 error_ptr = CALL(string)->equals(cvm, 1, 0);
    RX_ASSERT(error_ptr == 0);
    CALL(string)->free(cvm, quantum_str_ptr);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_1_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->equals(cvm, 1, 1);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(pointer)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_a_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "b");
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "abba");
    u64 quantum_str_ptr2 = CALL(string)->offset(cvm, quantum_str_ptr1, pattern_ptr);
    u64 error_ptr = CALL(string)->equals(cvm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string_pointer)->free(cvm, quantum_str_ptr2);
    CALL(string)->free(cvm, pattern_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_b_a, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "b");
    u64 source_ptr = CALL(string)->load(cvm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(cvm, source_ptr, pattern_ptr);
    u64 quantum_str_ptr2 = CALL(string)->offset(cvm, source_ptr, pattern_ptr);
    u64 data_ptr = CALL(string)->equals(cvm, quantum_str_ptr2, quantum_str_ptr1);
    RX_ASSERT(data_ptr != 0);
#ifndef USE_GC
    CALL(string_pointer)->free(cvm, quantum_str_ptr1);
    CALL(string_pointer)->free(cvm, quantum_str_ptr2);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string_pointer)->free(cvm, data_ptr);
    CALL(string)->free(cvm, source_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_1_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->equals(cvm, 1, 2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_pointer_equals, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "b");
    u64 string_str = CALL(string)->load(cvm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(cvm, string_str, pattern_ptr);
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->offset(cvm, string_str, pattern_ptr);
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 value = CALL(string)->equals(cvm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(value == 1);
#ifndef USE_GC
    CALL(string_pointer)->free(cvm, quantum_str_ptr1);
    CALL(string_pointer)->free(cvm, quantum_str_ptr2);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string)->free(cvm, string_str);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_1_3, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->equals(cvm, 1, 3);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_2_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->equals(cvm, 2, 1);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_a_string_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    CALL(string)->free(cvm, quantum_str_ptr2);
    u64 error_ptr = CALL(string)->equals(cvm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_string_free_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    CALL(string)->free(cvm, quantum_str_ptr1);
    u64 error_ptr = CALL(string)->equals(cvm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_string_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    u64 list_ptr2 = CALL(stack)->alloc(cvm);
    u64 error_ptr = CALL(string)->equals(cvm, quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(stack)->free(cvm, list_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_list_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr1 = CALL(stack)->alloc(cvm);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "a");
    u64 error_ptr = CALL(string)->equals(cvm, list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr2);
    CALL(stack)->free(cvm, list_ptr1);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 error_ptr = CALL(string)->compare(cvm, 0, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_0_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 error_ptr = CALL(string)->compare(cvm, 0, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr = CALL(string)->load(cvm, "");
    RX_ASSERT(quantum_str_ptr == 0);
    u64 error_ptr = CALL(string)->compare(cvm, 1, 0);
    RX_ASSERT(error_ptr == 0);
    CALL(string)->free(cvm, quantum_str_ptr);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_1_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->compare(cvm, 1, 1);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_a_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "b");
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "abba");
    u64 quantum_str_ptr2 = CALL(string)->offset(cvm, quantum_str_ptr1, pattern_ptr);
    u64 data_ptr = CALL(string)->compare(cvm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(data_ptr != 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string_pointer)->free(cvm, quantum_str_ptr2);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string_pointer)->free(cvm, data_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_b_a, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "b");
    u64 source_ptr = CALL(string)->load(cvm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(cvm, source_ptr, pattern_ptr);
    u64 quantum_str_ptr2 = CALL(string)->offset(cvm, source_ptr, pattern_ptr);
    u64 data_ptr = CALL(string)->compare(cvm, quantum_str_ptr2, quantum_str_ptr1);
    RX_ASSERT(data_ptr != 0);
#ifndef USE_GC
    CALL(string_pointer)->free(cvm, quantum_str_ptr1);
    CALL(string_pointer)->free(cvm, quantum_str_ptr2);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string_pointer)->free(cvm, data_ptr);
    CALL(string)->free(cvm, source_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_1_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->compare(cvm, 1, 2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_1_3, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->compare(cvm, 1, 3);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_2_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->compare(cvm, 2, 1);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_a_string_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    CALL(string)->free(cvm, quantum_str_ptr2);
    u64 error_ptr = CALL(string)->compare(cvm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_string_free_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    CALL(string)->free(cvm, quantum_str_ptr1);
    u64 error_ptr = CALL(string)->compare(cvm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_string_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    u64 list_ptr2 = CALL(stack)->alloc(cvm);
    u64 error_ptr = CALL(string)->compare(cvm, quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(stack)->free(cvm, list_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_list_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr1 = CALL(stack)->alloc(cvm);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "a");
    u64 error_ptr = CALL(string)->compare(cvm, list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr2);
    CALL(stack)->free(cvm, list_ptr1);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 error_ptr = CALL(string)->strcmp(cvm, 0, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_0_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 error_ptr = CALL(string)->strcmp(cvm, 0, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr = CALL(string)->load(cvm, "");
    RX_ASSERT(quantum_str_ptr == 0);
    u64 error_ptr = CALL(string)->strcmp(cvm, 1, 0);
    RX_ASSERT(error_ptr == 0);
    CALL(string)->free(cvm, quantum_str_ptr);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_1_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->strcmp(cvm, 1, 1);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_a_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "b");
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "abba");
    u64 quantum_str_ptr2 = CALL(string)->offset(cvm, quantum_str_ptr1, pattern_ptr);
    u64 data_ptr = CALL(string)->strcmp(cvm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(data_ptr != 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string_pointer)->free(cvm, quantum_str_ptr2);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string_pointer)->free(cvm, data_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_b_a, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "b");
    u64 source_ptr = CALL(string)->load(cvm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(cvm, source_ptr, pattern_ptr);
    u64 quantum_str_ptr2 = CALL(string)->offset(cvm, source_ptr, pattern_ptr);
    u64 data_ptr = CALL(string)->strcmp(cvm, quantum_str_ptr2, quantum_str_ptr1);
    RX_ASSERT(data_ptr != 0);
#ifndef USE_GC
    CALL(string_pointer)->free(cvm, quantum_str_ptr1);
    CALL(string_pointer)->free(cvm, quantum_str_ptr2);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string)->free(cvm, data_ptr);
    CALL(string)->free(cvm, source_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_1_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->strcmp(cvm, 1, 2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_1_3, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->strcmp(cvm, 1, 3);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_2_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->strcmp(cvm, 2, 1);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_a_string_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    CALL(string)->free(cvm, quantum_str_ptr2);
    u64 error_ptr = CALL(string)->strcmp(cvm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_string_free_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    CALL(string)->free(cvm, quantum_str_ptr1);
    u64 error_ptr = CALL(string)->strcmp(cvm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_string_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    u64 list_ptr2 = CALL(stack)->alloc(cvm);
    u64 error_ptr = CALL(string)->strcmp(cvm, quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(stack)->free(cvm, list_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_list_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr1 = CALL(stack)->alloc(cvm);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "a");
    u64 error_ptr = CALL(string)->strcmp(cvm, list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr2);
    CALL(stack)->free(cvm, list_ptr1);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 error_ptr = CALL(string)->move_right(cvm, 0, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_0_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 error_ptr = CALL(string)->move_right(cvm, 0, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr = CALL(string)->load(cvm, "");
    RX_ASSERT(quantum_str_ptr == 0);
    u64 error_ptr = CALL(string)->move_right(cvm, 1, 0);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_1_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->move_right(cvm, 1, 1);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_a_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "abba");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 error_ptr = CALL(string)->move_right(cvm, quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_b_a, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "a");
    u64 source_ptr = CALL(string)->load(cvm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(cvm, source_ptr, pattern_ptr);
    u64 error_ptr = CALL(string)->move_right(cvm, quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string_pointer)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string)->free(cvm, source_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_a_b_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "b");
    u64 source_ptr = CALL(string)->load(cvm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(cvm, source_ptr, pattern_ptr);
    u64 data_ptr = CALL(string)->move_right(cvm, quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
#ifndef USE_GC
    CALL(string_pointer)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string)->free(cvm, source_ptr);
    CALL(string_pointer)->free(cvm, data_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_a_b_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "a");
    u64 source_ptr = CALL(string)->load(cvm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(cvm, source_ptr, pattern_ptr);
    u64 error_ptr = CALL(string)->move_right(cvm, quantum_str_ptr1, 1);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, quantum_str_ptr1), "") == 0);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string_pointer)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string)->free(cvm, source_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_a_b_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "b");
    u64 source_ptr = CALL(string)->load(cvm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(cvm, source_ptr, pattern_ptr);
    CALL(string)->free(cvm, source_ptr);
    u64 error_ptr = CALL(string)->move_right(cvm, quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string_pointer)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string)->free(cvm, source_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_1_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->move_right(cvm, 1, 2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_1_3, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->move_right(cvm, 1, 3);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_2_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->move_right(cvm, 2, 1);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_a_string_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    CALL(string)->free(cvm, quantum_str_ptr2);
    u64 error_ptr = CALL(string)->move_right(cvm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_string_free_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    CALL(string)->free(cvm, quantum_str_ptr1);
    u64 error_ptr = CALL(string)->move_right(cvm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_string_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    u64 list_ptr2 = CALL(stack)->alloc(cvm);
    u64 error_ptr = CALL(string)->move_right(cvm, quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(string)->free(cvm, quantum_str_ptr1);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(stack)->free(cvm, list_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_list_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr1 = CALL(stack)->alloc(cvm);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "a");
    u64 error_ptr = CALL(string)->move_right(cvm, list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(stack)->free(cvm, list_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 error_ptr = CALL(string)->right(cvm, 0, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_0_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 error_ptr = CALL(string)->right(cvm, 0, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr = CALL(string)->load(cvm, "");
    RX_ASSERT(quantum_str_ptr == 0);
    u64 error_ptr = CALL(string)->right(cvm, 1, 0);
    RX_ASSERT(error_ptr == 0);
    CALL(string)->free(cvm, quantum_str_ptr);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_1_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 data_ptr = CALL(string)->right(cvm, quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, data_ptr), "") == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
    CALL(string_pointer)->free(cvm, data_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_a_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "abba");
    u64 error_ptr = CALL(string)->right(cvm, quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr != 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string_pointer)->free(cvm, error_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_b_a, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "a");
    u64 source_ptr = CALL(string)->load(cvm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(cvm, source_ptr, pattern_ptr);
    u64 error_ptr = CALL(string)->right(cvm, quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string_pointer)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string)->free(cvm, source_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_a_b_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "b");
    u64 source_ptr = CALL(string)->load(cvm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(cvm, source_ptr, pattern_ptr);
    u64 data_ptr = CALL(string)->right(cvm, quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
#ifndef USE_GC
    CALL(string_pointer)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string)->free(cvm, source_ptr);
    CALL(string_pointer)->free(cvm, data_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_a_b_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "a");
    u64 source_ptr = CALL(string)->load(cvm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(cvm, source_ptr, pattern_ptr);
    u64 error_ptr = CALL(string)->right(cvm, quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, quantum_str_ptr1), "") == 0);
#ifndef USE_GC
    CALL(string_pointer)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string)->free(cvm, source_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_a_b_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "b");
    u64 source_ptr = CALL(string)->load(cvm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(cvm, source_ptr, pattern_ptr);
    CALL(string)->free(cvm, source_ptr);
    u64 error_ptr = CALL(string)->right(cvm, quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string_pointer)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string)->free(cvm, source_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_1_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->right(cvm, 1, 2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_1_3, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->right(cvm, 1, 3);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_2_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "");
    u64 error_ptr = CALL(string)->right(cvm, quantum_str_ptr2, 1);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_a_string_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    CALL(string)->free(cvm, quantum_str_ptr2);
    u64 error_ptr = CALL(string)->right(cvm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_string_free_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    CALL(string)->free(cvm, quantum_str_ptr1);
    u64 error_ptr = CALL(string)->right(cvm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_string_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 list_ptr2 = CALL(stack)->alloc(cvm);
    u64 error_ptr = CALL(string)->right(cvm, quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(stack)->free(cvm, list_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_list_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr1 = CALL(stack)->alloc(cvm);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "a");
    u64 error_ptr = CALL(string)->right(cvm, list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(stack)->free(cvm, list_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 error_ptr = CALL(string)->move_left(cvm, 0, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_0_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 error_ptr = CALL(string)->move_left(cvm, 0, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr = CALL(string)->load(cvm, "");
    RX_ASSERT(quantum_str_ptr == 0);
    u64 error_ptr = CALL(string)->move_left(cvm, 1, 0);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_1_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->move_left(cvm, 1, 1);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_a_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "b");
    u64 source_ptr = CALL(string)->load(cvm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(cvm, source_ptr, pattern_ptr);
    u64 error_ptr = CALL(string)->move_left(cvm, quantum_str_ptr1, 4);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string_pointer)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string)->free(cvm, source_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_b_a, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "a");
    u64 source_ptr = CALL(string)->load(cvm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(cvm, source_ptr, pattern_ptr);
    u64 data_ptr = CALL(string)->move_left(cvm, quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
#ifndef USE_GC
    CALL(string_pointer)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string)->free(cvm, source_ptr);
    CALL(string_pointer)->free(cvm, data_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_a_b_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "b");
    u64 source_ptr = CALL(string)->load(cvm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(cvm, source_ptr, pattern_ptr);
    u64 data_ptr = CALL(string)->move_left(cvm, quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
#ifndef USE_GC
    CALL(string_pointer)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string)->free(cvm, source_ptr);
    CALL(string_pointer)->free(cvm, data_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_a_b_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "a");
    u64 source_ptr = CALL(string)->load(cvm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(cvm, source_ptr, pattern_ptr);
    u64 data_ptr = CALL(string)->move_left(cvm, quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, quantum_str_ptr1), "a") == 0);
#ifndef USE_GC
    CALL(string_pointer)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string)->free(cvm, source_ptr);
    CALL(string_pointer)->free(cvm, data_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_a_b_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "b");
    u64 source_ptr = CALL(string)->load(cvm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(cvm, source_ptr, pattern_ptr);
    CALL(string)->free(cvm, source_ptr);
    u64 error_ptr = CALL(string)->move_left(cvm, quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string_pointer)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string)->free(cvm, source_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_1_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->move_left(cvm, 1, 2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}
/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_1_3, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->move_left(cvm, 1, 3);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_2_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->move_left(cvm, 2, 1);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_a_string_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    CALL(string)->free(cvm, quantum_str_ptr2);
    u64 error_ptr = CALL(string)->move_left(cvm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_string_free_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    CALL(string)->free(cvm, quantum_str_ptr1);
    u64 error_ptr = CALL(string)->move_left(cvm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_string_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    u64 list_ptr2 = CALL(stack)->alloc(cvm);
    u64 error_ptr = CALL(string)->move_left(cvm, quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(stack)->free(cvm, list_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_list_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr1 = CALL(stack)->alloc(cvm);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "a");
    u64 error_ptr = CALL(string)->move_left(cvm, list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr2);
    CALL(stack)->free(cvm, list_ptr1);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 error_ptr = CALL(string)->left_strncpy(cvm, 0, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_0_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 error_ptr = CALL(string)->left_strncpy(cvm, 0, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr = CALL(string)->load(cvm, "");
    RX_ASSERT(quantum_str_ptr == 0);
    u64 error_ptr = CALL(string)->left_strncpy(cvm, 1, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_1_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->left_strncpy(cvm, 1, 1);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_a_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "b");
    u64 source_ptr = CALL(string)->load(cvm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(cvm, source_ptr, pattern_ptr);
    u64 error_ptr = CALL(string)->left_strncpy(cvm, quantum_str_ptr1, 4);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string_pointer)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string)->free(cvm, source_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_b_a, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "c");
    u64 source_ptr = CALL(string)->load(cvm, "abbc");
    u64 quantum_str_ptr1 = CALL(string)->offset(cvm, source_ptr, pattern_ptr);
    u64 data_ptr = CALL(string)->left_strncpy(cvm, quantum_str_ptr1, 3);
    RX_ASSERT(quantum_str_ptr1 != 0);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, quantum_str_ptr1), "") == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, data_ptr), "bbc") == 0);
#ifndef USE_GC
    CALL(string_pointer)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string)->free(cvm, source_ptr);
    CALL(string)->free(cvm, data_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_a_b_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "b");
    u64 source_ptr = CALL(string)->load(cvm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(cvm, source_ptr, pattern_ptr);
    u64 data_ptr = CALL(string)->left_strncpy(cvm, quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
#ifndef USE_GC
    CALL(string_pointer)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string)->free(cvm, source_ptr);
    CALL(string)->free(cvm, data_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_a_b_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "a");
    u64 source_ptr = CALL(string)->load(cvm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(cvm, source_ptr, pattern_ptr);
    u64 data_ptr = CALL(string)->left_strncpy(cvm, quantum_str_ptr1, 1);
    RX_ASSERT(quantum_str_ptr1 != 0);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, quantum_str_ptr1), "") == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, data_ptr), "a") == 0);
#ifndef USE_GC
    CALL(string_pointer)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string)->free(cvm, source_ptr);
    CALL(string)->free(cvm, data_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_a_b_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "b");
    u64 source_ptr = CALL(string)->load(cvm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(cvm, source_ptr, pattern_ptr);
    CALL(string)->free(cvm, source_ptr);
    u64 error_ptr = CALL(string)->left_strncpy(cvm, quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
    CALL(string_pointer)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string)->free(cvm, source_ptr);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_1_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->left_strncpy(cvm, 1, 2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}
/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_1_3, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->left_strncpy(cvm, 1, 3);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_2_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->left_strncpy(cvm, 2, 1);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_a_string_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    CALL(string)->free(cvm, quantum_str_ptr2);
    u64 error_ptr = CALL(string)->left_strncpy(cvm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_string_free_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    CALL(string)->free(cvm, quantum_str_ptr1);
    u64 error_ptr = CALL(string)->left_strncpy(cvm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_string_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 list_ptr2 = CALL(stack)->alloc(cvm);
    u64 error_ptr = CALL(string)->left_strncpy(cvm, quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(stack)->free(cvm, list_ptr2);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_list_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr1 = CALL(stack)->alloc(cvm);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->left_strncpy(cvm, list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr2);
    CALL(stack)->free(cvm, list_ptr1);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 error_ptr = CALL(string)->left(cvm, 0, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_0_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 error_ptr = CALL(string)->left(cvm, 0, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr = CALL(string)->load(cvm, "");
    RX_ASSERT(quantum_str_ptr == 0);
    u64 error_ptr = CALL(string)->left(cvm, 1, 0);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_1_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->left(cvm, 1, 1);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_a_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "b");
    u64 source_ptr = CALL(string)->load(cvm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(cvm, source_ptr, pattern_ptr);
    u64 error_ptr = CALL(string)->left(cvm, quantum_str_ptr1, 4);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string_pointer)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string)->free(cvm, source_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_b_a, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "c");
    u64 source_ptr = CALL(string)->load(cvm, "abbc");
    u64 quantum_str_ptr1 = CALL(string)->offset(cvm, source_ptr, pattern_ptr);
    u64 data_ptr = CALL(string)->left(cvm, quantum_str_ptr1, 3);
    RX_ASSERT(quantum_str_ptr1 != 0);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, quantum_str_ptr1), "") == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, data_ptr), "bbc") == 0);
#ifndef USE_GC
    CALL(string_pointer)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string)->free(cvm, source_ptr);
    CALL(string_pointer)->free(cvm, data_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_a_b_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "b");
    u64 source_ptr = CALL(string)->load(cvm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(cvm, source_ptr, pattern_ptr);
    u64 data_ptr = CALL(string)->left(cvm, quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
#ifndef USE_GC
    CALL(string_pointer)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string)->free(cvm, source_ptr);
    CALL(string_pointer)->free(cvm, data_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_a_b_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "a");
    u64 source_ptr = CALL(string)->load(cvm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(cvm, source_ptr, pattern_ptr);
    u64 data_ptr = CALL(string)->left(cvm, quantum_str_ptr1, 1);
    RX_ASSERT(quantum_str_ptr1 != 0);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, quantum_str_ptr1), "") == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, data_ptr), "a") == 0);
#ifndef USE_GC
    CALL(string_pointer)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string)->free(cvm, source_ptr);
    CALL(string_pointer)->free(cvm, data_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_a_b_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "b");
    u64 source_ptr = CALL(string)->load(cvm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(cvm, source_ptr, pattern_ptr);
    CALL(string)->free(cvm, source_ptr);
    u64 error_ptr = CALL(string)->left(cvm, quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string_pointer)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string)->free(cvm, source_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_1_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->left(cvm, 1, 2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_1_3, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->left(cvm, 1, 3);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_2_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->left(cvm, 2, 1);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_a_string_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    CALL(string)->free(cvm, quantum_str_ptr2);
    u64 error_ptr = CALL(string)->left(cvm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
    CALL(string)->free(cvm, quantum_str_ptr2);
    CALL(vm)->gc(cvm);
    CALL(string)->free(cvm, quantum_str_ptr2);
    CALL(vm)->destroy(cvm);
    CALL(string)->free(cvm, quantum_str_ptr2);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_string_free_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    CALL(string)->free(cvm, quantum_str_ptr1);
    u64 error_ptr = CALL(string)->left(cvm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_string_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    u64 list_ptr2 = CALL(stack)->alloc(cvm);
    u64 error_ptr = CALL(string)->left(cvm, quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(stack)->free(cvm, list_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_list_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr1 = CALL(stack)->alloc(cvm);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "a");
    u64 error_ptr = CALL(string)->left(cvm, list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(stack)->free(cvm, list_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 error_ptr = CALL(string)->strncpy(cvm, 0, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_0_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 error_ptr = CALL(string)->strncpy(cvm, 0, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr = CALL(string)->load(cvm, "");
    RX_ASSERT(quantum_str_ptr == 0);
    u64 error_ptr = CALL(string)->strncpy(cvm, 1, 0);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_1_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 quantum_str_ptr3 = CALL(string)->strncpy(cvm, 1, 1);
    RX_ASSERT(quantum_str_ptr3 == 3);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
    CALL(string)->free(cvm, quantum_str_ptr3);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_a_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "b");
    u64 source_ptr = CALL(string)->load(cvm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(cvm, source_ptr, pattern_ptr);
    u64 error_ptr = CALL(string)->strncpy(cvm, quantum_str_ptr1, 2);
    RX_ASSERT(error_ptr == 0);
    u64 data_ptr = CALL(string)->strncpy(cvm, quantum_str_ptr1, 1);
    u64 str_ptr1 = CALL(string)->offset(cvm, source_ptr, data_ptr);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(str_ptr1 != 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, str_ptr1), "") == 0);
#ifndef USE_GC
    CALL(string_pointer)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string)->free(cvm, source_ptr);
    CALL(string)->free(cvm, data_ptr);
    CALL(string_pointer)->free(cvm, str_ptr1);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_b_a, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "b");
    u64 source_ptr = CALL(string)->load(cvm, "abcd");
    u64 quantum_str_ptr1 = CALL(string)->offset(cvm, source_ptr, pattern_ptr);
    u64 data_ptr = CALL(string)->strncpy(cvm, quantum_str_ptr1, 2);
    RX_ASSERT(data_ptr != 0);
    CALL(string_pointer)->free(cvm, quantum_str_ptr1);
#ifndef USE_GC
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string)->free(cvm, source_ptr);
    CALL(string)->free(cvm, data_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_a_b_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "b");
    u64 source_ptr = CALL(string)->load(cvm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(cvm, source_ptr, pattern_ptr);
    u64 data_ptr = CALL(string)->strncpy(cvm, quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
    CALL(string_pointer)->free(cvm, quantum_str_ptr1);
#ifndef USE_GC
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string)->free(cvm, source_ptr);
    CALL(string)->free(cvm, data_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_a_b_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "a");
    u64 source_ptr = CALL(string)->load(cvm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(cvm, source_ptr, pattern_ptr);
    u64 data_ptr = CALL(string)->strncpy(cvm, quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr == 0);
    RX_ASSERT(quantum_str_ptr1 != 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(cvm, quantum_str_ptr1), "") == 0);
#ifndef USE_GC
    CALL(string_pointer)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, source_ptr);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string)->free(cvm, data_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_a_b_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "b");
    u64 source_ptr = CALL(string)->load(cvm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(cvm, source_ptr, pattern_ptr);
    CALL(string)->free(cvm, source_ptr);
    u64 error_ptr = CALL(string)->strncpy(cvm, quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string_pointer)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string)->free(cvm, source_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_a_b_2_overflow, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(cvm, "h");
    u64 source_ptr = CALL(string)->load(cvm, "abcdefgh");
    u64 quantum_str_ptr1 = CALL(string)->offset(cvm, source_ptr, pattern_ptr);
    u64 error_ptr = CALL(string)->strncpy(cvm, quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string_pointer)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, pattern_ptr);
    CALL(string)->free(cvm, source_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_1_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->strncpy(cvm, 1, 2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_1_3, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->strncpy(cvm, 1, 3);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_2_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 data_ptr = CALL(string)->strncpy(cvm, 2, 1);
    RX_ASSERT(data_ptr != 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
    CALL(string)->free(cvm, data_ptr);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_a_string_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    CALL(string)->free(cvm, quantum_str_ptr2);
    u64 error_ptr = CALL(string)->strncpy(cvm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_string_free_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    CALL(string)->free(cvm, quantum_str_ptr1);
    u64 error_ptr = CALL(string)->strncpy(cvm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(string)->free(cvm, quantum_str_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_string_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 list_ptr2 = CALL(stack)->alloc(cvm);
    u64 error_ptr = CALL(string)->strncpy(cvm, quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr1);
    CALL(stack)->free(cvm, list_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_list_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 list_ptr1 = CALL(stack)->alloc(cvm);
    u64 quantum_str_ptr2 = CALL(string)->load(cvm, "a");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->strncpy(cvm, list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(cvm, quantum_str_ptr2);
    CALL(stack)->free(cvm, list_ptr1);
#endif
}

static void parse_text(const_vm_ptr cvm, u64 text_string_ptr) {
    u64 gc_ptr = CALL(stack)->alloc(cvm);
    u64 text_size = CALL(string)->size(cvm, text_string_ptr);
    if (text_string_ptr == 0) {
        CALL(stack)->free(cvm, gc_ptr);
        return;
    }
    u64 stack_ptr1 = CALL(stack)->alloc(cvm);
    char* text = CALL(string)->unsafe(cvm, text_string_ptr);
    char* tmp = text;
    while (text != 0 && *tmp != 0 && text_size > 0) {
        while (*tmp != 0 && *tmp != '\n' && text_size > 0) {
            tmp++;
            text_size--;
        }
        if (text_size == 0) {
            u64 string_ptr = 0;
            while ((string_ptr = CALL(stack)->pop(cvm, stack_ptr1)) != 0) {
                CALL(string)->free(cvm, string_ptr);
            }
            CALL(stack)->free(cvm, stack_ptr1);
            CALL(stack)->free(cvm, gc_ptr);
            return;
        }
        *tmp++ = '\0';
        text_size--;
        u64 string_ptr = CALL(string)->load(cvm, text);
        CALL(stack)->push(cvm, stack_ptr1, string_ptr);
        text = tmp;
    }
    u64 data_ptr = 0;
    u64 stack_ptr2 = CALL(stack)->alloc(cvm); // this is leaking MP
    while ((data_ptr = CALL(stack)->pop(cvm, stack_ptr1)) != 0) {
        CALL(stack)->push(cvm, stack_ptr2, data_ptr);
    }
    CALL(stack)->free(cvm, stack_ptr1);
    CALL(stack)->push(cvm, gc_ptr, stack_ptr2);
    u64 quit = 0;
    while (quit == 0) {
        u64 string_ptr = CALL(stack)->pop(cvm, stack_ptr2);
        if (CALL(string)->size(cvm, string_ptr) == 0) {
            quit = 1;
            CALL(string)->free(cvm, string_ptr);
            CALL(string_pointer)->free(cvm, string_ptr);
            continue;
        }
        CALL(env)->puts(cvm, string_ptr);
        u64 pattern_ptr = CALL(stack)->pop(cvm, stack_ptr2);
        if (CALL(string)->size(cvm, pattern_ptr) == 0) {
            quit = 1;
            CALL(string)->free(cvm, string_ptr);
            CALL(string_pointer)->free(cvm, string_ptr);
            CALL(string)->free(cvm, pattern_ptr);
            CALL(string_pointer)->free(cvm, pattern_ptr);
            continue;
        }
        CALL(env)->puts(cvm, pattern_ptr);
        u64 size = CALL(string)->size(cvm, pattern_ptr);
        u64 string_pointer_ptr = 0;
        u64 current_ptr = string_ptr;
        while ((string_pointer_ptr = CALL(string)->strchr(cvm, current_ptr, pattern_ptr)) != 0) {
            u64 match_ptr = CALL(string)->match(cvm, string_pointer_ptr, pattern_ptr);
            if (match_ptr == 0) {
                CALL(string_pointer)->free(cvm, string_pointer_ptr);
                CALL(string)->free(cvm, string_ptr);
                CALL(string_pointer)->free(cvm, string_ptr);
                CALL(string)->free(cvm, pattern_ptr);
                CALL(string_pointer)->free(cvm, pattern_ptr);
                break;
            }
            if (CALL(string)->lessthan(cvm, string_pointer_ptr, match_ptr)) {
                u64 match_start_ptr = CALL(string)->left(cvm, match_ptr, size);
                u64 str_ncpy = CALL(string)->strncpy(cvm, match_start_ptr, size);
                u64 distance = CALL(string)->lessthan(cvm, string_ptr, match_start_ptr);
                if (distance > 0) {
                    u64 i = 0;
                    while (i++ < distance) {
                        printf(" ");
                    }
                }
                printf("%s[%lld]\n", CALL(string)->unsafe(cvm, str_ncpy), distance);
                CALL(string_pointer)->free(cvm, match_start_ptr);
                CALL(string)->free(cvm, str_ncpy);
            }
            CALL(string)->free(cvm, string_pointer_ptr);
            CALL(string_pointer)->free(cvm, string_pointer_ptr);
            current_ptr = match_ptr;
        }
#ifndef USE_GC
        CALL(string)->free(cvm, string_ptr);
        CALL(string_pointer)->free(cvm, string_ptr);
        CALL(string)->free(cvm, pattern_ptr);
        CALL(string_pointer)->free(cvm, pattern_ptr);
        CALL(string)->free(cvm, current_ptr);
        CALL(string_pointer)->free(cvm, current_ptr);
#endif
    }
#ifndef USE_GC
    CALL(stack)->free(cvm, stack_ptr2);
#endif
    CALL(stack)->free(cvm, stack_ptr1);
    CALL(stack)->free(cvm, stack_ptr2);
    CALL(stack)->free(cvm, gc_ptr);
}

static void run(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- rexo unit test code %s\n", __FILE__);
#endif
}

const tests_pointer_suite PRIVATE_API(tests_pointer_suite_definitions) = {
    .run = run
};
