/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 28, 2025 at 2:26:28 PM GMT+3
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

#include "system/api/api_v1.h"
#include "system/memory/memory_v1.h"

#include "virtual/os/os_v1.h"
#include "virtual/pointer/pointer_v1.h"
#include "virtual/types/data/data_v1.h"
#include "virtual/types/file/file_v1.h"
#include "virtual/types/object/object_v1.h"
#include "virtual/types/stack/stack_v1.h"
#include "virtual/types/string/string_v1.h"
#include "virtual/types/string_pointer/string_pointer_v1.h"
#include "virtual/types/user/user_v1.h"
#include "virtual/virtual/virtual_v1.h"

#include "internal/internal_v1.h"

#define USING_TESTS
#include "test.h"

/* definition */
CSYS_EXPORT extern const memory_methods* memory;
CVM_EXPORT extern const virtual_os_methods* os;
CVM_EXPORT extern const virtual_data_methods* data;
CVM_EXPORT extern const virtual_file_methods* file;
CVM_EXPORT extern const virtual_object_methods* object;
CVM_EXPORT extern const virtual_stack_methods* stack;
CVM_EXPORT extern const virtual_string_methods* string;
CVM_EXPORT extern const virtual_string_pointer_methods* string_pointer;
CVM_EXPORT extern const virtual_user_methods* user;
CVM_EXPORT extern const virtual_methods* virtual;
CVM_EXPORT extern const virtual_pointer_methods* pointer;

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    const_vm_ptr ctx;
}* TEST_DATA;

/*api*/
static const virtual_methods* virtual_methods_ptr;

/* mocks */
static const_pointer_ptr mock_virtual_read_zero(const_vm_ptr vm, u64 address, u64 type_id);

static const api_type* temp_api;

/* implementation */
static const_pointer_ptr mock_virtual_read_zero(const_vm_ptr vm, u64 address, u64 type_id) {
    return NULL_PTR;
}

RX_SET_UP(test_set_up) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = CALL(vm)->init(8);
    rx->ctx = vm;
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    CALL(vm)->gc();
    CALL(vm)->destroy();
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
RX_TEST_CASE(tests_pointer_v1, test_print_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    CALL(os)->putc(vm, 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_load_0, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    u64 empty_ptr = CALL(string)->load(vm, "\0");
    RX_ASSERT(empty_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_load_null_ptr, .fixture = test_fixture) {
    u64 null_ptr = 0;
#ifndef USE_GC
    CALL(vm)->release(null_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_load_null, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 empty_ptr = CALL(string)->load(vm, 0);
    RX_ASSERT(empty_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_load_empty, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 empty_ptr = CALL(string)->load(vm, "");
    RX_ASSERT(empty_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(vm, empty_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_print_load_empty, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 empty_ptr = CALL(string)->load(vm, "");
    CALL(os)->putc(vm, empty_ptr);
    RX_ASSERT(empty_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(vm, empty_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_load_copy, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 char_ptr = CALL(string)->load(vm, "/");
    u64 copy_ptr = CALL(string)->copy(vm, char_ptr);
    RX_ASSERT(char_ptr != 0);
    RX_ASSERT(copy_ptr != 0);
    RX_ASSERT(char_ptr != copy_ptr);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, char_ptr), "/") == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, copy_ptr), "/") == 0);
#ifndef USE_GC
    CALL(string)->free(vm, char_ptr);
    CALL(string)->free(vm, copy_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_load_copy_free_gc_destroy_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 char_ptr = CALL(string)->load(vm, "/");
    RX_ASSERT(char_ptr != 0);
#ifndef USE_GC
    CALL(string)->free(vm, char_ptr);
#endif
    CALL(string)->free(vm, char_ptr);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_load_copy_free_gc_free_destroy, .fixture = test_fixture) {
    const_vm_ptr vm = CALL(vm)->init(8);
    u64 char_ptr = CALL(string)->load(vm, "/");
    RX_ASSERT(char_ptr != 0);
#ifndef USE_GC
    CALL(string)->free(vm, char_ptr);
#endif
    CALL(vm)->gc();
    CALL(string)->free(vm, char_ptr);
    CALL(vm)->destroy();
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_load_copy_free_free_gc_destroy, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 char_ptr = CALL(string)->load(vm, "/");
    RX_ASSERT(char_ptr != 0);
#ifndef USE_GC
    CALL(string)->free(vm, char_ptr);
    CALL(string)->free(vm, char_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_load_copy_free_gc_destroy_destroy, .fixture = test_fixture) {
    const_vm_ptr vm = CALL(vm)->init(8);
    u64 char_ptr = CALL(string)->load(vm, "/");
    RX_ASSERT(char_ptr != 0);
#ifndef USE_GC
    CALL(string)->free(vm, char_ptr);
#endif
    CALL(vm)->gc();
    CALL(vm)->destroy();
    CALL(vm)->destroy();
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_init_init_load_free_gc_destroy, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 char_ptr = CALL(string)->load(vm, "/");
    RX_ASSERT(char_ptr != 0);
#ifndef USE_GC
    CALL(string)->free(vm, char_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_init_load_free_gc_gc_destroy, .fixture = test_fixture) {
    const_vm_ptr vm = CALL(vm)->init(8);
    u64 char_ptr = CALL(string)->load(vm, "/");
    RX_ASSERT(char_ptr != 0);
#ifndef USE_GC
    CALL(string)->free(vm, char_ptr);
#endif
    CALL(vm)->gc();
    CALL(vm)->gc();
    CALL(vm)->destroy();
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_init_load_free_destroy_gc, .fixture = test_fixture) {
    const_vm_ptr vm = CALL(vm)->init(8);
    u64 char_ptr = CALL(string)->load(vm, "/");
    RX_ASSERT(char_ptr != 0);
    CALL(string)->free(vm, char_ptr);
    CALL(vm)->destroy();
    CALL(vm)->gc();
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_load_copy_virtual_read_ptr_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    static virtual_methods mock_virtual_methods_definitions;
    /*api */
    memcpy(&mock_virtual_methods_definitions, virtual, sizeof(virtual_methods)); /* NOLINT: sizeof(virtual_methods*) */
    /* setup mocks */
    mock_virtual_methods_definitions.read = mock_virtual_read_zero;
    /* setup api endpoint */
    static const virtual_methods* mock_virtual_methods = &mock_virtual_methods_definitions;
    /* backup api calls */
    memcpy(&virtual_methods_ptr, &virtual, sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    /* init */
    u64 char_ptr = CALL(string)->load(vm, "/");
    /* prepare to mock api calls */
    memcpy(&virtual, &mock_virtual_methods, sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    /* virtual_string->free fails in virtual->read call */
    u64 copy_ptr = CALL(string)->copy(vm, char_ptr);
    /* restore api calls */
    memcpy(&virtual, &virtual_methods_ptr, sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    RX_ASSERT(char_ptr != 0);
    RX_ASSERT(copy_ptr == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, char_ptr), "/") == 0);
    CALL(string)->free(vm, char_ptr);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_load_copy_pointer_virtual_read_ptr_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    static virtual_methods mock_virtual_methods_definitions;
    /*api */
    memcpy(&mock_virtual_methods_definitions, virtual, sizeof(virtual_methods)); /* NOLINT: sizeof(virtual_methods*) */
    /* setup mocks */
    mock_virtual_methods_definitions.read = mock_virtual_read_zero;
    /* setup api endpoint */
    static const virtual_methods* mock_virtual_methods = &mock_virtual_methods_definitions;
    /* backup api calls */
    memcpy(&virtual_methods_ptr, &virtual, sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    /* prepare to mock api calls */
    memcpy(&virtual, &mock_virtual_methods, sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    u64 address = CALL(virtual)->alloc(vm, 2, TYPE_STRING_POINTER);
    u64 move_ptr = CALL(string)->move_left(vm, address, 1);
    RX_ASSERT(move_ptr == 0);
    CALL(pointer)->free(address, TYPE_STRING_POINTER);
    /* restore api calls */
    memcpy(&virtual, &virtual_methods_ptr, sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_load_copy_virtual_read_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    static virtual_methods mock_virtual_methods_definitions;
    /*api */
    memcpy(&mock_virtual_methods_definitions, virtual, sizeof(virtual_methods)); /* NOLINT: sizeof(virtual_methods*) */
    /* setup mocks */
    mock_virtual_methods_definitions.read = mock_virtual_read_zero;
    /* setup api endpoint */
    static const virtual_methods* mock_virtual_methods = &mock_virtual_methods_definitions;
    /* backup api calls */
    memcpy(&virtual_methods_ptr, &virtual, sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    /* init */
    u64 char_ptr = CALL(string)->load(vm, "/");
    /* prepare to mock api calls */
    memcpy(&virtual, &mock_virtual_methods, sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    /* virtual_string->free fails in virtual->read call */
    u64 copy_ptr = CALL(string)->copy(vm, char_ptr);
    /* restore api calls */
    memcpy(&virtual, &virtual_methods_ptr, sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    RX_ASSERT(char_ptr != 0);
    RX_ASSERT(copy_ptr == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, char_ptr), "/") == 0);
    CALL(string)->free(vm, char_ptr);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_load_push_peek_pop, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 char_ptr = CALL(string)->load(vm, "/");
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, char_ptr), "/") == 0);
#ifndef USE_GC
    CALL(string)->free(vm, char_ptr);
#endif
}

RX_TEST_CASE(tests_pointer_v1, test_load_free_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 char_ptr = CALL(string)->load(vm, "/");
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, char_ptr), "/") == 0);
#ifndef USE_GC
    CALL(string)->free(vm, char_ptr);
    CALL(string)->free(vm, char_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_strcat_load_alloc, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 char_ptr = CALL(string)->load(vm, "/");
    u64 pattern_ptr = CALL(string)->load(vm, "\0");
    CALL(string)->strcat(vm, pattern_ptr, char_ptr);
    RX_ASSERT(pattern_ptr == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, char_ptr), "/") == 0);
#ifndef USE_GC
    CALL(string)->free(vm, char_ptr);
    CALL(string)->free(vm, pattern_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_size, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 char_ptr = CALL(string)->load(vm, "/");
    u64 pattern_ptr = CALL(string)->load(vm, "\0");
    u64 string_size = CALL(data)->size(vm, pattern_ptr);
    RX_ASSERT(string_size == 0);
    RX_ASSERT(pattern_ptr == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, char_ptr), "/") == 0);
#ifndef USE_GC
    CALL(string)->free(vm, char_ptr);
    CALL(string)->free(vm, pattern_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_data_size, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 data_ptr = CALL(data)->alloc(vm, 2);
    u64 data_size = CALL(data)->size(vm, data_ptr);
    RX_ASSERT(data_size == 2);
#ifndef USE_GC
    CALL(data)->free(vm, data_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_pointer_size_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 char_ptr = CALL(string)->load(vm, "/");
    u64 pattern_ptr = CALL(string)->load(vm, "\0");
    u64 string_size = CALL(string)->size(vm, 0);
    RX_ASSERT(string_size == 0);
    RX_ASSERT(pattern_ptr == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, char_ptr), "/") == 0);
#ifndef USE_GC
    CALL(string)->free(vm, char_ptr);
    CALL(string)->free(vm, pattern_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_pointer_size_empty, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 char_ptr = CALL(string)->load(vm, " ");
    u64 pattern_ptr = CALL(string)->load(vm, "\0");
    u64 string_size = CALL(string)->size(vm, char_ptr);
    RX_ASSERT(string_size == 1);
    RX_ASSERT(pattern_ptr == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, char_ptr), " ") == 0);
#ifndef USE_GC
    CALL(string)->free(vm, char_ptr);
    CALL(string)->free(vm, pattern_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_pointer_size_object, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 object_ptr = CALL(object)->alloc(vm, 1);
    u64 pattern_ptr = CALL(string)->load(vm, "\0");
    u64 object_size = CALL(object)->size(vm, object_ptr);
    RX_ASSERT(object_size == 1);
    RX_ASSERT(object_ptr != 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(vm, object_ptr);
    CALL(string)->free(vm, pattern_ptr);
    CALL(object)->free(vm, object_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_object_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 object_ptr = CALL(object)->alloc(vm, 0);
    u64 pattern_ptr = CALL(string)->load(vm, "\0");
    u64 object_size = CALL(object)->size(vm, object_ptr);
    RX_ASSERT(object_size == 0);
    RX_ASSERT(object_ptr == 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(vm, object_ptr);
    CALL(string)->free(vm, pattern_ptr);
    CALL(object)->free(vm, object_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_object_alloc_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 object_ptr = CALL(object)->alloc(vm, 0);
    u64 pattern_ptr = CALL(string)->load(vm, "\0");
    u64 object_size = CALL(object)->size(vm, object_ptr);
    RX_ASSERT(object_size == 0);
    RX_ASSERT(object_ptr == 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(vm, object_ptr);
    CALL(string)->free(vm, pattern_ptr);
    CALL(object)->free(vm, object_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_object_load_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    const char* test_data = "Hello, world!";
    const void* ptr = (const void*)test_data;
    u64 object_ptr = CALL(object)->load(vm, ptr, strlen(ptr));
    CALL(object)->free(vm, object_ptr);
#ifndef USE_GC
    CALL(string)->free(vm, object_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_object_free_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 object_ptr = 0;
    CALL(object)->free(vm, object_ptr);
#ifndef USE_GC
    CALL(string)->free(vm, object_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_object_load_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 object_ptr = CALL(object)->load(vm, 0, 0x01234567);
    u64 pattern_ptr = CALL(string)->load(vm, "\0");
    u64 object_size = CALL(object)->size(vm, object_ptr);
    RX_ASSERT(object_size == 0);
    RX_ASSERT(object_ptr == 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(vm, object_ptr);
    CALL(string)->free(vm, pattern_ptr);
    CALL(object)->free(vm, object_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_object_load_string_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    const char* test_data = "Hello, world!";
    const void* ptr = (const void*)test_data;
    u64 object_ptr = CALL(object)->load(vm, ptr, 0);
    u64 pattern_ptr = CALL(string)->load(vm, "\0");
    u64 object_size = CALL(object)->size(vm, object_ptr);
    RX_ASSERT(object_size == 0);
    RX_ASSERT(object_ptr == 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(vm, object_ptr);
    CALL(string)->free(vm, pattern_ptr);
    CALL(object)->free(vm, object_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_object_load_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    const char* test_data = "Hello, world!";
    const void* ptr = (const void*)test_data;
    u64 object_ptr = CALL(object)->load(vm, ptr, strlen(ptr));
    u64 pattern_ptr = CALL(string)->load(vm, "\0");
    u64 object_size = CALL(object)->size(vm, object_ptr);
    RX_ASSERT(object_size == strlen(ptr));
    RX_ASSERT(object_ptr != 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(vm, object_ptr);
    CALL(string)->free(vm, pattern_ptr);
    CALL(object)->free(vm, object_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_object_load_string_unsafe_alloc_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 object_ptr = CALL(object)->alloc(vm, 0);
    u64 pattern_ptr = CALL(string)->load(vm, "\0");
    u64 object_size = CALL(object)->size(vm, object_ptr);
    const char* ch = (char*)CALL(object)->unsafe(vm, object_ptr);
    RX_ASSERT(ch == 0);
    RX_ASSERT(object_size == 0);
    RX_ASSERT(object_ptr == 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(vm, object_ptr);
    CALL(string)->free(vm, pattern_ptr);
    CALL(object)->free(vm, object_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_object_load_string_unsafe_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 object_ptr = CALL(object)->alloc(vm, 0);
    u64 pattern_ptr = CALL(string)->load(vm, "\0");
    u64 object_size = CALL(object)->size(vm, object_ptr);
    const char* ch = (char*)CALL(object)->unsafe(vm, pattern_ptr);
    RX_ASSERT(ch == 0);
    RX_ASSERT(object_size == 0);
    RX_ASSERT(object_ptr == 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(vm, object_ptr);
    CALL(string)->free(vm, pattern_ptr);
    CALL(object)->free(vm, object_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_object_load_string_unsafe_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    const char* test_data = "Hello, world!";
    const void* ptr = (const void*)test_data;
    u64 object_ptr = CALL(object)->load(vm, ptr, strlen(ptr));
    u64 pattern_ptr = CALL(string)->load(vm, "\0");
    u64 object_size = CALL(object)->size(vm, object_ptr);
    const char* ch = (char*)CALL(object)->unsafe(vm, 0);
    RX_ASSERT(ch == 0);
    RX_ASSERT(object_size == strlen(ptr));
    RX_ASSERT(object_ptr != 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(vm, object_ptr);
    CALL(string)->free(vm, pattern_ptr);
    CALL(object)->free(vm, object_ptr);
#endif
    CALL(vm)->gc();
    CALL(object)->free(vm, object_ptr);
    CALL(vm)->destroy();
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_object_load_string_unsafe, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    const char* test_data = "Hello, world!";
    const void* ptr = (const void*)test_data;
    u64 object_ptr = CALL(object)->load(vm, ptr, strlen(ptr));
    u64 pattern_ptr = CALL(string)->load(vm, "\0");
    u64 object_size = CALL(object)->size(vm, object_ptr);
    char* ch = (char*)CALL(object)->unsafe(vm, object_ptr);
    *ch = 'Z';
    RX_ASSERT(object_size == strlen(ptr));
    RX_ASSERT(object_ptr != 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(vm, object_ptr);
    CALL(string)->free(vm, pattern_ptr);
    CALL(object)->free(vm, object_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_strcat_alloc_alloc, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 char_ptr = CALL(string)->load(vm, "\0");
    u64 pattern_ptr = CALL(string)->load(vm, "\0");
    CALL(string)->strcat(vm, pattern_ptr, char_ptr);
    RX_ASSERT(char_ptr == 0);
    RX_ASSERT(pattern_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(vm, char_ptr);
    CALL(string)->free(vm, pattern_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_offset_ptr_diff, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 string_ptr1 = CALL(string)->load(vm, "192.168.0.111");
    u64 string_ptr2 = CALL(string)->load(vm, "192.168.0.12");
    u64 last_matched_ptr1 = CALL(string)->offset(vm, string_ptr1, string_ptr2);
    RX_ASSERT(last_matched_ptr1 == 0);
#ifndef USE_GC
    CALL(string)->free(vm, last_matched_ptr1);
    CALL(string)->free(vm, string_ptr1);
    CALL(string)->free(vm, string_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_offset_ptr_less, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 string_ptr1 = CALL(string)->load(vm, "192.168.0.11");
    u64 string_ptr2 = CALL(string)->load(vm, "8.0.112");
    u64 last_matched_ptr1 = CALL(string)->offset(vm, string_ptr1, string_ptr2);
    RX_ASSERT(last_matched_ptr1 == 0);
#ifndef USE_GC
    CALL(string)->free(vm, last_matched_ptr1);
    CALL(string)->free(vm, string_ptr1);
    CALL(string)->free(vm, string_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_offset_ptr_offset, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 string_ptr1 = CALL(string)->load(vm, "192.168.0.11");
    u64 string_ptr2 = CALL(string)->load(vm, "8.0.12");
    u64 last_matched_ptr1 = CALL(string)->offset(vm, string_ptr1, string_ptr2);
    RX_ASSERT(last_matched_ptr1 == 0);
#ifndef USE_GC
    CALL(string)->free(vm, last_matched_ptr1);
    CALL(string)->free(vm, string_ptr1);
    CALL(string)->free(vm, string_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_offset_ptr_offset_found, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 string_ptr1 = CALL(string)->load(vm, "192.168.0.111");
    u64 string_ptr2 = CALL(string)->load(vm, "8.0.");
    u64 last_matched_ptr1 = CALL(string)->offset(vm, string_ptr1, string_ptr2);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, last_matched_ptr1), "111") == 0);
#ifndef USE_GC
    CALL(string)->free(vm, last_matched_ptr1);
    CALL(string)->free(vm, string_ptr1);
    CALL(string)->free(vm, string_ptr2);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_free_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
#ifndef USE_GC
    u64 list_ptr = CALL(stack)->alloc(vm);
    u64 string_ptr = CALL(string)->load(vm, "hello");
    u64 e_ptr = CALL(string)->load(vm, "e");
    u64 string_pointer_ptr = CALL(string)->offset(vm, string_ptr, e_ptr);
    RX_ASSERT(strcmp("llo", CALL(string)->unsafe(vm, string_pointer_ptr)) == 0);
    CALL(string)->free(vm, list_ptr);
    CALL(string)->free(vm, string_pointer_ptr);
    CALL(string)->free(vm, string_ptr);
    CALL(string)->free(vm, e_ptr);
    CALL(stack)->free(vm, list_ptr);
#else
    CALL(stack)->alloc(vm);
    u64 string_ptr = CALL(string)->load(vm, "hello");
    u64 e_ptr = CALL(string)->load(vm, "e");
    CALL(string)->offset(vm, string_ptr, e_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(vm);
    u64 string_ptr = CALL(string)->load(vm, "hello");
    u64 e_ptr = CALL(string)->load(vm, "e");
    u64 string_pointer_ptr = CALL(string)->offset(vm, string_ptr, e_ptr);
    CALL(stack)->push(vm, list_ptr, string_pointer_ptr);
    CALL(stack)->push(vm, list_ptr, string_ptr);
#ifndef USE_GC
    CALL(string)->free(vm, e_ptr);
    CALL(stack)->free(vm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_offset_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(vm);
    u64 string_ptr = CALL(string)->load(vm, "hello");
    u64 last_matched_ptr = CALL(string)->offset(vm, list_ptr, string_ptr);
    RX_ASSERT(last_matched_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(vm, string_ptr);
    CALL(stack)->free(vm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_match, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(vm);
    u64 string_ptr = CALL(string)->load(vm, "192.192.0.1");
    u64 dot_ptr = CALL(string)->load(vm, "192.");
    u64 string_pointer_ptr1 = CALL(string)->match(vm, string_ptr, dot_ptr);
    u64 string_pointer_ptr2 = CALL(string)->match(vm, string_pointer_ptr1, dot_ptr);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, string_pointer_ptr1), "192.0.1") == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, string_pointer_ptr2), "0.1") == 0);
    CALL(stack)->push(vm, list_ptr, string_pointer_ptr1);
    CALL(stack)->push(vm, list_ptr, string_pointer_ptr2);
    CALL(stack)->push(vm, list_ptr, string_ptr);
#ifndef USE_GC
    CALL(string)->free(vm, dot_ptr);
    CALL(stack)->free(vm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_match_miss, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(vm);
    u64 string_ptr = CALL(string)->load(vm, "192.168.0.1");
    u64 dot_ptr = CALL(string)->load(vm, ":");
    u64 string_pointer_ptr1 = CALL(string)->match(vm, string_ptr, dot_ptr);
    RX_ASSERT(string_pointer_ptr1 == 0);
    CALL(stack)->push(vm, list_ptr, string_pointer_ptr1);
    CALL(stack)->push(vm, list_ptr, string_ptr);
#ifndef USE_GC
    CALL(string)->free(vm, dot_ptr);
    CALL(stack)->free(vm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_match_pattern, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(vm);
    u64 string_ptr = CALL(string)->load(vm, "192.168.0.112");
    u64 dot_ptr = CALL(string)->load(vm, "192.168.0.");
    u64 string_pointer_ptr1 = CALL(string)->match(vm, string_ptr, dot_ptr);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, string_pointer_ptr1), "112") == 0);
    CALL(stack)->push(vm, list_ptr, string_pointer_ptr1);
    CALL(stack)->push(vm, list_ptr, string_ptr);
#ifndef USE_GC
    CALL(string)->free(vm, dot_ptr);
    CALL(stack)->free(vm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_match_pattern_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(vm);
    u64 string_ptr = CALL(string)->load(vm, "192.168.0.112");
    u64 dot_ptr = CALL(string)->load(vm, "193");
    u64 string_pointer_ptr1 = CALL(string)->match(vm, string_ptr, dot_ptr);
    RX_ASSERT(string_pointer_ptr1 == 0);
    CALL(stack)->push(vm, list_ptr, string_pointer_ptr1);
    CALL(stack)->push(vm, list_ptr, string_ptr);
#ifndef USE_GC
    CALL(string)->free(vm, dot_ptr);
    CALL(stack)->free(vm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_match_pattern_none, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(vm);
    u64 string_ptr = CALL(string)->load(vm, "192.168.0.11");
    u64 dot_ptr = CALL(string)->load(vm, "192.168.0.111");
    u64 string_pointer_ptr1 = CALL(string)->match(vm, string_ptr, dot_ptr);
    RX_ASSERT(string_pointer_ptr1 == 0);
    CALL(stack)->push(vm, list_ptr, string_pointer_ptr1);
    CALL(stack)->push(vm, list_ptr, string_ptr);
#ifndef USE_GC
    CALL(string)->free(vm, dot_ptr);
    CALL(stack)->free(vm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_release_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 string_ptr = CALL(string)->load(vm, "192.168.0.11");
    CALL(stack)->release(vm, string_ptr);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_free_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 string_ptr = CALL(string)->load(vm, "192.168.0.11");
    CALL(stack)->free(vm, string_ptr);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_match_pattern_load_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 string_ptr = CALL(string)->load(vm, "192.168.0.1");
    u64 dot_ptr = CALL(string)->load(vm, "57.34.7.2");
    u64 error_ptr = CALL(string)->match(vm, string_ptr, dot_ptr);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_offset_pattern_load_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 string_ptr = CALL(string)->load(vm, "192.168.0.1");
    u64 dot_ptr = CALL(string)->load(vm, "57.34.7.2");
    u64 error_ptr = CALL(string)->offset(vm, string_ptr, dot_ptr);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_match_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 string_ptr = CALL(string)->load(vm, "192.168.0.1");
    u64 string_pointer_ptr = CALL(string)->match(vm, string_ptr, 0);
    RX_ASSERT(string_pointer_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(vm, string_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_match_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
#ifndef USE_GC
    u64 string_ptr = CALL(string)->load(vm, "192.168.0.1");
    u64 string_pointer_ptr = CALL(string)->match(vm, 0, 0);
    RX_ASSERT(string_pointer_ptr == 0);
    CALL(string)->free(vm, string_ptr);
#else
    CALL(string)->load(vm, "192.168.0.1");
    u64 string_pointer_ptr = CALL(string)->match(vm, 0, 0);
    RX_ASSERT(string_pointer_ptr == 0);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_match_0_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 string_ptr = CALL(string)->load(vm, "192.168.0.1");
    u64 string_pointer_ptr = CALL(string)->match(vm, 0, string_ptr);
    RX_ASSERT(string_pointer_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(vm, string_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_match_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(vm);
    u64 string_ptr = CALL(string)->load(vm, "hello");
    u64 last_matched_ptr = CALL(string)->match(vm, list_ptr, string_ptr);
    RX_ASSERT(last_matched_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(vm, string_ptr);
    CALL(stack)->free(vm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_strchr, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(vm);
    u64 string_ptr = CALL(string)->load(vm, "a.b.c/2");
    u64 dot_ptr = CALL(string)->load(vm, ".");
    u64 string_pointer_ptr1 = CALL(string)->strchr(vm, string_ptr, dot_ptr);
    u64 string_pointer_ptr2 = CALL(string)->match(vm, string_pointer_ptr1, dot_ptr);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, string_pointer_ptr1), ".b.c/2") == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, string_pointer_ptr2), "b.c/2") == 0);
    CALL(stack)->push(vm, list_ptr, string_pointer_ptr1);
    CALL(stack)->push(vm, list_ptr, string_pointer_ptr2);
    CALL(stack)->push(vm, list_ptr, string_ptr);
#ifndef USE_GC
    CALL(string)->free(vm, dot_ptr);
    CALL(stack)->free(vm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_strchr_miss, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(vm);
    u64 string_ptr = CALL(string)->load(vm, "192.168.0.1");
    u64 dot_ptr = CALL(string)->load(vm, ":");
    u64 string_pointer_ptr1 = CALL(string)->strchr(vm, string_ptr, dot_ptr);
    RX_ASSERT(string_pointer_ptr1 == 0);
    CALL(stack)->push(vm, list_ptr, string_pointer_ptr1);
    CALL(stack)->push(vm, list_ptr, string_ptr);
#ifndef USE_GC
    CALL(string)->free(vm, dot_ptr);
    CALL(stack)->free(vm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_strchr_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 string_ptr = CALL(string)->load(vm, "192.168.0.1");
    u64 string_pointer_ptr = CALL(string)->strchr(vm, string_ptr, 0);
    RX_ASSERT(string_pointer_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(vm, string_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_strchr_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
#ifndef USE_GC
    u64 string_ptr = CALL(string)->load(vm, "192.168.0.1");
    u64 string_pointer_ptr = CALL(string)->strchr(vm, 0, 0);
    RX_ASSERT(string_pointer_ptr == 0);
    CALL(string)->free(vm, string_ptr);
#else
    CALL(string)->load(vm, "192.168.0.1");
    u64 string_pointer_ptr = CALL(string)->strchr(vm, 0, 0);
    RX_ASSERT(string_pointer_ptr == 0);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_strchr_0_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 string_ptr = CALL(string)->load(vm, "192.168.0.1");
    u64 string_pointer_ptr = CALL(string)->strchr(vm, 0, string_ptr);
    RX_ASSERT(string_pointer_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(vm, string_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_strchr_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(vm);
    u64 string_ptr = CALL(string)->load(vm, "hello");
    u64 last_matched_ptr = CALL(string)->strchr(vm, list_ptr, string_ptr);
    RX_ASSERT(last_matched_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(vm, string_ptr);
    CALL(stack)->free(vm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_strrchr, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 string_ptr = CALL(string)->load(vm, "192.168.0.1");
    u64 dot_ptr = CALL(string)->load(vm, ".");
    u64 string_pointer_ptr1 = CALL(string)->strrchr(vm, string_ptr, dot_ptr);
    u64 string_pointer_ptr2 = CALL(string)->match(vm, string_pointer_ptr1, dot_ptr);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, string_pointer_ptr1), ".1") == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, string_pointer_ptr2), "1") == 0);
#ifndef USE_GC
    CALL(string)->free(vm, string_pointer_ptr1);
    CALL(string)->free(vm, string_pointer_ptr2);
    CALL(string)->free(vm, string_ptr);
    CALL(string)->free(vm, dot_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_strrchr_match_offset, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 string_ptr = CALL(string)->load(vm, "192.168.0.111");
    u64 dot_ptr = CALL(string)->load(vm, ".");
    u64 string_pointer_ptr1 = CALL(string)->strrchr(vm, string_ptr, dot_ptr);
    u64 string_pointer_ptr2 = CALL(string)->match(vm, string_pointer_ptr1, dot_ptr);
    u64 string_pointer_ptr3 = CALL(string)->offset(vm, string_ptr, dot_ptr);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, string_pointer_ptr1), ".111") == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, string_pointer_ptr2), "111") == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, string_pointer_ptr3), "111") == 0);
#ifndef USE_GC
    CALL(string)->free(vm, string_pointer_ptr1);
    CALL(string)->free(vm, string_pointer_ptr2);
    CALL(string)->free(vm, string_pointer_ptr3);
    CALL(string)->free(vm, string_ptr);
    CALL(string)->free(vm, dot_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_peekn_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(vm);
    RX_ASSERT(list_ptr != 0);
    u64 error_ptr = CALL(stack)->peekn(vm, 0, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_peekn_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(vm);
    u64 error_ptr = CALL(stack)->peekn(vm, list_ptr, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_peekn_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr1 = CALL(stack)->alloc(vm);
    u64 string_ptr1 = CALL(string)->load(vm, "192.168.0.1");
    CALL(stack)->push(vm, list_ptr1, string_ptr1);
    u64 list_ptr2 = CALL(stack)->peekn(vm, list_ptr1, 1);
    RX_ASSERT(list_ptr2 != 0);
    RX_ASSERT(CALL(stack)->size(vm, list_ptr2) != 0);
    u64 string_ptr2 = CALL(stack)->peek(vm, list_ptr2);
    RX_ASSERT(string_ptr2 != 0);
    RX_ASSERT(string_ptr1 == string_ptr2);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_peekn_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 string_ptr = CALL(string)->load(vm, "192.168.0.1");
    u64 list_ptr = CALL(stack)->alloc(vm);
    CALL(stack)->push(vm, list_ptr, string_ptr);
    u64 error_ptr = CALL(stack)->peekn(vm, list_ptr, 2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_popn_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(vm);
    RX_ASSERT(list_ptr != 0);
    u64 error_ptr = CALL(stack)->popn(vm, 0, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_popn_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(vm);
    u64 error_ptr = CALL(stack)->popn(vm, list_ptr, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_popn_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr1 = CALL(stack)->alloc(vm);
    u64 string_ptr1 = CALL(string)->load(vm, "192.168.0.1");
    CALL(stack)->push(vm, list_ptr1, string_ptr1);
    u64 list_ptr2 = CALL(stack)->popn(vm, list_ptr1, 1);
    RX_ASSERT(list_ptr2 != 0);
    RX_ASSERT(CALL(stack)->size(vm, list_ptr2) != 0);
    u64 string_ptr2 = CALL(stack)->peek(vm, list_ptr2);
    RX_ASSERT(string_ptr2 != 0);
    RX_ASSERT(string_ptr1 == string_ptr2);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_popn_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 string_ptr = CALL(string)->load(vm, "192.168.0.1");
    u64 list_ptr = CALL(stack)->alloc(vm);
    CALL(stack)->push(vm, list_ptr, string_ptr);
    u64 error_ptr = CALL(stack)->popn(vm, list_ptr, 2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_strchr_match_offset, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(vm);
    u64 list_match_ptr = CALL(stack)->alloc(vm);
    u64 string_ptr = CALL(string)->load(vm, "a.bc.bb.ba.a");
    u64 pattern_ptr = CALL(string)->load(vm, ".b");
    u64 string_pointer_ptr = 0;
    u64 match_ptr = string_ptr;
    u64 current_ptr = match_ptr;
    while ((string_pointer_ptr = CALL(string)->strchr(vm, current_ptr, pattern_ptr)) != 0) {
        CALL(stack)->push(vm, list_ptr, string_pointer_ptr);
        match_ptr = CALL(string)->match(vm, string_pointer_ptr, pattern_ptr);
        CALL(stack)->push(vm, list_match_ptr, match_ptr);
        current_ptr = match_ptr;
    }
    u64 size = CALL(stack)->size(vm, list_ptr);
    RX_ASSERT(size == 4);
    u64 string_ptr1 = CALL(stack)->pop(vm, list_ptr);
    u64 string_ptr2 = CALL(stack)->pop(vm, list_ptr);
    u64 string_ptr3 = CALL(stack)->pop(vm, list_ptr);
    u64 string_ptr4 = CALL(stack)->pop(vm, list_ptr);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, string_ptr1), ".a") == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, string_ptr2), ".ba.a") == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, string_ptr3), ".bb.ba.a") == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, string_ptr4), ".bc.bb.ba.a") == 0);
    RX_ASSERT(CALL(stack)->size(vm, list_ptr) == 0);
    u64 match_size = CALL(stack)->size(vm, list_match_ptr);
    RX_ASSERT(match_size == 3);
    u64 string_match_ptr1 = CALL(stack)->pop(vm, list_match_ptr);
    u64 string_match_ptr2 = CALL(stack)->pop(vm, list_match_ptr);
    u64 string_match_ptr3 = CALL(stack)->pop(vm, list_match_ptr);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, string_match_ptr1), "a.a") == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, string_match_ptr2), "b.ba.a") == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, string_match_ptr3), "c.bb.ba.a") == 0);
    RX_ASSERT(CALL(stack)->size(vm, list_match_ptr) == 0);
#ifndef USE_GC
    CALL(string)->free(vm, string_ptr1);
    CALL(string)->free(vm, string_ptr2);
    CALL(string)->free(vm, string_ptr3);
    CALL(string)->free(vm, string_ptr4);
    CALL(string)->free(vm, string_match_ptr1);
    CALL(string)->free(vm, string_match_ptr2);
    CALL(string)->free(vm, string_match_ptr3);
    CALL(string)->free(vm, string_ptr);
    CALL(string)->free(vm, pattern_ptr);
    CALL(stack)->free(vm, list_match_ptr);
    CALL(stack)->free(vm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_strrchr_miss, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(vm);
    u64 string_ptr = CALL(string)->load(vm, "192.168.0.1");
    u64 dot_ptr = CALL(string)->load(vm, ":");
    u64 string_pointer_ptr1 = CALL(string)->strrchr(vm, string_ptr, dot_ptr);
    RX_ASSERT(string_pointer_ptr1 == 0);
    CALL(stack)->push(vm, list_ptr, string_pointer_ptr1);
    CALL(stack)->push(vm, list_ptr, string_ptr);
#ifndef USE_GC
    CALL(string)->free(vm, dot_ptr);
    CALL(stack)->free(vm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_strrchr_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 string_ptr = CALL(string)->load(vm, "192.168.0.1");
    u64 string_pointer_ptr = CALL(string)->strrchr(vm, string_ptr, 0);
    RX_ASSERT(string_pointer_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(vm, string_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_strrchr_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
#ifndef USE_GC
    u64 string_ptr = CALL(string)->load(vm, "192.168.0.1");
    u64 string_pointer_ptr = CALL(string)->strrchr(vm, 0, 0);
    RX_ASSERT(string_pointer_ptr == 0);
    CALL(string)->free(vm, string_ptr);
#else
    CALL(string)->load(vm, "192.168.0.1");
    u64 string_pointer_ptr = CALL(string)->strrchr(vm, 0, 0);
    RX_ASSERT(string_pointer_ptr == 0);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_strrchr_0_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 string_ptr = CALL(string)->load(vm, "192.168.0.1");
    u64 string_pointer_ptr = CALL(string)->strrchr(vm, 0, string_ptr);
    RX_ASSERT(string_pointer_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(vm, string_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_strrchr_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(vm);
    u64 string_ptr = CALL(string)->load(vm, "hello");
    u64 last_matched_ptr = CALL(string)->strrchr(vm, list_ptr, string_ptr);
    RX_ASSERT(last_matched_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(vm, string_ptr);
    CALL(stack)->free(vm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_push_0_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    CALL(stack)->push(vm, 0, 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_push_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    CALL(stack)->push(vm, 1, 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_push_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(vm);
    CALL(stack)->push(vm, list_ptr, 0);
#ifndef USE_GC
    CALL(stack)->free(vm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_push_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(vm);
    CALL(stack)->push(vm, list_ptr, list_ptr);
#ifndef USE_GC
    CALL(stack)->free(vm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_push_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 string_ptr = CALL(string)->load(vm, "@");
    u64 list_ptr = CALL(stack)->alloc(vm);
    CALL(stack)->push(vm, string_ptr, list_ptr);
#ifndef USE_GC
    CALL(stack)->free(vm, list_ptr);
    CALL(string)->free(vm, string_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_size_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
#ifndef USE_GC
    u64 string_ptr = CALL(string)->load(vm, "@");
    u64 list_ptr = CALL(stack)->alloc(vm);
    u64 value = CALL(stack)->size(vm, string_ptr);
    RX_ASSERT(value == 0);
    CALL(stack)->free(vm, list_ptr);
    CALL(string)->free(vm, string_ptr);
#else
    u64 string_ptr = CALL(string)->load(vm, "@");
    CALL(stack)->alloc(vm);
    u64 value = CALL(stack)->size(vm, string_ptr);
    RX_ASSERT(value == 0);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_push_size, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 string_ptr = CALL(string)->load(vm, "@");
    u64 list_ptr = CALL(stack)->alloc(vm);
    CALL(stack)->push(vm, list_ptr, string_ptr);
    u64 value = CALL(stack)->size(vm, list_ptr);
    RX_ASSERT(value != 0);
#ifndef USE_GC
    CALL(stack)->free(vm, list_ptr);
    CALL(string)->free(vm, string_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_pop_0_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 value_ptr = CALL(stack)->pop(vm, 0);
    RX_ASSERT(value_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_pop_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 value_ptr = CALL(stack)->pop(vm, 1);
    RX_ASSERT(value_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_pop_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(vm);
    u64 value_ptr = CALL(stack)->pop(vm, list_ptr);
    RX_ASSERT(value_ptr == 0);
#ifndef USE_GC
    CALL(stack)->free(vm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_pop_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 string_ptr = CALL(string)->load(vm, "@");
    u64 list_ptr = CALL(stack)->alloc(vm);
    CALL(stack)->push(vm, list_ptr, string_ptr);
    u64 value_ptr = CALL(stack)->pop(vm, list_ptr);
    RX_ASSERT(value_ptr != 0);
#ifndef USE_GC
    CALL(stack)->free(vm, list_ptr);
    CALL(string)->free(vm, string_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_peek_0_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 value_ptr = CALL(stack)->peek(vm, 0);
    RX_ASSERT(value_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_peek_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 value_ptr = CALL(stack)->peek(vm, 1);
    RX_ASSERT(value_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_peek_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(vm);
    u64 value_ptr = CALL(stack)->peek(vm, list_ptr);
    RX_ASSERT(value_ptr == 0);
#ifndef USE_GC
    CALL(stack)->free(vm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_peek_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 string_ptr = CALL(string)->load(vm, "@");
    u64 list_ptr = CALL(stack)->alloc(vm);
    CALL(stack)->push(vm, list_ptr, string_ptr);
    u64 value_ptr = CALL(stack)->peek(vm, list_ptr);
    RX_ASSERT(value_ptr != 0);
#ifndef USE_GC
    CALL(stack)->free(vm, list_ptr);
    CALL(string)->free(vm, string_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_strcpy, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 path_ptr1 = CALL(string)->load(vm, "/");
    u64 path_ptr2 = CALL(string)->load(vm, "@");
    u64 path_copy_ptr = CALL(string)->copy(vm, path_ptr1);
    CALL(string)->strcat(vm, path_copy_ptr, path_ptr2);

    char* path1 = CALL(string)->unsafe(vm, path_ptr1);
    char* path2 = CALL(string)->unsafe(vm, path_ptr2);
    u64 path1_len = strlen(path1);
    u64 path2_len = strlen(path2);
    RX_ASSERT(path1_len > 0);
    RX_ASSERT(path2_len > 0);
    char* buf = CALL(system_memory)->alloc(path1_len + path2_len + 1);
    strcpy(buf, path1); /* NOLINT */
    strcat(buf, path2); /* NOLINT */
    char* path_copy = CALL(string)->unsafe(vm, path_copy_ptr);
    RX_ASSERT(strlen(path_copy) == strlen(buf));
    RX_ASSERT(strcmp(path_copy, buf) == 0);
    CALL(system_memory)->free(buf, path1_len + path2_len + 1);
#ifndef USE_GC
    CALL(string)->free(vm, path_ptr1);
    CALL(string)->free(vm, path_ptr2);
    CALL(string)->free(vm, path_copy_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_strcat_load_put_char, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    const char* ch = "hello, world!";
    u64 string_ptr = CALL(pointer)->copy(ch, 2, 0, TYPE_STRING);
    CALL(string)->put_char(vm, string_ptr, 'a');
    const char* actual = CALL(string)->unsafe(vm, string_ptr);
    RX_ASSERT(actual[0] == 'a');
    RX_ASSERT(actual[1] == 'e');
    CALL(vm)->release(string_ptr);
}
RX_TEST_CASE(tests_pointer_v1, test_strcat_load_put_char_offset, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    const char* ch = "hello, world!";
    u64 string_ptr = CALL(pointer)->copy(ch, 2, 14, TYPE_STRING);
    CALL(string)->put_char(vm, string_ptr, 'a');
    const char* actual = CALL(string)->unsafe(vm, string_ptr);
    RX_ASSERT(actual[0] == 'a');
    RX_ASSERT(actual[1] == 'e');
    CALL(vm)->release(string_ptr);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_strcat_load_alloc_copy, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 string_ptr = CALL(string)->load(vm, "/all_english_words.txt");
    u64 zero_ptr = CALL(string)->load(vm, "\0");
    u64 data_ptr = CALL(data)->alloc(vm, 1);
    u64 list_ptr = CALL(stack)->alloc(vm);
    u64 empty_ptr = CALL(string)->load(vm, "\0");

    u64 null_ptr = 0;
    u64 none_ptr = 0xfffffffff;

    u64 data_ptr1 = CALL(string)->copy(vm, null_ptr);
    u64 data_ptr2 = CALL(string)->copy(vm, zero_ptr);
    u64 data_ptr3 = CALL(string)->copy(vm, string_ptr);
    u64 data_ptr4 = CALL(string)->copy(vm, none_ptr);

    CALL(os)->putc(vm, string_ptr);
    CALL(os)->putc(vm, zero_ptr);
    CALL(os)->putc(vm, data_ptr);
    CALL(os)->putc(vm, list_ptr);
    CALL(os)->putc(vm, empty_ptr);
    CALL(os)->putc(vm, null_ptr);
    CALL(os)->putc(vm, none_ptr);

    CALL(string)->put_char(vm, string_ptr, 'a');
    CALL(string)->put_char(vm, zero_ptr, 'a');
    CALL(string)->put_char(vm, data_ptr, 'a');
    CALL(string)->put_char(vm, list_ptr, 'a');
    CALL(string)->put_char(vm, empty_ptr, 'a');
    CALL(string)->put_char(vm, null_ptr, 'a');
    CALL(string)->put_char(vm, none_ptr, 'a');

    CALL(string)->put_char(vm, string_ptr, '\0');
    CALL(string)->put_char(vm, zero_ptr, '\0');
    CALL(string)->put_char(vm, data_ptr, '\0');
    CALL(string)->put_char(vm, list_ptr, '\0');
    CALL(string)->put_char(vm, empty_ptr, '\0');
    CALL(string)->put_char(vm, null_ptr, '\0');
    CALL(string)->put_char(vm, none_ptr, '\0');

    CALL(string)->put_char(vm, string_ptr, '/');
    CALL(string)->strcpy(vm, string_ptr, string_ptr);
    CALL(string)->strcpy(vm, string_ptr, zero_ptr);
    CALL(string)->strcpy(vm, zero_ptr, string_ptr);
    CALL(string)->strcpy(vm, data_ptr, null_ptr);
    CALL(string)->strcpy(vm, list_ptr, null_ptr);
    CALL(string)->strcpy(vm, string_ptr, data_ptr);
    CALL(string)->strcpy(vm, string_ptr, empty_ptr);
    CALL(string)->strcpy(vm, data_ptr, null_ptr);
    CALL(string)->strcpy(vm, list_ptr, string_ptr);
    CALL(string)->strcpy(vm, data_ptr, null_ptr);
    CALL(string)->strcpy(vm, list_ptr, null_ptr);
    CALL(string)->strcpy(vm, string_ptr, data_ptr);
    CALL(string)->strcpy(vm, data_ptr, null_ptr);
    CALL(string)->strcpy(vm, list_ptr, string_ptr);
    CALL(string)->strcpy(vm, null_ptr, null_ptr);
    CALL(string)->strcpy(vm, string_ptr, null_ptr);
    CALL(string)->strcpy(vm, string_ptr, none_ptr);
    CALL(string)->strcpy(vm, none_ptr, string_ptr);

    CALL(string)->strcat(vm, string_ptr, string_ptr);
    CALL(string)->strcat(vm, string_ptr, zero_ptr);
    CALL(string)->strcat(vm, zero_ptr, string_ptr);
    CALL(string)->strcat(vm, data_ptr, null_ptr);
    CALL(string)->strcat(vm, list_ptr, null_ptr);
    CALL(string)->strcat(vm, string_ptr, data_ptr);
    CALL(string)->strcat(vm, string_ptr, empty_ptr);
    CALL(string)->strcat(vm, data_ptr, null_ptr);
    CALL(string)->strcat(vm, list_ptr, string_ptr);
    CALL(string)->strcat(vm, data_ptr, null_ptr);
    CALL(string)->strcat(vm, list_ptr, null_ptr);
    CALL(string)->strcat(vm, string_ptr, data_ptr);
    CALL(string)->strcat(vm, data_ptr, null_ptr);
    CALL(string)->strcat(vm, list_ptr, string_ptr);
    CALL(string)->strcat(vm, null_ptr, null_ptr);
    CALL(string)->strcat(vm, string_ptr, null_ptr);
    CALL(string)->strcat(vm, string_ptr, none_ptr);
    CALL(string)->strcat(vm, none_ptr, string_ptr);

    u64 last_matched_ptr1 = CALL(string)->offset(vm, string_ptr, string_ptr);
    CALL(string)->offset(vm, data_ptr, null_ptr);
    CALL(string)->offset(vm, list_ptr, null_ptr);
    CALL(string)->offset(vm, string_ptr, data_ptr);
    CALL(string)->offset(vm, string_ptr, empty_ptr);
    CALL(string)->offset(vm, data_ptr, null_ptr);
    CALL(string)->offset(vm, list_ptr, string_ptr);
    CALL(string)->offset(vm, data_ptr, null_ptr);
    CALL(string)->offset(vm, list_ptr, null_ptr);
    CALL(string)->offset(vm, string_ptr, data_ptr);
    CALL(string)->offset(vm, data_ptr, null_ptr);
    CALL(string)->offset(vm, list_ptr, string_ptr);
    CALL(string)->offset(vm, null_ptr, null_ptr);
    CALL(string)->offset(vm, string_ptr, null_ptr);

    u64 last_matched_ptr2 = CALL(string)->offset(vm, string_ptr, string_ptr);
    CALL(string)->offset(vm, string_ptr, zero_ptr);
    CALL(string)->offset(vm, data_ptr, zero_ptr);
    CALL(string)->offset(vm, list_ptr, zero_ptr);
    CALL(string)->offset(vm, string_ptr, data_ptr);
    CALL(string)->offset(vm, zero_ptr, zero_ptr);
    CALL(string)->offset(vm, zero_ptr, data_ptr);
    CALL(string)->offset(vm, zero_ptr, list_ptr);
    CALL(string)->offset(vm, zero_ptr, string_ptr);
    CALL(string)->offset(vm, empty_ptr, empty_ptr);
    CALL(string)->offset(vm, empty_ptr, string_ptr);
    CALL(string)->offset(vm, empty_ptr, zero_ptr);

    CALL(string)->offset(vm, null_ptr, null_ptr);
    CALL(string)->offset(vm, null_ptr, empty_ptr);
    CALL(string)->offset(vm, null_ptr, string_ptr);
    CALL(string)->offset(vm, null_ptr, data_ptr);
    CALL(string)->offset(vm, null_ptr, list_ptr);
    CALL(string)->offset(vm, empty_ptr, null_ptr);
    CALL(string)->offset(vm, empty_ptr, empty_ptr);
    CALL(string)->offset(vm, empty_ptr, string_ptr);
    CALL(string)->offset(vm, empty_ptr, data_ptr);
    CALL(string)->offset(vm, empty_ptr, list_ptr);
    CALL(string)->offset(vm, string_ptr, null_ptr);
    CALL(string)->offset(vm, string_ptr, empty_ptr);

    u64 last_matched_ptr3 = CALL(string)->offset(vm, string_ptr, string_ptr);
    CALL(string)->offset(vm, string_ptr, data_ptr);
    CALL(string)->offset(vm, string_ptr, list_ptr);
    CALL(string)->offset(vm, data_ptr, null_ptr);
    CALL(string)->offset(vm, data_ptr, empty_ptr);
    CALL(string)->offset(vm, data_ptr, string_ptr);
    CALL(string)->offset(vm, data_ptr, data_ptr);
    CALL(string)->offset(vm, data_ptr, list_ptr);
    CALL(string)->offset(vm, list_ptr, null_ptr);
    CALL(string)->offset(vm, list_ptr, empty_ptr);
    CALL(string)->offset(vm, list_ptr, string_ptr);
    CALL(string)->offset(vm, list_ptr, data_ptr);
    CALL(string)->offset(vm, list_ptr, list_ptr);
    CALL(string)->offset(vm, string_ptr, none_ptr);
    CALL(string)->offset(vm, none_ptr, string_ptr);

    CALL(string)->unsafe(vm, null_ptr);
    CALL(string)->unsafe(vm, empty_ptr);
    CALL(string)->unsafe(vm, string_ptr);
    CALL(string)->unsafe(vm, data_ptr);
    CALL(string)->unsafe(vm, list_ptr);
    CALL(string)->unsafe(vm, none_ptr);

    CALL(file)->alloc(vm, zero_ptr, zero_ptr);
    CALL(file)->alloc(vm, string_ptr, zero_ptr);
    CALL(file)->alloc(vm, data_ptr, zero_ptr);
    CALL(file)->alloc(vm, list_ptr, zero_ptr);
    CALL(file)->alloc(vm, zero_ptr, string_ptr);
    CALL(file)->alloc(vm, zero_ptr, data_ptr);
    CALL(file)->alloc(vm, zero_ptr, list_ptr);
    CALL(file)->alloc(vm, string_ptr, string_ptr);
    CALL(file)->alloc(vm, data_ptr, null_ptr);
    CALL(file)->alloc(vm, list_ptr, null_ptr);
    CALL(file)->alloc(vm, string_ptr, data_ptr);
    CALL(file)->alloc(vm, string_ptr, empty_ptr);
    CALL(file)->alloc(vm, data_ptr, null_ptr);
    CALL(file)->alloc(vm, list_ptr, string_ptr);
    CALL(file)->alloc(vm, data_ptr, null_ptr);
    CALL(file)->alloc(vm, list_ptr, null_ptr);
    CALL(file)->alloc(vm, string_ptr, data_ptr);
    CALL(file)->alloc(vm, data_ptr, null_ptr);
    CALL(file)->alloc(vm, list_ptr, string_ptr);
    CALL(file)->alloc(vm, null_ptr, null_ptr);
    CALL(file)->alloc(vm, string_ptr, null_ptr);
    CALL(file)->alloc(vm, string_ptr, none_ptr);
    CALL(file)->alloc(vm, none_ptr, string_ptr);

    const u64* data_unsafe_ptr1 = CALL(data)->unsafe(vm, empty_ptr);
    const u64* data_unsafe_ptr2 = CALL(data)->unsafe(vm, null_ptr);
    const u64* data_unsafe_ptr3 = CALL(data)->unsafe(vm, string_ptr);
    const u64* data_unsafe_ptr4 = CALL(data)->unsafe(vm, list_ptr);
    const u64* data_unsafe_ptr5 = CALL(data)->unsafe(vm, none_ptr);

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

    u64 undefined_ptr = CALL(string)->copy(vm, list_ptr);

    RX_ASSERT(undefined_ptr == 0);
    RX_ASSERT(last_matched_ptr1 != 0);
    RX_ASSERT(last_matched_ptr2 != 0);
    RX_ASSERT(last_matched_ptr3 != 0);
#ifndef USE_GC
    CALL(string)->free(vm, last_matched_ptr1);
    CALL(string)->free(vm, last_matched_ptr2);
    CALL(string)->free(vm, last_matched_ptr3);
    CALL(string)->free(vm, last_matched_ptr2);
    CALL(string)->free(vm, last_matched_ptr3);
    CALL(string)->free(vm, null_ptr);
    CALL(string)->free(vm, zero_ptr);
    CALL(string)->free(vm, string_ptr);
    CALL(string)->free(vm, data_ptr3);
    CALL(string)->free(vm, list_ptr);
    CALL(string)->free(vm, empty_ptr);
    CALL(string)->free(vm, data_ptr);
    CALL(string)->free(vm, string_ptr);
    CALL(string)->free(vm, data_ptr3);
    CALL(string)->free(vm, empty_ptr);
    CALL(string)->free(vm, data_ptr);
    CALL(data)->free(vm, zero_ptr);
    CALL(data)->free(vm, data_ptr);
    CALL(data)->free(vm, data_ptr3);
    CALL(data)->free(vm, empty_ptr);
    CALL(data)->free(vm, null_ptr);
    CALL(data)->free(vm, data_ptr);
    CALL(data)->free(vm, list_ptr);
    CALL(data)->free(vm, data_ptr);
    CALL(data)->free(vm, data_ptr3);
    CALL(data)->free(vm, none_ptr);
    CALL(stack)->free(vm, zero_ptr);
    CALL(stack)->free(vm, data_ptr);
    CALL(stack)->free(vm, empty_ptr);
    CALL(stack)->free(vm, null_ptr);
    CALL(stack)->free(vm, data_ptr);
    CALL(stack)->free(vm, list_ptr);
    CALL(stack)->free(vm, data_ptr);
    CALL(stack)->free(vm, list_ptr);
    CALL(stack)->free(vm, none_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_strcat_alloc_load, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 zero_ptr = CALL(string)->load(vm, "\0");
    u64 char_ptr = CALL(string)->load(vm, "/");
    CALL(string)->strcat(vm, zero_ptr, char_ptr);
    RX_ASSERT(zero_ptr == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, char_ptr), "/") == 0);
#ifndef USE_GC
    CALL(string)->free(vm, zero_ptr);
    CALL(string)->free(vm, char_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_strcat_load_alloc_alloc, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 char_ptr = CALL(string)->load(vm, "/");
    u64 pattern_ptr = CALL(string)->load(vm, "\0");
    u64 empty_ptr = CALL(string)->load(vm, "\0");
    CALL(string)->strcat(vm, pattern_ptr, char_ptr);
    CALL(string)->strcpy(vm, empty_ptr, pattern_ptr);
    RX_ASSERT(pattern_ptr == 0);
    RX_ASSERT(empty_ptr == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, char_ptr), "/") == 0);
#ifndef USE_GC
    CALL(string)->free(vm, empty_ptr);
    CALL(string)->free(vm, pattern_ptr);
    CALL(string)->free(vm, char_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_strcat_load_load, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 char_ptr = CALL(string)->load(vm, "/");
    u64 pattern_ptr = CALL(string)->load(vm, "*");
    CALL(string)->strcat(vm, pattern_ptr, char_ptr);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, pattern_ptr), "*/") == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, char_ptr), "/") == 0);
#ifndef USE_GC
    CALL(string)->free(vm, pattern_ptr);
    CALL(string)->free(vm, char_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_strcpy_load_alloc, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "/");
    u64 empty_ptr = CALL(string)->load(vm, "\0");
    CALL(string)->strcpy(vm, empty_ptr, pattern_ptr);
    RX_ASSERT(empty_ptr == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, pattern_ptr), "/") == 0);
#ifndef USE_GC
    CALL(string)->free(vm, empty_ptr);
    CALL(string)->free(vm, pattern_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_strcpy_load_load, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 char_ptr = CALL(string)->load(vm, "/input.txt");
    u64 pattern_ptr = CALL(string)->load(vm, "*");
    CALL(string)->strcpy(vm, pattern_ptr, char_ptr);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, pattern_ptr), "/input.txt") == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, char_ptr), "/input.txt") == 0);
#ifndef USE_GC
    CALL(string)->free(vm, pattern_ptr);
    CALL(string)->free(vm, char_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_os_getenv, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 ui_mode_ptr = CALL(string)->load(vm, "ASDASD_UI_MODE");
    u64 file_path_ptr = CALL(os)->getenv(vm, ui_mode_ptr);
    RX_ASSERT(file_path_ptr == 0);
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
    const_vm_ptr vm = rx->ctx;
    u64 value = CALL(data)->alloc(vm, 0);
    RX_ASSERT(value == 0);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_data_alloc_0, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    u64 value = CALL(data)->alloc(vm, 0);
    RX_ASSERT(value == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_data_free, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(data)->free(vm, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_data_size, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    u64 value = CALL(data)->size(vm, 0);
    RX_ASSERT(value == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_data_unsafe, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    void* value = CALL(data)->unsafe(vm, 0);
    RX_ASSERT(value == 0);
}

/************/
/* file API */
/************/

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_file_alloc, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    u64 value = CALL(file)->alloc(vm, 0, 0);
    RX_ASSERT(value == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_load_file_alloc, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 file_path_ptr = CALL(string)->load(vm, "data");
    u64 mode_ptr = CALL(string)->load(vm, "rb");
    const_pointer_ptr const_ptr = CALL(virtual)->read(vm, file_path_ptr, TYPE_STRING);
    safe_void_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr->data;
    u8* data_ptr = safe_ptr.ptr;
    char* file_data = (char*)data_ptr;
    file_data[0] = 0;
    RX_ASSERT(data_ptr != 0);
    u64 value = CALL(file)->alloc(vm, file_path_ptr, mode_ptr);
    RX_ASSERT(value == 0);
    CALL(string)->free(vm, file_path_ptr);
    CALL(string)->free(vm, mode_ptr);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_file_free, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(file)->free(vm, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_file_data, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    u64 value = CALL(file)->data(vm, 0);
    RX_ASSERT(value == 0);
}

/**************/
/* object API */
/**************/

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_object_alloc, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    u64 value = CALL(object)->alloc(vm, 0);
    RX_ASSERT(value == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_object_free, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(object)->free(vm, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_object_unsafe, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    void* value = CALL(object)->unsafe(vm, 0);
    RX_ASSERT(value == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_object_load, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    u64 value = CALL(object)->load(vm, 0, 0);
    RX_ASSERT(value == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_object_size, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    u64 value = CALL(object)->size(vm, 0);
    RX_ASSERT(value == 0);
}

/*************/
/* stack API */
/*************/

RX_TEST_CASE(tests_pointer_v1, test_vm_stack_alloc, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    u64 value = CALL(stack)->alloc(vm);
    RX_ASSERT(value == 0);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_stack_free, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(stack)->free(vm, 0);
    RX_ASSERT(0 != 1);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_stack_peek, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    u64 value = CALL(stack)->peek(vm, 0);
    RX_ASSERT(value == 0);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_stack_peekn, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    u64 value = CALL(stack)->peekn(vm, 0, 0);
    RX_ASSERT(value == 0);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_stack_pop, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    u64 value = CALL(stack)->pop(vm, 0);
    RX_ASSERT(value == 0);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_stack_popn, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    u64 value = CALL(stack)->popn(vm, 0, 0);
    RX_ASSERT(value == 0);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_stack_push, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(stack)->push(vm, 0, 0);
    RX_ASSERT(0 != 1);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_stack_size, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    u64 value = CALL(stack)->size(vm, 0);
    RX_ASSERT(value == 0);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_stack_release, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(stack)->release(vm, 0);
    RX_ASSERT(0 != 1);
}

/**************/
/* string API */
/**************/

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_free, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(string)->free(vm, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_copy, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(string)->copy(vm, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_strcpy, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(string)->strcpy(vm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_strcat, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(string)->strcat(vm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_strrchr, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(string)->strrchr(vm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_strchr, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(string)->strchr(vm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_match, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(string)->match(vm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_offset, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(string)->offset(vm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_load, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(string)->load(vm, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_put_char, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(string)->put_char(vm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_unsafe, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(string)->unsafe(vm, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_size, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    CALL(string)->size(vm, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_size_0, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(string)->size(vm, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_lessthan, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(string)->lessthan(vm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_greaterthan, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(string)->greaterthan(vm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_equals, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(string)->equals(vm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_compare, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(string)->compare(vm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_left, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(string)->left(vm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_strncpy, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(string)->strncpy(vm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_left_strncpy, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(string)->left_strncpy(vm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_right, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(string)->right(vm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_move_left, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(string)->move_left(vm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_move_right, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(string)->move_right(vm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_strcmp, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(string)->strcmp(vm, 0, 0);
    RX_ASSERT(0 != 1);
}

/**********************/
/* string_pointer API */
/**********************/

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_string_pointer, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(string_pointer)->free(vm, 0);
    RX_ASSERT(0 != 1);
}

/************/
/* user API */
/************/

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_user_alloc, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    u64 value = CALL(user)->alloc(vm);
    RX_ASSERT(value == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_user_free, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(user)->free(vm, 0);
    RX_ASSERT(0 != 1);
}

/**********/
/* os API */
/**********/

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_os_getenv, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    u64 value = CALL(os)->getenv(vm, 0);
    RX_ASSERT(value == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_os_getcwd, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    u64 value = CALL(os)->getcwd(vm);
    RX_ASSERT(value == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_os_putc, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(os)->putc(vm, 0);
    RX_ASSERT(0 != 1);
}

/***************/
/* pointer API */
/***************/

RX_TEST_CASE(tests_pointer_v1, test_vm_read_data, .fixture = test_fixture_pointer) {
    const void* data_ptr = CALL(pointer)->read(0, 0);
    RX_ASSERT(data_ptr == 0);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_alloc_data_read_data, .fixture = test_fixture_pointer) {
    const void* data_ptr = CALL(pointer)->read(0, 0);
    RX_ASSERT(data_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_read_data_1_0, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    const void* data_ptr = CALL(pointer)->read(1, 0);
    RX_ASSERT(data_ptr == 0);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_alloc, .fixture = test_fixture_pointer) {
    void* data_ptr = 0;
    u64 address = CALL(pointer)->alloc(&data_ptr, 0, TYPE_STRING);
    RX_ASSERT(address == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_alloc_safe_0_0_0, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    u64 address = CALL(pointer)->alloc(0, 0, 0);
    RX_ASSERT(address == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_alloc_0_0, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    void* data_ptr = 0;
    u64 address = CALL(pointer)->alloc(&data_ptr, 0, 0);
    RX_ASSERT(address == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_alloc_1_0, .fixture = test_fixture) {
    void* data_ptr = 0;
    u64 address = CALL(pointer)->alloc(&data_ptr, 1, 0);
    RX_ASSERT(address == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_alloc_0_1, .fixture = test_fixture) {
    void* data_ptr;
    u64 address = CALL(pointer)->alloc(&data_ptr, 0, 1);
    RX_ASSERT(address == 0);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_alloc_safe, .fixture = test_fixture_pointer) {
    void* data_ptr = 0;
    u64 address = CALL(pointer)->copy(&data_ptr, 0, 0, TYPE_STRING);
    RX_ASSERT(address == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_alloc_safe_0_0_0, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    u64 address = CALL(pointer)->copy(0, 0, 0, 0);
    RX_ASSERT(address == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_alloc_safe_0_0, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    void* data_ptr = 0;
    u64 address = CALL(pointer)->copy(&data_ptr, 0, 0, 0);
    RX_ASSERT(address == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_alloc_safe_1_0, .fixture = test_fixture) {
    void* data_ptr = 0;
    u64 address = CALL(pointer)->copy(&data_ptr, 1, 0, 0);
    RX_ASSERT(address == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_virtual_pointer_0_1, .fixture = test_fixture) {
    void* data_ptr;
    u64 address = CALL(pointer)->copy(&data_ptr, 0, 0, 1);
    RX_ASSERT(address == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_release_data_1_0, .fixture = test_fixture) {
    u64 result = CALL(pointer)->free(1, 0);
    RX_ASSERT(result == FALSE);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_copy_ch_2_1_02, .fixture = test_fixture) {
    const char* ch = "hello, world!";
    u64 address1 = CALL(pointer)->copy(ch, strlen(ch), 0, TYPE_OBJECT);
    u64 address2 = CALL(pointer)->copy(ch, 13, 1, TYPE_OBJECT);
    RX_ASSERT(address1 != 0);
    RX_ASSERT(address2 != 0);
    const char* data1 = CALL(pointer)->read(address1, TYPE_OBJECT);
    const char* data2 = CALL(pointer)->read(address2, TYPE_OBJECT);
    RX_ASSERT(data1 != 0);
    RX_ASSERT(data2[1] == 0);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_alloc_safe, .fixture = test_fixture) {
    CALL(pointer)->copy(0, 0, 0, 0);
    RX_ASSERT(0 != 1);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_alloc_safe_0_0_0_0, .fixture = test_fixture_pointer) {
    CALL(pointer)->copy(0, 0, 0, 0);
    RX_ASSERT(0 != 1);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_alloc_safe_ch_0_0_0, .fixture = test_fixture) {
    const char* ch = "hello, world!";
    CALL(pointer)->copy(ch, 0, 0, 0);
    RX_ASSERT(0 != 1);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_alloc_safe_ch_1_0_0, .fixture = test_fixture) {
    const char* ch = "hello, world!";
    u64 result = CALL(pointer)->copy(ch, 1, 0, 0);
    RX_ASSERT(result == 0);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_alloc_safe_ch_1_1_0, .fixture = test_fixture) {
    const char* ch = "hello, world!";
    u64 result = CALL(pointer)->copy(ch, 1, 1, 0);
    RX_ASSERT(result == 0);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_alloc_safe_ch_1_2_0, .fixture = test_fixture) {
    const char* ch = "hello, world!";
    u64 result = CALL(pointer)->copy(ch, 1, 2, 0);
    RX_ASSERT(result == 0);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_free, .fixture = test_fixture_pointer) {
    CALL(vm)->release(0);
    RX_ASSERT(0 != 1);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_release_type, .fixture = test_fixture_pointer) {
    CALL(pointer)->free(0, 0);
    RX_ASSERT(0 != 1);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_alloc_safe_0, .fixture = test_fixture) {
    CALL(pointer)->copy(0, 0, 0, 0);
    RX_ASSERT(0 != 1);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_release_0, .fixture = test_fixture) {
    CALL(vm)->release(0);
    RX_ASSERT(0 != 1);
}

RX_TEST_CASE(tests_pointer_v1, test_vm_pointer_read_safe_alloc_safe_size_offset, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    const char* src = "hello, world!";
    u64 address = CALL(string)->load(vm, src);
    const_pointer_ptr const_ptr = CALL(virtual)->read(vm, address, TYPE_STRING);
    safe_void_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr->data;
    u8* data_ptr = safe_ptr.ptr;
    char* string_data = (char*)data_ptr;
    RX_ASSERT(string_data != 0);
}

/***************/
/* virtual API */
/***************/

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_virtual_init, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(system)->init(vm, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_virtual_destroy, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(system)->destroy(vm);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_virtual_alloc_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    CALL(virtual)->alloc(vm, 1, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_virtual_alloc, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(virtual)->alloc(vm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_virtual_free, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(virtual)->free(vm, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_virtual_read, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(virtual)->type(vm, 0);
    RX_ASSERT(0 != 1);
}
/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_virtual_read_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    CALL(virtual)->type(vm, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_virtual_read_type, .fixture = test_fixture) {
    const_vm_ptr vm = 0;
    CALL(virtual)->read(vm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_virtual_read_type_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    CALL(virtual)->read(vm, 0, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_vm_virtual_read_type_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    CALL(virtual)->read(vm, 1, 0);
    RX_ASSERT(0 != 1);
}

/* ---------------------------------------------------------------------- */
/* END SECTION: zero memory checks / uninitialized calls                  */
/* ---------------------------------------------------------------------- */

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_os_getenv_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 file_path_ptr = CALL(os)->getenv(vm, 0);
    RX_ASSERT(file_path_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_os_getenv_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(vm);
    u64 file_path_ptr = CALL(os)->getenv(vm, list_ptr);
    RX_ASSERT(file_path_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_load_open_file_close_file, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 file_path_ptr = CALL(os)->getcwd(vm);
    u64 file_name_ptr = CALL(string)->load(vm, "data/file.txt");
    CALL(string)->strcat(vm, file_path_ptr, file_name_ptr);
    CALL(string)->free(vm, file_name_ptr);
    u64 mode_ptr = CALL(string)->load(vm, "rb");
    u64 f_ptr = CALL(file)->alloc(vm, file_path_ptr, mode_ptr);
    u64 data_ptr = CALL(file)->data(vm, f_ptr);
    RX_ASSERT(f_ptr == 0);
    RX_ASSERT(data_ptr == 0);
#ifndef USE_GC
    CALL(file)->free(vm, f_ptr);
    CALL(data)->free(vm, data_ptr);
    CALL(string)->free(vm, mode_ptr);
    CALL(string)->free(vm, file_name_ptr);
    CALL(string)->free(vm, file_path_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_offset_strcpy, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 name_ptr = CALL(string)->load(vm, "name");
    u64 at_ptr = CALL(string)->load(vm, "@");
    u64 domain_ptr = CALL(string)->load(vm, "domain.org");
    CALL(string)->strcat(vm, name_ptr, at_ptr);
    CALL(string)->strcat(vm, name_ptr, domain_ptr);

    u64 path_ptr1 = CALL(string)->load(vm, "name@domain.org");
    u64 path_ptr2 = CALL(string)->copy(vm, at_ptr);

    u64 domain_name = CALL(string)->offset(vm, path_ptr1, path_ptr2);
    CALL(string)->strcpy(vm, name_ptr, domain_name);

    char* domain = CALL(string)->unsafe(vm, domain_ptr);
    char* name = CALL(string)->unsafe(vm, name_ptr);
    RX_ASSERT(strcmp(domain, "domain.org") == 0);
    RX_ASSERT(strcmp(name, "domain.org") == 0);

#ifndef USE_GC
    CALL(string)->free(vm, domain_name);
    CALL(string)->free(vm, name_ptr);
    CALL(string)->free(vm, path_ptr1);
    CALL(string)->free(vm, path_ptr2);
    CALL(string)->free(vm, name_ptr);
    CALL(string)->free(vm, domain_ptr);
    CALL(string)->free(vm, at_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_offset_strcpy_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 name_ptr = CALL(string)->load(vm, "name");
    u64 at_ptr = CALL(string)->load(vm, "@");
    u64 domain_ptr = CALL(string)->load(vm, "domain.org");
    CALL(string)->strcat(vm, name_ptr, at_ptr);
    CALL(string)->strcat(vm, name_ptr, domain_ptr);

    u64 path_ptr1 = CALL(string)->load(vm, "name@domain.org");
    u64 path_ptr2 = CALL(string)->copy(vm, at_ptr);

    u64 domain_name = CALL(string)->offset(vm, path_ptr1, path_ptr2);
    CALL(string)->free(vm, path_ptr1);
    CALL(string)->strcpy(vm, name_ptr, domain_name);

    char* domain = CALL(string)->unsafe(vm, domain_ptr);
    char* name = CALL(string)->unsafe(vm, name_ptr);
    RX_ASSERT(strcmp(domain, "domain.org") == 0);
    RX_ASSERT(strcmp(name, "name@domain.org") == 0);

#ifndef USE_GC
    CALL(string)->free(vm, domain_name);
    CALL(string)->free(vm, name_ptr);
    CALL(string)->free(vm, path_ptr2);
    CALL(string)->free(vm, name_ptr);
    CALL(string)->free(vm, domain_ptr);
    CALL(string)->free(vm, at_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_print_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 printing_ptr = CALL(string)->load(vm, "hello, world!");
    CALL(string)->free(vm, printing_ptr);
    CALL(os)->putc(vm, printing_ptr);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_sting_free_0, .fixture = test_fixture) {
    static virtual_methods mock_virtual_methods_definitions;
    /*api */
    memcpy(&mock_virtual_methods_definitions, virtual, sizeof(virtual_methods)); /* NOLINT: sizeof(virtual_methods*) */
    /* setup mocks */
    mock_virtual_methods_definitions.read = mock_virtual_read_zero;
    /* setup api endpoint */
    static const virtual_methods* mock_virtual_methods = &mock_virtual_methods_definitions;
    /* backup api calls */
    memcpy(&virtual_methods_ptr, &virtual, sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    /* init */
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    /* prepare to mock api calls */
    memcpy(&virtual, &mock_virtual_methods, sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    /* virtual_string->free fails in virtual->read call */
    CALL(string)->free(vm, 0);
    /* restore api calls */
    memcpy(&virtual, &virtual_methods_ptr, sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    /* cleanup */
    CALL(vm)->gc();
    /* destroy */
    CALL(vm)->destroy();
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_sting_free_ptr_0, .fixture = test_fixture) {
    static virtual_methods mock_virtual_methods_definitions;
    /*api */
    memcpy(&mock_virtual_methods_definitions, virtual, sizeof(virtual_methods)); /* NOLINT: sizeof(virtual_methods*) */
    /* setup mocks */
    mock_virtual_methods_definitions.read = mock_virtual_read_zero;
    /* setup api endpoint */
    static const virtual_methods* mock_virtual_methods = &mock_virtual_methods_definitions;
    /* backup api calls */
    memcpy(&virtual_methods_ptr, &virtual, sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    /* init */
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    /* prepare to mock api calls */
    memcpy(&virtual, &mock_virtual_methods, sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    /* virtual_string->free fails in virtual->read call */
    CALL(string)->free(vm, 0);
    /* restore api calls */
    memcpy(&virtual, &virtual_methods_ptr, sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_print_string_pointer_virtual_read_type, .fixture = test_fixture) {
    static virtual_methods mock_virtual_methods_definitions;
    /*api */
    memcpy(&mock_virtual_methods_definitions, virtual, sizeof(virtual_methods)); /* NOLINT: sizeof(virtual_methods*) */
    /* setup mocks */
    /* setup api endpoint */
    static const virtual_methods* mock_virtual_methods = &mock_virtual_methods_definitions;
    /* backup api calls */
    memcpy(&virtual_methods_ptr, &virtual, sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    /* init */
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 printing_ptr = CALL(string)->load(vm, "hello, world!");
    u64 comma_ptr = CALL(string)->load(vm, ",");
    u64 substring_index_ptr = CALL(string)->offset(vm, printing_ptr, comma_ptr);
    CALL(os)->putc(vm, substring_index_ptr);
    CALL(string)->free(vm, printing_ptr);
    /* prepare to mock api calls */
    memcpy(&virtual, &mock_virtual_methods, sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    CALL(string)->free(vm, substring_index_ptr);
    /* restore api calls */
    memcpy(&virtual, &virtual_methods_ptr, sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    CALL(string)->free(vm, comma_ptr);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_free_string_pointer_virtual_read_type, .fixture = test_fixture) {
    static virtual_methods mock_virtual_methods_definitions;
    /*api */
    memcpy(&mock_virtual_methods_definitions, virtual, sizeof(virtual_methods)); /* NOLINT: sizeof(virtual_methods*) */
    /* setup mocks */
    /* setup api endpoint */
    static const virtual_methods* mock_virtual_methods = &mock_virtual_methods_definitions;
    /* backup api calls */
    memcpy(&virtual_methods_ptr, &virtual, sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    /* init */
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 printing_ptr = CALL(string)->load(vm, "hello, world!");
    u64 comma_ptr = CALL(string)->load(vm, ",");
    u64 space_ptr = CALL(string)->load(vm, " ");
    u64 exclamation_ptr = CALL(string)->load(vm, "!");
    u64 substring_index_ptr = CALL(string)->offset(vm, printing_ptr, comma_ptr);
    u64 substring_space_ptr = CALL(string)->offset(vm, substring_index_ptr, space_ptr);
    u64 substring_exclamation_ptr = CALL(string)->offset(vm, substring_space_ptr, exclamation_ptr);
    CALL(os)->putc(vm, substring_index_ptr);
    /* prepare to mock api calls */
    memcpy(&virtual, &mock_virtual_methods, sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    CALL(string_pointer)->free(vm, substring_exclamation_ptr);
    CALL(string)->free(vm, substring_space_ptr);
    CALL(string_pointer)->free(vm, substring_index_ptr);
    CALL(string)->free(vm, printing_ptr);
    /* restore api calls */
    memcpy(&virtual, &virtual_methods_ptr, sizeof(virtual_methods*)); /* NOLINT: sizeof(virtual_methods*) */
    CALL(string)->free(vm, comma_ptr);
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_print, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 printing_ptr = CALL(string)->load(vm, "hello, world!");
    CALL(os)->putc(vm, printing_ptr);
#ifndef USE_GC
    CALL(string)->free(vm, printing_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_print_string_pointer, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 printing_ptr = CALL(string)->load(vm, "hello, world!");
    u64 comma_ptr = CALL(string)->load(vm, ",");
    u64 substring_index_ptr = CALL(string)->offset(vm, printing_ptr, comma_ptr);
    CALL(os)->putc(vm, substring_index_ptr);
#ifndef USE_GC
    CALL(string)->free(vm, printing_ptr);
    CALL(string)->free(vm, substring_index_ptr);
    CALL(string)->free(vm, comma_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_print_string_pointer_copy, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    const char* substring_expected = " world!";
    u64 printing_ptr = CALL(string)->load(vm, "hello, world!");
    u64 comma_ptr = CALL(string)->load(vm, ",");
    u64 substring_index_ptr = CALL(string)->offset(vm, printing_ptr, comma_ptr);
    u64 substring_ptr = CALL(string)->copy(vm, substring_index_ptr);

    char* substring_actual = CALL(string)->unsafe(vm, substring_ptr);
    RX_ASSERT(strcmp(substring_expected, substring_actual) == 0);

#ifndef USE_GC
    CALL(string)->free(vm, printing_ptr);
    CALL(string)->free(vm, comma_ptr);
    CALL(string)->free(vm, substring_index_ptr);
    CALL(string)->free(vm, substring_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_size, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(vm);
    u64 size_actual = CALL(string)->size(vm, list_ptr);
    RX_ASSERT(size_actual == 0);
#ifndef USE_GC
    CALL(stack)->free(vm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_unsafe, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr = CALL(stack)->alloc(vm);
    char* ptr_actual = CALL(string)->unsafe(vm, list_ptr);
    char* ptr_expected = 0;
    RX_ASSERT(ptr_expected == ptr_actual);
#ifndef USE_GC
    CALL(stack)->free(vm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_list_offset, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 string_ptr = CALL(string)->load(vm, "himem.sys");
    u64 list_ptr = CALL(stack)->alloc(vm);
    u64 size_actual = CALL(string)->offset(vm, string_ptr, list_ptr);
    u64 size_expected = 0;
    RX_ASSERT(size_expected == size_actual);
#ifndef USE_GC
    CALL(stack)->free(vm, list_ptr);
    CALL(string)->free(vm, string_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, teststring_pointer_unsafe, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 printing_ptr = CALL(string)->load(vm, "hello, world!");
    u64 comma_ptr = CALL(string)->load(vm, ",");
    u64 substring_index_ptr = CALL(string)->offset(vm, printing_ptr, comma_ptr);

    char* expected_value = CALL(string)->unsafe(vm, substring_index_ptr);
    const char* actual_value = " world!";
    RX_ASSERT(strcmp(expected_value, actual_value) == 0);

    CALL(string)->free(vm, substring_index_ptr);
    CALL(os)->putc(vm, substring_index_ptr);
#ifndef USE_GC
    CALL(string)->free(vm, printing_ptr);
    CALL(string)->free(vm, comma_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_pointer_size, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 printing_ptr = CALL(string)->load(vm, "hello, world!");
    u64 comma_ptr = CALL(string)->load(vm, ",");
    u64 substring_index_ptr = CALL(string)->offset(vm, printing_ptr, comma_ptr);
    u64 substring_ptr = CALL(string)->copy(vm, substring_index_ptr);

    u64 size_expected = strlen(" world!"); /* adds one 0 to termination byte */
    u64 size_actual = CALL(string)->size(vm, substring_ptr);
    RX_ASSERT(size_expected == size_actual);

#ifndef USE_GC
    CALL(string)->free(vm, printing_ptr);
    CALL(string)->free(vm, comma_ptr);
    CALL(string)->free(vm, substring_index_ptr);
    CALL(string)->free(vm, substring_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_string_offset_subsearch, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 printing_ptr = CALL(string)->load(vm, "hello, world!");
    u64 comma_ptr = CALL(string)->load(vm, ",");
    u64 w_ptr = CALL(string)->load(vm, "w");
    u64 substring_index_ptr1 = CALL(string)->offset(vm, printing_ptr, comma_ptr);
    u64 substring_index_ptr2 = CALL(string)->offset(vm, substring_index_ptr1, w_ptr);
    u64 substring_ptr = CALL(string)->copy(vm, substring_index_ptr2);

    u64 size_expected = strlen("orld!"); /* adds one 0 to termination byte */
    u64 size_actual = CALL(string)->size(vm, substring_ptr);
    RX_ASSERT(size_expected == size_actual);

#ifndef USE_GC
    CALL(string)->free(vm, printing_ptr);
    CALL(string)->free(vm, comma_ptr);
    CALL(string)->free(vm, substring_index_ptr1);
    CALL(string)->free(vm, substring_index_ptr2);
    CALL(string)->free(vm, substring_ptr);
    CALL(string)->free(vm, w_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_print_string_pointer_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 printing_ptr = CALL(string)->load(vm, "hello, world!");
    u64 comma_ptr = CALL(string)->load(vm, ",");
    u64 substring_index_ptr = CALL(string)->offset(vm, printing_ptr, comma_ptr);
    CALL(string)->free(vm, substring_index_ptr);
    CALL(os)->putc(vm, substring_index_ptr);
#ifndef USE_GC
    CALL(string)->free(vm, printing_ptr);
    CALL(string)->free(vm, comma_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_offset_strcat, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 name_ptr = CALL(string)->load(vm, "name");
    u64 at_ptr = CALL(string)->load(vm, "@");
    CALL(string)->strcat(vm, name_ptr, at_ptr);

    u64 path_ptr1 = CALL(string)->load(vm, "name@domain.org");
    u64 path_ptr2 = CALL(string)->copy(vm, at_ptr);

    u64 domain_name = CALL(string)->offset(vm, path_ptr1, path_ptr2);
    CALL(string)->strcat(vm, name_ptr, domain_name);

    char* domain = CALL(string)->unsafe(vm, path_ptr1);
    char* name = CALL(string)->unsafe(vm, name_ptr);
    u64 domain_len = strlen(domain);
    u64 name_len = strlen(name);
    RX_ASSERT(domain_len > 0);
    RX_ASSERT(name_len > 0);
    RX_ASSERT(domain_len == name_len);
    RX_ASSERT(strcmp(domain, name) == 0);

#ifndef USE_GC
    CALL(string)->free(vm, domain_name);
    CALL(string)->free(vm, name_ptr);
    CALL(string)->free(vm, path_ptr1);
    CALL(string)->free(vm, path_ptr2);
    CALL(string)->free(vm, name_ptr);
    CALL(string)->free(vm, at_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_file_read_invalid_type, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
#ifndef USE_GC
    u64 list_ptr = CALL(stack)->alloc(vm);
    u64 data_ptr = CALL(file)->data(vm, list_ptr);
    CALL(data)->free(vm, data_ptr);
    CALL(file)->free(vm, list_ptr);
    CALL(stack)->free(vm, list_ptr);
#else
    u64 list_ptr = CALL(stack)->alloc(vm);
    CALL(file)->data(vm, list_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_load_open_match_last_unsafe_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 file_path_ptr = CALL(os)->getcwd(vm);
    u64 file_name_ptr = CALL(string)->load(vm, "/all_english_words.txt//");
    CALL(string)->strcat(vm, file_path_ptr, file_name_ptr);
    u64 pattern_ptr = CALL(string)->load(vm, "//");
    u64 last_match_ptr = CALL(string)->offset(vm, file_path_ptr, pattern_ptr);
    CALL(string)->free(vm, pattern_ptr);
    CALL(string)->put_char(vm, last_match_ptr, '\0');
    CALL(string)->free(vm, last_match_ptr);
    CALL(string)->strcat(vm, file_path_ptr, file_name_ptr);
    CALL(string)->free(vm, file_name_ptr);
    u64 mode_ptr = CALL(string)->load(vm, "rb");
    u64 f_ptr = CALL(file)->alloc(vm, file_path_ptr, mode_ptr);
    CALL(string)->free(vm, file_path_ptr);
    CALL(string)->free(vm, mode_ptr);
    u64 data_ptr = CALL(file)->data(vm, f_ptr);
    CALL(file)->free(vm, f_ptr);
    CALL(os)->putc(vm, data_ptr);
#ifndef USE_GC
    CALL(data)->free(vm, data_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_load_open_match_last_unsafe_free_unsuppported_calls, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 ptr1 = CALL(string)->load(vm, "qwerty//u");
    u64 ptr2 = CALL(string)->load(vm, "asdfghjkl");
    u64 pattern_ptr = CALL(string)->load(vm, "/u");
    u64 last_match_ptr = CALL(string)->offset(vm, ptr1, pattern_ptr);
    CALL(string)->strcat(vm, last_match_ptr, ptr2);
    CALL(string)->free(vm, pattern_ptr);
    CALL(string)->put_char(vm, last_match_ptr, '\0');
#ifndef USE_GC
    CALL(string)->free(vm, ptr1);
    CALL(string)->free(vm, ptr2);
    CALL(string)->free(vm, last_match_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_user, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
#ifndef USE_GC
    u64 ptr1 = CALL(user)->alloc(vm);
    CALL(user)->free(vm, ptr1);
#else
    CALL(user)->alloc(vm);
#endif
}

/* test init */
RX_TEST_CASE(tests_pointer_v1, test_load_open_file_unsafe_hashtable, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 file_path_ptr = CALL(os)->getcwd(vm);
    u64 file_name_ptr = CALL(string)->load(vm, "/data/all_english_words.txt");
    CALL(string)->strcat(vm, file_path_ptr, file_name_ptr);
    CALL(string)->free(vm, file_name_ptr);
    u64 mode_ptr = CALL(string)->load(vm, "rb");
    u64 f_ptr = CALL(file)->alloc(vm, file_path_ptr, mode_ptr);
    if (f_ptr != 0) {
        u64 data_ptr = CALL(file)->data(vm, f_ptr);
        u64 list_ptr = CALL(stack)->alloc(vm);
        CALL(file)->free(vm, f_ptr);
        u8* file_data = CALL(data)->unsafe(vm, data_ptr);
        for (int i = 0; i < 100; i++) {
            u8* tmp = file_data;
            while (*tmp != 0 && *tmp != '\n') {
                tmp++;
            }
            *tmp++ = '\0';
            u64 string_ptr = CALL(string)->load(vm, (char*)file_data);
            CALL(stack)->push(vm, list_ptr, string_ptr);
            CALL(os)->putc(vm, string_ptr);
            file_data = tmp;
        }
        CALL(stack)->free(vm, list_ptr);
        CALL(data)->free(vm, data_ptr);
    }
#ifndef USE_GC
    CALL(string)->free(vm, mode_ptr);
    CALL(string)->free(vm, file_name_ptr);
    CALL(string)->free(vm, file_path_ptr);
#endif
}

RX_TEST_CASE(tests_pointer_v1, test_load_load_match_last, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 str_ptr = CALL(string)->load(vm, "Hello, world!");
    u64 ch_ptr = CALL(string)->load(vm, "z");
    u64 last_match_ptr = CALL(string)->offset(vm, str_ptr, ch_ptr);
    RX_ASSERT(last_match_ptr == 0);
#ifndef USE_GC
    CALL(string)->free(vm, last_match_ptr);
    CALL(string)->free(vm, str_ptr);
    CALL(string)->free(vm, ch_ptr);
#endif
}

/* internal */
static void parse_text(const_vm_ptr vm, u64 text_string_ptr);

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_init, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm_expected = rx->ctx;
    const_vm_ptr vm_actual = CALL(vm)->init(8);
    RX_ASSERT(vm_actual == vm_expected);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_load, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    CALL(string)->load(vm, "asdf");
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_init_string_load_9, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    CALL(string)->load(vm, "0");
    CALL(string)->load(vm, "1");
    CALL(string)->load(vm, "2");
    CALL(string)->load(vm, "3");
    CALL(string)->load(vm, "4");
    CALL(string)->load(vm, "5");
    CALL(string)->load(vm, "6");
    CALL(string)->load(vm, "7");
    CALL(string)->load(vm, "8");
    CALL(string)->load(vm, "9");
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_strings, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 text_string_ptr0 = CALL(string)->load(vm, "a");
    u64 text_string_ptr1 = CALL(string)->load(vm, "a\nb");
    u64 text_string_ptr2 = CALL(string)->load(vm, "ab\nabc\n");
    u64 text_string_ptr3 = CALL(string)->load(vm, "adadadsadsadad\ndad\nadsaddasaddad\nsad\n");
    u64 text_string_ptr4 = CALL(string)->load(vm, "ab\ndad\nabcd\nbcd\n");
    u64 text_string_ptr5 = CALL(string)->load(vm, "ab\nc\nabc\nbcd\n");
    u64 text_string_ptr6 = CALL(string)->load(vm, "abc\nabcd\nbcde\nabc\n");
    u64 text_string_ptr7 = CALL(string)->load(vm, "abc\n\n");
    parse_text(vm, 0);
    parse_text(vm, text_string_ptr0);
    parse_text(vm, text_string_ptr1);
    parse_text(vm, text_string_ptr2);
    parse_text(vm, text_string_ptr3);
    parse_text(vm, text_string_ptr4);
    parse_text(vm, text_string_ptr5);
    parse_text(vm, text_string_ptr6);
    parse_text(vm, text_string_ptr7);
    CALL(string)->free(vm, text_string_ptr0);
    CALL(string)->free(vm, text_string_ptr1);
    CALL(string)->free(vm, text_string_ptr2);
    CALL(string)->free(vm, text_string_ptr3);
    CALL(string)->free(vm, text_string_ptr4);
    CALL(string)->free(vm, text_string_ptr5);
    CALL(string)->free(vm, text_string_ptr6);
    CALL(string)->free(vm, text_string_ptr7);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_pointer_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    CALL(string_pointer)->free(vm, 1);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_pointer_lessthan_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "b");
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "abba");
    u64 quantum_str_ptr2 = CALL(string)->offset(vm, quantum_str_ptr1, pattern_ptr);
    u64 data_ptr = CALL(string)->lessthan(vm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(data_ptr != 0);
    CALL(string_pointer)->free(vm, data_ptr);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 error_ptr = CALL(string)->lessthan(vm, 0, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_0_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 error_ptr = CALL(string)->lessthan(vm, 0, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr = CALL(string)->load(vm, "");
    RX_ASSERT(quantum_str_ptr == 0);
    u64 error_ptr = CALL(string)->lessthan(vm, 1, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_1_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->lessthan(vm, 1, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_a_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "b");
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "abba");
    u64 quantum_str_ptr2 = CALL(string)->offset(vm, quantum_str_ptr1, pattern_ptr);
    u64 data_ptr = CALL(string)->lessthan(vm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(data_ptr != 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_b_a, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "b");
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "abba");
    u64 quantum_str_ptr2 = CALL(string)->offset(vm, quantum_str_ptr1, pattern_ptr);
    u64 error_ptr = CALL(string)->lessthan(vm, quantum_str_ptr2, quantum_str_ptr1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_1_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->lessthan(vm, 1, 2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_1_3, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->lessthan(vm, 1, 2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_2_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->lessthan(vm, 2, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_a_string_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    CALL(string)->free(vm, quantum_str_ptr2);
    u64 error_ptr = CALL(string)->lessthan(vm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_string_free_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    CALL(string)->free(vm, quantum_str_ptr1);
    u64 error_ptr = CALL(string)->lessthan(vm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_string_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    u64 list_ptr2 = CALL(stack)->alloc(vm);
    u64 error_ptr = CALL(string)->lessthan(vm, quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_lessthan_list_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr1 = CALL(stack)->alloc(vm);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "a");
    u64 error_ptr = CALL(string)->lessthan(vm, list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 error_ptr = CALL(string)->greaterthan(vm, 0, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_0_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 error_ptr = CALL(string)->greaterthan(vm, 0, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr = CALL(string)->load(vm, "");
    RX_ASSERT(quantum_str_ptr == 0);
    u64 error_ptr = CALL(string)->greaterthan(vm, 1, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_1_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->greaterthan(vm, 1, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_a_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "b");
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "abba");
    u64 quantum_str_ptr2 = CALL(string)->offset(vm, quantum_str_ptr1, pattern_ptr);
    u64 error_ptr = CALL(string)->greaterthan(vm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_b_a, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "b");
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "abba");
    u64 quantum_str_ptr2 = CALL(string)->offset(vm, quantum_str_ptr1, pattern_ptr);
    u64 data_ptr = CALL(string)->greaterthan(vm, quantum_str_ptr2, quantum_str_ptr1);
    RX_ASSERT(data_ptr != 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_1_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->greaterthan(vm, 1, 2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_1_3, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->greaterthan(vm, 1, 3);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_2_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->greaterthan(vm, 2, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_a_string_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    CALL(string)->free(vm, quantum_str_ptr2);
    u64 error_ptr = CALL(string)->greaterthan(vm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_string_free_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    CALL(string)->free(vm, quantum_str_ptr1);
    u64 error_ptr = CALL(string)->greaterthan(vm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_string_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 list_ptr2 = CALL(stack)->alloc(vm);
    u64 error_ptr = CALL(string)->greaterthan(vm, quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_greaterthan_list_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr1 = CALL(stack)->alloc(vm);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "a");
    u64 error_ptr = CALL(string)->greaterthan(vm, list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 error_ptr = CALL(string)->equals(vm, 0, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_0_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 error_ptr = CALL(string)->equals(vm, 0, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr = CALL(string)->load(vm, "");
    RX_ASSERT(quantum_str_ptr == 0);
    u64 error_ptr = CALL(string)->equals(vm, 1, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_1_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->equals(vm, 1, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_a_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "b");
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "abba");
    u64 quantum_str_ptr2 = CALL(string)->offset(vm, quantum_str_ptr1, pattern_ptr);
    u64 error_ptr = CALL(string)->equals(vm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_b_a, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "b");
    u64 source_ptr = CALL(string)->load(vm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(vm, source_ptr, pattern_ptr);
    u64 quantum_str_ptr2 = CALL(string)->offset(vm, source_ptr, pattern_ptr);
    u64 data_ptr = CALL(string)->equals(vm, quantum_str_ptr2, quantum_str_ptr1);
    RX_ASSERT(data_ptr != 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_1_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->equals(vm, 1, 2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_pointer_equals, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "b");
    u64 string_str = CALL(string)->load(vm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(vm, string_str, pattern_ptr);
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->offset(vm, string_str, pattern_ptr);
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 value = CALL(string)->equals(vm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(value == 1);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_1_3, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->equals(vm, 1, 3);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_2_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->equals(vm, 2, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_a_string_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    CALL(string)->free(vm, quantum_str_ptr2);
    u64 error_ptr = CALL(string)->equals(vm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_string_free_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    CALL(string)->free(vm, quantum_str_ptr1);
    u64 error_ptr = CALL(string)->equals(vm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_string_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    u64 list_ptr2 = CALL(stack)->alloc(vm);
    u64 error_ptr = CALL(string)->equals(vm, quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_equals_list_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr1 = CALL(stack)->alloc(vm);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "a");
    u64 error_ptr = CALL(string)->equals(vm, list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 error_ptr = CALL(string)->compare(vm, 0, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_0_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 error_ptr = CALL(string)->compare(vm, 0, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr = CALL(string)->load(vm, "");
    RX_ASSERT(quantum_str_ptr == 0);
    u64 error_ptr = CALL(string)->compare(vm, 1, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_1_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->compare(vm, 1, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_a_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "b");
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "abba");
    u64 quantum_str_ptr2 = CALL(string)->offset(vm, quantum_str_ptr1, pattern_ptr);
    u64 data_ptr = CALL(string)->compare(vm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(data_ptr != 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_b_a, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "b");
    u64 source_ptr = CALL(string)->load(vm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(vm, source_ptr, pattern_ptr);
    u64 quantum_str_ptr2 = CALL(string)->offset(vm, source_ptr, pattern_ptr);
    u64 data_ptr = CALL(string)->compare(vm, quantum_str_ptr2, quantum_str_ptr1);
    RX_ASSERT(data_ptr != 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_1_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->compare(vm, 1, 2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_1_3, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->compare(vm, 1, 3);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_2_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->compare(vm, 2, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_a_string_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    CALL(string)->free(vm, quantum_str_ptr2);
    u64 error_ptr = CALL(string)->compare(vm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_string_free_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    CALL(string)->free(vm, quantum_str_ptr1);
    u64 error_ptr = CALL(string)->compare(vm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_string_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    u64 list_ptr2 = CALL(stack)->alloc(vm);
    u64 error_ptr = CALL(string)->compare(vm, quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_compare_list_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr1 = CALL(stack)->alloc(vm);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "a");
    u64 error_ptr = CALL(string)->compare(vm, list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 error_ptr = CALL(string)->strcmp(vm, 0, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_0_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 error_ptr = CALL(string)->strcmp(vm, 0, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr = CALL(string)->load(vm, "");
    RX_ASSERT(quantum_str_ptr == 0);
    u64 error_ptr = CALL(string)->strcmp(vm, 1, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_1_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->strcmp(vm, 1, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_a_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "b");
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "abba");
    u64 quantum_str_ptr2 = CALL(string)->offset(vm, quantum_str_ptr1, pattern_ptr);
    u64 data_ptr = CALL(string)->strcmp(vm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(data_ptr != 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_b_a, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "b");
    u64 source_ptr = CALL(string)->load(vm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(vm, source_ptr, pattern_ptr);
    u64 quantum_str_ptr2 = CALL(string)->offset(vm, source_ptr, pattern_ptr);
    u64 data_ptr = CALL(string)->strcmp(vm, quantum_str_ptr2, quantum_str_ptr1);
    RX_ASSERT(data_ptr != 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_1_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->strcmp(vm, 1, 2);
    RX_ASSERT(error_ptr == 0);
}
/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_1_3, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->strcmp(vm, 1, 3);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_2_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->strcmp(vm, 2, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_a_string_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    CALL(string)->free(vm, quantum_str_ptr2);
    u64 error_ptr = CALL(string)->strcmp(vm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_string_free_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    CALL(string)->free(vm, quantum_str_ptr1);
    u64 error_ptr = CALL(string)->strcmp(vm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_string_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    u64 list_ptr2 = CALL(stack)->alloc(vm);
    u64 error_ptr = CALL(string)->strcmp(vm, quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strcmp_list_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr1 = CALL(stack)->alloc(vm);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "a");
    u64 error_ptr = CALL(string)->strcmp(vm, list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 error_ptr = CALL(string)->move_right(vm, 0, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_0_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 error_ptr = CALL(string)->move_right(vm, 0, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr = CALL(string)->load(vm, "");
    RX_ASSERT(quantum_str_ptr == 0);
    u64 error_ptr = CALL(string)->move_right(vm, 1, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_1_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->move_right(vm, 1, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_a_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "abba");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 error_ptr = CALL(string)->move_right(vm, quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_b_a, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "a");
    u64 source_ptr = CALL(string)->load(vm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(vm, source_ptr, pattern_ptr);
    u64 error_ptr = CALL(string)->move_right(vm, quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_a_b_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "b");
    u64 source_ptr = CALL(string)->load(vm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(vm, source_ptr, pattern_ptr);
    u64 data_ptr = CALL(string)->move_right(vm, quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_a_b_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "a");
    u64 source_ptr = CALL(string)->load(vm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(vm, source_ptr, pattern_ptr);
    u64 error_ptr = CALL(string)->move_right(vm, quantum_str_ptr1, 1);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, quantum_str_ptr1), "") == 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_a_b_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "b");
    u64 source_ptr = CALL(string)->load(vm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(vm, source_ptr, pattern_ptr);
    CALL(string)->free(vm, source_ptr);
    u64 error_ptr = CALL(string)->move_right(vm, quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_1_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->move_right(vm, 1, 2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_1_3, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->move_right(vm, 1, 3);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_2_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->move_right(vm, 2, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_a_string_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    CALL(string)->free(vm, quantum_str_ptr2);
    u64 error_ptr = CALL(string)->move_right(vm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_string_free_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    CALL(string)->free(vm, quantum_str_ptr1);
    u64 error_ptr = CALL(string)->move_right(vm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_string_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    u64 list_ptr2 = CALL(stack)->alloc(vm);
    u64 error_ptr = CALL(string)->move_right(vm, quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_right_list_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr1 = CALL(stack)->alloc(vm);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "a");
    u64 error_ptr = CALL(string)->move_right(vm, list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 error_ptr = CALL(string)->right(vm, 0, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_0_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 error_ptr = CALL(string)->right(vm, 0, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr = CALL(string)->load(vm, "");
    RX_ASSERT(quantum_str_ptr == 0);
    u64 error_ptr = CALL(string)->right(vm, 1, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_1_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 data_ptr = CALL(string)->right(vm, quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, data_ptr), "") == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_a_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "abba");
    u64 error_ptr = CALL(string)->right(vm, quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr != 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_b_a, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "a");
    u64 source_ptr = CALL(string)->load(vm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(vm, source_ptr, pattern_ptr);
    u64 error_ptr = CALL(string)->right(vm, quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_a_b_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "b");
    u64 source_ptr = CALL(string)->load(vm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(vm, source_ptr, pattern_ptr);
    u64 data_ptr = CALL(string)->right(vm, quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_a_b_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "a");
    u64 source_ptr = CALL(string)->load(vm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(vm, source_ptr, pattern_ptr);
    u64 error_ptr = CALL(string)->right(vm, quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, quantum_str_ptr1), "") == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_a_b_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "b");
    u64 source_ptr = CALL(string)->load(vm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(vm, source_ptr, pattern_ptr);
    CALL(string)->free(vm, source_ptr);
    u64 error_ptr = CALL(string)->right(vm, quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_1_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->right(vm, 1, 2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_1_3, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->right(vm, 1, 3);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_2_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "");
    u64 error_ptr = CALL(string)->right(vm, quantum_str_ptr2, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_a_string_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    CALL(string)->free(vm, quantum_str_ptr2);
    u64 error_ptr = CALL(string)->right(vm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_string_free_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    CALL(string)->free(vm, quantum_str_ptr1);
    u64 error_ptr = CALL(string)->right(vm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_string_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 list_ptr2 = CALL(stack)->alloc(vm);
    u64 error_ptr = CALL(string)->right(vm, quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_right_list_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr1 = CALL(stack)->alloc(vm);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "a");
    u64 error_ptr = CALL(string)->right(vm, list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 error_ptr = CALL(string)->move_left(vm, 0, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_0_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 error_ptr = CALL(string)->move_left(vm, 0, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr = CALL(string)->load(vm, "");
    RX_ASSERT(quantum_str_ptr == 0);
    u64 error_ptr = CALL(string)->move_left(vm, 1, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_1_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->move_left(vm, 1, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_a_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "b");
    u64 source_ptr = CALL(string)->load(vm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(vm, source_ptr, pattern_ptr);
    u64 error_ptr = CALL(string)->move_left(vm, quantum_str_ptr1, 4);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_b_a, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "a");
    u64 source_ptr = CALL(string)->load(vm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(vm, source_ptr, pattern_ptr);
    u64 data_ptr = CALL(string)->move_left(vm, quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_a_b_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "b");
    u64 source_ptr = CALL(string)->load(vm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(vm, source_ptr, pattern_ptr);
    u64 data_ptr = CALL(string)->move_left(vm, quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_a_b_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "a");
    u64 source_ptr = CALL(string)->load(vm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(vm, source_ptr, pattern_ptr);
    u64 data_ptr = CALL(string)->move_left(vm, quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, quantum_str_ptr1), "a") == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_a_b_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "b");
    u64 source_ptr = CALL(string)->load(vm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(vm, source_ptr, pattern_ptr);
    CALL(string)->free(vm, source_ptr);
    u64 error_ptr = CALL(string)->move_left(vm, quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_1_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->move_left(vm, 1, 2);
    RX_ASSERT(error_ptr == 0);
}
/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_1_3, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->move_left(vm, 1, 3);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_2_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->move_left(vm, 2, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_a_string_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    CALL(string)->free(vm, quantum_str_ptr2);
    u64 error_ptr = CALL(string)->move_left(vm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_string_free_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    CALL(string)->free(vm, quantum_str_ptr1);
    u64 error_ptr = CALL(string)->move_left(vm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_string_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    u64 list_ptr2 = CALL(stack)->alloc(vm);
    u64 error_ptr = CALL(string)->move_left(vm, quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_move_left_list_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr1 = CALL(stack)->alloc(vm);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "a");
    u64 error_ptr = CALL(string)->move_left(vm, list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 error_ptr = CALL(string)->left_strncpy(vm, 0, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_0_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 error_ptr = CALL(string)->left_strncpy(vm, 0, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr = CALL(string)->load(vm, "");
    RX_ASSERT(quantum_str_ptr == 0);
    u64 error_ptr = CALL(string)->left_strncpy(vm, 1, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_1_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->left_strncpy(vm, 1, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_a_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "b");
    u64 source_ptr = CALL(string)->load(vm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(vm, source_ptr, pattern_ptr);
    u64 error_ptr = CALL(string)->left_strncpy(vm, quantum_str_ptr1, 4);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_b_a, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "c");
    u64 source_ptr = CALL(string)->load(vm, "abbc");
    u64 quantum_str_ptr1 = CALL(string)->offset(vm, source_ptr, pattern_ptr);
    u64 data_ptr = CALL(string)->left_strncpy(vm, quantum_str_ptr1, 3);
    RX_ASSERT(quantum_str_ptr1 != 0);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, quantum_str_ptr1), "") == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, data_ptr), "bbc") == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_a_b_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "b");
    u64 source_ptr = CALL(string)->load(vm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(vm, source_ptr, pattern_ptr);
    u64 data_ptr = CALL(string)->left_strncpy(vm, quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_a_b_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "a");
    u64 source_ptr = CALL(string)->load(vm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(vm, source_ptr, pattern_ptr);
    u64 data_ptr = CALL(string)->left_strncpy(vm, quantum_str_ptr1, 1);
    RX_ASSERT(quantum_str_ptr1 != 0);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, quantum_str_ptr1), "") == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, data_ptr), "a") == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_a_b_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "b");
    u64 source_ptr = CALL(string)->load(vm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(vm, source_ptr, pattern_ptr);
    CALL(string)->free(vm, source_ptr);
    u64 error_ptr = CALL(string)->left_strncpy(vm, quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_1_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->left_strncpy(vm, 1, 2);
    RX_ASSERT(error_ptr == 0);
}
/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_1_3, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->left_strncpy(vm, 1, 3);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_2_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->left_strncpy(vm, 2, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_a_string_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    CALL(string)->free(vm, quantum_str_ptr2);
    u64 error_ptr = CALL(string)->left_strncpy(vm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_string_free_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    CALL(string)->free(vm, quantum_str_ptr1);
    u64 error_ptr = CALL(string)->left_strncpy(vm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_string_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 list_ptr2 = CALL(stack)->alloc(vm);
    u64 error_ptr = CALL(string)->left_strncpy(vm, quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_strncpy_list_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr1 = CALL(stack)->alloc(vm);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->left_strncpy(vm, list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 error_ptr = CALL(string)->left(vm, 0, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_0_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 error_ptr = CALL(string)->left(vm, 0, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr = CALL(string)->load(vm, "");
    RX_ASSERT(quantum_str_ptr == 0);
    u64 error_ptr = CALL(string)->left(vm, 1, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_1_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->left(vm, 1, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_a_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "b");
    u64 source_ptr = CALL(string)->load(vm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(vm, source_ptr, pattern_ptr);
    u64 error_ptr = CALL(string)->left(vm, quantum_str_ptr1, 4);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_b_a, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "c");
    u64 source_ptr = CALL(string)->load(vm, "abbc");
    u64 quantum_str_ptr1 = CALL(string)->offset(vm, source_ptr, pattern_ptr);
    u64 data_ptr = CALL(string)->left(vm, quantum_str_ptr1, 3);
    RX_ASSERT(quantum_str_ptr1 != 0);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, quantum_str_ptr1), "") == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, data_ptr), "bbc") == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_a_b_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "b");
    u64 source_ptr = CALL(string)->load(vm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(vm, source_ptr, pattern_ptr);
    u64 data_ptr = CALL(string)->left(vm, quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_a_b_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "a");
    u64 source_ptr = CALL(string)->load(vm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(vm, source_ptr, pattern_ptr);
    u64 data_ptr = CALL(string)->left(vm, quantum_str_ptr1, 1);
    RX_ASSERT(quantum_str_ptr1 != 0);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, quantum_str_ptr1), "") == 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, data_ptr), "a") == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_a_b_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "b");
    u64 source_ptr = CALL(string)->load(vm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(vm, source_ptr, pattern_ptr);
    CALL(string)->free(vm, source_ptr);
    u64 error_ptr = CALL(string)->left(vm, quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_1_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->left(vm, 1, 2);
    RX_ASSERT(error_ptr == 0);
}
/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_1_3, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->left(vm, 1, 3);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_2_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->left(vm, 2, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_a_string_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    CALL(string)->free(vm, quantum_str_ptr2);
    u64 error_ptr = CALL(string)->left(vm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
    CALL(string)->free(vm, quantum_str_ptr2);
    CALL(vm)->gc();
    CALL(string)->free(vm, quantum_str_ptr2);
    CALL(vm)->destroy();
    CALL(string)->free(vm, quantum_str_ptr2);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_string_free_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    CALL(string)->free(vm, quantum_str_ptr1);
    u64 error_ptr = CALL(string)->left(vm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_string_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    u64 list_ptr2 = CALL(stack)->alloc(vm);
    u64 error_ptr = CALL(string)->left(vm, quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_left_list_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr1 = CALL(stack)->alloc(vm);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "a");
    u64 error_ptr = CALL(string)->left(vm, list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_0_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 error_ptr = CALL(string)->strncpy(vm, 0, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_0_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 error_ptr = CALL(string)->strncpy(vm, 0, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_1_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr = CALL(string)->load(vm, "");
    RX_ASSERT(quantum_str_ptr == 0);
    u64 error_ptr = CALL(string)->strncpy(vm, 1, 0);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_1_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->strncpy(vm, 1, 1);
    RX_ASSERT(error_ptr == 3);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_a_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "b");
    u64 source_ptr = CALL(string)->load(vm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(vm, source_ptr, pattern_ptr);
    u64 error_ptr = CALL(string)->strncpy(vm, quantum_str_ptr1, 2);
    RX_ASSERT(error_ptr == 0);
    u64 data_ptr = CALL(string)->strncpy(vm, quantum_str_ptr1, 1);
    u64 str_ptr1 = CALL(string)->offset(vm, source_ptr, data_ptr);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(str_ptr1 != 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, str_ptr1), "") == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_b_a, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "b");
    u64 source_ptr = CALL(string)->load(vm, "abcd");
    u64 quantum_str_ptr1 = CALL(string)->offset(vm, source_ptr, pattern_ptr);
    u64 data_ptr = CALL(string)->strncpy(vm, quantum_str_ptr1, 2);
    RX_ASSERT(data_ptr != 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_a_b_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "b");
    u64 source_ptr = CALL(string)->load(vm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(vm, source_ptr, pattern_ptr);
    u64 data_ptr = CALL(string)->strncpy(vm, quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr != 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_a_b_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "a");
    u64 source_ptr = CALL(string)->load(vm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(vm, source_ptr, pattern_ptr);
    u64 data_ptr = CALL(string)->strncpy(vm, quantum_str_ptr1, 1);
    RX_ASSERT(data_ptr == 0);
    RX_ASSERT(quantum_str_ptr1 != 0);
    RX_ASSERT(strcmp(CALL(string)->unsafe(vm, quantum_str_ptr1), "") == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_a_b_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "b");
    u64 source_ptr = CALL(string)->load(vm, "abba");
    u64 quantum_str_ptr1 = CALL(string)->offset(vm, source_ptr, pattern_ptr);
    CALL(string)->free(vm, source_ptr);
    u64 error_ptr = CALL(string)->strncpy(vm, quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_a_b_2_overflow, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 pattern_ptr = CALL(string)->load(vm, "h");
    u64 source_ptr = CALL(string)->load(vm, "abcdefgh");
    u64 quantum_str_ptr1 = CALL(string)->offset(vm, source_ptr, pattern_ptr);
    u64 error_ptr = CALL(string)->strncpy(vm, quantum_str_ptr1, 1);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_1_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->strncpy(vm, 1, 2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_1_3, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->strncpy(vm, 1, 3);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_2_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 data_ptr = CALL(string)->strncpy(vm, 2, 1);
    RX_ASSERT(data_ptr != 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_a_string_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    CALL(string)->free(vm, quantum_str_ptr2);
    u64 error_ptr = CALL(string)->strncpy(vm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_string_free_b, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "b");
    RX_ASSERT(quantum_str_ptr2 != 0);
    CALL(string)->free(vm, quantum_str_ptr1);
    u64 error_ptr = CALL(string)->strncpy(vm, quantum_str_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_string_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 quantum_str_ptr1 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr1 != 0);
    u64 list_ptr2 = CALL(stack)->alloc(vm);
    u64 error_ptr = CALL(string)->strncpy(vm, quantum_str_ptr1, list_ptr2);
    RX_ASSERT(error_ptr == 0);
}

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_string_strncpy_list_string, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr vm = rx->ctx;
    u64 list_ptr1 = CALL(stack)->alloc(vm);
    u64 quantum_str_ptr2 = CALL(string)->load(vm, "a");
    RX_ASSERT(quantum_str_ptr2 != 0);
    u64 error_ptr = CALL(string)->strncpy(vm, list_ptr1, quantum_str_ptr2);
    RX_ASSERT(error_ptr == 0);
}

static void parse_text(const_vm_ptr vm, u64 text_string_ptr) {
    u64 gc_ptr = CALL(stack)->alloc(vm);
    u64 text_size = CALL(string)->size(vm, text_string_ptr);
    if (text_string_ptr == 0) {
        return;
    }
    u64 list_ptr = CALL(stack)->alloc(vm);
    char* text = CALL(string)->unsafe(vm, text_string_ptr);
    char* tmp = text;
    while (*tmp != 0 && text_size > 0) {
        while (*tmp != 0 && *tmp != '\n' && text_size > 0) {
            tmp++;
            text_size--;
        }
        if (text_size == 0) {
            CALL(stack)->free(vm, list_ptr);
            return;
        }
        *tmp++ = '\0';
        text_size--;
        u64 string_ptr = CALL(string)->load(vm, text);
        CALL(stack)->push(vm, list_ptr, string_ptr);
        text = tmp;
    }
    u64 data_ptr = 0;
    u64 list_data_ptr = CALL(stack)->alloc(vm);
    while ((data_ptr = CALL(stack)->pop(vm, list_ptr)) != 0) {
        CALL(stack)->push(vm, list_data_ptr, data_ptr);
    }
    CALL(stack)->free(vm, list_ptr);
    CALL(stack)->push(vm, gc_ptr, list_data_ptr);
    u64 quit = 0;
    while (quit == 0) {
        u64 string_ptr = CALL(stack)->pop(vm, list_data_ptr);
        if (CALL(string)->size(vm, string_ptr) == 0) {
            quit = 1;
            continue;
        }
        CALL(os)->putc(vm, string_ptr);
        u64 pattern_ptr = CALL(stack)->pop(vm, list_data_ptr);
        if (CALL(string)->size(vm, pattern_ptr) == 0) {
            quit = 1;
            continue;
        }
        CALL(os)->putc(vm, pattern_ptr);
        u64 size = CALL(string)->size(vm, pattern_ptr);
        u64 string_pointer_ptr = 0;
        u64 current_ptr = string_ptr;
        while ((string_pointer_ptr = CALL(string)->strchr(vm, current_ptr, pattern_ptr)) != 0) {
            u64 match_ptr = CALL(string)->match(vm, string_pointer_ptr, pattern_ptr);
            if (match_ptr == 0) {
                break;
            }
            if (CALL(string)->lessthan(vm, string_pointer_ptr, match_ptr)) {
                u64 match_start_ptr = CALL(string)->left(vm, match_ptr, size);
                u64 str_ncpy = CALL(string)->strncpy(vm, match_start_ptr, size);
                u64 distance = CALL(string)->lessthan(vm, string_ptr, match_start_ptr);
                if (distance > 0) {
                    u64 i = 0;
                    while (i++ < distance) {
                        printf(" ");
                    }
                }
                printf("%s[%lld]\n", CALL(string)->unsafe(vm, str_ncpy), distance);
                CALL(string_pointer)->free(vm, match_start_ptr);
                CALL(string)->free(vm, str_ncpy);
            }
            CALL(string_pointer)->free(vm, string_pointer_ptr);
            current_ptr = match_ptr;
        }
    }
    CALL(stack)->release(vm, gc_ptr);
    CALL(stack)->free(vm, gc_ptr);
}

static void run(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- rexo unit test code %s\n", __FILE__);
#endif
}

const tests_pointer_suite PRIVATE_API(tests_pointer_suite_definitions) = {
    .run = run
};
