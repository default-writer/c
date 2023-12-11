/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   11 December 2023 at 9:16:31 GMT+3
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

#include "../../test.h"

#include "test_vm_v1.h"

#include "vm/v1/pointer/pointer_v1.h"
#include "vm/v1/system/types_v1.h"
#include "vm/v1/virtual/virtual_v1.h"

#define DEFAULT_SIZE 0x100

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    struct vm* ctx;
}* TEST_DATA;

RX_SET_UP(test_set_up) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct vm** ctx = &rx->ctx;
    virtual_v1->init(ctx, 0);
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct vm** ctx = &rx->ctx;
    virtual_v1->destroy(ctx);
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

/* test init */
RX_TEST_CASE(tests, test_vm_read_type_0, .fixture = test_fixture) {
    struct pointer* ptr = virtual_v1->read_type(0, TYPE_DATA);
    RX_ASSERT(ptr == 0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_destroy_0, .fixture = test_fixture) {
    struct vm** ptr = 0;
    virtual_v1->destroy(ptr);
    RX_ASSERT(ptr == 0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_destroy_null, .fixture = test_fixture) {
    struct vm* vm_ptr = 0;
    struct vm** ptr = &vm_ptr;
    virtual_v1->destroy(ptr);
    RX_ASSERT(vm_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_alloc_user, .fixture = test_fixture) {
    struct pointer* ptr = pointer_v1->alloc(0, TYPE_NULL);
    u64 virtual_ptr = virtual_v1->alloc(ptr);
    const struct pointer* vm_ptr = virtual_v1->read(virtual_ptr);
    RX_ASSERT(ptr != 0);
    RX_ASSERT(virtual_ptr != 0);
    RX_ASSERT(vm_ptr == ptr);
    pointer_v1->release(ptr);
}

/* test init */
RX_TEST_CASE(tests, test_vm_read_data, .fixture = test_fixture) {
    struct pointer* ptr = pointer_v1->alloc(0, TYPE_DATA);
    u64 virtual_ptr = virtual_v1->alloc(ptr);
    const struct pointer* vm_ptr = virtual_v1->read(virtual_ptr);
    RX_ASSERT(ptr != 0);
    RX_ASSERT(virtual_ptr != 0);
    RX_ASSERT(vm_ptr == ptr);
    pointer_v1->release(ptr);
}

/* test init */
RX_TEST_CASE(tests, test_vm_read_1, .fixture = test_fixture) {
    struct pointer* ptr = pointer_v1->alloc(0, TYPE_DATA);
    u64 virtual_ptr = virtual_v1->alloc(ptr);
    const struct pointer* vm_ptr = virtual_v1->read(1);
    RX_ASSERT(ptr != 0);
    RX_ASSERT(virtual_ptr != 0);
    RX_ASSERT(vm_ptr == ptr);
    pointer_v1->release(ptr);
}

/* test init */
RX_TEST_CASE(tests, test_vm_read_9, .fixture = test_fixture) {
    struct pointer* ptr = pointer_v1->alloc(0, TYPE_DATA);
    u64 virtual_ptr = virtual_v1->alloc(ptr);
    const struct pointer* vm_ptr = virtual_v1->read(vm_options_v1->size + 1);
    RX_ASSERT(ptr != 0);
    RX_ASSERT(virtual_ptr != 0);
    RX_ASSERT(vm_ptr == 0);
    pointer_v1->release(ptr);
}

/* test init */
RX_TEST_CASE(tests, test_virtual_free_0, .fixture = test_fixture) {
    pointer_v1->release(0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_free_0, .fixture = test_fixture) {
    virtual_v1->free(0);
    RX_ASSERT(0 == 0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_read_0, .fixture = test_fixture) {
    const struct pointer* ptr = virtual_v1->read(0);
    RX_ASSERT(ptr == 0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_init_0, .fixture = test_fixture) {
    RX_ASSERT(0 == 0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_read_type_1, .fixture = test_fixture) {
    struct pointer* ptr = virtual_v1->read_type(1, TYPE_DATA);
    RX_ASSERT(ptr == 0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_write_0, .fixture = test_fixture) {
    u64 ptr = virtual_v1->alloc(0);
    RX_ASSERT(ptr == 0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_read_type_write_1_read_1, .fixture = test_fixture) {
    struct pointer* ptr = pointer_v1->alloc(0, TYPE_DATA);
    u64 virtual_ptr = virtual_v1->alloc(ptr);
    struct pointer* vm_ptr = virtual_v1->read_type(virtual_ptr, TYPE_DATA);
    RX_ASSERT(ptr != 0);
    RX_ASSERT(virtual_ptr != 0);
    RX_ASSERT(vm_ptr == ptr);
    pointer_v1->release(ptr);
}

/* test init */
RX_TEST_CASE(tests, test_vm_read_type_write_1_read_2, .fixture = test_fixture) {
    struct pointer* ptr = pointer_v1->alloc(0, TYPE_DATA);
    u64 virtual_ptr = virtual_v1->alloc(ptr);
    struct pointer* vm_ptr = virtual_v1->read_type(virtual_ptr + 1, TYPE_DATA);
    RX_ASSERT(ptr != 0);
    RX_ASSERT(virtual_ptr != 0);
    RX_ASSERT(vm_ptr == 0);
    pointer_v1->release(ptr);
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
    pointer_v1->init(8);
    u64 list_ptr = type_list_v1->alloc();
    type_list_v1->free(list_ptr);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests, test_list_user, .fixture = test_pointer_fixture) {
    pointer_v1->init(8);
    u64 list_ptr = type_list_v1->alloc();
    u64 user_ptr = type_user_v1->alloc();
    type_list_v1->push(list_ptr, user_ptr);
    type_user_v1->free(user_ptr);
    type_list_v1->free(list_ptr);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests, test_list_data, .fixture = test_pointer_fixture) {
    pointer_v1->init(8);
    u64 list_ptr = type_list_v1->alloc();
    u64 data_ptr = type_data_v1->alloc(1024);
    type_list_v1->push(list_ptr, data_ptr);
    type_data_v1->free(data_ptr);
    type_list_v1->free(list_ptr);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests, test_list_free_user_free, .fixture = test_pointer_fixture) {
    pointer_v1->init(8);
    u64 list_ptr = type_list_v1->alloc();
    u64 user_ptr = type_user_v1->alloc();
    type_list_v1->push(list_ptr, user_ptr);
    type_list_v1->free(list_ptr);
    type_user_v1->free(user_ptr);
    pointer_v1->gc();
    pointer_v1->destroy();
}

/* test init */
RX_TEST_CASE(tests, test_user_free_0, .fixture = test_pointer_fixture) {
    pointer_v1->init(8);
    type_user_v1->free(0);
    pointer_v1->gc();
    pointer_v1->destroy();
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

const struct test_suite vm_v1_test_suite_definition_v1 = {
    .run = run
};
