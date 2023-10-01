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
#include "test_list-alloc.h"

/* macros */
#define MAX_ALLOC_SIZE 1024
#define N_ELEMENTS 1024 /* defines N-element collection*/
#define PTR_SIZE sizeof(void*) /* size of a pointer */
#define ALLOC_SIZE(n) (((n) + 1) * PTR_SIZE) /* buffer size in bytes = size of 8 items */

/* definition */
extern const struct list list_alloc_definition;
/* definition */
extern struct list_parameters list_parameters_definition;

/* initializes the new context's head element */
struct list_data* list_init(void);
/* destroys the memory stack */
void list_destroy(struct list_data* ptr);

/* allocates memory pointer for list object */
static struct list_data* new_list(void) {
    /* returns list object */
    return list_init();
}

/* releases memory pointer for list object */
static void delete_list(struct list_data** ctx) {
    /* gets pointer */
    struct list_data* ptr = *ctx;
    /* destroys the list */
    list_destroy(ptr);
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
    const struct list* list = &list_alloc_definition;
    u8* payload = (void*)0x7bde8421;
    void* is_null[] = {
        list->peek(current),
        list->pop(current)
    };
    RX_ASSERT(0 == is_null[0]);
    RX_ASSERT(0 == is_null[1]);
    list->push(current, payload);
#ifdef USE_VM_DEBUG_INFO
    list->print_head(current);
#endif
    list->push(current, ++payload);
#ifdef USE_VM_DEBUG_INFO
    list->print_head(current);
#endif
    list->push(current, ++payload);
#ifdef USE_VM_DEBUG_INFO
    list->print_head(current);
#endif
    list->push(current, ++payload);
#ifdef USE_VM_DEBUG_INFO
    list->print_head(current);
#endif
    list->push(current, ++payload);
#ifdef USE_VM_DEBUG_INFO
    list->print_head(current);
#endif
#ifdef USE_VM_DEBUG_INFO
    list->print(current);
#endif
    const void* q_peek0 = list->peek(current);
    CLEAN(q_peek0)
    void* q_pop0 = list->pop(current);
#ifdef USE_VM_DEBUG_INFO
    list->print(current);
#endif
    const void* q_pop1 = list->pop(current);
    CLEAN(q_pop1)
#ifdef USE_VM_DEBUG_INFO
    list->print(current);
#endif
    const void* q_pop2 = list->pop(current);
    CLEAN(q_pop2)
#ifdef USE_VM_DEBUG_INFO
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
#ifdef USE_VM_DEBUG_INFO
    list->print(current);
#endif
    const void* q_pop5 = list->pop(current);
    CLEAN(q_pop5)
#ifdef USE_VM_DEBUG_INFO
    list->print(current);
#endif
    const void* q_peek4 = list->peek(current);
    list->push(current, q_pop0);
    CLEAN(q_peek4)
#ifdef USE_VM_DEBUG_INFO
    list->print(current);
#endif
    const void* q_pop6 = list->pop(current);
    CLEAN(q_pop6)
#ifdef USE_VM_DEBUG_INFO
    list->print(current);
#endif
    const void* q_pop7 = list->pop(current);
    CLEAN(q_pop7)
#ifdef USE_VM_DEBUG_INFO
    list->print(current);
#endif
    const void* q_peek5 = list->peek(current);
    CLEAN(q_peek5)
#ifdef USE_VM_DEBUG_INFO
    list->print(current);
#endif
}

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    struct list_data* ctx;
} * TEST_DATA;

/* Initialize the data structure. Its allocation is handled by Rexo. */
RX_SET_UP(test_set_up) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* initialize list */
    *ctx = new_list();
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* gets the current memory pointer */
    struct list_data* ptr = *ctx;
    /* cleans up */
    delete_list(&ptr);
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

/* test init */
RX_TEST_CASE(list_alloc_tests, test_alloc_free, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    u64* alloc[MAX_ALLOC_SIZE];
    for (u64 i = 0; i < MAX_ALLOC_SIZE; i++) {
        alloc[i] = memory->alloc(i);
    }
    for (u64 i = 0; i < MAX_ALLOC_SIZE; i++) {
        memory->set(alloc[i], (u8)(i & 0xff), i);
    }
    for (u64 i = 0; i < MAX_ALLOC_SIZE; i++) {
        memory->free(alloc[i], i);
    }
    /* ensures counter is initialized to 0 */
    RX_ASSERT(*ctx != 0);
}

/* test init */
RX_TEST_CASE(list_alloc_tests, test_empty_list_count_equals_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* ensures counter is initialized to 0 */
    RX_ASSERT(*ctx != 0);
}

/* test peek */
RX_TEST_CASE(list_alloc_tests, test_standard_list_peek_does_not_changes_stack, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list* list = &list_alloc_definition;
    /* prepares the payload */
    u8* payload = (void*)0x7bde8421;
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
RX_TEST_CASE(list_alloc_tests, test_empty_list_pop_equals_0, .fixture = test_fixture) {
    struct list_data* ctx = 0;
    /* declares pointer to list functions definitions */
    const struct list* list = &list_alloc_definition;
    /* pops from the list */
    const struct list_data* head = list->pop(&ctx);
    /* ensures head is not initialized */
    RX_ASSERT(head == 0);
}

/* test pop from 0 pointer */
RX_TEST_CASE(list_alloc_tests, test_empty_list_peek_equals_0, .fixture = test_fixture) {
    struct list_data* ctx = 0;
    /* declares pointer to list functions definitions */
    const struct list* list = &list_alloc_definition;
    /* peeks from the list */
    const struct list_data* head = list->peek(&ctx);
    /* ensures head is not initialized */
    RX_ASSERT(head == 0);
}

/* test alloc */
RX_TEST_CASE(list_alloc_tests, test_alloc_count_eq_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list* list = &list_alloc_definition;
    /* prepares the payload */
    u8* payload = (void*)0x7bde8421;
    /* pushes to the list */
    list->push(ctx, payload);
    /* ensures data is added to the list */
    RX_ASSERT(*ctx != 0);
}

RX_TEST_CASE(list_alloc_tests, test_alloc_payload, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list* list = &list_alloc_definition;
    /* prepares the payload */
    u8* payload = (void*)0x7bde8421;
    /* pushes to the list */
    list->push(ctx, payload);
    /* peeks from the list */
    const void* head = list->peek(ctx);
    /* ensures data is added to the list */
    RX_ASSERT(head == payload);
}

RX_TEST_CASE(list_alloc_tests, test_alloc_pop_count_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list* list = &list_alloc_definition;
    /* prepares the payload */
    u8* payload = (void*)0x7bde8421;
    /* pushes to the list */
    list->push(ctx, payload);
    /* pops from the list */
    const void* head = list->pop(ctx);
    /* ensures data is added to the list */
    RX_ASSERT(head != 0);
}

RX_TEST_CASE(list_alloc_tests, test_global_alloc_0, .fixture = test_fixture) {
    void* data = memory->alloc(0);
    void* expected_value = 0;
    void* actual_value = data;
    RX_ASSERT(expected_value == actual_value);
}

RX_TEST_CASE(list_alloc_tests, test_global_alloc_1, .fixture = test_fixture) {
    void* data = memory->alloc(1);
    void* expected_value = 0;
    void* actual_value = data;
    RX_ASSERT(expected_value != actual_value);
    memory->free(data, 1);
}

RX_TEST_CASE(list_alloc_tests, test_global_free_1, .fixture = test_fixture) {
    u64* data = memory->alloc(1);
    memory->free(data, 1);
}

RX_TEST_CASE(list_alloc_tests, test_alloc_pop_payload, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list* list = &list_alloc_definition;
    /* prepares the payload */
    u8* payload = (void*)0x7bde8421;
    /* pushes to the list */
    list->push(ctx, payload);
    /* pops from the list */
    const void* head = list->pop(ctx);
    /* ensures data is added to the list */
    RX_ASSERT(head == payload);
}

/* test peek */
RX_TEST_CASE(list_alloc_tests, test_list_peek_is_zero, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list* list = &list_alloc_definition;
    /* peeks from the list */
    const void* head = list->peek(ctx);
    /* ensures head is not initialized */
    RX_ASSERT(head == 0);
}

/* test pop */
RX_TEST_CASE(list_alloc_tests, test_list_pop_is_zero, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list* list = &list_alloc_definition;
    /* pops from the list */
    const void* head = list->pop(ctx);
    /* ensures head is not initialized */
    RX_ASSERT(head == 0);
}

/* test push */
RX_TEST_CASE(list_alloc_tests, test_alloc_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list* list = &list_alloc_definition;
    /* prepares the payload */
    u8* payload = (void*)0x7bde8421;
    /* pushes to the list multiple times */
    list->push(ctx, payload);
    /* peeks from the list */
    const void* head = list->peek(ctx);
    /* ensures data is added to the list */
    RX_ASSERT(head == payload);
}

/* test push */
RX_TEST_CASE(list_alloc_tests, test_alloc_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list* list = &list_alloc_definition;
    /* prepares the payload */
    u8* payload = (void*)0x7bde8421;
    /* pushes to the list multiple times */
    list->push(ctx, payload);
    list->push(ctx, payload);
    /* peeks from the list */
    const void* head = list->peek(ctx);
    /* ensures data is added to the list */
    RX_ASSERT(head == payload);
}

/* test push */
RX_TEST_CASE(list_alloc_tests, test_alloc_3, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list* list = &list_alloc_definition;
    /* prepares the payload */
    u8* payload = (void*)0x7bde8421;
    /* pushes to the list multiple times */
    list->push(ctx, payload);
    list->push(ctx, payload);
    list->push(ctx, payload);
    /* peeks from the list */
    const void* head = list->peek(ctx);
    /* ensures data is added to the list */
    RX_ASSERT(head == payload);
}

/* test push */
RX_TEST_CASE(list_alloc_tests, test_alloc_5, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list* list = &list_alloc_definition;
    /* prepares the payload */
    u8* payload = (void*)0x7bde8421;
    /* pushes to the list multiple times */
    /* 1 */
    list->push(ctx, payload);
    list->push(ctx, payload);
    list->push(ctx, payload);
    list->push(ctx, payload);
    /* 5 */
    list->push(ctx, payload);
    /* peeks from the list */
    const void* head = list->peek(ctx);
    /* ensures data is added to the list */
    RX_ASSERT(head == payload);
}

/* test push */
RX_TEST_CASE(list_alloc_tests, test_alloc_7, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list* list = &list_alloc_definition;
    /* prepares the payload */
    u8* payload = (void*)0x7bde8421;
    /* pushes to the list multiple times */
    /* 1 */
    list->push(ctx, payload);
    list->push(ctx, payload);
    list->push(ctx, payload);
    list->push(ctx, payload);
    /* 5 */
    list->push(ctx, payload);
    list->push(ctx, payload);
    list->push(ctx, payload);
    /* peeks from the list */
    const void* head = list->peek(ctx);
    /* ensures data is added to the list */
    RX_ASSERT(head == payload);
}

/* test push */
RX_TEST_CASE(list_alloc_tests, test_alloc_9, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list* list = &list_alloc_definition;
    /* prepares the payload */
    u8* payload = (void*)0x7bde8421;
    /* pushes to the list multiple times */
    /* 1 */
    list->push(ctx, payload);
    list->push(ctx, payload);
    list->push(ctx, payload);
    list->push(ctx, payload);
    /* 5 */
    list->push(ctx, payload);
    list->push(ctx, payload);
    list->push(ctx, payload);
    list->push(ctx, payload);
    /* 9 */
    list->push(ctx, payload);
    /* peeks from the list */
    const void* head = list->peek(ctx);
    /* ensures data is added to the list */
    RX_ASSERT(head == payload);
}

/* test push */
RX_TEST_CASE(list_alloc_tests, test_alloc_4, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list* list = &list_alloc_definition;
    /* prepares the payload */
    u8* payload = (void*)0x7bde8421;
    /* pushes to the list multiple times */
    list->push(ctx, payload);
    list->push(ctx, payload);
    list->push(ctx, payload);
    list->push(ctx, payload);
    /* peeks from the list */
    const void* head = list->peek(ctx);
    /* ensures data is added to the list */
    RX_ASSERT(head == payload);
}

/* test push */
RX_TEST_CASE(list_alloc_tests, test_alloc_8, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list* list = &list_alloc_definition;
    /* prepares the payload */
    u8* payload = (void*)0x7bde8421;
    /* pushes to the list multiple times */
    list->push(ctx, payload);
    list->push(ctx, payload);
    list->push(ctx, payload);
    list->push(ctx, payload);
    list->push(ctx, payload);
    list->push(ctx, payload);
    list->push(ctx, payload);
    list->push(ctx, payload);
    /* peeks from the list */
    const void* head = list->peek(ctx);
    /* ensures data is added to the list */
    RX_ASSERT(head == payload);
}

/* test loop push */
RX_TEST_CASE(list_alloc_tests, test_list_push_pop, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    /* declares pointer to list functions definitions */
    const struct list* list = &list_alloc_definition;
    /* prepares the payload */
    u8* payload = (void*)0x7bde8421;
    /* record buffer has N items */
    void** _recorded = memory->alloc(ALLOC_SIZE(N_ELEMENTS));
    /* pushes all pseudo-random values */
    /* pushes to the list multiple times */
    int i = 0;
    do {
        /* generates random values */
        void* _payload = (void*)0x76543210;
        /* records value */
        _recorded[i] = _payload;
        /* pushes to the list */
        list->push(ctx, _payload);
        /* next step */
        i++;
    } while (i < N_ELEMENTS);
    /* records the last value */
    _recorded[i] = payload;
    /* pushes to the list */
    list->push(ctx, payload);
    /* prints the list */
#ifdef USE_VM_DEBUG_INFO
    list->print(ctx);
#endif
    /* peeks from the list */
    const void* head = list->peek(ctx);
    /* ensures payload is on top of the stack */
    RX_ASSERT(head == payload);
    /* ensures data is added to the list */
    do {
        /* pops from the list */
        const void* ptr = list->pop(ctx);
        /* ensures recorded values matches to the list values */
        RX_ASSERT(ptr == _recorded[i]);
        /* next step */
        i--;
    } while (i >= 0);
    /* releases memory allocated for the data */
    memory->free(_recorded, ALLOC_SIZE(N_ELEMENTS));
}

static int run(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- acceptance test code\n");
#endif
    struct list_parameters* parameters = &list_parameters_definition;
    parameters->block_size = 1;
    using_list1(list_using);
    parameters->block_size = 256;
    using_list2(list_using);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- rexo unit test code\n");
#endif
    /* Execute the main function that runs the test cases found. */
    int result = rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;
    return result;
}

const struct test_suite list_alloc_test_suite_definition = {
    .run = run
};
