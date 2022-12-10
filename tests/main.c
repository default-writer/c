#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "rexo/include/rexo.h"

#include "std/api.h"
#include "list/data.h"
#include "common/alloc.h"
#include "common/object.h"
#include "common/print.h"

#ifndef USE_MEMORY_LEAKS_DETECTION
const char* __asan_default_options() { return "detect_leaks=0"; }
#endif

extern const struct list_methods list_methods;

struct list_data* new_list()
{
    const struct list_methods* list = &list_methods;
    struct list_data* ctx;
    // init list
    list->init(&ctx, _new);
    // returns created object
    return ctx;
}

void delete_list(struct list_data* ctx)
{
    const struct list_methods* list = &list_methods;
    // destroy list
    list->destroy(&ctx, _delete, _next);
}

// default list usage scenario
void using_list(void (*list_using)(struct list_data** const)) {
    // initialize current context (stack)
    struct list_data* ctx = new_list();

    // call user method
    list_using(&ctx);

    // destroy list
    delete_list(ctx);
}

// default list usage scenario
void using_list2(void (*list_using)(struct list_data** const)) {
    // initialize current context (stack)
    struct list_data* ctx = new_list();

    // call user method
    list_using(&ctx);

    // destroy list
    delete_list(ctx);
}

// use list
void list_using(struct list_data** const current) {
    // access context's functions pointer
    const struct list_methods* list = &list_methods;
    void* payload = (void*)0xdeadbeef;
    struct list_data* is_null[] = {
        list->peek(current),
        list->pop(current)
    };
    if (0 != is_null[0]) {
        return;
    }
    if (0 != is_null[1]) {
        return;
    }
    struct list_data* tmp1 = list->alloc(payload);
    list->push(current, tmp1);
#ifdef USE_MEMORY_DEBUG_INFO
    list_print_head(current, _data);
#endif
    struct list_data* tmp2 = list->alloc(++payload);
    list->push(current, tmp2);
#ifdef USE_MEMORY_DEBUG_INFO
    list_print_head(current, _data);
#endif
    struct list_data* tmp3 = list->alloc(++payload);
    list->push(current, tmp3);
#ifdef USE_MEMORY_DEBUG_INFO
    list_print_head(current, _data);
#endif
    struct list_data* tmp4 = list->alloc(++payload);
    list->push(current, tmp4);
#ifdef USE_MEMORY_DEBUG_INFO
    list_print_head(current, _data);
#endif
    struct list_data* tmp5 = list->alloc(++payload);
    list->push(current, tmp5);
#ifdef USE_MEMORY_DEBUG_INFO
    list_print_head(current, _data);
#endif
#ifdef USE_MEMORY_DEBUG_INFO
    list_print(current, _next);
#endif
    struct list_data* q_pop0 = list->pop(current); 
    list->free(&q_pop0);
#ifdef USE_MEMORY_DEBUG_INFO
    list_print(current, _next);
#endif
    struct list_data* q_pop1 = list->pop(current); 
    list->free(&q_pop1);
#ifdef USE_MEMORY_DEBUG_INFO
    list_print(current, _next);
#endif
    struct list_data* q_pop2 = list->pop(current); 
    list->free(&q_pop2);
#ifdef USE_MEMORY_DEBUG_INFO
    list_print(current, _next);
#endif
    struct list_data* q_pop3 = list->pop(current); 
    list->push(current, q_pop3);
    q_pop3 = list->pop(current); 
    list->free(&q_pop3);
#ifdef USE_MEMORY_DEBUG_INFO
    list_print(current, _next);
#endif
    struct list_data* q_pop4 = list->pop(current); 
    list->free(&q_pop4);
#ifdef USE_MEMORY_DEBUG_INFO
    list_print(current, _next);
#endif
    struct list_data* q_pop5 = list->peek(current); 
    list->free(&q_pop5);
    list->push(current, q_pop0);
#ifdef USE_MEMORY_DEBUG_INFO
    list_print(current, _next);
#endif
    list->free(&q_pop0);
#ifdef USE_MEMORY_DEBUG_INFO
    list_print(current, _next);
#endif
}

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    struct list_data* ctx;
} *TEST_DATA;

/* Initialize the data structure. Its allocation is handled by Rexo. */
RX_SET_UP(test_set_up)
{
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // access context's functions pointer
    const struct list_methods* list = &list_methods;

    // initilize list
    list->init(ctx, _new);

    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down)
{
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // access context's functions pointer
    const struct list_methods* list = &list_methods;
    // destroy list
    list->destroy(ctx, _delete, _next);
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

// test init
RX_TEST_CASE(myTestSuite, test_empty_list_count_equals_0, .fixture = test_fixture)
{
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;

    // enshure that counter is initialized to 0
    RX_ASSERT(*ctx != 0);
}

// test alloc
RX_TEST_CASE(myTestSuite, test_list_alloc_count_eq_1, .fixture = test_fixture)
{
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;

    // create list
    const struct list_methods* list = &list_methods;
    void* payload = (void*)0xdeadbeef;

    struct list_data* tmp = list->alloc(payload);
    list->push(ctx, tmp);

    // ensure that data being added to list
    RX_ASSERT(*ctx != 0);
}

RX_TEST_CASE(myTestSuite, test_list_alloc_payload, .fixture = test_fixture)
{
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;

    // create list
    const struct list_methods* list = &list_methods;
    void* payload = (void*)0xdeadbeef;

    struct list_data* tmp = list->alloc(payload);
    list->push(ctx, tmp);

    const struct list_data* head = list->peek(ctx);

    // ensure that data being added to list
    RX_ASSERT(head->payload == payload);
}

RX_TEST_CASE(myTestSuite, test_list_alloc_pop_count_0, .fixture = test_fixture)
{
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;

    // create list
    const struct list_methods* list = &list_methods;
    void* payload = (void*)0xdeadbeef;

    struct list_data* tmp = list->alloc(payload);
    list->push(ctx, tmp);

    struct list_data* head = list->pop(ctx);

    list->free(&head);

    RX_ASSERT(head == 0);
}

RX_TEST_CASE(myTestSuite, test_list_alloc_pop_payload, .fixture = test_fixture)
{
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;

    // create list
    const struct list_methods* list = &list_methods;
    void* payload = (void*)0xdeadbeef;

    struct list_data* tmp = list->alloc(payload);
    list->push(ctx, tmp);

    struct list_data* head = list->pop(ctx);
    const void* head_payload = head->payload;
    list->free(&head);

    // ensure that data being added to list
    RX_ASSERT(head_payload == payload);
}

RX_TEST_CASE(myTestSuite, test_list_alloc_and_prev_next_equals_0, .fixture = test_fixture)
{
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;

    // create list
    const struct list_methods* list = &list_methods;
    void* payload = (void*)0xdeadbeef;

    struct list_data* tmp = list->alloc(payload);
    list->push(ctx, tmp);

    struct list_data* current = list->pop(ctx);
    const struct list_data* prev = current->next;
    const struct list_data* next = current->prev;
    const void* head_payload = current->payload;
    list->free(&current);

    // ensure that data being added to list
    RX_ASSERT(head_payload == payload);
    RX_ASSERT(prev == 0);
    RX_ASSERT(next == 0);
}

RX_TEST_CASE(myTestSuite, test_list_free_head, .fixture = test_fixture)
{
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;

    // create list
    const struct list_methods* list = &list_methods;
    void* payload = (void*)0xdeadbeef;

    struct list_data* tmp = list->alloc(payload);
    list->push(ctx, tmp);

    struct list_data* head = list->pop(ctx);
    list->free(&head);

    // ensure that data pointer is zero
    RX_ASSERT(head == 0);
}

// test peek
RX_TEST_CASE(myTestSuite, test_list_peek_is_zero, .fixture = test_fixture)
{
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;

    // create list
    const struct list_methods* list = &list_methods;

    const struct list_data* head = list->peek(ctx);

    // ensure that data being added to list
    RX_ASSERT(head == 0);
}

// test pop
RX_TEST_CASE(myTestSuite, test_list_pop_is_zero, .fixture = test_fixture)
{
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;

    // create list
    const struct list_methods* list = &list_methods;

    const struct list_data* head = list->pop(ctx);

    // ensure that data being added to list
    RX_ASSERT(head == 0);
}

int main()
{
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- acceptance test code\n");
#endif
    // some messy code
    using_list(list_using);
    using_list2(list_using);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- rexo unit test code\n");
#endif
    /* Execute the main function that runs the test cases found. */
    return rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;
}