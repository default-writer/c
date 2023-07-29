/*
 *
 * MIT License
 *
 * Copyright (c) 2023 Artur Mustafin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */
#include "test_list-micro.h"

extern const struct list list_micro_definition;

/* allocates memory pointer for list object */
static struct list_data* new_list(void) {
    /* declares pointer to list functions definitions */
    const struct list* list = &list_micro_definition;
    struct list_data* ctx = 0;
    /* initializes the list */
    list->init(&ctx);
    /* returns list object */
    return ctx;
}

/* releases memory pointer for list object */
static void delete_list(struct list_data** ctx) {
    /* declares pointer to list functions definitions */
    const struct list* list = &list_micro_definition;
    /* destroys the list */
    list->destroy(ctx);
    /* cleans up */
    *ctx = 0;
}

/* runs default list usage scenario */
static void using_list1(void (*list_using)(struct list_data** const)) {
    /* initialize current context (stack) */
    struct list_data* ctx = new_list();
    /* call user method */
    list_using(&ctx);
    /* destroy list */
    delete_list(&ctx);
}

/* runs default list usage scenario */
static void using_list2(void (*list_using)(struct list_data** const)) {
    /* initialize current context (stack) */
    struct list_data* ctx = new_list();
    /* call user method */
    list_using(&ctx);
    /* destroy list */
    delete_list(&ctx);
}

/* uses the list */
static void list_using(struct list_data** current) {
    /* declares pointer to list functions definitions */
    const struct list* list = &list_micro_definition;
    u8* payload = (void*)0xdeadbeef;
    void* is_null[] = {
        list->peek(current),
        list->pop(current)
    };
    RX_ASSERT(0 == is_null[0]);
    RX_ASSERT(0 == is_null[1]);
    list->push(current, payload);
#ifdef VM_DEBUG_INFO
    list->print_head(current);
#endif
    list->push(current, ++payload);
#ifdef VM_DEBUG_INFO
    list->print_head(current);
#endif
    list->push(current, ++payload);
#ifdef VM_DEBUG_INFO
    list->print_head(current);
#endif
    list->push(current, ++payload);
#ifdef VM_DEBUG_INFO
    list->print_head(current);
#endif
    list->push(current, ++payload);
#ifdef VM_DEBUG_INFO
    list->print_head(current);
#endif
#ifdef VM_DEBUG_INFO
    list->print(current);
#endif
    const void* q_peek0 = list->peek(current);
    CLEAN(q_peek0)
    void* q_pop0 = list->pop(current);
#ifdef VM_DEBUG_INFO
    list->print(current);
#endif
    const void* q_pop1 = list->pop(current);
    CLEAN(q_pop1)
#ifdef VM_DEBUG_INFO
    list->print(current);
#endif
    const void* q_pop2 = list->pop(current);
    CLEAN(q_pop2)
#ifdef VM_DEBUG_INFO
    list->print(current);
#endif
    const void* q_peek1 = list->peek(current);
    void* q_pop3 = list->pop(current);
    const void* q_peek2 = list->peek(current);
    list->push(current, q_pop3);
    const void* q_peek3 = list->peek(current);
    RX_ASSERT(q_peek1 != q_peek2);
    RX_ASSERT(q_peek2 != q_peek3);
    RX_ASSERT(q_peek1 == q_peek3);
    const void* q_pop4 = list->pop(current);
    CLEAN(q_pop4)
#ifdef VM_DEBUG_INFO
    list->print(current);
#endif
    const void* q_pop5 = list->pop(current);
    CLEAN(q_pop5)
#ifdef VM_DEBUG_INFO
    list->print(current);
#endif
    const void* q_peek4 = list->peek(current);
    list->push(current, q_pop0);
    CLEAN(q_peek4)
#ifdef VM_DEBUG_INFO
    list->print(current);
#endif
    const void* q_pop6 = list->pop(current);
    CLEAN(q_pop6)
#ifdef VM_DEBUG_INFO
    list->print(current);
#endif
    const void* q_pop7 = list->pop(current);
    CLEAN(q_pop7)
#ifdef VM_DEBUG_INFO
    list->print(current);
#endif
    const void* q_peek5 = list->peek(current);
    CLEAN(q_peek5)
#ifdef VM_DEBUG_INFO
    list->print(current);
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
    /* declares pointer to list functions definitions */
    const struct list* list = &list_micro_definition;
    /* initializes to 0 */
    *ctx = 0;
    /* initialize list */
    list->init(ctx);
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list* list = &list_micro_definition;
    /* destroy list */
    list->destroy(ctx);
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
    /* declares pointer to list functions definitions */
    const struct list* list = &list_micro_definition;
    /* pushed to the list */
    const void* data_ptr = list->pop(0);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(list_micro_tests, test_pop_null_ptr, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list* list = &list_micro_definition;
    /* pushed to the list */
    struct list_data* ptr = 0;
    struct list_data** null_ptr = &ptr;
    const void* data_ptr = list->pop(null_ptr);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(list_micro_tests, test_peek_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list* list = &list_micro_definition;
    /* pushed to the list */
    const void* data_ptr = list->peek(0);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(list_micro_tests, test_peek_null_ptr, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list* list = &list_micro_definition;
    /* pushed to the list */
    struct list_data* ptr = 0;
    struct list_data** null_ptr = &ptr;
    const void* data_ptr = list->peek(null_ptr);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(list_micro_tests, test_push_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list* list = &list_micro_definition;
    /* pushed to the list */
    void* payload = (void*)0x12345678;
    list->push(0, payload);
    /* ensures there is no result on 0 */
    const void* data_ptr = list->peek(0);
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(list_micro_tests, test_push_null_ptr, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list* list = &list_micro_definition;
    /* pushed to the list */
    struct list_data* ptr = 0;
    struct list_data** null_ptr = &ptr;
    void* payload = (void*)0x12345678;
    list->push(null_ptr, payload);
    /* ensures there is no result on 0 */
    const void* data_ptr = list->peek(0);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(list_micro_tests, test_init_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list* list = &list_micro_definition;
    /* pushed to the list */
    list->init(0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(list_micro_tests, test_init_null_ptr, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list* list = &list_micro_definition;
    /* pushed to the list */
    struct list_data* ptr = 0;
    struct list_data** null_ptr = &ptr;
    list->init(null_ptr);
    const void* data_ptr = list->peek(null_ptr);
    list->destroy(null_ptr);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(list_micro_tests, test_destroy_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list* list = &list_micro_definition;
    /* pushed to the list */
    list->destroy(0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
}

/* test case */
RX_TEST_CASE(list_micro_tests, test_destroy_null_ptr, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list* list = &list_micro_definition;
    /* pushed to the list */
    struct list_data* ptr = 0;
    struct list_data** null_ptr = &ptr;
    const void* data_ptr = list->peek(null_ptr);
    list->destroy(null_ptr);
    /* ensures there is no result on 0 */
    RX_ASSERT(data_ptr == 0);
    /* ensures pop does not zeroes the head pointer */
    RX_ASSERT(*ctx != 0);
}

/* test peek */
RX_TEST_CASE(list_micro_tests, test_standard_list_peek_does_not_changes_stack, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list* list = &list_micro_definition;
    /* prepares the payload */
    u8* payload = (void*)0xdeadbeef;
    /* pushed to the list */
    list->push(ctx, payload);
    /* gets the head pointer to the list */
    const struct list_data* ptr = *ctx;
    /* peeks from the list */
    const void* head = list->peek(ctx);
    /* ensures payload is on top of the stack */
    RX_ASSERT(head == payload);
    /* ensures peek does not changes the head pointer */
    RX_ASSERT(ptr == *ctx);
}

/* test pop from 0 pointer */
RX_TEST_CASE(list_micro_tests, test_empty_list_pop_equals_0, .fixture = test_fixture) {
    struct list_data* ctx = 0;
    /* declares pointer to list functions definitions */
    const struct list* list = &list_micro_definition;
    /* pops from the list */
    const struct list_data* head = list->pop(&ctx);
    /* ensures head is not initialized */
    RX_ASSERT(head == 0);
}

/* test pop from 0 pointer */
RX_TEST_CASE(list_micro_tests, test_empty_list_peek_equals_0, .fixture = test_fixture) {
    struct list_data* ctx = 0;
    /* declares pointer to list functions definitions */
    const struct list* list = &list_micro_definition;
    /* peeks from the list */
    const struct list_data* head = list->peek(&ctx);
    /* ensures head is not initialized */
    RX_ASSERT(head == 0);
}

/* test alloc */
RX_TEST_CASE(list_micro_tests, test_alloc_count_eq_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list* list = &list_micro_definition;
    /* prepares the payload */
    u8* payload = (void*)0xdeadbeef;
    /* pushes to the list */
    list->push(ctx, payload);
    /* ensures data is added to the list */
    RX_ASSERT(*ctx != 0);
}

RX_TEST_CASE(list_micro_tests, test_alloc_payload, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list* list = &list_micro_definition;
    /* prepares the payload */
    u8* payload = (void*)0xdeadbeef;
    /* pushes to the list */
    list->push(ctx, payload);
    /* peeks from the list */
    const void* head = list->peek(ctx);
    /* ensures data is added to the list */
    RX_ASSERT(head == payload);
}

RX_TEST_CASE(list_micro_tests, test_alloc_pop_count_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list* list = &list_micro_definition;
    /* prepares the payload */
    u8* payload = (void*)0xdeadbeef;
    /* pushes to the list */
    list->push(ctx, payload);
    /* pops from the list */
    const void* head = list->pop(ctx);
    /* ensures data is added to the list */
    RX_ASSERT(head != 0);
}

RX_TEST_CASE(list_micro_tests, test_alloc_pop_payload, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list* list = &list_micro_definition;
    /* prepares the payload */
    u8* payload = (void*)0xdeadbeef;
    /* pushes to the list */
    list->push(ctx, payload);
    /* pops from the list */
    const void* head = list->pop(ctx);
    /* ensures data is added to the list */
    RX_ASSERT(head == payload);
}

/* test peek */
RX_TEST_CASE(list_micro_tests, test_list_peek_is_zero, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list* list = &list_micro_definition;
    /* peeks from the list */
    const void* head = list->peek(ctx);
    /* ensures head is not initialized */
    RX_ASSERT(head == 0);
}

/* test pop */
RX_TEST_CASE(list_micro_tests, test_list_pop_is_zero, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list* list = &list_micro_definition;
    /* pops from the list */
    const void* head = list->pop(ctx);
    /* ensures head is not initialized */
    RX_ASSERT(head == 0);
}

static int run(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- acceptance test code\n");
#endif
    using_list1(list_using);
    using_list2(list_using);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- rexo unit test code\n");
#endif
    /* Execute the main function that runs the test cases found. */
    int result = rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;
    return result;
}

const struct test_suite list_micro_test_suite_definition = {
    .run = run
};