#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "list-micro/api.h"

/* Force Rexo's compatibility with C89. */
#define RX_ENABLE_C89_COMPAT

#include <rexo.h>

// queue/list context: head
struct list_context_class {
    // head element
    struct list_data* head;
};

struct list_data* new_list()
{
    const struct list_class* list = &list_definition;
    struct list_data* ctx;
    // init list
    list->self->init(&ctx);
    // returns created object
    return ctx;
} 

void delete_list(struct list_data* ctx)
{
    const struct list_class* list = &list_definition;
    // destroy list
    list->self->destroy(&ctx);
}

// default list usage scenario
void using_list(void (*list_using)(struct list_data** const)) {
    // initialize current context (stack)
    struct list_data* ctx = new_list();

    // call user method
    list_using(&ctx);

    // finalize class
    delete_list(ctx);
}

// default list usage scenario
void using_list2(void (*list_using)(struct list_data** const)) {
    // initialize current context (stack)
    struct list_data* ctx = new_list();

    // call user method
    list_using(&ctx);

    // finalize class
    delete_list(ctx);
}

// use list
void list_using(struct list_data** const current) {
    // access context's functions pointer
    const struct list_class* list = &list_definition;
    ADDR* payload = (ADDR*)0xdeadbeef;
    void* is_null[] = {
        list->self->pop(current)
    };
    if (0 != is_null[0]) {
        return;
    }
    list->self->push(current, payload);
    #ifdef DEBUG_ALLOC
    list_print_head(current);
    #endif
    list->self->push(current, ++payload);
    #ifdef DEBUG_ALLOC
    list_print_head(current);
    #endif
    list->self->push(current, ++payload);
    #ifdef DEBUG_ALLOC
    list_print_head(current);
    #endif
    list->self->push(current, ++payload);
    #ifdef DEBUG_ALLOC
    list_print_head(current);
    #endif
    list->self->push(current, ++payload);
    #ifdef DEBUG_ALLOC
    list_print_head(current);
    #endif
    printf("\n");
    #ifdef DEBUG_ALLOC
    list_print(current);
    #endif
    void* q_pop0 = list->self->pop(current);
    q_pop0 = q_pop0;
    #ifdef DEBUG_ALLOC
    list_print(current);
    #endif
    void* q_pop1 = list->self->pop(current); 
    q_pop1 = q_pop1;
    #ifdef DEBUG_ALLOC
    list_print(current);
    #endif
    void* q_pop2 = list->self->pop(current); 
    q_pop2 = q_pop2;
    #ifdef DEBUG_ALLOC
    list_print(current);
    #endif
    void* q_pop3 = list->self->pop(current); 
    list->self->push(current, q_pop3);
    q_pop3 = list->self->pop(current); 
    #ifdef DEBUG_ALLOC
    list_print(current);
    #endif
    void* q_pop4 = list->self->pop(current);
    q_pop4 = q_pop4;
    #ifdef DEBUG_ALLOC
    list_print(current);
    #endif
    void* q_pop5 = list->self->pop(current); 
    q_pop5 = q_pop5;
    #ifdef DEBUG_ALLOC
    list_print(current);
    #endif
    void* q_pop6 = list->self->pop(current); 
    q_pop6 = q_pop6;
    #ifdef DEBUG_ALLOC
    list_print(current);
    #endif
}

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    struct list_context_class ctx;
} *TEST_DATA;

/* Initialize the data structure. Its allocation is handled by Rexo. */
RX_SET_UP(test_set_up)
{
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_context_class* ctx = &rx->ctx;
    // access context's functions pointer
    const struct list_class* list = &list_definition;
    
    // initialize list
    list->self->init(&ctx->head);
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down)
{
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_context_class* ctx = &rx->ctx;
    // access context's functions pointer
    const struct list_class* list = &list_definition;
    
    // destroy list
    list->self->destroy(&ctx->head);
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

// test init
RX_TEST_CASE(myTestSuite, test_empty_list_count_equals_0, .fixture = test_fixture)
{
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const struct list_context_class* ctx = &rx->ctx;

    // enshure that counter is initialized to 0
    RX_REQUIRE(ctx->head != 0);
}

// test alloc
RX_TEST_CASE(myTestSuite, test_list_alloc_count_eq_1, .fixture = test_fixture)
{
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_context_class* ctx = &rx->ctx;

    // create list
    const struct list_class* list = &list_definition;
    void* payload = (void*)0xdeadbeef;

    list->self->push(&ctx->head, payload);

    // ensure that data being added to list
    RX_REQUIRE(ctx->head != 0);
}

RX_TEST_CASE(myTestSuite, test_list_alloc_pop_count_0, .fixture = test_fixture)
{
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_context_class* ctx = &rx->ctx;

    // create list
    const struct list_class* list = &list_definition;
    void* payload = (void*)0xdeadbeef;

    list->self->push(&ctx->head, payload);
    void* head = list->self->pop(&ctx->head);

    RX_REQUIRE(head != 0);
}

RX_TEST_CASE(myTestSuite, test_list_alloc_pop_payload, .fixture = test_fixture)
{
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_context_class* ctx = &rx->ctx;

    // create list
    const struct list_class* list = &list_definition;
    void* payload = (void*)0xdeadbeef;

    list->self->push(&ctx->head, payload);
    void* head = list->self->pop(&ctx->head);

    // ensure that data being added to list
    RX_REQUIRE(head == payload);
}

// test pop
RX_TEST_CASE(myTestSuite, test_list_pop_is_zero, .fixture = test_fixture)
{
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_context_class* ctx = &rx->ctx;

    // create list
    const struct list_class* list = &list_definition;

    void* head = list->self->pop(&ctx->head);

    // ensure that data being added to list
    RX_REQUIRE(head == 0);
}

int main()
{
#ifdef DEBUG
    printf("---- acceptance test code\n");
    printf("\n");
#endif
    // some messy code
    using_list(list_using);
    using_list2(list_using);
#ifdef DEBUG
    printf("\n");
#endif
#ifdef DEBUG
    printf("---- rexo unit test code\n");
    printf("\n");
#endif
    /* Execute the main function that runs the test cases found. */
    return rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;
}