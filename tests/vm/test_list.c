/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 3, 2025 at 11:19:43 AM GMT+3
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

#include "test_list.h"

#include "system/list/list_v1.h"

#define USING_TESTS
#include "test.h"

/* allocates memory pointer for list object */
static stack_ptr new_list(void) {
    stack_ptr ctx = 0;
    /* initializes the list */
    CALL(list)->init(&ctx);
    /* returns list object */
    return ctx;
}

/* releases memory pointer for list object */
static void delete_list(stack_ptr* ctx) {
    /* destroys the list */
    CALL(list)->destroy(ctx);
    /* cleans up */
    *ctx = 0;
}

/* runs default list usage scenario */
static void run_list1(void (*tests)(stack_ptr* const)) {
    /* initialize current context (stack) */
    stack_ptr ctx = new_list();
    /* call user method */
    tests(&ctx);
    /* destroy list */
    delete_list(&ctx);
}

/* runs default list usage scenario */
static void run_list2(void (*tests)(stack_ptr* const)) {
    /* initialize current context (stack) */
    stack_ptr ctx = new_list();
    /* call user method */
    tests(&ctx);
    /* destroy list */
    delete_list(&ctx);
}

/* uses the list */
static void tests(stack_ptr* current) {
    u8* payload = (void_ptr)0xdeadbeef;
    void_ptr is_null[] = {
        CALL(list)->peek(current),
        CALL(list)->pop(current)
    };
    RX_ASSERT(0 == is_null[0]);
    RX_ASSERT(0 == is_null[1]);
    CALL(list)->push(current, payload);
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(list)->print_head(current);
#endif
    CALL(list)->push(current, ++payload);
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(list)->print_head(current);
#endif
    CALL(list)->push(current, ++payload);
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(list)->print_head(current);
#endif
    CALL(list)->push(current, ++payload);
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(list)->print_head(current);
#endif
    CALL(list)->push(current, ++payload);
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(list)->print_head(current);
#endif
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(list)->print(current);
#endif
    const_void_ptr q_peek0 = CALL(list)->peek(current);
    CLEAN(q_peek0)
    void_ptr q_pop0 = CALL(list)->pop(current);
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(list)->print(current);
#endif
    const_void_ptr q_pop1 = CALL(list)->pop(current);
    CLEAN(q_pop1)
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(list)->print(current);
#endif
    const_void_ptr q_pop2 = CALL(list)->pop(current);
    CLEAN(q_pop2)
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(list)->print(current);
#endif
    const_void_ptr q_peek1 = CALL(list)->peek(current);
    void_ptr q_pop3 = CALL(list)->pop(current);
    const_void_ptr q_peek2 = CALL(list)->peek(current);
    CALL(list)->push(current, q_pop3);
    const_void_ptr q_peek3 = CALL(list)->peek(current);
    RX_ASSERT(q_peek1 != q_peek2);
    RX_ASSERT(q_peek2 != q_peek3);
    RX_ASSERT(q_peek1 == q_peek3);
    const_void_ptr q_pop4 = CALL(list)->pop(current);
    CLEAN(q_pop4)
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(list)->print(current);
#endif
    const_void_ptr q_pop5 = CALL(list)->pop(current);
    CLEAN(q_pop5)
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(list)->print(current);
#endif
    const_void_ptr q_peek4 = CALL(list)->peek(current);
    CALL(list)->push(current, q_pop0);
    CLEAN(q_peek4)
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(list)->print(current);
#endif
    const_void_ptr q_pop6 = CALL(list)->pop(current);
    CLEAN(q_pop6)
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(list)->print(current);
#endif
    const_void_ptr q_pop7 = CALL(list)->pop(current);
    CLEAN(q_pop7)
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(list)->print(current);
#endif
    const_void_ptr q_peek5 = CALL(list)->peek(current);
    CLEAN(q_peek5)
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(list)->print(current);
#endif
}

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    stack_ptr ctx;
}* TEST_DATA;

/* Initialize the data structure. Its allocation is handled by Rexo. */
RX_SET_UP(test_set_up) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* initializes to 0 */
    *ctx = 0;
    /* initialize list */
    CALL(list)->init(ctx);
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* destroy list */
    CALL(list)->destroy(ctx);
    /* initializes to 0 */
    *ctx = 0;
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

/* test init */
RX_TEST_CASE(tests_list_v1, test_empty_list_count_equals_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* ensures counter is initialized to 0 */
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(tests_list_v1, test_pop_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* pushed to the list */
    const_void_ptr data_ptr = CALL(list)->pop(0);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(tests_list_v1, test_pop_null_ptr, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* pushed to the list */
    stack_ptr ptr = 0;
    stack_ptr* null_ptr = &ptr;
    const_void_ptr data_ptr = CALL(list)->pop(null_ptr);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(tests_list_v1, test_peek_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* pushed to the list */
    const_void_ptr data_ptr = CALL(list)->peek(0);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(tests_list_v1, test_peek_null_ptr, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* pushed to the list */
    stack_ptr ptr = 0;
    stack_ptr* null_ptr = &ptr;
    const_void_ptr data_ptr = CALL(list)->peek(null_ptr);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(tests_list_v1, test_push_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* pushed to the list */
    void_ptr payload = (void_ptr)0x12345678;
    CALL(list)->push(0, payload);
    /* ensures there is no result on 0 */
    const_void_ptr data_ptr = CALL(list)->peek(0);
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(tests_list_v1, test_push_null_ptr, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* pushed to the list */
    stack_ptr ptr = 0;
    stack_ptr* null_ptr = &ptr;
    void_ptr payload = (void_ptr)0x12345678;
    CALL(list)->push(null_ptr, payload);
    /* ensures there is no result on 0 */
    const_void_ptr data_ptr = CALL(list)->peek(0);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(tests_list_v1, test_init_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* pushed to the list */
    CALL(list)->init(0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(tests_list_v1, test_init_null_ptr, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* pushed to the list */
    stack_ptr ptr = 0;
    stack_ptr* null_ptr = &ptr;
    CALL(list)->init(null_ptr);
    const_void_ptr data_ptr = CALL(list)->peek(null_ptr);
    CALL(list)->destroy(null_ptr);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(tests_list_v1, test_init_init, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* pushed to the list */
    stack_ptr ptr = 0;
    stack_ptr* null_ptr = &ptr;
    CALL(list)->init(null_ptr);
    CALL(list)->init(null_ptr);
    const_void_ptr data_ptr = CALL(list)->peek(null_ptr);
    CALL(list)->destroy(null_ptr);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(tests_list_v1, test_destroy_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* pushed to the list */
    CALL(list)->destroy(0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(tests_list_v1, test_destroy_null_ptr, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* pushed to the list */
    stack_ptr ptr = 0;
    stack_ptr* null_ptr = &ptr;
    const_void_ptr data_ptr = CALL(list)->peek(null_ptr);
    CALL(list)->destroy(null_ptr);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
}

/* test peek */
RX_TEST_CASE(tests_list_v1, test_standard_list_peek_does_not_changes_stack, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* prepares the payload */
    u8* payload = (void_ptr)0xdeadbeef;
    /* pushed to the list */
    CALL(list)->push(ctx, payload);
    /* gets the head pointer to the list */
    const stack_ptr ptr = *ctx;
    /* peeks from the list */
    const_void_ptr head = CALL(list)->peek(ctx);
    /* ensures payload is on top of the stack */
    RX_ASSERT(head == payload);
    /* ensures peek does not changes the head pointer */
    RX_ASSERT(ptr == *ctx);
}

/* test pop from 0 pointer */
RX_TEST_CASE(tests_list_v1, test_empty_list_pop_equals_0, .fixture = test_fixture) {
    stack_ptr ctx = 0;
    /* pops from the list */
    const stack_ptr head = CALL(list)->pop(&ctx);
    /* ensures head is not initialized */
    RX_ASSERT(head == 0);
}

/* test pop from 0 pointer */
RX_TEST_CASE(tests_list_v1, test_empty_list_peek_equals_0, .fixture = test_fixture) {
    stack_ptr ctx = 0;
    /* peeks from the list */
    const stack_ptr head = CALL(list)->peek(&ctx);
    /* ensures head is not initialized */
    RX_ASSERT(head == 0);
}

/* test alloc */
RX_TEST_CASE(tests_list_v1, test_alloc_count_eq_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* prepares the payload */
    u8* payload = (void_ptr)0xdeadbeef;
    /* pushes to the list */
    CALL(list)->push(ctx, payload);
    /* ensures data is added to the list */
    RX_ASSERT(*ctx != 0);
}

RX_TEST_CASE(tests_list_v1, test_alloc_payload, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* prepares the payload */
    u8* payload = (void_ptr)0xdeadbeef;
    /* pushes to the list */
    CALL(list)->push(ctx, payload);
    /* peeks from the list */
    const_void_ptr head = CALL(list)->peek(ctx);
    /* ensures data is added to the list */
    RX_ASSERT(head == payload);
}

RX_TEST_CASE(tests_list_v1, test_alloc_pop_count_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* prepares the payload */
    u8* payload = (void_ptr)0xdeadbeef;
    /* pushes to the list */
    CALL(list)->push(ctx, payload);
    /* pops from the list */
    const_void_ptr head = CALL(list)->pop(ctx);
    /* ensures data is added to the list */
    RX_ASSERT(head != 0);
}

RX_TEST_CASE(tests_list_v1, test_alloc_pop_payload, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* prepares the payload */
    u8* payload = (void_ptr)0xdeadbeef;
    /* pushes to the list */
    CALL(list)->push(ctx, payload);
    /* pops from the list */
    const_void_ptr head = CALL(list)->pop(ctx);
    /* ensures data is added to the list */
    RX_ASSERT(head == payload);
}

/* test peek */
RX_TEST_CASE(tests_list_v1, test_list_peek_is_zero, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* peeks from the list */
    const_void_ptr head = CALL(list)->peek(ctx);
    /* ensures head is not initialized */
    RX_ASSERT(head == 0);
}

/* test pop */
RX_TEST_CASE(tests_list_v1, test_list_pop_is_zero, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    stack_ptr* ctx = &rx->ctx;
    /* pops from the list */
    const_void_ptr head = CALL(list)->pop(ctx);
    /* ensures head is not initialized */
    RX_ASSERT(head == 0);
}

static void run(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- acceptance test code\n");
#endif
    run_list1(tests);
    run_list2(tests);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- rexo unit test code %s\n", __FILE__);
#endif
}

const tests_list_test_suite PRIVATE_API(tests_list_test_suite_definitions) = {
    .run = run
};
