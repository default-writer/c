#include "rexo/include/rexo.h"
#include "std/common.h"
#include "list-experimental/data.h"
#include "common/alloc.h"
#include "common/parameters.h"

/* defines N-element collection*/
#define N_ELEMENTS 256
/* buffer size in bytes = size of 8 items */
#define ALLOC_SIZE(n) ((n) + 1) * sizeof(void*)

extern const struct list list_experimental_definition;
extern const struct list_parameters list_parameters_definition;

struct list_data* _new();
void _delete(struct list_data* ptr);
size_t _size();
struct list_data* list_next(struct list_data* ptr);
void* list_data(struct list_data* ptr);
void list_delete(struct list_data* ptr);

void list_init(struct list_data** current);
void list_destroy(struct list_data** current);

/* LCG Park-Miller function */
u64 lcg_parkmiller();

/* allocates memory pointer for list object */
static struct list_data* new_list() {
    struct list_data* ctx = 0;
    // initializes the list
    list_init(&ctx);
    // returns list object
    return ctx;
}

/* releases memory pointer for list object */
static void delete_list(struct list_data** ctx) {
    // destroys the list
    list_destroy(ctx);
    // cleans up
    *ctx = 0;
}

// prints head on current context (stack)
static void array_print_head(struct list_data**  current) {
    // get current context's head
    struct list_data* ptr = *current;
    // gets data pointer
    void** data = ptr->data[0];
    // prints data value
    printf("*: 0x%016llx >0x%016llx\n", (u64)ptr->data[0], (u64)*data);
}

// prints all stack trace to output
static void array_print(struct list_data**  current) {
    // get current context's head
    struct list_data* ptr = *current;
    // sets the counter
    int i = 0;
    // assigns current's head pointer to the temporary
    void* end = ptr->data[0];
    if (end != ptr->data)
    {
        do {
            // gets data pointer
            void** data = ptr->data[0];
            // until we found root element (element with no previous element reference)
            do {
                // debug output of memory dump
                printf("%d: 0x%016llx *0x%016llx\n", ++i, (u64)data, (u64)*data);
            } while (ptr->data != --data/*root*/);
            // gets next data pointer
            ptr = list_next(ptr);
        } while (ptr != 0);
    }
}

// runs default list usage scenario
static void using_list(void (*list_using)(struct list_data** const)) {
    // initialize current context (stack)
    struct list_data* ctx = new_list();
    // call user method
    list_using(&ctx);
    // destroy list
    delete_list(&ctx);
}

// runs default list usage scenario
static void using_list2(void (*list_using)(struct list_data** const)) {
    // initialize current context (stack)
    struct list_data* ctx = new_list();
    // call user method
    list_using(&ctx);
    // destroy list
    delete_list(&ctx);
}

// uses the list
static void list_using(struct list_data**  current) {
    // declares pointer to list functions definitions
    const struct list* list = &list_experimental_definition;
    u8* payload = (void*)0xdeadbeef;
    void* is_null[] = {
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
    void* q_peek0 = list->peek(current);
    ZEROPTR(q_peek0)
    void* q_pop0 = list->pop(current);
#ifdef USE_MEMORY_DEBUG_INFO
    array_print(current);
#endif
    void* q_pop1 = list->pop(current);
    ZEROPTR(q_pop1)
#ifdef USE_MEMORY_DEBUG_INFO
    array_print(current);
#endif
    void* q_pop2 = list->pop(current);
    ZEROPTR(q_pop2)
#ifdef USE_MEMORY_DEBUG_INFO
    array_print(current);
#endif
    void* q_peek1 = list->peek(current);
    void* q_pop3 = list->pop(current);
    void* q_peek2 = list->peek(current);
    list->push(current, q_pop3);
    void* q_peek3 = list->peek(current);
    RX_ASSERT(q_peek1 != q_peek2);
    RX_ASSERT(q_peek2 != q_peek3);
    RX_ASSERT(q_peek1 == q_peek3);
    void* q_pop4 = list->pop(current);
    ZEROPTR(q_pop4)
#ifdef USE_MEMORY_DEBUG_INFO
    array_print(current);
#endif
    void* q_pop5 = list->pop(current);
    ZEROPTR(q_pop5)
#ifdef USE_MEMORY_DEBUG_INFO
    array_print(current);
#endif
    void* q_peek4 = list->peek(current);
    list->push(current, q_pop0);
    ZEROPTR(q_peek4)
#ifdef USE_MEMORY_DEBUG_INFO
    array_print(current);
#endif
    void* q_pop6 = list->pop(current);
    ZEROPTR(q_pop6)
#ifdef USE_MEMORY_DEBUG_INFO
    array_print(current);
#endif
    void* q_pop7 = list->pop(current);
    ZEROPTR(q_pop7)
#ifdef USE_MEMORY_DEBUG_INFO
    array_print(current);
#endif
    void* q_peek5 = list->peek(current);
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

/* test peek */
RX_TEST_CASE(myTestSuite, test_standard_list_peek_does_not_changes_stack, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // declares pointer to list functions definitions
    const struct list* list = &list_experimental_definition;
    // prepares the payload
    u8* payload = (void*)0xdeadbeef;
    // pushed to the list
    list->push(ctx, payload);
    // gets the head pointer to the list
    struct list_data* ptr = *ctx;
    // peeks from the list
    void* head = list->peek(ctx);
    // ensures payload is on top of the stack
    RX_ASSERT(head == payload);
    // ensures peek does not changes the head pointer
    RX_ASSERT(ptr == *ctx);
}

/* test pop from 0 pointer */
RX_TEST_CASE(myTestSuite, test_empty_list_pop_equals_0, .fixture = test_fixture) {
    struct list_data* ctx = 0;
    // declares pointer to list functions definitions
    const struct list* list = &list_experimental_definition;
    // pops from the list
    struct list_data* head = list->pop(&ctx);
    // ensures head is not initialized
    RX_ASSERT(head == 0);
}

/* test pop from 0 pointer */
RX_TEST_CASE(myTestSuite, test_empty_list_peek_equals_0, .fixture = test_fixture) {
    struct list_data* ctx = 0;
    // declares pointer to list functions definitions
    const struct list* list = &list_experimental_definition;
    // peeks from the list
    struct list_data* head = list->peek(&ctx);
    // ensures head is not initialized
    RX_ASSERT(head == 0);
}

// test alloc
RX_TEST_CASE(myTestSuite, test_list_alloc_count_eq_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // declares pointer to list functions definitions
    const struct list* list = &list_experimental_definition;
    // prepares the payload
    u8* payload = (void*)0xdeadbeef;
    // pushed to the list
    list->push(ctx, payload);
    // ensures data is added to the list
    RX_ASSERT(*ctx != 0);
}

RX_TEST_CASE(myTestSuite, test_list_alloc_payload, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // declares pointer to list functions definitions
    const struct list* list = &list_experimental_definition;
    // prepares the payload
    u8* payload = (void*)0xdeadbeef;
    // pushes to the list
    list->push(ctx, payload);
    // peeks from the list
    void* head = list->peek(ctx);
    // ensures data is added to the list
    RX_ASSERT(head == payload);
}

RX_TEST_CASE(myTestSuite, test_list_alloc_pop_count_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // declares pointer to list functions definitions
    const struct list* list = &list_experimental_definition;
    // prepares the payload
    u8* payload = (void*)0xdeadbeef;
    // pushes to the list
    list->push(ctx, payload);
    // pops from the list
    void* head = list->pop(ctx);
    // ensures data is added to the list
    RX_ASSERT(head != 0);
}

RX_TEST_CASE(myTestSuite, test_list_alloc_pop_payload, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // declares pointer to list functions definitions
    const struct list* list = &list_experimental_definition;
    // prepares the payload
    u8* payload = (void*)0xdeadbeef;
    // pushes to the list
    list->push(ctx, payload);
    // pops from the list
    void* head = list->pop(ctx);
    // ensures data is added to the list
    RX_ASSERT(head == payload);
}

// test peek
RX_TEST_CASE(myTestSuite, test_list_peek_is_zero, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // declares pointer to list functions definitions
    const struct list* list = &list_experimental_definition;
    // peeks from the list
    void* head = list->peek(ctx);
    // ensures head is not initialized
    RX_ASSERT(head == 0);
}

// test pop
RX_TEST_CASE(myTestSuite, test_list_pop_is_zero, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // declares pointer to list functions definitions
    const struct list* list = &list_experimental_definition;
    // pops from the list
    void* head = list->pop(ctx);
    // ensures head is not initialized
    RX_ASSERT(head == 0);
}

// test push
RX_TEST_CASE(myTestSuite, test_list_alloc_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // declares pointer to list functions definitions
    const struct list* list = &list_experimental_definition;
    // prepares the payload
    u8* payload = (void*)0xdeadbeef;
    // pushes to the list multiple times
    list->push(ctx, payload);
    // peeks from the list
    void* head = list->peek(ctx);
    // ensures data is added to the list
    RX_ASSERT(head == payload);
}

// test push
RX_TEST_CASE(myTestSuite, test_alloc_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // declares pointer to list functions definitions
    const struct list* list = &list_experimental_definition;
    // prepares the payload
    u8* payload = (void*)0xdeadbeef;
    // pushes to the list multiple times
    list->push(ctx, payload);
    list->push(ctx, payload);
    // peeks from the list
    void* head = list->peek(ctx);
    // ensures data is added to the list
    RX_ASSERT(head == payload);
}

// test push
RX_TEST_CASE(myTestSuite, test_alloc_3, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // declares pointer to list functions definitions
    const struct list* list = &list_experimental_definition;
    // prepares the payload
    u8* payload = (void*)0xdeadbeef;
    // pushes to the list multiple times
    list->push(ctx, payload);
    list->push(ctx, payload);
    list->push(ctx, payload);
    // peeks from the list
    void* head = list->peek(ctx);
    // ensures data is added to the list
    RX_ASSERT(head == payload);
}

// test push
RX_TEST_CASE(myTestSuite, test_alloc_5, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // declares pointer to list functions definitions
    const struct list* list = &list_experimental_definition;
    // prepares the payload
    u8* payload = (void*)0xdeadbeef;
    // pushes to the list multiple times
    // 1
    list->push(ctx, payload);
    list->push(ctx, payload);
    list->push(ctx, payload);
    list->push(ctx, payload);
    // 5
    list->push(ctx, payload);
    // peeks from the list
    void* head = list->peek(ctx);
    // ensures data is added to the list
    RX_ASSERT(head == payload);
}

// test push
RX_TEST_CASE(myTestSuite, test_alloc_7, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // declares pointer to list functions definitions
    const struct list* list = &list_experimental_definition;
    // prepares the payload
    u8* payload = (void*)0xdeadbeef;
    // pushes to the list multiple times
    // 1
    list->push(ctx, payload);
    list->push(ctx, payload);
    list->push(ctx, payload);
    list->push(ctx, payload);
    // 5
    list->push(ctx, payload);
    list->push(ctx, payload);
    list->push(ctx, payload);
    // peeks from the list
    void* head = list->peek(ctx);
    // ensures data is added to the list
    RX_ASSERT(head == payload);
}

// test push
RX_TEST_CASE(myTestSuite, test_alloc_9, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // declares pointer to list functions definitions
    const struct list* list = &list_experimental_definition;
    // prepares the payload
    u8* payload = (void*)0xdeadbeef;
    // pushes to the list multiple times
    // 1
    list->push(ctx, payload);
    list->push(ctx, payload);
    list->push(ctx, payload);
    list->push(ctx, payload);
    // 5
    list->push(ctx, payload);
    list->push(ctx, payload);
    list->push(ctx, payload);
    list->push(ctx, payload);
    // 9
    list->push(ctx, payload);
    // peeks from the list
    void* head = list->peek(ctx);
    // ensures data is added to the list
    RX_ASSERT(head == payload);
}

// test push
RX_TEST_CASE(myTestSuite, test_alloc_4, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // declares pointer to list functions definitions
    const struct list* list = &list_experimental_definition;
    // prepares the payload
    u8* payload = (void*)0xdeadbeef;
    // pushes to the list multiple times
    list->push(ctx, payload);
    list->push(ctx, payload);
    list->push(ctx, payload);
    list->push(ctx, payload);
    // peeks from the list
    void* head = list->peek(ctx);
    // ensures data is added to the list
    RX_ASSERT(head == payload);
}

// test push
RX_TEST_CASE(myTestSuite, test_alloc_8, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // declares pointer to list functions definitions
    const struct list* list = &list_experimental_definition;
    // prepares the payload
    u8* payload = (void*)0xdeadbeef;
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
    void* head = list->peek(ctx);
    // ensures data is added to the list
    RX_ASSERT(head == payload);
}

// test loop push
RX_TEST_CASE(myTestSuite, test_list_push_pop, .fixture = test_fixture) {
    // declares pointer to list parameters definitions
    const struct list_parameters* parameters = &list_parameters_definition;
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // declares pointer to list functions definitions
    const struct list* list = &list_experimental_definition;
    // prepares the payload
    u8* payload = (void*)0xdeadbeef;
    // record buffer has N items
    void** _recorded = _list_alloc(1, ALLOC_SIZE(parameters->block_size));
    // pushes all pseudo-random values
    // pushes to the list multiple times
    int i=0;
    do {
        // generates random values
        void* _payload = (void*)lcg_parkmiller();
        // records value
        _recorded[i] = _payload;
        // pushes to the list
        list->push(ctx, _payload);
        // next step
        i++;
    } while (i < N_ELEMENTS);
    // records the last value
    _recorded[i] = payload;
    // pushes to the list
    list->push(ctx, payload);
    // prints the list
    array_print(ctx);
    // peeks from the list
    void* head = list->peek(ctx);
    // ensures payload is on top of the stack
    RX_ASSERT(head == payload);
    // ensures data is added to the list
    do {
        // pops from the list
        void* ptr = list->pop(ctx);
        // prints values
        printf("%d: 0x%016llx *0x%016llx\n", i, (u64)ptr, (u64)_recorded[i]);
        // ensures recorded values matches to the list values
        RX_ASSERT(ptr == _recorded[i]);
        // next step
        i--;
    } while (i >= 0);
    // releases memory allocated for the data
    _list_free(_recorded, ALLOC_SIZE(parameters->block_size));
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