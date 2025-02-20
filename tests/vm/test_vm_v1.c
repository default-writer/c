/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   February 19, 2025 at 8:46:21 PM GMT+3
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

#include "test_vm_v1.h"

#include "sys/options/options_v1.h"
#include "vm/pointer/pointer_v1.h"
#include "vm/types/data/data_v1.h"
#include "vm/types/stack/stack_v1.h"
#include "vm/virtual/virtual_v1.h"
#include "vm/vm_v1.h"

#define DEFAULT_SIZE 0x100

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    struct vm* ctx;
}* TEST_DATA;

RX_SET_UP(test_set_up) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct vm** ctx = &rx->ctx;
    CALL(virtual)->init(ctx, 0);
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct vm** ctx = &rx->ctx;
    CALL(virtual)->destroy(ctx);
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

/* test init */
RX_TEST_CASE(tests, test_vm_read_virtual_0, .fixture = test_fixture) {
    pointer_ptr ptr = CALL(virtual)->read_type(0, TYPE_DATA);
    RX_ASSERT(ptr == 0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_destroy_0, .fixture = test_fixture) {
    struct vm** ptr = 0;
    CALL(virtual)->destroy(ptr);
    RX_ASSERT(ptr == 0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_destroy_null, .fixture = test_fixture) {
    struct vm* vm_ptr = 0;
    struct vm** ptr = &vm_ptr;
    CALL(virtual)->destroy(ptr);
    RX_ASSERT(vm_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_alloc_user, .fixture = test_fixture) {
    pointer_ptr ptr = CALL(pointer)->alloc(0, TYPE_NULL);
    u64 virtual_ptr = CALL(virtual)->alloc(ptr);
    const_pointer_ptr vm_ptr = CALL(virtual)->read(virtual_ptr);
    RX_ASSERT(ptr != 0);
    RX_ASSERT(virtual_ptr != 0);
    RX_ASSERT(vm_ptr == ptr);
    CALL(pointer)->release(ptr);
}

/* test init */
RX_TEST_CASE(tests, test_vm_read_data, .fixture = test_fixture) {
    pointer_ptr ptr = CALL(pointer)->alloc(0, TYPE_DATA);
    u64 virtual_ptr = CALL(virtual)->alloc(ptr);
    const_pointer_ptr vm_ptr = CALL(virtual)->read(virtual_ptr);
    RX_ASSERT(ptr != 0);
    RX_ASSERT(virtual_ptr != 0);
    RX_ASSERT(vm_ptr == ptr);
    CALL(pointer)->release(ptr);
}

/* test init */
RX_TEST_CASE(tests, test_vm_read_1, .fixture = test_fixture) {
    pointer_ptr ptr = CALL(pointer)->alloc(0, TYPE_DATA);
    u64 virtual_ptr = CALL(virtual)->alloc(ptr);
    const_pointer_ptr vm_ptr = CALL(virtual)->read(1);
    RX_ASSERT(ptr != 0);
    RX_ASSERT(virtual_ptr != 0);
    RX_ASSERT(vm_ptr == ptr);
    CALL(pointer)->release(ptr);
}

/* test init */
RX_TEST_CASE(tests, test_vm_read_9, .fixture = test_fixture) {
    pointer_ptr ptr = CALL(pointer)->alloc(0, TYPE_DATA);
    u64 virtual_ptr = CALL(virtual)->alloc(ptr);
    const_pointer_ptr vm_ptr = CALL(virtual)->read(CALL(sys_options)->size + 1);
    RX_ASSERT(ptr != 0);
    RX_ASSERT(virtual_ptr != 0);
    RX_ASSERT(vm_ptr == 0);
    CALL(pointer)->release(ptr);
}

/* test init */
RX_TEST_CASE(tests, test_virtual_free_0, .fixture = test_fixture) {
    CALL(pointer)->release(0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_free_0, .fixture = test_fixture) {
    CALL(virtual)->free(0);
    RX_ASSERT(0 == 0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_read_0, .fixture = test_fixture) {
    const_pointer_ptr ptr = CALL(virtual)->read(0);
    RX_ASSERT(ptr == 0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_init_0, .fixture = test_fixture) {
    RX_ASSERT(0 == 0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_read_virtual_1, .fixture = test_fixture) {
    pointer_ptr ptr = CALL(virtual)->read_type(1, TYPE_DATA);
    RX_ASSERT(ptr == 0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_write_0, .fixture = test_fixture) {
    u64 ptr = CALL(virtual)->alloc(0);
    RX_ASSERT(ptr == 0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_read_virtual_write_1_read_1, .fixture = test_fixture) {
    pointer_ptr ptr = CALL(pointer)->alloc(0, TYPE_DATA);
    u64 virtual_ptr = CALL(virtual)->alloc(ptr);
    pointer_ptr vm_ptr = CALL(virtual)->read_type(virtual_ptr, TYPE_DATA);
    RX_ASSERT(ptr != 0);
    RX_ASSERT(virtual_ptr != 0);
    RX_ASSERT(vm_ptr == ptr);
    CALL(pointer)->release(ptr);
}

/* test init */
RX_TEST_CASE(tests, test_vm_read_virtual_write_1_read_2, .fixture = test_fixture) {
    pointer_ptr ptr = CALL(pointer)->alloc(0, TYPE_DATA);
    u64 virtual_ptr = CALL(virtual)->alloc(ptr);
    pointer_ptr vm_ptr = CALL(virtual)->read_type(virtual_ptr + 1, TYPE_DATA);
    RX_ASSERT(ptr != 0);
    RX_ASSERT(virtual_ptr != 0);
    RX_ASSERT(vm_ptr == 0);
    CALL(pointer)->release(ptr);
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
    /* nothing */
}

/* Define the fixture. */
RX_FIXTURE(test_pointer_fixture, TEST_POINTER_DATA, .set_up = test_pointer_set_up, .tear_down = test_pointer_tear_down);

/* test init */
RX_TEST_CASE(tests, test_list, .fixture = test_pointer_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr = CALL(virtual_stack)->alloc();
    CALL(virtual_stack)->free(list_ptr);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests, test_list_user, .fixture = test_pointer_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr = CALL(virtual_stack)->alloc();
    u64 user_ptr = CALL(virtual_user)->alloc();
    CALL(virtual_stack)->push(list_ptr, user_ptr);
    CALL(virtual_user)->free(user_ptr);
    CALL(virtual_stack)->free(list_ptr);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests, test_list_data, .fixture = test_pointer_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr = CALL(virtual_stack)->alloc();
    u64 data_ptr = CALL(virtual_data)->alloc(1024);
    CALL(virtual_stack)->push(list_ptr, data_ptr);
    CALL(virtual_data)->free(data_ptr);
    CALL(virtual_stack)->free(list_ptr);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests, test_list_free_user_free, .fixture = test_pointer_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr = CALL(virtual_stack)->alloc();
    u64 user_ptr = CALL(virtual_user)->alloc();
    CALL(virtual_stack)->push(list_ptr, user_ptr);
    CALL(virtual_stack)->free(list_ptr);
    CALL(virtual_user)->free(user_ptr);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests, test_user_free_0, .fixture = test_pointer_fixture) {
    CALL(pointer)->init(8);
    CALL(virtual_user)->free(0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

static int run(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- acceptance test code\n");
#endif
    /* safe */
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- rexo unit test code\n");
#endif
    /* Execute the main function that runs the test cases found. */
    int result = rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;
    return result;
}

const vm_v1_test_suite PRIVATE_API(vm_v1_test_suite_definitions) = {
    .run = run
};
