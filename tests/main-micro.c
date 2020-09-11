#define DEBUG

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "list-micro/api.h"

#ifdef DEBUG

typedef long long unsigned int ADDR;
#endif

/* Force Rexo's compatibility with C89. */
#define RX_ENABLE_C89_COMPAT

#include <rexo.h>

// queue/list context: head
struct list_context { 
    // head element
    struct list* head;
};

// default list usage scenario
void using_list(void (*list_using)(struct list** const)) {
    // initialize current context (stack)
    struct list_context* ctx = (struct list_context*)calloc(1, sizeof(struct list_context));
    // create list
    const struct list_vtable* list = &list_vt;
    // initialize list
    list->init(&ctx->head);
    // call user method
    list_using(&ctx->head);
    // destroy list
    list->destroy(&ctx->head);
    // free curent context (stack)
    free(ctx);
}

// print head on current context (stack)
void print_head(struct list** const current) {
    // get current context's head
    struct list* tmp = *current;
#ifdef DEBUG
    // visualise item
    printf("alloc: 0x%llx 0x%llx\n", (ADDR)tmp, (ADDR)tmp->payload);
#endif
}

// print all stack trace to output
// in a single loop, print out all elements except root element (which does not have a payload)
// as a result, all stack will be printed in last-to-first order (reverse)
void list_print(struct list** const current) {
    // get current context's head
    struct list* head = *current;
    // get root element
    // struct list *root = ctx->root;
    // sets the counter
    int i = 0; 
    // assigns current's head pointer to the temporary
    struct list* tmp = head;
    if (tmp != 0)
    {
        // until we found root element (element with no previous element reference)
        do {
#ifdef DEBUG
            // debug output of memory dump
            printf("%d: 0x%llx 0x%llx\n", ++i, (ADDR)tmp, (ADDR)tmp->payload);
#endif
            // remember temprary's prior pointer value to temporary
            tmp = tmp->prev;
        } while (tmp != 0/*root*/);
    }
    // stop on root element
}

// use list
void list_using(struct list** const current) {
    // access context's functions pointer
    const struct list_vtable* list = &list_vt;
    void* payload = (void*)0xdeadbeef;
    void* is_null[] = {
        list->pop(current)
    };
    if (0 != is_null[0]) {
        return;
    }
    list->push(current, payload);
    print_head(current);
    list->push(current, ++payload);
    print_head(current);
    list->push(current, ++payload);
    print_head(current);
    list->push(current, ++payload);
    print_head(current);
    list->push(current, ++payload);
    print_head(current);
#ifdef DEBUG
    printf("\n");
#endif
#ifdef DEBUG
    list_print(current);
#endif
    void* q_pop0 = list->pop(current); 
#ifdef DEBUG
    list_print(current);
#endif
    void* q_pop1 = list->pop(current); 
#ifdef DEBUG
    list_print(current);
#endif
    void* q_pop2 = list->pop(current); 
#ifdef DEBUG
    list_print(current);
#endif
    void* q_pop3 = list->pop(current); 
    list->push(current, q_pop3);
    q_pop3 = list->pop(current); 
#ifdef DEBUG
    list_print(current);
#endif
    void* q_pop4 = list->pop(current); 
#ifdef DEBUG
    list_print(current);
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
    const struct list_vtable* list = &list_vt;
    // initialize list
    list->init(&ctx->head);

    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down)
{
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_context* ctx = &rx->ctx;
    // access context's functions pointer
    const struct list_vtable* list = &list_vt;
    // destroy list
    list->destroy(&ctx->head);
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

// test init
RX_TEST_CASE(myTestSuite, test_empty_list_count_equals_0, .fixture = test_fixture)
{
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const struct list_context* ctx = &rx->ctx;

    // enshure that counter is initialized to 0
    RX_REQUIRE(ctx->head != 0);
}

// test alloc
RX_TEST_CASE(myTestSuite, test_list_alloc_count_eq_1, .fixture = test_fixture)
{
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_context* ctx = &rx->ctx;

    // create list
    const struct list_vtable* list = &list_vt;
    void* payload = (void*)0xdeadbeef;

    list->push(&ctx->head, payload);

    // ensure that data being added to list
    RX_REQUIRE(ctx->head != 0);
}

RX_TEST_CASE(myTestSuite, test_list_alloc_pop_count_0, .fixture = test_fixture)
{
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_context* ctx = &rx->ctx;

    // create list
    const struct list_vtable* list = &list_vt;
    void* payload = (void*)0xdeadbeef;

    list->push(&ctx->head, payload);
    void* head = list->pop(&ctx->head);

    RX_REQUIRE(head != 0);
}

RX_TEST_CASE(myTestSuite, test_list_alloc_pop_payload, .fixture = test_fixture)
{
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_context* ctx = &rx->ctx;

    // create list
    const struct list_vtable* list = &list_vt;
    void* payload = (void*)0xdeadbeef;

    list->push(&ctx->head, payload);
    void* head = list->pop(&ctx->head);

    // ensure that data being added to list
    RX_REQUIRE(head == payload);
}

// test pop
RX_TEST_CASE(myTestSuite, test_list_pop_is_zero, .fixture = test_fixture)
{
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_context* ctx = &rx->ctx;

    // create list
    const struct list_vtable* list = &list_vt;

    void* head = list->pop(&ctx->head);

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