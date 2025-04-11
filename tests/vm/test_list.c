/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 11, 2025 at 11:32:52 AM GMT+3
 *
 */
/*
    Copyright (C) 2022-2047 Artur Mustafin (artur.mustafin@gmail.com)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its
       contributors may be used to endorse or promote products derived from
       this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "test_list.h"

#include "system/os/os_v1.h"

#include "virtual/list/list_v1.h"
#include "virtual/vm/vm_v1.h"

#define USING_TESTS
#include "test.h"

typedef struct test_data* test_data_ptr;

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    const_vm_ptr ctx;
    stack_ptr stack;
} test_data_type;

static void test(const_vm_ptr cvm, stack_ptr stack);
static void run_test(void (*test)(const_vm_ptr cvm, stack_ptr const));

RX_SET_UP(test_set_up) {
    test_data_ptr rx = (test_data_ptr)RX_DATA;
    rx->ctx = CALL(vm)->init(8);
    rx->stack = CALL(list)->init(rx->ctx);
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    test_data_ptr rx = (test_data_ptr)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    CALL(list)->destroy(cvm, rx->stack);
    CALL(vm)->destroy(rx->ctx);
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, test_data_type, .set_up = test_set_up, .tear_down = test_tear_down);

/* test init */
RX_TEST_CASE(tests_list_v1, test_list_init_0, .fixture = test_fixture) {
    u64 result = (u64)CALL(list)->init(0);
    RX_ASSERT(result == 0);
}

/* test init */
RX_TEST_CASE(tests_list_v1, test_list_destroy_0, .fixture = test_fixture) {
    u64 result = CALL(list)->destroy(0, 0);
    RX_ASSERT(result == 0);
}

/* test init */
RX_TEST_CASE(tests_list_v1, test_list_peek_0, .fixture = test_fixture) {
    u64 result = (u64)CALL(list)->peek(0, 0);
    RX_ASSERT(result == 0);
}

/* test init */
RX_TEST_CASE(tests_list_v1, test_list_pop_0, .fixture = test_fixture) {
    u64 result = (u64)CALL(list)->pop(0, 0);
    RX_ASSERT(result == 0);
}

/* test init */
RX_TEST_CASE(tests_list_v1, test_list_push_0_0_0, .fixture = test_fixture) {
    u64 result = (u64)CALL(list)->push(0, 0, 0);
    RX_ASSERT(result == 0);
}

/* test init */
RX_TEST_CASE(tests_list_v1, test_list_push_0, .fixture = test_fixture) {
    test_data_type rx = *((test_data_ptr)RX_DATA);
    const_vm_ptr cvm = rx.ctx;
    stack_ptr stack = rx.stack;
    CALL(list)->push(cvm, stack, 0);
}

/* test init */
RX_TEST_CASE(tests_list_v1, test_list_diff_0_0_0, .fixture = test_fixture) {
    test_data_type rx = *((test_data_ptr)RX_DATA);
    const_vm_ptr cvm = rx.ctx;
    stack_ptr stack = 0;
    CALL(list)->diff(cvm, stack, 0, 0);
}
/* test init */
RX_TEST_CASE(tests_list_v1, test_list_diff_right_0_0_0, .fixture = test_fixture) {
    test_data_type rx = *((test_data_ptr)RX_DATA);
    const_vm_ptr cvm = rx.ctx;
    stack_ptr stack = 0;
    CALL(list)->diff_right(cvm, stack, 0, 0);
}

/* test init */
RX_TEST_CASE(tests_list_v1, test_list_diff_left_0_0_0, .fixture = test_fixture) {
    test_data_type rx = *((test_data_ptr)RX_DATA);
    const_vm_ptr cvm = rx.ctx;
    stack_ptr stack = 0;
    CALL(list)->diff_left(cvm, stack, 0, 0);
}

/* test init */
RX_TEST_CASE(tests_list_v1, test_list_diff_0_0, .fixture = test_fixture) {
    test_data_type rx = *((test_data_ptr)RX_DATA);
    const_vm_ptr cvm = rx.ctx;
    stack_ptr stack = rx.stack;
    CALL(list)->diff(cvm, stack, 0, 0);
}

/* test init */
RX_TEST_CASE(tests_list_v1, test_list_diff_right_0_0, .fixture = test_fixture) {
    test_data_type rx = *((test_data_ptr)RX_DATA);
    const_vm_ptr cvm = rx.ctx;
    stack_ptr stack = rx.stack;
    CALL(list)->diff_right(cvm, stack, 0, 0);
}

/* test init */
RX_TEST_CASE(tests_list_v1, test_list_diff_left_0_0, .fixture = test_fixture) {
    test_data_type rx = *((test_data_ptr)RX_DATA);
    const_vm_ptr cvm = rx.ctx;
    stack_ptr stack = rx.stack;
    CALL(list)->diff_left(cvm, stack, 0, 0);
}

/* test init */
RX_TEST_CASE(tests_list_v1, test_list_diff_0, .fixture = test_fixture) {
    test_data_type rx = *((test_data_ptr)RX_DATA);
    const_vm_ptr cvm = rx.ctx;
    stack_ptr stack1 = rx.stack;
    stack_ptr stack2 = CALL(list)->init(cvm);
    CALL(list)->diff(cvm, stack1, stack2, 0);
    CALL(list)->destroy(cvm, stack2);
}

/* test init */
RX_TEST_CASE(tests_list_v1, test_list_diff_0_0_0_0, .fixture = test_fixture) {
    u64 result = CALL(list)->diff(0, 0, 0, 0);
    RX_ASSERT(result == 0);
}

/* test init */
RX_TEST_CASE(tests_list_v1, test_list_diff_left_0_0_0_0, .fixture = test_fixture) {
    u64 result = CALL(list)->diff_left(0, 0, 0, 0);
    RX_ASSERT(result == 0);
}

/* test init */
RX_TEST_CASE(tests_list_v1, test_list_diff_right_0_0_0_0, .fixture = test_fixture) {
    u64 result = CALL(list)->diff_right(0, 0, 0, 0);
    RX_ASSERT(result == 0);
}

/* test init */
RX_TEST_CASE(tests_list_v1, test_list_diff_right_0, .fixture = test_fixture) {
    test_data_type rx = *((test_data_ptr)RX_DATA);
    const_vm_ptr cvm = rx.ctx;
    stack_ptr stack1 = rx.stack;
    stack_ptr stack2 = CALL(list)->init(cvm);
    CALL(list)->diff_right(cvm, stack1, stack2, 0);
    CALL(list)->destroy(cvm, stack2);
}

/* test init */
RX_TEST_CASE(tests_list_v1, test_list_diff_left_0, .fixture = test_fixture) {
    test_data_type rx = *((test_data_ptr)RX_DATA);
    const_vm_ptr cvm = rx.ctx;
    stack_ptr stack1 = rx.stack;
    stack_ptr stack2 = CALL(list)->init(cvm);
    CALL(list)->diff_left(cvm, stack1, stack2, 0);
    CALL(list)->destroy(cvm, stack2);
}

/* test init */
RX_TEST_CASE(tests_list_v1, test_empty_list_count_equals_0, .fixture = test_fixture) {
    test_data_type rx = *((test_data_ptr)RX_DATA);
    const_vm_ptr cvm = rx.ctx;
    stack_ptr stack = rx.stack;
    RX_ASSERT(stack->size == 0);
    /* ensures counter is initialized to 0 */
    RX_ASSERT(*cvm != 0);
}

/* test case */
RX_TEST_CASE(tests_list_v1, test_pop_0, .fixture = test_fixture) {
    test_data_type rx = *((test_data_ptr)RX_DATA);
    const_vm_ptr cvm = rx.ctx;
    /* pushed to the list */
    const_void_ptr data_ptr = (const_void_ptr)CALL(list)->pop(cvm, 0);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*cvm != 0);
}

/* test case */
RX_TEST_CASE(tests_list_v1, test_pop_null_ptr, .fixture = test_fixture) {
    test_data_type rx = *((test_data_ptr)RX_DATA);
    const_vm_ptr cvm = rx.ctx;
    /* pushed to the list */
    stack_ptr null_ptr = 0;
    const_void_ptr data_ptr = (const_void_ptr)CALL(list)->pop(cvm, null_ptr);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*cvm != 0);
}

/* test case */
RX_TEST_CASE(tests_list_v1, test_peek_0, .fixture = test_fixture) {
    test_data_type rx = *((test_data_ptr)RX_DATA);
    const_vm_ptr cvm = rx.ctx;
    /* pushed to the list */
    const_void_ptr data_ptr = (const_void_ptr)CALL(list)->peek(cvm, 0);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*cvm != 0);
}

/* test case */
RX_TEST_CASE(tests_list_v1, test_peek_null_ptr, .fixture = test_fixture) {
    test_data_type rx = *((test_data_ptr)RX_DATA);
    const_vm_ptr cvm = rx.ctx;
    /* pushed to the list */
    stack_ptr null_ptr = 0;
    const_void_ptr data_ptr = (const_void_ptr)CALL(list)->peek(cvm, null_ptr);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*cvm != 0);
}

/* test case */
RX_TEST_CASE(tests_list_v1, test_push_0, .fixture = test_fixture) {
    test_data_type rx = *((test_data_ptr)RX_DATA);
    const_vm_ptr cvm = rx.ctx;
    /* pushed to the list */
    void_ptr data = (void_ptr)0x12345678;
    CALL(list)->push(cvm, 0, data);
    /* ensures there is no result on 0 */
    const_void_ptr data_ptr = (const_void_ptr)CALL(list)->peek(cvm, 0);
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*cvm != 0);
}

/* test case */
RX_TEST_CASE(tests_list_v1, test_push_null_ptr, .fixture = test_fixture) {
    test_data_type rx = *((test_data_ptr)RX_DATA);
    const_vm_ptr cvm = rx.ctx;
    /* pushed to the list */
    stack_ptr null_ptr = 0;
    void_ptr data = (void_ptr)0x12345678;
    CALL(list)->push(cvm, null_ptr, data);
    /* ensures there is no result on 0 */
    const_void_ptr data_ptr = (const_void_ptr)CALL(list)->peek(cvm, 0);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*cvm != 0);
}

/* test case */
RX_TEST_CASE(tests_list_v1, test_init_null_ptr, .fixture = test_fixture) {
    test_data_type rx = *((test_data_ptr)RX_DATA);
    const_vm_ptr cvm = rx.ctx;
    /* pushed to the list */
    stack_ptr null_ptr = 0;
    null_ptr = CALL(list)->init(cvm);
    const_void_ptr data_ptr = (const_void_ptr)CALL(list)->peek(cvm, null_ptr);
    CALL(list)->destroy(cvm, null_ptr);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*cvm != 0);
}

/* test case */
RX_TEST_CASE(tests_list_v1, test_destroy_0, .fixture = test_fixture) {
    test_data_type rx = *((test_data_ptr)RX_DATA);
    const_vm_ptr cvm = rx.ctx;
    /* pushed to the list */
    CALL(list)->destroy(cvm, 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*cvm != 0);
}

/* test case */
RX_TEST_CASE(tests_list_v1, test_destroy_null_ptr, .fixture = test_fixture) {
    test_data_type rx = *((test_data_ptr)RX_DATA);
    const_vm_ptr cvm = rx.ctx;
    /* pushed to the list */
    stack_ptr null_ptr = 0;
    const_void_ptr data_ptr = (const_void_ptr)CALL(list)->peek(cvm, null_ptr);
    CALL(list)->destroy(cvm, null_ptr);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*cvm != 0);
}

/* test peek */
RX_TEST_CASE(tests_list_v1, test_standard_list_peek_does_not_changes_stack, .fixture = test_fixture) {
    test_data_type rx = *((test_data_ptr)RX_DATA);
    const_vm_ptr cvm = rx.ctx;
    stack_ptr stack = rx.stack;
    /* prepares the data */
    u8* data = (void_ptr)0xdeadbeef;
    /* pushed to the list */
    CALL(list)->push(cvm, stack, data);
    /* gets the head pointer to the list */
    const stack_ptr ptr = stack;
    /* peeks from the list */
    const_void_ptr head = (const_void_ptr)CALL(list)->peek(cvm, stack);
    /* ensures data is on top of the stack */
    RX_ASSERT(head == data);
    /* ensures peek does not changes the head pointer */
    RX_ASSERT(ptr == stack);
}

/* test pop from 0 pointer */
RX_TEST_CASE(tests_list_v1, test_empty_list_pop_equals_0, .fixture = test_fixture) {
    test_data_type rx = *((test_data_ptr)RX_DATA);
    const_vm_ptr cvm = rx.ctx;
    stack_ptr ctx = 0;
    /* pops from the list */
    const_stack_ptr head = (const_stack_ptr)CALL(list)->pop(cvm, ctx);
    /* ensures head is not initialized */
    RX_ASSERT(head == 0);
}

/* test pop from 0 pointer */
RX_TEST_CASE(tests_list_v1, test_empty_list_peek_equals_0, .fixture = test_fixture) {
    test_data_type rx = *((test_data_ptr)RX_DATA);
    const_vm_ptr cvm = rx.ctx;
    stack_ptr ctx = 0;
    /* peeks from the list */
    const_stack_ptr head = (const_stack_ptr)CALL(list)->peek(cvm, ctx);
    /* ensures head is not initialized */
    RX_ASSERT(head == 0);
}

/* test alloc */
RX_TEST_CASE(tests_list_v1, test_alloc_count_eq_1, .fixture = test_fixture) {
    test_data_type rx = *((test_data_ptr)RX_DATA);
    const_vm_ptr cvm = rx.ctx;
    stack_ptr stack = rx.stack;
    /* prepares the data */
    u8* data = (void_ptr)0xdeadbeef;
    /* pushes to the list */
    CALL(list)->push(cvm, stack, data);
    /* ensures data is added to the list */
    RX_ASSERT(*cvm != 0);
}

RX_TEST_CASE(tests_list_v1, test_alloc_data, .fixture = test_fixture) {
    test_data_type rx = *((test_data_ptr)RX_DATA);
    const_vm_ptr cvm = rx.ctx;
    stack_ptr stack = rx.stack;
    /* prepares the data */
    u8* data = (void_ptr)0xdeadbeef;
    /* pushes to the list */
    CALL(list)->push(cvm, stack, data);
    /* peeks from the list */
    const_void_ptr head = (const_void_ptr)CALL(list)->peek(cvm, stack);
    /* ensures data is added to the list */
    RX_ASSERT(head == data);
}

RX_TEST_CASE(tests_list_v1, test_alloc_pop_count_0, .fixture = test_fixture) {
    test_data_type rx = *((test_data_ptr)RX_DATA);
    const_vm_ptr cvm = rx.ctx;
    stack_ptr stack = rx.stack;
    /* prepares the data */
    u8* data = (void_ptr)0xdeadbeef;
    /* pushes to the list */
    CALL(list)->push(cvm, stack, data);
    /* pops from the list */
    const_void_ptr head = (const_void_ptr)CALL(list)->pop(cvm, stack);
    /* ensures data is added to the list */
    RX_ASSERT(head != 0);
}

RX_TEST_CASE(tests_list_v1, test_alloc_pop_data, .fixture = test_fixture) {
    test_data_type rx = *((test_data_ptr)RX_DATA);
    const_vm_ptr cvm = rx.ctx;
    stack_ptr stack = rx.stack;
    /* prepares the data */
    u8* data = (void_ptr)0xdeadbeef;
    /* pushes to the list */
    CALL(list)->push(cvm, stack, data);
    /* pops from the list */
    const_void_ptr head = (const_void_ptr)CALL(list)->pop(cvm, stack);
    /* ensures data is added to the list */
    RX_ASSERT(head == data);
}

/* test peek */
RX_TEST_CASE(tests_list_v1, test_list_peek_is_zero, .fixture = test_fixture) {
    test_data_type rx = *((test_data_ptr)RX_DATA);
    const_vm_ptr cvm = rx.ctx;
    stack_ptr stack = rx.stack;
    /* peeks from the list */
    const_void_ptr head = (const_void_ptr)CALL(list)->peek(cvm, stack);
    /* ensures head is not initialized */
    RX_ASSERT(head == 0);
}

/* test pop */
RX_TEST_CASE(tests_list_v1, test_list_pop_is_zero, .fixture = test_fixture) {
    test_data_type rx = *((test_data_ptr)RX_DATA);
    const_vm_ptr cvm = rx.ctx;
    stack_ptr stack = rx.stack;
    /* pops from the list */
    const_void_ptr head = (const_void_ptr)CALL(list)->pop(cvm, stack);
    /* ensures head is not initialized */
    RX_ASSERT(head == 0);
}

/* runs default list usage scenario */
static void run_test(void (*test)(const_vm_ptr cvm, stack_ptr const)) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    /* initialize current context (stack) */
    stack_ptr ctx = CALL(list)->init(cvm);
    /* call user method */
    test(cvm, ctx);
    /* destroy list */
    CALL(list)->destroy(cvm, ctx);
    CALL(vm)->gc(cvm);
    CALL(vm)->destroy(cvm);
}

/* uses the list */
static void test(const_vm_ptr cvm, stack_ptr stack) {
    u8* data = (void_ptr)0xdeadbeef;
    u64 is_null[] = {
        (u64)CALL(list)->peek(cvm, stack),
        (u64)CALL(list)->pop(cvm, stack)
    };
    RX_ASSERT(0 == is_null[0]);
    RX_ASSERT(0 == is_null[1]);
    CALL(list)->push(cvm, stack, (void_ptr)data);
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(list)->print_head(stack);
#endif
    CALL(list)->push(cvm, stack, (void_ptr)++data);
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(list)->print_head(stack);
#endif
    CALL(list)->push(cvm, stack, (void_ptr)++data);
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(list)->print_head(stack);
#endif
    CALL(list)->push(cvm, stack, (void_ptr)++data);
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(list)->print_head(stack);
#endif
    CALL(list)->push(cvm, stack, (void_ptr)++data);
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(list)->print_head(stack);
#endif
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(list)->print(stack);
#endif
    const_void_ptr q_peek0 = (const_void_ptr)CALL(list)->peek(cvm, stack);
    CLEAN(q_peek0)
    const_void_ptr q_pop0 = (const_void_ptr)CALL(list)->pop(cvm, stack);
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(list)->print(stack);
#endif
    const_void_ptr q_pop1 = (const_void_ptr)CALL(list)->pop(cvm, stack);
    CLEAN(q_pop1)
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(list)->print(stack);
#endif
    const_void_ptr q_pop2 = (const_void_ptr)CALL(list)->pop(cvm, stack);
    CLEAN(q_pop2)
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(list)->print(stack);
#endif
    const_void_ptr q_peek1 = (const_void_ptr)CALL(list)->peek(cvm, stack);
    void_ptr q_pop3 = (void_ptr)CALL(list)->pop(cvm, stack);
    const_void_ptr q_peek2 = (const_void_ptr)CALL(list)->peek(cvm, stack);
    CALL(list)->push(cvm, stack, q_pop3);
    const_void_ptr q_peek3 = (const_void_ptr)CALL(list)->peek(cvm, stack);
    RX_ASSERT(q_peek1 != q_peek2);
    RX_ASSERT(q_peek2 != q_peek3);
    RX_ASSERT(q_peek1 == q_peek3);
    const_void_ptr q_pop4 = (const_void_ptr)CALL(list)->pop(cvm, stack);
    CLEAN(q_pop4)
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(list)->print(stack);
#endif
    const_void_ptr q_pop5 = (const_void_ptr)CALL(list)->pop(cvm, stack);
    CLEAN(q_pop5)
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(list)->print(stack);
#endif
    const_void_ptr q_peek4 = (const_void_ptr)CALL(list)->peek(cvm, stack);
    safe_void_ptr safe_ptr;
    safe_ptr.const_ptr = q_pop0;
    void_ptr ptr0 = safe_ptr.ptr;
    CALL(list)->push(cvm, stack, ptr0);
    CLEAN(q_peek4)
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(list)->print(stack);
#endif
    const_void_ptr q_pop6 = (const_void_ptr)CALL(list)->pop(cvm, stack);
    CLEAN(q_pop6)
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(list)->print(stack);
#endif
    const_void_ptr q_pop7 = (const_void_ptr)CALL(list)->pop(cvm, stack);
    CLEAN(q_pop7)
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(list)->print(stack);
#endif
    const_void_ptr q_peek5 = (const_void_ptr)CALL(list)->peek(cvm, stack);
    CLEAN(q_peek5)
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(list)->print(stack);
#endif
}

static void run(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- acceptance test code\n");
#endif
    run_test(test);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- rexo unit test code %s\n", __FILE__);
#endif
}

const tests_list_test_suite PRIVATE_API(tests_list_test_suite_definitions) = {
    .run = run
};
