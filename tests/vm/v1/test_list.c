/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   February 2, 2025 at 8:49:21 PM GMT+3
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

#define RXP_DEBUG_TESTS

#include <rexo.h>

#include "../../test.h"

#include "list/list_v1.h"
#include "std/macros.h"

#include "vm/v1/vm_v1.h"

/* allocates memory pointer for list object */
static struct list_data* new_list(void) {
    struct list_data* ctx = 0;
    /* initializes the list */
    list_v1->init(&ctx);
    /* returns list object */
    return ctx;
}

/* releases memory pointer for list object */
static void delete_list(struct list_data** ctx) {
    /* destroys the list */
    list_v1->destroy(ctx);
    /* cleans up */
    *ctx = 0;
}

/* runs default list usage scenario */
static void run_list1(void (*tests)(struct list_data** const)) {
    /* initialize current context (stack) */
    struct list_data* ctx = new_list();
    /* call user method */
    tests(&ctx);
    /* destroy list */
    delete_list(&ctx);
}

/* runs default list usage scenario */
static void run_list2(void (*tests)(struct list_data** const)) {
    /* initialize current context (stack) */
    struct list_data* ctx = new_list();
    /* call user method */
    tests(&ctx);
    /* destroy list */
    delete_list(&ctx);
}

/* uses the list */
static void tests(struct list_data** current) {
    u8* payload = (void*)0x7bde8421;
    void* is_null[] = {
        list_v1->peek(current),
        list_v1->pop(current)
    };
    RX_ASSERT(0 == is_null[0]);
    RX_ASSERT(0 == is_null[1]);
    list_v1->push(current, payload);
#ifdef USE_MEMORY_DEBUG_INFO
    list_v1->print_head(current);
#endif
    list_v1->push(current, ++payload);
#ifdef USE_MEMORY_DEBUG_INFO
    list_v1->print_head(current);
#endif
    list_v1->push(current, ++payload);
#ifdef USE_MEMORY_DEBUG_INFO
    list_v1->print_head(current);
#endif
    list_v1->push(current, ++payload);
#ifdef USE_MEMORY_DEBUG_INFO
    list_v1->print_head(current);
#endif
    list_v1->push(current, ++payload);
#ifdef USE_MEMORY_DEBUG_INFO
    list_v1->print_head(current);
#endif
#ifdef USE_MEMORY_DEBUG_INFO
    list_v1->print(current);
#endif
    const void* q_peek0 = list_v1->peek(current);
    CLEAN(q_peek0)
    void* q_pop0 = list_v1->pop(current);
#ifdef USE_MEMORY_DEBUG_INFO
    list_v1->print(current);
#endif
    const void* q_pop1 = list_v1->pop(current);
    CLEAN(q_pop1)
#ifdef USE_MEMORY_DEBUG_INFO
    list_v1->print(current);
#endif
    const void* q_pop2 = list_v1->pop(current);
    CLEAN(q_pop2)
#ifdef USE_MEMORY_DEBUG_INFO
    list_v1->print(current);
#endif
    const void* q_peek1 = list_v1->peek(current);
    void* q_pop3 = list_v1->pop(current);
    const void* q_peek2 = list_v1->peek(current);
    list_v1->push(current, q_pop3);
    const void* q_peek3 = list_v1->peek(current);
    RX_ASSERT(q_peek1 != q_peek2);
    RX_ASSERT(q_peek2 != q_peek3);
    RX_ASSERT(q_peek1 == q_peek3);
    const void* q_pop4 = list_v1->pop(current);
    CLEAN(q_pop4)
#ifdef USE_MEMORY_DEBUG_INFO
    list_v1->print(current);
#endif
    const void* q_pop5 = list_v1->pop(current);
    CLEAN(q_pop5)
#ifdef USE_MEMORY_DEBUG_INFO
    list_v1->print(current);
#endif
    const void* q_peek4 = list_v1->peek(current);
    list_v1->push(current, q_pop0);
    CLEAN(q_peek4)
#ifdef USE_MEMORY_DEBUG_INFO
    list_v1->print(current);
#endif
    const void* q_pop6 = list_v1->pop(current);
    CLEAN(q_pop6)
#ifdef USE_MEMORY_DEBUG_INFO
    list_v1->print(current);
#endif
    const void* q_pop7 = list_v1->pop(current);
    CLEAN(q_pop7)
#ifdef USE_MEMORY_DEBUG_INFO
    list_v1->print(current);
#endif
    const void* q_peek5 = list_v1->peek(current);
    CLEAN(q_peek5)
#ifdef USE_MEMORY_DEBUG_INFO
    list_v1->print(current);
#endif
}

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    struct list_data* ctx;
}* TEST_DATA;

/* Initialize the data structure. Its allocation is handled by Rexo. */
RX_SET_UP(test_set_up) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* initializes to 0 */
    *ctx = 0;
    /* initialize list */
    list_v1->init(ctx);
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* destroy list */
    list_v1->destroy(ctx);
    /* initializes to 0 */
    *ctx = 0;
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

/* test init */
RX_TEST_CASE(list_micro_tests, test_empty_list_count_equals_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* ensures counter is initialized to 0 */
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(list_micro_tests, test_pop_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* pushed to the list */
    const void* data_ptr = list_v1->pop(0);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(list_micro_tests, test_pop_null_ptr, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* pushed to the list */
    struct list_data* ptr = 0;
    struct list_data** null_ptr = &ptr;
    const void* data_ptr = list_v1->pop(null_ptr);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(list_micro_tests, test_peek_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* pushed to the list */
    const void* data_ptr = list_v1->peek(0);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(list_micro_tests, test_peek_null_ptr, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* pushed to the list */
    struct list_data* ptr = 0;
    struct list_data** null_ptr = &ptr;
    const void* data_ptr = list_v1->peek(null_ptr);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(list_micro_tests, test_push_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* pushed to the list */
    void* payload = (void*)0x12345678;
    list_v1->push(0, payload);
    /* ensures there is no result on 0 */
    const void* data_ptr = list_v1->peek(0);
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(list_micro_tests, test_push_null_ptr, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* pushed to the list */
    struct list_data* ptr = 0;
    struct list_data** null_ptr = &ptr;
    void* payload = (void*)0x12345678;
    list_v1->push(null_ptr, payload);
    /* ensures there is no result on 0 */
    const void* data_ptr = list_v1->peek(0);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(list_micro_tests, test_init_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* pushed to the list */
    list_v1->init(0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(list_micro_tests, test_init_null_ptr, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* pushed to the list */
    struct list_data* ptr = 0;
    struct list_data** null_ptr = &ptr;
    list_v1->init(null_ptr);
    const void* data_ptr = list_v1->peek(null_ptr);
    list_v1->destroy(null_ptr);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(list_micro_tests, test_destroy_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* pushed to the list */
    list_v1->destroy(0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(list_micro_tests, test_destroy_null_ptr, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* pushed to the list */
    struct list_data* ptr = 0;
    struct list_data** null_ptr = &ptr;
    const void* data_ptr = list_v1->peek(null_ptr);
    list_v1->destroy(null_ptr);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
}

/* test peek */
RX_TEST_CASE(list_micro_tests, test_standard_list_peek_does_not_changes_stack, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* prepares the payload */
    u8* payload = (void*)0x7bde8421;
    /* pushed to the list */
    list_v1->push(ctx, payload);
    /* gets the head pointer to the list */
    const struct list_data* ptr = *ctx;
    /* peeks from the list */
    const void* head = list_v1->peek(ctx);
    /* ensures payload is on top of the stack */
    RX_ASSERT(head == payload);
    /* ensures peek does not changes the head pointer */
    RX_ASSERT(ptr == *ctx);
}

/* test pop from 0 pointer */
RX_TEST_CASE(list_micro_tests, test_empty_list_pop_equals_0, .fixture = test_fixture) {
    struct list_data* ctx = 0;
    /* pops from the list */
    const struct list_data* head = list_v1->pop(&ctx);
    /* ensures head is not initialized */
    RX_ASSERT(head == 0);
}

/* test pop from 0 pointer */
RX_TEST_CASE(list_micro_tests, test_empty_list_peek_equals_0, .fixture = test_fixture) {
    struct list_data* ctx = 0;
    /* peeks from the list */
    const struct list_data* head = list_v1->peek(&ctx);
    /* ensures head is not initialized */
    RX_ASSERT(head == 0);
}

/* test alloc */
RX_TEST_CASE(list_micro_tests, test_alloc_count_eq_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* prepares the payload */
    u8* payload = (void*)0x7bde8421;
    /* pushes to the list */
    list_v1->push(ctx, payload);
    /* ensures data is added to the list */
    RX_ASSERT(*ctx != 0);
}

RX_TEST_CASE(list_micro_tests, test_alloc_payload, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* prepares the payload */
    u8* payload = (void*)0x7bde8421;
    /* pushes to the list */
    list_v1->push(ctx, payload);
    /* peeks from the list */
    const void* head = list_v1->peek(ctx);
    /* ensures data is added to the list */
    RX_ASSERT(head == payload);
}

RX_TEST_CASE(list_micro_tests, test_alloc_pop_count_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* prepares the payload */
    u8* payload = (void*)0x7bde8421;
    /* pushes to the list */
    list_v1->push(ctx, payload);
    /* pops from the list */
    const void* head = list_v1->pop(ctx);
    /* ensures data is added to the list */
    RX_ASSERT(head != 0);
}

RX_TEST_CASE(list_micro_tests, test_alloc_pop_payload, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* prepares the payload */
    u8* payload = (void*)0x7bde8421;
    /* pushes to the list */
    list_v1->push(ctx, payload);
    /* pops from the list */
    const void* head = list_v1->pop(ctx);
    /* ensures data is added to the list */
    RX_ASSERT(head == payload);
}

/* test peek */
RX_TEST_CASE(list_micro_tests, test_list_peek_is_zero, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* peeks from the list */
    const void* head = list_v1->peek(ctx);
    /* ensures head is not initialized */
    RX_ASSERT(head == 0);
}

/* test pop */
RX_TEST_CASE(list_micro_tests, test_list_pop_is_zero, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* pops from the list */
    const void* head = list_v1->pop(ctx);
    /* ensures head is not initialized */
    RX_ASSERT(head == 0);
}

static int run(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- acceptance test code\n");
#endif
    run_list1(tests);
    run_list2(tests);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- rexo unit test code\n");
#endif
    /* Execute the main function that runs the test cases found. */
    int result = rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;
    return result;
}

const test_suite list_micro_test_suite_definition_v1 = {
    .run = run
};
