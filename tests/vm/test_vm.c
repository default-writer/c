/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 28, 2025 at 10:49:26 AM GMT+3
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

#include "test_vm.h"

#include "std/macros.h"
#include "system/options/options_v1.h"
#include "virtual/pointer/pointer_v1.h"
#include "virtual/types/data/data_v1.h"
#include "virtual/types/stack/stack_v1.h"
#include "virtual/types/string/string_v1.h"
#include "virtual/types/user/user_v1.h"
#include "virtual/virtual/virtual_v1.h"

#define USING_TESTS
#include "test.h"

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    vm_ptr ctx;
}* TEST_DATA;

RX_SET_UP(test_set_up) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    rx->ctx = 0;
    vm_ptr* ctx = &rx->ctx;
    CALL(system)->init(ctx, 0);
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    vm_ptr* ctx = &rx->ctx;
    CALL(system)->destroy(ctx);
}

RX_SET_UP(test_set_up_pointer_init) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = CALL(vm)->init(8);
    rx->ctx = *cvm;
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down_pointer_destroy) {
    CALL(vm)->gc();
    CALL(vm)->destroy();
}

/* Define the fixture. */
RX_FIXTURE(test_fixture_pointer, TEST_DATA, .set_up = test_set_up_pointer_init, .tear_down = test_tear_down_pointer_destroy);

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_read_virtual_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = &rx->ctx;
    const_pointer_ptr ptr = CALL(virtual)->read(cvm, 0, TYPE_DATA);
    RX_ASSERT(ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_destroy_0, .fixture = test_fixture) {
    vm_ptr* ptr = 0;
    CALL(system)->destroy(ptr);
    RX_ASSERT(ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_destroy_null, .fixture = test_fixture) {
    vm_ptr ref = 0;
    vm_ptr* ptr = &ref;
    CALL(system)->destroy(ptr);
    RX_ASSERT(ref == 0);
}
/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_copy_safe_0, .fixture = test_fixture) {
    const char* data = "hello";
    u64 size = strlen(data) + 1;
    u64 virtual_ptr = CALL(pointer)->copy(data, size, size - 1, TYPE_USER);
    RX_ASSERT(virtual_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_copy_safe, .fixture = test_fixture_pointer) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = &rx->ctx;
    const char* data = "hello";
    u64 size = strlen(data) + 1;
    u64 virtual_ptr = CALL(pointer)->copy(data, size, size - 1, TYPE_USER);
    RX_ASSERT(virtual_ptr != 0);
    CALL(string)->free(cvm, virtual_ptr);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_read__address, .fixture = test_fixture_pointer) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = &rx->ctx;
    const_pointer_ptr tmp;
    u64 virtual_ptr = CALL(virtual)->alloc(cvm, 1, TYPE_DATA);
    u64 address = CALL(options)->size + 1;
    const_pointer_ptr ref = CALL(virtual)->read(cvm, address, TYPE_DATA);
    RX_ASSERT(virtual_ptr != 0);
    RX_ASSERT(ref == 0);
    CALL(pointer)->free(address, TYPE_DATA);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_read_type, .fixture = test_fixture_pointer) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = &rx->ctx;
    const_pointer_ptr tmp;
    u64 virtual_ptr = CALL(virtual)->alloc(cvm, 1, TYPE_DATA);
    u64 address = CALL(options)->size + 1;
    u64 type_id = CALL(virtual)->type(cvm, address);
    RX_ASSERT(virtual_ptr != 0);
    RX_ASSERT(type_id == 0);
    CALL(pointer)->free(address, TYPE_DATA);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_virtual_free_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = &rx->ctx;
    CALL(virtual)->free(cvm, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_free_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = &rx->ctx;
    CALL(virtual)->free(cvm, 1);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_read_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = &rx->ctx;
    u64 type_id = CALL(virtual)->type(cvm, 1);
    RX_ASSERT(type_id == TYPE_NULL);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_read_type_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = &rx->ctx;
    const_pointer_ptr ptr = CALL(virtual)->read(cvm, 0, 0);
    RX_ASSERT(ptr == 0);
}
/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_init_0, .fixture = test_fixture) {
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_read_virtual_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = &rx->ctx;
    const_pointer_ptr ptr = CALL(virtual)->read(cvm, 1, TYPE_DATA);
    RX_ASSERT(ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_write_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = &rx->ctx;
    u64 ptr = CALL(virtual)->alloc(cvm, 0, 0);
    RX_ASSERT(ptr == 0);
}

/* -------------------------------------------------------- *
 *                                                          *
 *                                                          *
 * test pointer                                             *
 *                                                          *
 *                                                          *
 * -------------------------------------------------------- */

typedef struct test_pointer_data {
    void* ptr;
}* TEST_POINTER_DATA;

RX_SET_UP(test_pointer_set_up) {
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_pointer_tear_down) {
    /* nothing to cleanup */
}

/* Define the fixture. */
RX_FIXTURE(test_pointer_fixture, TEST_POINTER_DATA, .set_up = test_pointer_set_up, .tear_down = test_pointer_tear_down);

/* test init */
RX_TEST_CASE(tests_vm_v1, test_list, .fixture = test_pointer_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    u64 list_ptr = CALL(stack)->alloc(cvm);
    CALL(stack)->free(cvm, list_ptr);
    CALL(vm)->gc();
    CALL(vm)->destroy();
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_list_user, .fixture = test_pointer_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    u64 list_ptr = CALL(stack)->alloc(cvm);
    u64 user_ptr = CALL(user)->alloc(cvm);
    CALL(stack)->push(cvm, list_ptr, user_ptr);
    CALL(user)->free(cvm, user_ptr);
    CALL(stack)->free(cvm, list_ptr);
    CALL(vm)->gc();
    CALL(vm)->destroy();
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_list_data, .fixture = test_pointer_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    u64 list_ptr = CALL(stack)->alloc(cvm);
    u64 data_ptr = CALL(data)->alloc(cvm, 1024);
    CALL(stack)->push(cvm, list_ptr, data_ptr);
    CALL(data)->free(cvm, data_ptr);
    CALL(stack)->free(cvm, list_ptr);
    CALL(vm)->gc();
    CALL(vm)->destroy();
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_list_free_user_free, .fixture = test_pointer_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    u64 list_ptr = CALL(stack)->alloc(cvm);
    u64 user_ptr = CALL(user)->alloc(cvm);
    CALL(stack)->push(cvm, list_ptr, user_ptr);
    CALL(stack)->free(cvm, list_ptr);
    CALL(user)->free(cvm, user_ptr);
    CALL(vm)->gc();
    CALL(vm)->destroy();
}
/* test init */
RX_TEST_CASE(tests_vm_v1, test_user_free_0, .fixture = test_pointer_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    CALL(user)->free(cvm, 0);
    CALL(vm)->gc();
    CALL(vm)->destroy();
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_pointer_alloc_virtual_alloc_release, .fixture = test_pointer_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    u64 ptr_id = CALL(virtual)->alloc(cvm, 8, TYPE_USER + 1);
    const_pointer_ptr data_ptr = CALL(virtual)->read(cvm, ptr_id, TYPE_USER + 1);
    RX_ASSERT(data_ptr != 0);
    CALL(pointer)->free(ptr_id, TYPE_USER + 1);
    RX_ASSERT(data_ptr != 0);
    CALL(vm)->release(0);
    CALL(vm)->gc();
    CALL(vm)->destroy();
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_pointer_alloc_gc_destroy_free_unsafe, .fixture = test_pointer_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    u64 ptr_id = CALL(virtual)->alloc(cvm, 8, TYPE_DATA);
    CALL(vm)->gc();
    CALL(vm)->destroy();
    CALL(vm)->release(ptr_id);
    CALL(data)->free(cvm, ptr_id);
    void* u = CALL(data)->unsafe(cvm, ptr_id);
    RX_ASSERT(u == 0);
}
/* test init */
RX_TEST_CASE(tests_vm_v1, test_pointer_alloc_release_0gc_destroy_read_type, .fixture = test_pointer_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    u64 ptr_id = CALL(virtual)->alloc(cvm, 8, TYPE_DATA);
    CALL(pointer)->free(ptr_id, TYPE_DATA);
    CALL(vm)->gc();
    CALL(vm)->destroy();
    const_pointer_ptr data_ptr = CALL(virtual)->read(cvm, ptr_id, TYPE_DATA);
    RX_ASSERT(data_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_pointer_alloc_gc_destroy_read_type_release_0, .fixture = test_pointer_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    u64 ptr_id = CALL(virtual)->alloc(cvm, 8, TYPE_DATA);
    CALL(vm)->gc();
    CALL(vm)->destroy();
    const_pointer_ptr data_ptr = CALL(virtual)->read(cvm, ptr_id, TYPE_DATA);
    RX_ASSERT(data_ptr == 0);
    CALL(pointer)->free(ptr_id, TYPE_DATA);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_pointer_alloc_destroy_read_type_release_0, .fixture = test_pointer_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    u64 ptr_id = CALL(virtual)->alloc(cvm, 8, TYPE_DATA);
    CALL(vm)->release(ptr_id);
    CALL(vm)->destroy();
    const_pointer_ptr data_ptr = CALL(virtual)->read(cvm, ptr_id, TYPE_DATA);
    RX_ASSERT(data_ptr == 0);
    CALL(pointer)->free(ptr_id, TYPE_DATA);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_pointer_alloc_alloc_ref_write_gc_destroy_read_type_release_0, .fixture = test_pointer_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    u64 ptr_id = CALL(virtual)->alloc(cvm, 8, TYPE_DATA);
    CALL(vm)->release(ptr_id);
    CALL(vm)->gc();
    CALL(vm)->destroy();
    const_pointer_ptr data_ptr = CALL(virtual)->read(cvm, ptr_id, TYPE_DATA);
    RX_ASSERT(data_ptr == 0);
    CALL(pointer)->free(ptr_id, TYPE_DATA);
}

static void run(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- acceptance test code\n");
#endif
    /* safe */
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- rexo unit test code %s\n", __FILE__);
#endif
}

const tests_vm_test_suite PRIVATE_API(tests_vm_test_suite_definitions) = {
    .run = run
};
