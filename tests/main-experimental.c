#include "rexo/include/rexo.h"

#include "std/list.h"
#include "list-experimental/data.h"

#define ZEROPTR(ptr) if (ptr != 0) { ptr = 0; }

#ifndef USE_MEMORY_LEAKS_DETECTION
const char* __asan_default_options() { return "detect_leaks=0"; }
#endif

extern const struct list list_experimental_definition;

static inline struct list_data* new_list() {
    // returns created object
    return _new();
}

static inline void delete_list(struct list_data** ctx) {
    // gets pointer
    struct list_data* ptr = *ctx;
    // destroys list
    _delete(ptr);
    // cleans up
    *ctx = 0;
}

// print head on current context (stack)
static inline void array_print_head(struct list_data** const current) {
    // get current context's head
    const struct list_data* ptr = *current;
    // gets data pointer
    const void** data = ptr->data[0];
    // prints data value
    printf("*: 0x%016llx >0x%016llx\n", (LPTR)ptr->data[0], (LPTR)*data);
}


// print all stack trace to output
// in a single loop, print out all elements except root element (which does not have a payload)
// as a result, all stack will be printed in last-to-first order (reverse)
static inline void array_print(struct list_data** const current) {
    // get current context's head
    const struct list_data* ptr = *current;
    // sets the counter
    int i = 0;
    // assigns current's head pointer to the temporary
    const void* end = ptr->data[0];
    if (end != ptr->data)
    {
        do {
            // gets data pointer
            void** data = ptr->data[0];
            // until we found root element (element with no previous element reference)
            do {
                // debug output of memory dump
                printf("%d: 0x%016llx *0x%016llx\n", ++i, (LPTR)data, (LPTR)*data);
            } while (ptr->data != --data/*root*/);
            ptr = ptr->next;
        } while (ptr != 0);
    }
    // stop on root element
}

/* LCG Park-Miller state */
static int lcg_state = 0xdeadbeef;

/* LCG Park-Miller function */
static inline int lcg_parkmiller() {
	lcg_state = (LPTR)lcg_state* 48271 % 0x7fffffff;
    return lcg_state;
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

// use list
static inline void list_using(struct list_data** const current) {
    // access context's functions pointer
    const struct list* list = &list_experimental_definition;
    const LPTR* payload = (LPTR*)0xdeadbeef;
    const void* is_null[] = {
        list->peek(current),
        list->pop(current)
    };
    RX_ASSERT(0 == is_null[0]);
    RX_ASSERT(0 == is_null[1]);
    list->push(current, payload);
#ifdef USE_MEMORY_DEBUG_INFO
    array_print_head(current);
#endif
    list->push(current, ++payload);
#ifdef USE_MEMORY_DEBUG_INFO
    array_print_head(current);
#endif
    list->push(current, ++payload);
#ifdef USE_MEMORY_DEBUG_INFO
    array_print_head(current);
#endif
    list->push(current, ++payload);
#ifdef USE_MEMORY_DEBUG_INFO
    array_print_head(current);
#endif
    list->push(current, ++payload);
#ifdef USE_MEMORY_DEBUG_INFO
    array_print_head(current);
#endif
#ifdef USE_MEMORY_DEBUG_INFO
    array_print(current);
#endif
    const void* q_peek0 = list->peek(current);
    ZEROPTR(q_peek0)
    const void* q_pop0 = list->pop(current);
#ifdef USE_MEMORY_DEBUG_INFO
    array_print(current);
#endif
    const void* q_pop1 = list->pop(current);
    ZEROPTR(q_pop1)
#ifdef USE_MEMORY_DEBUG_INFO
    array_print(current);
#endif
    const void* q_pop2 = list->pop(current);
    ZEROPTR(q_pop2)
#ifdef USE_MEMORY_DEBUG_INFO
    array_print(current);
#endif
    const void* q_peek1 = list->peek(current);
    const void* q_pop3 = list->pop(current);
    const void* q_peek2 = list->peek(current);
    list->push(current, q_pop3);
    const void* q_peek3 = list->peek(current);
    RX_ASSERT(q_peek1 != q_peek2);
    RX_ASSERT(q_peek2 != q_peek3);
    RX_ASSERT(q_peek1 == q_peek3);
    const void* q_pop4 = list->pop(current);
    ZEROPTR(q_pop4)
#ifdef USE_MEMORY_DEBUG_INFO
    array_print(current);
#endif
    const void* q_pop5 = list->pop(current);
    ZEROPTR(q_pop5)
#ifdef USE_MEMORY_DEBUG_INFO
    array_print(current);
#endif
    const void* q_peek4 = list->peek(current);
    list->push(current, q_pop0);
    ZEROPTR(q_peek4)
#ifdef USE_MEMORY_DEBUG_INFO
    array_print(current);
#endif
    const void* q_pop6 = list->pop(current);
    ZEROPTR(q_pop6)
#ifdef USE_MEMORY_DEBUG_INFO
    array_print(current);
#endif
    const void* q_pop7 = list->pop(current);
    ZEROPTR(q_pop7)
#ifdef USE_MEMORY_DEBUG_INFO
    array_print(current);
#endif
    const void* q_peek5 = list->peek(current);
    ZEROPTR(q_peek5)
#ifdef USE_MEMORY_DEBUG_INFO
    array_print(current);
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
    // initialize list
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

// test init
RX_TEST_CASE(myTestSuite, test_empty_list_count_equals_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // ensures counter is initialized to 0
    RX_ASSERT(*ctx != 0);
}

/* test pop from 0 pointer */
RX_TEST_CASE(myTestSuite, test_empty_list_pop_equals_0, .fixture = test_fixture) {
    struct list_data* ctx = 0;
    // creates the list
    const struct list* list = &list_experimental_definition;
    // pops from the list
    const struct list_data* head = list->pop(&ctx);
    // ensures counter is initialized to 0
    RX_ASSERT(head == 0);
}

/* test pop from 0 pointer */
RX_TEST_CASE(myTestSuite, test_empty_list_peek_equals_0, .fixture = test_fixture) {
    struct list_data* ctx = 0;
    // creates the list
    const struct list* list = &list_experimental_definition;
    // peeks from the list
    const struct list_data* head = list->peek(&ctx);
    // ensures counter is initialized to 0
    RX_ASSERT(head == 0);
}

// test alloc
RX_TEST_CASE(myTestSuite, test_list_alloc_count_eq_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // creates the list
    const struct list* list = &list_experimental_definition;
    // prepares the payload
    const void* payload = (void*)0xdeadbeef;
    // pused to the lsit
    list->push(ctx, payload);
    // ensures data is added to the list
    RX_ASSERT(*ctx != 0);
}

RX_TEST_CASE(myTestSuite, test_list_alloc_payload, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // creates the list
    const struct list* list = &list_experimental_definition;
    // prepares the payload
    const void* payload = (void*)0xdeadbeef;
    // pushes to the list
    list->push(ctx, payload);
    // peeks from the list
    const void* head = list->peek(ctx);
    // ensures data is added to the list
    RX_ASSERT(head == payload);
}

RX_TEST_CASE(myTestSuite, test_list_alloc_pop_count_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // creates the list
    const struct list* list = &list_experimental_definition;
    // prepares the payload
    const void* payload = (void*)0xdeadbeef;
    // pushes to the list
    list->push(ctx, payload);
    // pops from the list
    const void* head = list->pop(ctx);
    // ensures data is added to the list
    RX_ASSERT(head != 0);
}

RX_TEST_CASE(myTestSuite, test_list_alloc_pop_payload, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // creates the list
    const struct list* list = &list_experimental_definition;
    // prepares the payload
    const void* payload = (void*)0xdeadbeef;
    // pushes to the list
    list->push(ctx, payload);
    // pops from the list
    const void* head = list->pop(ctx);
    // ensures data is added to the list
    RX_ASSERT(head == payload);
}

// test peek
RX_TEST_CASE(myTestSuite, test_list_peek_is_zero, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // creates the list
    const struct list* list = &list_experimental_definition;
    // pops from the list
    const void* head = list->peek(ctx);
    // ensures no data is added to the list
    RX_ASSERT(head == 0);
}

// test pop
RX_TEST_CASE(myTestSuite, test_list_pop_is_zero, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // creates the list
    const struct list* list = &list_experimental_definition;
    // pops from the list
    const void* head = list->pop(ctx);
    // ensures data is added to the list
    RX_ASSERT(head == 0);
}

RX_TEST_CASE(myTestSuite, test_list_realloc, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // creates the list
    const struct list* list = &list_experimental_definition;
    // prepares the payload
    const void* payload = (void*)0xdeadbeef;
    // pushes to the list multiple times
    list->push(ctx, payload);
    list->push(ctx, payload);
    list->push(ctx, payload);
    list->push(ctx, payload);
    list->push(ctx, payload);
    list->push(ctx, payload);
    list->push(ctx, payload);
    list->push(ctx, payload);
    // peeks from the list
    const void* head = list->peek(ctx);
    // ensures data is added to the list
    RX_ASSERT(head == payload);
}


RX_TEST_CASE(myTestSuite, test_list_push_pop, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // creates the list
    const struct list* list = &list_experimental_definition;
    // prepares the payload
    const void* payload = (void*)0xdeadbeef;
    // record buffer has 9 items
    const void* _recorded[8+1] = { };
    // pushes all pseudo-random values
    // pushes to the list multiple times
    int i=0;
    do {
        const void* _payload = (void*)(LPTR)lcg_parkmiller();
        _recorded[i] = _payload;
        list->push(ctx, _payload);
    } while (++i < 8);
    // pushes to the list
    list->push(ctx, payload);
    array_print(ctx);
    _recorded[i] = payload;
    // ensures data is added to the list
    do {
        RX_ASSERT(list->pop(ctx) == _recorded[i]);
    } while (--i >= 0);
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