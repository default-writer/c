#define DEBUG

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "list-light-api.h"

#ifdef DEBUG

typedef long long unsigned int ADDR;
#endif

/* Force Rexo's compatibility with C89. */
#define RX_ENABLE_C89_COMPAT

#include <rexo.h>

// default list usage scenario
void using_list(void (*list_using)(struct list_context* const)) {
    // initialize current context (stack)
    struct list_context* ctx = (struct list_context*)calloc(1, sizeof(struct list_context));
    // create list
    const struct list_light_vtable* list = &list_light_vt;
    // initilize list
    list->init(ctx);
    // call user method
    list_using(ctx);
    // destroy list
    list->destroy(ctx);
    // free curent context (stack)
    free(ctx);
}

// print head on current context (stack)
void print_head(struct list_context* const ctx) {
    // get current context's head
    struct list* tmp = ctx->head;
#ifdef DEBUG
    // visualise item
    printf("alloc: 0x%llx 0x%llx\n", (ADDR)tmp, (ADDR)tmp->payload);
#endif
}

// print item
void print_item(void* payload) {
#ifdef DEBUG
    // visualise item
    printf("item: 0x%llx\n", (ADDR)payload);
#endif
}

// print all stack trace to output
// in a single loop, print out all ements except root element (which does not have a payload)
// as a result, all stack will be printed in last-to-first order (reverse)
void list_print(struct list_context* const ctx) {
    // get current context's head
    struct list* head = ctx->head;
    // get root element
    struct list *root = ctx->root;
    // sets the counter
    int i = 0; 
    // assigns current's head pointer to the temporary
    struct list* tmp = head;
    // until we found root element (element with no previous element reference)
    while (tmp != root) {
#ifdef DEBUG
        // debug output of memory dump
        printf("%d: 0x%llx 0x%llx\n", ++i, (ADDR)tmp, (ADDR)tmp->payload);
#endif
        // remember temprary's prior pointer value to temporary
        tmp = tmp->prev;
    }
    // stop on root element
}

// use list
void list_using(struct list_context* const ctx) {
    // access context's functions pointer
    const struct list_light_vtable* list = &list_light_vt;
    void* payload = (void*)0xdeadbeef;
    void* is_null[] = {
        list->peek(ctx),
        list->pop(ctx)
    };
    if (0 != is_null[0]) {
        return;
    }
    if (0 != is_null[1]) {
        return;
    }
    list->push(ctx, payload);
    print_head(ctx);
    list->push(ctx, ++payload);
    print_head(ctx);
    list->push(ctx, ++payload);
    print_head(ctx);
    list->push(ctx, ++payload);
    print_head(ctx);
    list->push(ctx, ++payload);
    print_head(ctx);
#ifdef DEBUG
    printf("\n");
#endif
#ifdef DEBUG
    list_print(ctx);
#endif
    void* q_pop0 = list->pop(ctx); 
#ifdef DEBUG
    list_print(ctx);
#endif
    void* q_pop1 = list->pop(ctx); 
#ifdef DEBUG
    list_print(ctx);
#endif
    void* q_pop2 = list->pop(ctx); 
#ifdef DEBUG
    list_print(ctx);
#endif
    void* q_pop3 = list->pop(ctx); 
    list->push(ctx, &q_pop3);
    q_pop3 = list->pop(ctx); 
#ifdef DEBUG
    list_print(ctx);
#endif
    void* q_pop4 = list->pop(ctx); 
#ifdef DEBUG
    list_print(ctx);
#endif
    void* q_pop5 = list->peek(ctx); 
    list->push(ctx, &q_pop0);
#ifdef DEBUG
    list_print(ctx);
#endif
    void* q_pop6 = list->pop(ctx); 
#ifdef DEBUG
    list_print(ctx);
#endif
}

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    struct list_context ctx;
} *TEST_DATA;

/* Initialize the data structure. Its allocation is handled by Rexo. */
RX_SET_UP(test_set_up)
{
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_context* ctx = &rx->ctx;
    // access context's functions pointer
    const struct list_light_vtable* list = &list_light_vt;
    // initilize list
    list->init(ctx);

    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down)
{
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_context* ctx = &rx->ctx;
    // access context's functions pointer
    const struct list_light_vtable* list = &list_light_vt;
    // destroy list
    list->destroy(ctx);
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

// test init
RX_TEST_CASE(myTestSuite, test_empty_list_count_equals_0, .fixture = test_fixture)
{
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const struct list_context* ctx = &rx->ctx;

    // enshure that counter is initialized to 0
    RX_INT_REQUIRE_EQUAL(ctx->count, 0);
}

// test alloc
RX_TEST_CASE(myTestSuite, test_list_alloc_count_eq_1, .fixture = test_fixture)
{
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_context* ctx = &rx->ctx;

    // create list
    const struct list_light_vtable* list = &list_light_vt;
    void* payload = (void*)0xdeadbeef;

    list->push(ctx, payload);

    // ensure that data being added to list
    RX_INT_REQUIRE_EQUAL(ctx->count, 1);
}

RX_TEST_CASE(myTestSuite, test_list_alloc_payload, .fixture = test_fixture)
{
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_context* ctx = &rx->ctx;

    // create list
    const struct list_light_vtable* list = &list_light_vt;
    void* payload = (void*)0xdeadbeef;

    list->push(ctx, payload);
    void* head = list->peek(ctx);

    // ensure that data being added to list
    RX_REQUIRE(head == payload);
}

RX_TEST_CASE(myTestSuite, test_list_alloc_pop_count_0, .fixture = test_fixture)
{
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_context* ctx = &rx->ctx;

    // create list
    const struct list_light_vtable* list = &list_light_vt;
    void* payload = (void*)0xdeadbeef;

    list->push(ctx, payload);
    void* head = list->pop(ctx);

    // ensure that data being added to list
    RX_INT_REQUIRE_EQUAL(ctx->count, 0);
}

RX_TEST_CASE(myTestSuite, test_list_alloc_pop_payload, .fixture = test_fixture)
{
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_context* ctx = &rx->ctx;

    // create list
    const struct list_light_vtable* list = &list_light_vt;
    void* payload = (void*)0xdeadbeef;

    list->push(ctx, payload);
    void* head = list->pop(ctx);

    // ensure that data being added to list
    RX_REQUIRE(head == payload);
}

// test peek
RX_TEST_CASE(myTestSuite, test_list_peek_is_zero, .fixture = test_fixture)
{
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_context* ctx = &rx->ctx;

    // create list
    const struct list_light_vtable* list = &list_light_vt;

    void* head = list->peek(ctx);

    // ensure that data being added to list
    RX_REQUIRE(head == 0);
}

// test pop
RX_TEST_CASE(myTestSuite, test_list_pop_is_zero, .fixture = test_fixture)
{
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_context* ctx = &rx->ctx;

    // create list
    const struct list_light_vtable* list = &list_light_vt;

    void* head = list->pop(ctx);

    // ensure that data being added to list
    RX_REQUIRE(head == 0);
}

int main(int argc, const char *argv)
{
#ifdef DEBUG
    printf("---- acceptance test code\n");
    printf("\n");
#endif
    // some messy code
    using_list(list_using);
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