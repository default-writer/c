#define DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <rexo.h>

#include "list.h"

#ifdef DEBUG
// address type (for debugging printf function)
typedef long long unsigned int ADDR;
#endif

// default list usage scenario
void using_list(void (*list_using)(list_context * const)) {
    // initialize current context (stack)
    list_context context;
    // create list
    list_vtable* list = &list_vt;
    // initilize list
    list->init(&context);
    // call user method
    list_using(&context);
    // destroy list
    list->destroy(&context);
}

// print head on current context (stack)
void print_head(list_context * const ctx) {
    // get current context's head
    list_ptr* head = &(ctx->head);
    // gets pre-allocated stack value as temporary
    list_ptr tmp;
    // assigns current's head pointer to the temporary
    tmp.ptr = head->ptr;  
#ifdef DEBUG
    // visualise item
    printf("alloc: 0x%llx 0x%llx\n", (ADDR)tmp.ptr, (ADDR)tmp.ptr->payload);
#endif
}

// print item
void print_item(list_ptr* item) {
    // gets pre-allocated stack value as temporary
    list_ptr tmp;
    // assigns current's head pointer to the temporary
    tmp.ptr = item->ptr;  
#ifdef DEBUG
    // visualise item
    printf("item: 0x%llx 0x%llx\n", (ADDR)tmp.ptr, (ADDR)tmp.ptr->payload);
#endif
}

// print all stack trace to output
// in a single loop, print out all ements except root element (which does not have a payload)
// as a result, all stack will be printed in last-to-first order (reverse)
void list_print(list_context * const ctx) {
    // get current context's head
    list_ptr* head = &(ctx->head);
    // sets the counter
    int i = 0;
    // gets pre-allocated stack value as temporary
    list_ptr tmp;
    // assigns current's head pointer to the temporary
    tmp.ptr = head->ptr;
    // until we found root element (element with no previous element reference)
    while (tmp.ptr->prev.ptr != 0) {
#ifdef DEBUG
        // debug output of memory dump
        printf("%d: 0x%llx 0x%llx\n", ++i, (ADDR)tmp.ptr, (ADDR)tmp.ptr->payload);
#endif
        // remember temprary's prior pointer value to temporary
        tmp.ptr = tmp.ptr->prev.ptr;
    }
    // stop on root element
#ifdef DEBUG
    // visualise loop break 
    printf("\n");
#endif
}

// use list
void list_using(list_context* const ctx) {
    // access context's functions pointer
    list_vtable* list = &list_vt;
    abstract_ptr payload = (abstract_ptr)0xdeadbeef;
    list_ptr is_null[] = {
        list->peek(ctx),
        list->pop(ctx),
        list->root(ctx)
    };
    if (list_ptr_null.ptr != is_null[0].ptr) {
        return;
    }
    if (list_ptr_null.ptr != is_null[1].ptr) {
        return;
    }
    if (list_ptr_null.ptr != is_null[2].ptr) {
        return;
    }
    list->alloc(ctx, payload);

    print_head(ctx);
    list->alloc(ctx, ++payload);
    print_head(ctx);
    list->alloc(ctx, ++payload);
    print_head(ctx);
    list->alloc(ctx, ++payload);
    print_head(ctx);
    list->alloc(ctx, ++payload);
    print_head(ctx);
#ifdef DEBUG
    printf("\n");
#endif
    list_ptr root = list->root(ctx);
    print_item(&root);
#ifdef DEBUG
    printf("\n");
#endif
#ifdef DEBUG
    list_print(ctx);
#endif
    list_ptr q_pop0 = list->pop(ctx); 
    list->free(ctx, q_pop0);
#ifdef DEBUG
    list_print(ctx);
#endif
    list_ptr q_pop1 = list->pop(ctx); 
    list->free(ctx, q_pop1);
#ifdef DEBUG
    list_print(ctx);
#endif
    list_ptr q_pop2 = list->pop(ctx); 
    list->free(ctx, q_pop2);
#ifdef DEBUG
    list_print(ctx);
#endif
    list_ptr q_pop3 = list->pop(ctx); 
    list->push(ctx, q_pop3);
    q_pop3 = list->pop(ctx); 
    list->free(ctx, q_pop3);
#ifdef DEBUG
    list_print(ctx);
#endif
    list_ptr q_pop4 = list->pop(ctx); 
    list->free(ctx, q_pop4);
#ifdef DEBUG
    list_print(ctx);
#endif
    list_ptr q_pop5 = list->peek(ctx); 
    list->free(ctx, q_pop5);
#ifdef DEBUG
    list_print(ctx);
#endif
}

/* Data structure to use at the core of our fixture. */
struct test_data {
    list_context context;
};

/* Initialize the data structure. Its allocation is handled by Rexo. */
RX_SET_UP(test_set_up)
{
    /*
       The macro `RX_DATA` references our data as a pointer to `void` that
       needs to be cast to the correct type before being used.
    */
    list_context* data = (list_context *)RX_DATA;

    /* Initialize it! */
    // access context's functions pointer
    list_vtable* list = &list_vt;
    // initilize list
    list->init(data);

    /* Let Rexo know that everything went well. */
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down)
{
    list_context* data = (list_context *)RX_DATA;
    // access context's functions pointer
    list_vtable* list = &list_vt;
    // destroy list
    list->destroy(data);
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, struct test_data, .set_up = test_set_up, .tear_down = test_tear_down);

// test init
RX_TEST_CASE(myTestSuite, test_empty_list_count_equals_0, .fixture = test_fixture)
{
    struct test_data *data;
    data = (struct test_data *)RX_DATA;

    // emshure that counter is initilized to 0
    RX_INT_REQUIRE_EQUAL(data->context.count, 0);
}

// test alloc
RX_TEST_CASE(myTestSuite, test_list_push_count_eq_1, .fixture = test_fixture)
{
    struct test_data *data;
    data = (struct test_data *)RX_DATA;

    // create list
    list_vtable* list = &list_vt;
    abstract_ptr payload = (abstract_ptr)0xdeadbeef;

    list->alloc(&data->context, payload);

    // ensure that data being added to list
    RX_INT_REQUIRE_EQUAL(data->context.count, 1);
}

// test push
RX_TEST_CASE(myTestSuite, test_list_push_count_eq_1, .fixture = test_fixture)
{
    struct test_data *data;
    data = (struct test_data *)RX_DATA;

    // create list
    list_vtable* list = &list_vt;
    abstract_ptr payload = (abstract_ptr)0xdeadbeef;

    list->alloc(&data->context, payload);

    //list->push()

    // ensure that data being added to list
    RX_INT_REQUIRE_EQUAL(data->context.count, 1);
}

int main(int argc, const char **argv)
{
    using_list(list_using);
    /* Execute the main function that runs the test cases found. */
    return rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;
}