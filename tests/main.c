#include "rexo/include/rexo.h"

#include "std/list.h"
#include "list/data.h"

extern  struct list list_definition;

struct list_data* _new();
void _delete(struct list_data* ptr);
size_t _size();
struct list_data* list_next(struct list_data* ptr);
void* list_data(struct list_data* ptr);
void list_delete(struct list_data* ptr);

/* allocates memory pointer for list object */
static struct list_data* new_list() {
    struct list* list = &list_definition;
    struct list_data* ctx = 0;
    // init list
    list->init(&ctx, _new);
    // returns list object
    return ctx;
}

/* releases memory pointer for list object */
static void delete_list(struct list_data** ctx) {
    struct list* list = &list_definition;
    // destroys list
    list->destroy(ctx, _delete, list_next);
    // cleans up
    *ctx = 0;
}

// default list usage scenario
static void using_list(void (*list_using)(struct list_data** const)) {
    // initialize current context (stack)
    struct list_data* ctx = new_list();
    // call user method
    list_using(&ctx);
    // destroy list
    delete_list(&ctx);
}

// default list usage scenario
static void using_list2(void (*list_using)(struct list_data** const)) {
    // initialize current context (stack)
    struct list_data* ctx = new_list();
    // call user method
    list_using(&ctx);
    // destroy list
    delete_list(&ctx);
}

// print head on current context (stack)
static void list_print_head(struct list_data**  current,  void* (*_data)(struct list_data*)) {
    // get current context's head
    struct list_data* ptr = *current;
    // visualize item
    printf("*: 0x%016llx >0x%016llx\n", (__u_int64_t)ptr, (__u_int64_t)_data(ptr));
}

// print all stack trace to output
// in a single loop, print out all elements except root element (which does not have a payload)
// as a result, all stack will be printed in last-to-first order (reverse)
static void list_print(struct list_data**  current,struct list_data* (*_list_next)(struct list_data*),  void* (*_list_data)(struct list_data*)) {
    // sets the counter
    int i = 0;
    // assigns current's head pointer to the temporary
    struct list_data* tmp = *current;
    if (tmp != 0)
    {
        // until we found root element (element with no previous element reference)
        do {
            // debug output of memory dump
            printf("%d: 0x%016llx *0x%016llx\n", ++i, (__u_int64_t)tmp, (__u_int64_t)_list_data(tmp));
            // remember temporary's prior pointer value to temporary
            tmp = _list_next(tmp);
        } while (tmp != 0/*root*/);
    }
    // stop on root element
}

// use list
static void list_using(struct list_data**  current) {
    // access context's functions pointer
    struct list* list = &list_definition;
    // prepares the payload
    __u_int8_t* payload = (void*)0xdeadbeef;
    // peeks and pops from empty list
    struct list_data* is_null[] = {
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
    struct list* list = &list_definition;
    // initializes to 0
    *ctx = 0;
    // initialize list
    list->init(ctx, _new);
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // access context's functions pointer
    struct list* list = &list_definition;
    // destroy list
    list->destroy(ctx, _delete, list_next);
    // initializes to 0
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
    struct list* list = &list_definition;
    // prepares the payload
    __u_int8_t* payload = (void*)0xdeadbeef;
    // allocates the memory for the structure
    struct list_data* tmp = list->alloc(payload);
    // pushed to the list
    list->push(ctx, tmp);
    // gets the head pointer to the list
    struct list_data* ptr = *ctx;
    // peeks from the list
    struct list_data* head = list->peek(ctx);
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
    struct list* list = &list_definition;
    // pups out the current head element
    struct list_data* head = list->pop(&ctx);
    // ensures no data added to the list
    RX_ASSERT(head == 0);
}

/* test pop from 0 pointer */
RX_TEST_CASE(myTestSuite, test_empty_list_peek_equals_0, .fixture = test_fixture) {
    struct list_data* ctx = 0;
    // creates the list
    struct list* list = &list_definition;
    // peeks up the current head element
    struct list_data* head = list->peek(&ctx);
    // ensures no data added to the list
    RX_ASSERT(head == 0);
}

// test alloc
RX_TEST_CASE(myTestSuite, test_list_alloc_count_eq_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // creates the list
    struct list* list = &list_definition;
    // prepares the payload
    __u_int8_t* payload = (void*)0xdeadbeef;
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
    struct list* list = &list_definition;
    // prepares the payload
    __u_int8_t* payload = (void*)0xdeadbeef;
    // allocates the memory for the structure
    struct list_data* tmp = list->alloc(payload);
    // pushes to the list
    list->push(ctx, tmp);
    // peeks from list
    struct list_data* head = list->peek(ctx);
    // ensures data is added to the list
    RX_ASSERT(head->data == payload);
}

RX_TEST_CASE(myTestSuite, test_list_alloc_pop_count_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // creates the list
    struct list* list = &list_definition;
    // prepares the payload
    __u_int8_t* payload = (void*)0xdeadbeef;
    // allocates the memory for the structure
    struct list_data* tmp = list->alloc(payload);
    list->push(ctx, tmp);
    // pop from the list
    struct list_data* head = list->pop(ctx);
    // releases memory allocated for the data
    list->free(&head);
    // ensures no data added to the list
    RX_ASSERT(head == 0);
}

RX_TEST_CASE(myTestSuite, test_list_alloc_pop_payload, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // creates the list
    struct list* list = &list_definition;
    // prepares the payload
    __u_int8_t* payload = (void*)0xdeadbeef;
    // allocates the memory for the structure
    struct list_data* tmp = list->alloc(payload);
    // pushes to the list
    list->push(ctx, tmp);
    // pops from the list
    struct list_data* head = list->pop(ctx);
    // retrieves the data from the top of the list
    void* head_payload = head->data;
    // releases memory allocated for the data
    list->free(&head);
    // ensures data is added to the list
    RX_ASSERT(head_payload == payload);
}

RX_TEST_CASE(myTestSuite, test_list_alloc_and_prev_next_equals_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // creates the list
    struct list* list = &list_definition;
    // prepares the payload
    __u_int8_t* payload = (void*)0xdeadbeef;
    // allocates the memory for the structure
    struct list_data* tmp = list->alloc(payload);
    // pushes to the list
    list->push(ctx, tmp);
    // peeks from the list
    struct list_data* head = list->peek(ctx);
    // ensures peek does data cleanup
    RX_ASSERT(head->prev == 0);
    // pops from the list
    struct list_data* current = list->pop(ctx);
    struct list_data* prev = current->next;
    struct list_data* next = current->prev;
    void* head_payload = current->data;
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
    struct list* list = &list_definition;
    // prepares the payload
    __u_int8_t* payload = (void*)0xdeadbeef;
    // allocates the memory for the structure
    struct list_data* tmp = list->alloc(payload);
    list->push(ctx, tmp);
    // pops from the list
    struct list_data* head = list->pop(ctx);
    // releases memory allocated for the data
    list->free(&head);
    // ensures no data added to the list
    RX_ASSERT(head == 0);
}

// test peek
RX_TEST_CASE(myTestSuite, test_list_peek_is_zero, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // creates the list
    struct list* list = &list_definition;
    // peeks from the list
    struct list_data* head = list->peek(ctx);
    // ensures no data added to the list
    RX_ASSERT(head == 0);
}

// test pop
RX_TEST_CASE(myTestSuite, test_list_pop_is_zero, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // creates the list
    struct list* list = &list_definition;
    // pop from the list
    struct list_data* head = list->pop(ctx);
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