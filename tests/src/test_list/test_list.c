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
#include "test_list.h"
#include "list/v1/list_v1.h"

extern const struct list_methods_v1 list_definition;

/* allocates memory pointer for list object */
static struct list_v1* new_list(void) {
    /* declares pointer to list functions definitions */
    const struct list_methods_v1* list = &list_definition;
    struct list_v1* ctx = 0;
    /* initializes the list */
    list->init(&ctx);
    /* returns list object */
    return ctx;
}

/* releases memory pointer for list object */
static void delete_list(struct list_v1** ctx) {
    /* declares pointer to list functions definitions */
    const struct list_methods_v1* list = &list_definition;
    /* destroys the list */
    list->destroy(ctx);
    /* cleans up */
    *ctx = 0;
}

/* runs default list usage scenario */
static void using_list1(void (*list_using)(struct list_v1** const)) {
    /* initialize current context (stack) */
    struct list_v1* ctx = new_list();
    /* call user method */
    list_using(&ctx);
    /* destroy list */
    delete_list(&ctx);
}

/* runs default list usage scenario */
static void using_list2(void (*list_using)(struct list_v1** const)) {
    /* initialize current context (stack) */
    struct list_v1* ctx = new_list();
    /* call user method */
    list_using(&ctx);
    /* destroy list */
    delete_list(&ctx);
}

/* uses the list */
static void list_using(struct list_v1** current) {
    /* declares pointer to list functions definitions */
    const struct list_methods_v1* list = &list_definition;
    /* prepares the payload */
    u8* payload = (void*)0x7bde8421;
    /* peeks and pops from empty list */
    struct list_v1* is_null[] = {
        list->peek(current),
        list->pop(current)
    };
    RX_ASSERT(0 == is_null[0]);
    RX_ASSERT(0 == is_null[1]);
    struct list_v1* tmp1 = list->alloc(payload);
    list->push(current, tmp1);
#ifdef USE_VM_DEBUG_INFO
    list->print_head(current);
#endif
    struct list_v1* tmp2 = list->alloc(++payload);
    list->push(current, tmp2);
#ifdef USE_VM_DEBUG_INFO
    list->print_head(current);
#endif
    struct list_v1* tmp3 = list->alloc(++payload);
    list->push(current, tmp3);
#ifdef USE_VM_DEBUG_INFO
    list->print_head(current);
#endif
    struct list_v1* tmp4 = list->alloc(++payload);
    list->push(current, tmp4);
#ifdef USE_VM_DEBUG_INFO
    list->print_head(current);
#endif
    struct list_v1* tmp5 = list->alloc(++payload);
    list->push(current, tmp5);
#ifdef USE_VM_DEBUG_INFO
    list->print_head(current);
#endif
#ifdef USE_VM_DEBUG_INFO
    list->print(current);
#endif
    struct list_v1* q_pop0 = list->pop(current);
    list->free(&q_pop0);
#ifdef USE_VM_DEBUG_INFO
    list->print(current);
#endif
    struct list_v1* q_pop1 = list->pop(current);
    list->free(&q_pop1);
#ifdef USE_VM_DEBUG_INFO
    list->print(current);
#endif
    struct list_v1* q_pop2 = list->pop(current);
    list->free(&q_pop2);
#ifdef USE_VM_DEBUG_INFO
    list->print(current);
#endif
    struct list_v1* q_pop3 = list->pop(current);
    list->push(current, q_pop3);
    q_pop3 = list->pop(current);
    list->free(&q_pop3);
#ifdef USE_VM_DEBUG_INFO
    list->print(current);
#endif
    struct list_v1* q_pop4 = list->pop(current);
    list->free(&q_pop4);
#ifdef USE_VM_DEBUG_INFO
    list->print(current);
#endif
    struct list_v1* q_pop5 = list->peek(current);
    list->free(&q_pop5);
    list->push(current, q_pop0);
#ifdef USE_VM_DEBUG_INFO
    list->print(current);
#endif
    list->free(&q_pop0);
#ifdef USE_VM_DEBUG_INFO
    list->print(current);
#endif
}

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    struct list_v1* ctx;
}* TEST_DATA;

/* Initialize the data structure. Its allocation is handled by Rexo. */
RX_SET_UP(test_set_up) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_v1** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list_methods_v1* list = &list_definition;
    /* initializes to 0 */
    *ctx = 0;
    /* initialize list */
    list->init(ctx);
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_v1** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list_methods_v1* list = &list_definition;
    /* destroy list */
    list->destroy(ctx);
    /* initializes to 0 */
    *ctx = 0;
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

/* test init */
RX_TEST_CASE(list_tests, test_list_data_v1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_v1** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list_methods_v1* list = &list_definition;
    /* prepares the payload */
    u8* payload = (void*)0x7bde8421;

    /* allocates the memory for the structure */
    struct list_v1* tmp = list->alloc(payload);
    /* reads the data */
    const void* data = tmp->data;

    /* ensures data is added to the list */
    RX_ASSERT(data != 0);
    /* ensures payload is on top of the stack */
    RX_ASSERT(tmp->data == payload);

    /* peeks from the list */
    list->free(&tmp);

    /* gets the head pointer to the list */
    const struct list_v1* ptr = *ctx;

    /* ensures peek does not changes the head pointer */
    RX_ASSERT(ptr == *ctx);
    RX_ASSERT(*ctx != 0);
}

/* test init */
RX_TEST_CASE(list_tests, test_empty_list_count_equals_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_v1** ctx = &rx->ctx;
    /* ensures counter is initialized to 0 */
    RX_ASSERT(*ctx != 0);
}

/* test peek */
RX_TEST_CASE(list_tests, test_standard_list_peek_does_not_changes_stack, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_v1** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list_methods_v1* list = &list_definition;
    /* prepares the payload */
    u8* payload = (void*)0x7bde8421;
    /* allocates the memory for the structure */
    struct list_v1* tmp = list->alloc(payload);
    /* pushed to the list */
    list->push(ctx, tmp);
    /* gets the head pointer to the list */
    const struct list_v1* ptr = *ctx;
    /* peeks from the list */
    const struct list_v1* head = list->peek(ctx);
    /* ensures data is added to the list */
    RX_ASSERT(head != 0);
    /* ensures payload is on top of the stack */
    RX_ASSERT(head->data == payload);
    /* ensures peek does not changes the head pointer */
    RX_ASSERT(ptr == *ctx);
}

/* test pop from 0 pointer */
RX_TEST_CASE(list_tests, test_empty_list_pop_equals_0, .fixture = test_fixture) {
    struct list_v1* ctx = 0;
    /* declares pointer to list functions definitions */
    const struct list_methods_v1* list = &list_definition;
    /* pups out the current head element */
    const struct list_v1* head = list->pop(&ctx);
    /* ensures head is not initialized */
    RX_ASSERT(head == 0);
}

/* test pop from 0 pointer */
RX_TEST_CASE(list_tests, test_empty_list_peek_equals_0, .fixture = test_fixture) {
    struct list_v1* ctx = 0;
    /* declares pointer to list functions definitions */
    const struct list_methods_v1* list = &list_definition;
    /* peeks up the current head element */
    const struct list_v1* head = list->peek(&ctx);
    /* ensures head is not initialized */
    RX_ASSERT(head == 0);
}

/* test alloc */
RX_TEST_CASE(list_tests, test_alloc_count_eq_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_v1** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list_methods_v1* list = &list_definition;
    /* prepares the payload */
    u8* payload = (void*)0x7bde8421;
    /* allocates the memory for the structure */
    struct list_v1* tmp = list->alloc(payload);
    /* pushes to the list */
    list->push(ctx, tmp);
    /* ensures data is added to the list */
    RX_ASSERT(*ctx != 0);
}

RX_TEST_CASE(list_tests, test_alloc_payload, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_v1** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list_methods_v1* list = &list_definition;
    /* prepares the payload */
    u8* payload = (void*)0x7bde8421;
    /* allocates the memory for the structure */
    struct list_v1* tmp = list->alloc(payload);
    /* pushes to the list */
    list->push(ctx, tmp);
    /* peeks from list */
    const struct list_v1* head = list->peek(ctx);
    /* ensures data is added to the list */
    RX_ASSERT(head->data == payload);
}

RX_TEST_CASE(list_tests, test_alloc_pop_count_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_v1** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list_methods_v1* list = &list_definition;
    /* prepares the payload */
    u8* payload = (void*)0x7bde8421;
    /* allocates the memory for the structure */
    struct list_v1* tmp = list->alloc(payload);
    list->push(ctx, tmp);
    /* pop from the list */
    struct list_v1* head = list->pop(ctx);
    /* releases memory allocated for the data */
    list->free(&head);
    /* ensures head is not initialized */
    RX_ASSERT(head == 0);
}

RX_TEST_CASE(list_tests, test_alloc_pop_payload, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_v1** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list_methods_v1* list = &list_definition;
    /* prepares the payload */
    u8* payload = (void*)0x7bde8421;
    /* allocates the memory for the structure */
    struct list_v1* tmp = list->alloc(payload);
    /* pushes to the list */
    list->push(ctx, tmp);
    /* pops from the list */
    struct list_v1* head = list->pop(ctx);
    /* retrieves the data from the top of the list */
    const void* head_payload = head->data;
    /* releases memory allocated for the data */
    list->free(&head);
    /* ensures data is added to the list */
    RX_ASSERT(head_payload == payload);
}

RX_TEST_CASE(list_tests, test_alloc_and_prev_next_equals_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_v1** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list_methods_v1* list = &list_definition;
    /* prepares the payload */
    u8* payload = (void*)0x7bde8421;
    /* allocates the memory for the structure */
    struct list_v1* tmp = list->alloc(payload);
    /* pushes to the list */
    list->push(ctx, tmp);
    /* peeks from the list */
    const struct list_v1* head = list->peek(ctx);
    /* ensures peek does data release */
    RX_ASSERT(head->prev == 0);
    /* pops from the list */
    struct list_v1* current = list->pop(ctx);
    const struct list_v1* prev = current->next;
    const struct list_v1* next = current->prev;
    const void* head_payload = current->data;
    list->free(&current);
    /* ensures data is added to the list */
    RX_ASSERT(head_payload == payload);
#ifdef USE_MEMORY_CLEANUP
    /* ensures peek does data release */
    RX_ASSERT(prev == 0);
    /* ensures peek does data release */
    RX_ASSERT(next == 0);
#endif
}

RX_TEST_CASE(list_tests, test_free_head, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_v1** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list_methods_v1* list = &list_definition;
    /* prepares the payload */
    u8* payload = (void*)0x7bde8421;
    /* allocates the memory for the structure */
    struct list_v1* tmp = list->alloc(payload);
    list->push(ctx, tmp);
    /* pops from the list */
    struct list_v1* head = list->pop(ctx);
    /* releases memory allocated for the data */
    list->free(&head);
    /* ensures head is not initialized */
    RX_ASSERT(head == 0);
}

/* test peek */
RX_TEST_CASE(list_tests, test_list_peek_is_zero, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_v1** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list_methods_v1* list = &list_definition;
    /* peeks from the list */
    const struct list_v1* head = list->peek(ctx);
    /* ensures head is not initialized */
    RX_ASSERT(head == 0);
}

/* test pop */
RX_TEST_CASE(list_tests, test_list_pop_is_zero, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_v1** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list_methods_v1* list = &list_definition;
    /* pop from the list */
    const struct list_v1* head = list->pop(ctx);
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

const struct test_suite list_test_suite_definition = {
    .run = run
};
