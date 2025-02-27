/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   February 27, 2025 at 8:02:46 AM GMT+3
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

#include "rexo.h"
#include "std/data.h"
#define mmap _mmap
#define munmap _munmap

#include "std/macros.h"

#define USING_TESTS

#define USING_MOCK_MMAP

#include "test_memory.h"

#include "sys/memory/memory_v1.h"

#include "test.h"

#define MAX_MOCK_ALLOCS 1024
#define MAX_MEMORY_BLOCK_SIZE 4096
#define MAX_MEMORY_SIZE (MAX_MOCK_ALLOCS * MAX_MEMORY_BLOCK_SIZE)

typedef struct {
    void* start;
    void* end;
} mock_alloc_t;

static u64 memory_block[MAX_MEMORY_BLOCK_SIZE] = { 0 };
static mock_alloc_t mock_allocs[MAX_MOCK_ALLOCS] = { 0 };
static int mock_alloc_count = 0;
static void* mock_mmap(u64 size);
static u64 mock_munmap(void* addr, u64 length);

static void* mock_mmap(u64 size) {
    void* new_addr = mock_alloc_count == 0 ? (u8*)memory_block : (u8*)mock_allocs[mock_alloc_count - 1].end;
    u8* start = new_addr;
    u8* end = (u8*)new_addr + size;
    mock_allocs[mock_alloc_count].start = start;
    mock_allocs[mock_alloc_count].end = end;
    mock_alloc_count = (mock_alloc_count + 1) % MAX_MOCK_ALLOCS;
    return new_addr;
}

static u64 mock_munmap(void* addr, u64 length) {
    for (int i = 0; i < mock_alloc_count; i++) {
        void* end = (void*)((u8*)addr + length);
        if (mock_allocs[i].start == addr && mock_allocs[i].end == end) {
            if (mock_alloc_count > 1) {
                mock_allocs[i].start = mock_allocs[mock_alloc_count - 1].start;
                mock_allocs[i].end = mock_allocs[mock_alloc_count - 1].end;
            }
            mock_alloc_count--;
            mock_allocs[mock_alloc_count].start = 0;
            mock_allocs[mock_alloc_count].end = 0;
            return (u64)mock_allocs[i].start;
        }
    }
    return 0;
}

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    stack_ptr ctx;
}* TEST_DATA;

/* Initialize the data structure. Its allocation is handled by Rexo. */
RX_SET_UP(test_set_up) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    *ctx = calloc(1, sizeof(stack_element));
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* destroy list */
    free(*ctx);
    /* initializes to 0 */
    *ctx = 0;
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

/* test init */
RX_TEST_CASE(memory_micro_tests, test_empty_memory_count_equals_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* ensures counter is initialized to 0 */
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(memory_micro_tests, test_alloc_0, .fixture = test_fixture) {
    /* pushed to the list */
    const void* data_ptr = CALL(sys_memory)->alloc(0);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
}

/* test case */
RX_TEST_CASE(memory_micro_tests, test_realloc_0_0, .fixture = test_fixture) {
    /* pushed to the list */
    stack_ptr ptr = 0;
    stack_ptr* null_ptr = &ptr;
    const void* data_ptr = CALL(sys_memory)->realloc(null_ptr, 0, 0);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == null_ptr);
}

/* test case */
RX_TEST_CASE(memory_micro_tests, test_realloc_24_8, .fixture = test_fixture) {
    stack_ptr ptr = 0;
    stack_ptr* null_ptr = &ptr;
    const void* data_ptr = CALL(sys_memory)->realloc(null_ptr, 24, 8);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == null_ptr);
}

/* test case */
RX_TEST_CASE(memory_micro_tests, test_alloc_free, .fixture = test_fixture) {
    stack_ptr new = CALL(sys_memory)->alloc(sizeof(stack_element));
    /* pushed to the list */
    CALL(sys_memory)->free(new, sizeof(stack_element));
}

/* test case */
RX_TEST_CASE(memory_micro_tests, test_free, .fixture = test_fixture) {
    /* pushed to the list */
    CALL(sys_memory)->free(0, sizeof(stack_element));
}
/* test case */
RX_TEST_CASE(memory_micro_tests, test_mmap_free, .fixture = test_fixture) {
    /* pushed to the list */
    CALL(sys_memory_mmap)->free(0, sizeof(stack_element));
}

/* test case */
RX_TEST_CASE(memory_micro_tests, test_mmap_alloc_mmap_free, .fixture = test_fixture) {
    stack_ptr new = CALL(sys_memory)->vma_alloc(sizeof(stack_element));
    /* pushed to the list */
    CALL(sys_memory)->vma_free(new, sizeof(stack_element));
}

/* test case */
RX_TEST_CASE(memory_micro_tests, test_alloc_ff_free_ff, .fixture = test_fixture) {
    stack_ptr new = CALL(sys_memory)->alloc(0xffffffffffffffff);
    /* pushed to the list */
    ASSERT_DEBUG(new == 0);
    CALL(sys_memory)->free(new, 0xffffffffffffffff);
}

/* test case */
RX_TEST_CASE(memory_micro_tests, test_alloc_d7_free_d7, .fixture = test_fixture) {
    stack_ptr new = CALL(sys_memory)->alloc(0xffffffffffffffd7 - 1);
    /* pushed to the list */
    ASSERT_DEBUG(new == 0);
    CALL(sys_memory)->free(new, 0xffffffffffffffd7 - 1);
}

/* test case */
RX_TEST_CASE(memory_micro_tests, test_alloc_d6_free_d6, .fixture = test_fixture) {
    stack_ptr new = CALL(sys_memory)->alloc(0xffffffffffffffd7 - 1);
    /* pushed to the list */
    ASSERT_DEBUG(new == 0);
    CALL(sys_memory)->free(new, 0xffffffffffffffd7 - 1);
}

/* test case */
RX_TEST_CASE(memory_micro_tests, test_mmap_alloc_ff_mmap_free_ff, .fixture = test_fixture) {
    stack_ptr new = CALL(sys_memory)->vma_alloc(0xffffffffffffffff);
    /* pushed to the list */
    ASSERT_DEBUG(new == 0);
    CALL(sys_memory)->vma_free(new, 0xffffffffffffffff);
}

/* test case */
RX_TEST_CASE(memory_micro_tests, test_mmap_alloc_d7_mmap_free_d7, .fixture = test_fixture) {
    stack_ptr new = CALL(sys_memory)->vma_alloc(0xffffffffffffffd7);
    /* pushed to the list */
    ASSERT_DEBUG(new == 0);
    CALL(sys_memory)->vma_free(new, 0xffffffffffffffd7);
}

/* test case */
RX_TEST_CASE(memory_micro_tests, test_mmap_alloc_1_mmap_realloc_d8, .fixture = test_fixture) {
    stack_ptr new = CALL(sys_memory)->vma_alloc(1);
    /* pushed to the list */
    ASSERT_DEBUG(new != 0);
    CALL(sys_memory)->vma_realloc(0, 0xffffffffffffffd7, 0xffffffffffffffd8);
    CALL(sys_memory)->vma_free(new, 1);
}

/* test case */
RX_TEST_CASE(memory_micro_tests, test_mmap_alloc_1_mmap_realloc_0, .fixture = test_fixture) {
    stack_ptr new = CALL(sys_memory)->vma_alloc(1);
    /* pushed to the list */
    ASSERT_DEBUG(new != 0);
    CALL(sys_memory)->vma_realloc(new, 1, 0);
    CALL(sys_memory)->vma_free(new, 1);
}

/* test case */
RX_TEST_CASE(memory_micro_tests, test_mmap_alloc_d7_mmap_realloc_d8, .fixture = test_fixture) {
    stack_ptr new = CALL(sys_memory)->vma_alloc(0xff);
    /* pushed to the list */
    ASSERT_DEBUG(new != 0);
    u64* ptr = CALL(sys_memory)->vma_realloc(new, 0xff, 0xffffffffffffffd7);
    RX_ASSERT(ptr == 0);
}

/* test case */
RX_TEST_CASE(memory_micro_tests, test_mmap_alloc_d7_mmap_realloc_d6, .fixture = test_fixture) {
    stack_ptr new = CALL(sys_memory)->vma_alloc(0xffffffffffffffd7);
    /* pushed to the list */
    ASSERT_DEBUG(new == 0);
    CALL(sys_memory)->vma_realloc(new, 0xffffffffffffffd7, 0xffffffffffffffd6);
}

/* test case */
RX_TEST_CASE(memory_micro_tests, test_mmap_alloc_0_mmap_free, .fixture = test_fixture) {
    stack_ptr new = CALL(sys_memory)->vma_alloc(0);
    /* pushed to the list */
    ASSERT_DEBUG(new == 0);
    CALL(sys_memory)->vma_free(new, 0);
}

/* test case */
RX_TEST_CASE(memory_micro_tests, test_mmap_alloc_1_mmap_free, .fixture = test_fixture) {
    stack_ptr new = CALL(sys_memory)->vma_alloc(1);
    /* pushed to the list */
    ASSERT_DEBUG(new != 0);
    CALL(sys_memory)->vma_free(new, 1);
}

/* test case */
RX_TEST_CASE(memory_micro_tests, test_mmap_region_merge_a_c, .fixture = test_fixture) {
    alloc_ptr _alloc = PRIVATE_API(memory_mmap_methods_definitions).alloc;
    free_ptr _free = PRIVATE_API(memory_mmap_methods_definitions).free;

    PRIVATE_API(memory_mmap_methods_definitions).alloc = mock_mmap;
    PRIVATE_API(memory_mmap_methods_definitions).free = mock_munmap;

    u64 page_size = MAX_MEMORY_BLOCK_SIZE;
    void* a = CALL(sys_memory)->vma_alloc(page_size);
    ASSERT_DEBUG(a != 0);
    void* b = CALL(sys_memory)->vma_alloc(page_size);
    ASSERT_DEBUG(b != 0);
    void* c = CALL(sys_memory)->vma_alloc(page_size);
    ASSERT_DEBUG(c != 0);
    CALL(sys_memory)->vma_free(b, page_size);
    CALL(sys_memory)->vma_free(a, page_size);
    CALL(sys_memory)->vma_free(c, page_size);

    PRIVATE_API(memory_mmap_methods_definitions).alloc = _alloc;
    PRIVATE_API(memory_mmap_methods_definitions).free = _free;
}

/* test case */
RX_TEST_CASE(memory_micro_tests, test_mmap_region_merge_b_c, .fixture = test_fixture) {
    alloc_ptr _alloc = PRIVATE_API(memory_mmap_methods_definitions).alloc;
    free_ptr _free = PRIVATE_API(memory_mmap_methods_definitions).free;

    PRIVATE_API(memory_mmap_methods_definitions).alloc = mock_mmap;
    PRIVATE_API(memory_mmap_methods_definitions).free = mock_munmap;

    u64 page_size = MAX_MEMORY_BLOCK_SIZE;
    void* a = CALL(sys_memory)->vma_alloc(page_size);
    ASSERT_DEBUG(a != 0);
    void* b = CALL(sys_memory)->vma_alloc(page_size);
    ASSERT_DEBUG(b != 0);
    void* c = CALL(sys_memory)->vma_alloc(page_size);
    ASSERT_DEBUG(c != 0);
    void* d = CALL(sys_memory)->vma_alloc(page_size);
    ASSERT_DEBUG(c != 0);
    void* e = CALL(sys_memory)->vma_alloc(page_size);
    ASSERT_DEBUG(c != 0);
    void* f = CALL(sys_memory)->vma_alloc(page_size);
    ASSERT_DEBUG(c != 0);
    CALL(sys_memory)->vma_free(c, page_size);
    CALL(sys_memory)->vma_free(b, page_size);
    CALL(sys_memory)->vma_free(e, page_size);
    CALL(sys_memory)->vma_free(d, page_size);
    CALL(sys_memory)->vma_free(b, page_size);
    CALL(sys_memory)->vma_free(a, page_size);

    PRIVATE_API(memory_mmap_methods_definitions).alloc = _alloc;
    PRIVATE_API(memory_mmap_methods_definitions).free = _free;
}

/* test case */
RX_TEST_CASE(memory_micro_tests, test_mmap_free_0, .fixture = test_fixture) {
    u64 page_size = MAX_MEMORY_BLOCK_SIZE;
    void* a = CALL(sys_memory)->vma_alloc(page_size);
    ASSERT_DEBUG(a != 0);
    CALL(sys_memory)->vma_free(a, 0);
    CALL(sys_memory)->vma_free(a, page_size);
}

/* test case */
RX_TEST_CASE(memory_micro_tests, test_free_0, .fixture = test_fixture) {
    alloc_ptr _alloc = PRIVATE_API(memory_mmap_methods_definitions).alloc;
    free_ptr _free = PRIVATE_API(memory_mmap_methods_definitions).free;

    void* ptr = 0;
    PRIVATE_API(memory_mmap_methods_definitions).free = mock_munmap;

    u64 page_size = MAX_MEMORY_BLOCK_SIZE;
    u64 result = CALL(sys_memory_mmap)->free(ptr, page_size);
    RX_ASSERT(result == 0);

    PRIVATE_API(memory_mmap_methods_definitions).alloc = _alloc;
    PRIVATE_API(memory_mmap_methods_definitions).free = _free;
}

static int run(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- rexo unit test code\n");
#endif
    /* Execute the main function that runs the test cases found. */
    int result = rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;
    return result;
}

const memory_micro_test_suite PRIVATE_API(memory_micro_test_suite_definitions) = {
    .run = run
};
