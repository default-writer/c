#include "rexo/include/rexo.h"

#include "std/list.h"
#include "list/data.h"

#define ZEROPTR(ptr) if (ptr != 0) { ptr = 0; }

#ifndef USE_MEMORY_LEAKS_DETECTION
const char* __asan_default_options() { return "detect_leaks=0"; }
#endif

extern const struct list list_definition;

static inline struct list_data* new_list() {
    const struct list* list = &list_definition;
    struct list_data* ctx = 0;
    // init list
    list->init(&ctx, _new);
    // returns created object
    return ctx;
}

static inline void delete_list(struct list_data** ctx) {
    const struct list* list = &list_definition;
    // destroys list
    list->destroy(ctx, _delete, list_next);
    // cleans up
    *ctx = 0;
}

// default list usage scenario
static inline void using_list(void (*list_using)(struct list_data** const)) {
    // initialize current context (stack)
    struct list_data* ctx = new_list();
    // call user method
    list_using(&ctx);
    // destroy list
    delete_list(&ctx);
}

// default list usage scenario
static inline void using_list2(void (*list_using)(struct list_data** const)) {
    // initialize current context (stack)
    struct list_data* ctx = new_list();
    // call user method
    list_using(&ctx);
    // destroy list
    delete_list(&ctx);
}

// print head on current context (stack)
static inline void list_print_head(struct list_data** const current, const void* (*_data)(const struct list_data*)) {
    // get current context's head
    struct list_data* ptr = *current;
    // visualise item
    printf("*: 0x%016llx >0x%016llx\n", (LPTR)ptr, (LPTR)_data(ptr));
}

// print all stack trace to output
// in a single loop, print out all elements except root element (which does not have a payload)
// as a result, all stack will be printed in last-to-first order (reverse)
static inline void list_print(struct list_data** const current, struct list_data* (*list_next)(struct list_data*), const void* (*list_data)(const struct list_data*)) {
    // sets the counter
    int i = 0;
    // assigns current's head pointer to the temporary
    struct list_data* tmp = *current;
    if (tmp != 0)
    {
        // until we found root element (element with no previous element reference)
        do {
            // debug output of memory dump
            printf("%d: 0x%016llx *0x%016llx\n", ++i, (LPTR)tmp, (LPTR)list_data(tmp));
            // remember temprary's prior pointer value to temporary
            tmp = list_next(tmp);
        } while (tmp != 0/*root*/);
    }
    // stop on root element
}

// use list
static inline void list_using(struct list_data** const current) {
    // access context's functions pointer
    const struct list* list = &list_definition;
    // prepares the payload
    const void* payload = (void*)0xdeadbeef;
    // peeks and pops from empty list
    const struct list_data* is_null[] = {
        list->peek(current),
        list->pop(current)
    };
    RX_ASSERT(0 == is_null[0]);
    RX_ASSERT(0 == is_null[1]);
    struct list_data* tmp1 = list->alloc(payload);
    list->push(current, tmp1);
#ifdef USE_MEMORY_DEBUG_INFO
    list_print_head(current, list_data);
#endif
    struct list_data* tmp2 = list->alloc(++payload);
    list->push(current, tmp2);
#ifdef USE_MEMORY_DEBUG_INFO
    list_print_head(current, list_data);
#endif
    struct list_data* tmp3 = list->alloc(++payload);
    list->push(current, tmp3);
#ifdef USE_MEMORY_DEBUG_INFO
    list_print_head(current, list_data);
#endif
    struct list_data* tmp4 = list->alloc(++payload);
    list->push(current, tmp4);
#ifdef USE_MEMORY_DEBUG_INFO
    list_print_head(current, list_data);
#endif
    struct list_data* tmp5 = list->alloc(++payload);
    list->push(current, tmp5);
#ifdef USE_MEMORY_DEBUG_INFO
    list_print_head(current, list_data);
#endif
#ifdef USE_MEMORY_DEBUG_INFO
    list_print(current, list_next, list_data);
#endif
    struct list_data* q_pop0 = list->pop(current);
    list->free(&q_pop0);
#ifdef USE_MEMORY_DEBUG_INFO
    list_print(current, list_next, list_data);
#endif
    struct list_data* q_pop1 = list->pop(current);
    list->free(&q_pop1);
#ifdef USE_MEMORY_DEBUG_INFO
    list_print(current, list_next, list_data);
#endif
    struct list_data* q_pop2 = list->pop(current);
    list->free(&q_pop2);
#ifdef USE_MEMORY_DEBUG_INFO
    list_print(current, list_next, list_data);
#endif
    struct list_data* q_pop3 = list->pop(current);
    list->push(current, q_pop3);
    q_pop3 = list->pop(current);
    list->free(&q_pop3);
#ifdef USE_MEMORY_DEBUG_INFO
    list_print(current, list_next, list_data);
#endif
    struct list_data* q_pop4 = list->pop(current);
    list->free(&q_pop4);
#ifdef USE_MEMORY_DEBUG_INFO
    list_print(current, list_next, list_data);
#endif
    struct list_data* q_pop5 = list->peek(current);
    list->free(&q_pop5);
    list->push(current, q_pop0);
#ifdef USE_MEMORY_DEBUG_INFO
    list_print(current, list_next, list_data);
#endif
    list->free(&q_pop0);
#ifdef USE_MEMORY_DEBUG_INFO
    list_print(current, list_next, list_data);
#endif
}

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    struct list_data* ctx;
} *TEST_DATA;

/* Initialize the data structure. Its allocation is handled by Rexo. */
RX_SET_UP(test_set_up) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // access context's functions pointer
    const struct list* list = &list_definition;
    *ctx = 0;
    // initialize list
    list->init(ctx, _new);
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // access context's functions pointer
    const struct list* list = &list_definition;
    // destroy list
    list->destroy(ctx, _delete, list_next);
    *ctx = 0;
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

// test init
RX_TEST_CASE(myTestSuite, test_empty_list_count_equals_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // ensures counter is initialized to 0
    RX_ASSERT(*ctx != 0);
}

/* test peek */
RX_TEST_CASE(myTestSuite, test_standard_list_peek_does_not_changes_stack, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // creates the list
    const struct list* list = &list_definition;
    // prepares the payload
    const void* payload = (void*)0xdeadbeef;
    // allocates the memory for the structure
    struct list_data* tmp = list->alloc(payload);
    // pused to the lsit
    list->push(ctx, tmp);
    // gets the head pointer to the list
    const struct list_data* ptr = *ctx;
    // peeks from the list
    const struct list_data* head = list->peek(ctx);
    // ensures data is added to the list
    RX_ASSERT(head != 0);
    // ensures payload is on top of the stack
    RX_ASSERT(head->data == payload);
    // ensures peek does not changes the head pointer
    RX_ASSERT(ptr == *ctx);
}

/* test pop from 0 pointer */
RX_TEST_CASE(myTestSuite, test_empty_list_pop_equals_0, .fixture = test_fixture) {
    struct list_data* ctx = 0;
    // creates the list
    const struct list* list = &list_definition;
    // pups out the current head element
    const struct list_data* head = list->pop(&ctx);
    // ensures no data added to the list
    RX_ASSERT(head == 0);
}

/* test pop from 0 pointer */
RX_TEST_CASE(myTestSuite, test_empty_list_peek_equals_0, .fixture = test_fixture) {
    struct list_data* ctx = 0;
    // creates the list
    const struct list* list = &list_definition;
    // peeks up the current head element
    const struct list_data* head = list->peek(&ctx);
    // ensures no data added to the list
    RX_ASSERT(head == 0);
}

// test alloc
RX_TEST_CASE(myTestSuite, test_list_alloc_count_eq_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // creates the list
    const struct list* list = &list_definition;
    // prepares the payload
    const void* payload = (void*)0xdeadbeef;
    // allocates the memory for the structure
    struct list_data* tmp = list->alloc(payload);
    // pushes to the list
    list->push(ctx, tmp);
    // ensures data is added to the list
    RX_ASSERT(*ctx != 0);
}

RX_TEST_CASE(myTestSuite, test_list_alloc_payload, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // creates the list
    const struct list* list = &list_definition;
    // prepares the payload
    const void* payload = (void*)0xdeadbeef;
    // allocates the memory for the structure
    struct list_data* tmp = list->alloc(payload);
    // pushes to the list
    list->push(ctx, tmp);
    // peeks from list
    const struct list_data* head = list->peek(ctx);
    // ensures data is added to the list
    RX_ASSERT(head->data == payload);
}

RX_TEST_CASE(myTestSuite, test_list_alloc_pop_count_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // creates the list
    const struct list* list = &list_definition;
    // prepares the payload
    const void* payload = (void*)0xdeadbeef;
    // allocates the memory for the structure
    struct list_data* tmp = list->alloc(payload);
    list->push(ctx, tmp);
    // pop from the list
    struct list_data* head = list->pop(ctx);
    // frees memory allocated for the data
    list->free(&head);
    // ensures no data added to the list
    RX_ASSERT(head == 0);
}

RX_TEST_CASE(myTestSuite, test_list_alloc_pop_payload, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // creates the list
    const struct list* list = &list_definition;
    // prepares the payload
    const void* payload = (void*)0xdeadbeef;
    // allocates the memory for the structure
    struct list_data* tmp = list->alloc(payload);
    // pushes to the list
    list->push(ctx, tmp);
    // pops from the list
    struct list_data* head = list->pop(ctx);
    // retrieves the data from the top of the list
    const void* head_payload = head->data;
    // frees memory allocated for the data
    list->free(&head);
    // ensures data is added to the list
    RX_ASSERT(head_payload == payload);
}

RX_TEST_CASE(myTestSuite, test_list_alloc_and_prev_next_equals_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // creates the list
    const struct list* list = &list_definition;
    // prepares the payload
    const void* payload = (void*)0xdeadbeef;
    // allocates the memory for the structure
    struct list_data* tmp = list->alloc(payload);
    // pushes to the list
    list->push(ctx, tmp);
    // peeks from the list
    const struct list_data* head = list->peek(ctx);
    // ensures peek does data cleanup
    RX_ASSERT(head->prev == 0);
    // pops from the list
    struct list_data* current = list->pop(ctx);
    const struct list_data* prev = current->next;
    const struct list_data* next = current->prev;
    const void* head_payload = current->data;
    list->free(&current);
    // ensures data is added to the list
    RX_ASSERT(head_payload == payload);
#ifdef USE_MEMORY_CLEANUP
    // ensures peek does data cleanup
    RX_ASSERT(prev == 0);
    // ensures peek does data cleanup
    RX_ASSERT(next == 0);
#endif
}

RX_TEST_CASE(myTestSuite, test_list_free_head, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // creates the list
    const struct list* list = &list_definition;
    // prepares the payload
    const void* payload = (void*)0xdeadbeef;
    // allocates the memory for the structure
    struct list_data* tmp = list->alloc(payload);
    list->push(ctx, tmp);
    // pops from the list
    struct list_data* head = list->pop(ctx);
    // frees memory allocated for the data
    list->free(&head);
    // ensures no data added to the list
    RX_ASSERT(head == 0);
}

// test peek
RX_TEST_CASE(myTestSuite, test_list_peek_is_zero, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // creates the list
    const struct list* list = &list_definition;
    // peeks from the list
    const struct list_data* head = list->peek(ctx);
    // ensures no data added to the list
    RX_ASSERT(head == 0);
}

// test pop
RX_TEST_CASE(myTestSuite, test_list_pop_is_zero, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // creates the list
    const struct list* list = &list_definition;
    // pop from the list
    const struct list_data* head = list->pop(ctx);
    // ensures no data added to the list
    RX_ASSERT(head == 0);
}

int main() {
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