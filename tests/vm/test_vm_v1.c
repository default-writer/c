/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 7, 2025 at 4:31:09 PM GMT+3
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

#include "test_vm_v1.h"

#include "std/macros.h"
#include "system/options/options_v1.h"
#include "virtual/pointer/pointer_v1.h"
#include "virtual/types/data/data_v1.h"
#include "virtual/types/file/file_v1.h"
#include "virtual/types/stack/stack_v1.h"
#include "virtual/types/string/string_v1.h"
#include "virtual/types/user/user_v1.h"
#include "virtual/virtual/virtual_v1.h"

#include "test.h"

#define DEFAULT_SIZE 0x100

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    vm_ptr ctx;
}* TEST_DATA;

RX_SET_UP(test_set_up) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    vm_ptr* ctx = &rx->ctx;
    CALL(virtual)->init(ctx, 0);
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    vm_ptr* ctx = &rx->ctx;
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
    vm_ptr* ptr = 0;
    CALL(virtual)->destroy(ptr);
    RX_ASSERT(ptr == 0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_destroy_null, .fixture = test_fixture) {
    vm_ptr ref = 0;
    vm_ptr* ptr = &ref;
    CALL(virtual)->destroy(ptr);
    RX_ASSERT(ref == 0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_copy_guard, .fixture = test_fixture) {
    const char* data = "hello";
    u64 size = strlen(data) + 1;
    const_pointer_ptr data_ptr = CALL(pointer)->copy_guard(data, size, size - 1, TYPE_USER);
    u64 virtual_ptr = CALL(virtual)->alloc(data_ptr);
    RX_ASSERT(virtual_ptr != 0);
    RX_ASSERT(data_ptr != 0);
    CALL(pointer)->release(data_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_vm_copy_guard_size_size, .fixture = test_fixture) {
    const char* data = "hello";
    u64 size = strlen(data) + 1;
    const_pointer_ptr data_ptr = CALL(pointer)->copy_guard(data, size, size, TYPE_USER);
    RX_ASSERT(data_ptr == 0);
    CALL(pointer)->gc();
}

/* test init */
RX_TEST_CASE(tests, test_vm_read_guard, .fixture = test_fixture) {
    pointer_ptr ptr = CALL(pointer)->alloc(0, TYPE_USER);
    u64 virtual_ptr = CALL(virtual)->alloc(ptr);
    pointer_ptr dest_ptr = CALL(virtual)->read_type(virtual_ptr, TYPE_USER);
    pointer_ptr ref = CALL(pointer)->read_guard(dest_ptr, 1);
    RX_ASSERT(ptr != 0);
    RX_ASSERT(virtual_ptr != 0);
    RX_ASSERT(ref == 0);
    CALL(pointer)->release(ptr);
}

/* test init */
RX_TEST_CASE(tests, test_vm_read_guard_1, .fixture = test_fixture) {
    pointer_ptr ptr = CALL(pointer)->alloc(4, TYPE_DATA);
    u64 virtual_ptr = CALL(virtual)->alloc(ptr);
    u64 src_ptr = CALL(virtual)->memcpy(4, "data", virtual_ptr);
    pointer_ptr dest_ptr = CALL(virtual)->read_type(src_ptr, TYPE_DATA);
    u8* data_ptr = CALL(pointer)->read_guard(dest_ptr, 1);
    RX_ASSERT(data_ptr[0] == 'd');
    RX_ASSERT(data_ptr[1] == '\0');
    RX_ASSERT(data_ptr[2] == 't');
    RX_ASSERT(data_ptr[3] == 'a');
    RX_ASSERT(ptr != 0);
    RX_ASSERT(virtual_ptr != 0);
    RX_ASSERT(ptr != 0);
    RX_ASSERT(dest_ptr != 0);
    CALL(pointer)->release(ptr);
    CALL(pointer)->release(dest_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_vm_read_guard_0, .fixture = test_fixture) {
    pointer_ptr ptr = CALL(pointer)->alloc(4, TYPE_DATA);
    u64 file_path_ptr = CALL(string)->load("data");
    pointer_ptr dest_ptr = CALL(virtual)->read_type(file_path_ptr, TYPE_STRING);
    u8* data_ptr = CALL(pointer)->read_guard(dest_ptr, 0);
    u64 mode_ptr = CALL(string)->load("rb");
    u64 f_ptr = CALL(file)->alloc(file_path_ptr, mode_ptr);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(f_ptr == 0);
    CALL(string)->free(file_path_ptr);
    CALL(string)->free(mode_ptr);
    CALL(pointer)->release(ptr);
}

/* test init */
RX_TEST_CASE(tests, test_vm_guard_0, .fixture = test_fixture) {
    pointer_ptr ptr = CALL(pointer)->alloc(4, TYPE_DATA);
    u64 file_path_ptr = CALL(string)->load("data");
    pointer_ptr dest_ptr = CALL(virtual)->read_type(file_path_ptr, TYPE_STRING);
    CALL(pointer)->guard(dest_ptr, 0);
    u64 mode_ptr = CALL(string)->load("rb");
    u64 f_ptr = CALL(file)->alloc(file_path_ptr, mode_ptr);
    RX_ASSERT(f_ptr == 0);
    CALL(string)->free(file_path_ptr);
    CALL(string)->free(mode_ptr);
    CALL(pointer)->release(ptr);
}

/* test init */
RX_TEST_CASE(tests, test_vm_alloc_user, .fixture = test_fixture) {
    pointer_ptr ptr = CALL(pointer)->alloc(0, TYPE_NULL);
    u64 virtual_ptr = CALL(virtual)->alloc(ptr);
    pointer_ptr ref = CALL(virtual)->read(virtual_ptr);
    RX_ASSERT(ptr != 0);
    RX_ASSERT(virtual_ptr != 0);
    RX_ASSERT(ref == ptr);
    CALL(pointer)->release(ptr);
}

/* test init */
RX_TEST_CASE(tests, test_vm_alloc_free, .fixture = test_fixture) {
    pointer_ptr ptr = CALL(pointer)->alloc(0, TYPE_NULL);
    u64 virtual_ptr = CALL(virtual)->alloc(ptr);
    pointer_ptr ref = CALL(virtual)->read(virtual_ptr);
    RX_ASSERT(ptr != 0);
    RX_ASSERT(virtual_ptr != 0);
    RX_ASSERT(ref == ptr);
    CALL(virtual)->free(1);
    CALL(pointer)->release(ptr);
}

/* test init */
RX_TEST_CASE(tests, test_vm_read_data, .fixture = test_fixture) {
    pointer_ptr ptr = CALL(pointer)->alloc(0, TYPE_DATA);
    u64 virtual_ptr = CALL(virtual)->alloc(ptr);
    pointer_ptr ref = CALL(virtual)->read(virtual_ptr);
    RX_ASSERT(ptr != 0);
    RX_ASSERT(virtual_ptr != 0);
    RX_ASSERT(ref == ptr);
    CALL(pointer)->release(ptr);
}

/* test init */
RX_TEST_CASE(tests, test_vm_read_1, .fixture = test_fixture) {
    pointer_ptr ptr = CALL(pointer)->alloc(0, TYPE_DATA);
    u64 virtual_ptr = CALL(virtual)->alloc(ptr);
    pointer_ptr ref = CALL(virtual)->read(1);
    RX_ASSERT(ptr != 0);
    RX_ASSERT(virtual_ptr != 0);
    RX_ASSERT(ref == ptr);
    CALL(pointer)->release(ptr);
}

/* test init */
RX_TEST_CASE(tests, test_vm_read_9, .fixture = test_fixture) {
    pointer_ptr ptr = CALL(pointer)->alloc(0, TYPE_DATA);
    u64 virtual_ptr = CALL(virtual)->alloc(ptr);
    pointer_ptr ref = CALL(virtual)->read(CALL(system_options)->size + 1);
    RX_ASSERT(ptr != 0);
    RX_ASSERT(virtual_ptr != 0);
    RX_ASSERT(ref == 0);
    CALL(pointer)->release(ptr);
}

/* test init */
RX_TEST_CASE(tests, test_pointer_release_0, .fixture = test_fixture) {
    CALL(pointer)->release(0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests, test_virtual_free_0, .fixture = test_fixture) {
    CALL(virtual)->free(0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests, test_vm_free_0, .fixture = test_fixture) {
    CALL(virtual)->free(1);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests, test_vm_read_0, .fixture = test_fixture) {
    pointer_ptr ptr = CALL(virtual)->read(0);
    RX_ASSERT(ptr == 0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_init_0, .fixture = test_fixture) {
    RX_ASSERT(0 != 1);
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
    pointer_ptr ref = CALL(virtual)->read_type(virtual_ptr, TYPE_DATA);
    RX_ASSERT(ptr != 0);
    RX_ASSERT(virtual_ptr != 0);
    RX_ASSERT(ref == ptr);
    CALL(pointer)->release(ptr);
}

/* test init */
RX_TEST_CASE(tests, test_vm_read_virtual_write_1_read_2, .fixture = test_fixture) {
    pointer_ptr ptr = CALL(pointer)->alloc(0, TYPE_DATA);
    u64 virtual_ptr = CALL(virtual)->alloc(ptr);
    pointer_ptr ref = CALL(virtual)->read_type(virtual_ptr + 1, TYPE_DATA);
    RX_ASSERT(ptr != 0);
    RX_ASSERT(virtual_ptr != 0);
    RX_ASSERT(ref == 0);
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
    u64 list_ptr = CALL(stack)->alloc();
    CALL(stack)->free(list_ptr);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests, test_list_user, .fixture = test_pointer_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr = CALL(stack)->alloc();
    u64 user_ptr = CALL(user)->alloc();
    CALL(stack)->push(list_ptr, user_ptr);
    CALL(user)->free(user_ptr);
    CALL(stack)->free(list_ptr);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests, test_list_data, .fixture = test_pointer_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr = CALL(stack)->alloc();
    u64 data_ptr = CALL(data)->alloc(1024);
    CALL(stack)->push(list_ptr, data_ptr);
    CALL(data)->free(data_ptr);
    CALL(stack)->free(list_ptr);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests, test_list_free_user_free, .fixture = test_pointer_fixture) {
    CALL(pointer)->init(8);
    u64 list_ptr = CALL(stack)->alloc();
    u64 user_ptr = CALL(user)->alloc();
    CALL(stack)->push(list_ptr, user_ptr);
    CALL(stack)->free(list_ptr);
    CALL(user)->free(user_ptr);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}
/* test init */
RX_TEST_CASE(tests, test_user_free_0, .fixture = test_pointer_fixture) {
    CALL(pointer)->init(8);
    CALL(user)->free(0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests, test_pointer_alloc_release, .fixture = test_pointer_fixture) {
    CALL(pointer)->init(8);

    pointer_ptr alloc_ptr = CALL(pointer)->alloc(8, TYPE_USER + 1);
    CALL(pointer)->release(alloc_ptr);

    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests, test_pointer_alloc_virtual_alloc_release, .fixture = test_pointer_fixture) {
    CALL(pointer)->init(8);

    pointer_ptr alloc_ptr = CALL(pointer)->alloc(8, TYPE_USER + 1);
    u64 ptr_id = CALL(virtual)->alloc(alloc_ptr);

    pointer_ptr data_ptr = CALL(virtual)->read_type(ptr_id, TYPE_USER + 1);
    ASSERT_DEBUG(data_ptr != 0);
    CALL(pointer)->release(data_ptr);

    ASSERT_DEBUG(data_ptr != 0);
    ASSERT_DEBUG(alloc_ptr == data_ptr);

    CALL(pointer)->free(0);
    CALL(pointer)->gc();
    CALL(pointer)->destroy();
}

/* test init */
RX_TEST_CASE(tests, test_pointer_alloc_gc_destroy_free_unsafe, .fixture = test_pointer_fixture) {
    CALL(pointer)->init(8);
    pointer_ptr byte_data = CALL(pointer)->alloc(8, TYPE_DATA);
    u64 ptr_id = CALL(virtual)->alloc(byte_data);

    virtual_pointer_ptr vptr = *((virtual_pointer_ptr*)byte_data);
    RX_ASSERT(vptr != 0);

    CALL(pointer)->gc();
    CALL(pointer)->destroy();
    CALL(pointer)->free(ptr_id);
    CALL(data)->free(ptr_id);
    void* u = CALL(data)->unsafe(ptr_id);

    ASSERT_DEBUG(u == 0);
}
/* test init */
RX_TEST_CASE(tests, test_pointer_alloc_release_0gc_destroy_read_type, .fixture = test_pointer_fixture) {
    CALL(pointer)->init(8);
    pointer_ptr byte_data = CALL(pointer)->alloc(8, TYPE_DATA);
    u64 ptr_id = CALL(virtual)->alloc(byte_data);

    virtual_pointer_ptr vptr = *((virtual_pointer_ptr*)byte_data);
    RX_ASSERT(vptr != 0);

    CALL(pointer)->release(0);

    CALL(pointer)->gc();
    CALL(pointer)->destroy();

    pointer_ptr data_ptr = CALL(virtual)->read_type(ptr_id, TYPE_DATA);
    ASSERT_DEBUG(data_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests, test_pointer_alloc_gc_destroy_read_type_release_0, .fixture = test_pointer_fixture) {
    CALL(pointer)->init(8);
    pointer_ptr byte_data = CALL(pointer)->alloc(8, TYPE_DATA);
    u64 ptr_id = CALL(virtual)->alloc(byte_data);

    virtual_pointer_ptr vptr = *((virtual_pointer_ptr*)byte_data);
    RX_ASSERT(vptr != 0);

    CALL(pointer)->gc();
    CALL(pointer)->destroy();

    pointer_ptr data_ptr = CALL(virtual)->read_type(ptr_id, TYPE_DATA);
    ASSERT_DEBUG(data_ptr == 0);
    CALL(pointer)->release(0);
}

/* test init */
RX_TEST_CASE(tests, test_pointer_alloc_destroy_read_type_release_0, .fixture = test_pointer_fixture) {
    CALL(pointer)->init(8);
    pointer_ptr byte_data = CALL(pointer)->alloc(8, TYPE_DATA);
    u64 ptr_id = CALL(virtual)->alloc(byte_data);

    virtual_pointer_ptr vptr = *((virtual_pointer_ptr*)byte_data);
    RX_ASSERT(vptr != 0);

    CALL(pointer)->free(ptr_id);

    CALL(pointer)->destroy();

    pointer_ptr data_ptr = CALL(virtual)->read_type(ptr_id, TYPE_DATA);
    ASSERT_DEBUG(data_ptr == 0);
    CALL(pointer)->release(0);
}

/* test init */
RX_TEST_CASE(tests, test_pointer_alloc_alloc_ref_write_gc_destroy_read_type_release_0, .fixture = test_pointer_fixture) {
    CALL(pointer)->init(8);
    pointer_ptr byte_data = CALL(pointer)->alloc(8, TYPE_DATA);
    u64 ptr_id = CALL(virtual)->alloc(byte_data);

    virtual_pointer_ptr vptr = *((virtual_pointer_ptr*)byte_data);
    RX_ASSERT(vptr != 0);

    CALL(pointer)->free(ptr_id);

    CALL(pointer)->gc();
    CALL(pointer)->destroy();

    pointer_ptr data_ptr = CALL(virtual)->read_type(ptr_id, TYPE_DATA);
    ASSERT_DEBUG(data_ptr == 0);
    CALL(pointer)->release(0);
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
