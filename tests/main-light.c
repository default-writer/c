#include "list-light/data.h"

#include "rexo/include/rexo.h"

extern const struct list list_light_definition;

/* allocates memory pointer for list object */
static struct list_data* new_list() {
    // declares pointer to list functions definitions
    const struct list* list = &list_light_definition;
    struct list_data* ctx = 0;
    // initializes the list
    list->init(&ctx);
    // returns list object
    return ctx;
}

/* releases memory pointer for list object */
static void delete_list(struct list_data** ctx) {
    // declares pointer to list functions definitions
    const struct list* list = &list_light_definition;
    // destroys the list
    list->destroy(ctx);
    // cleans up
    *ctx = 0;
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
    const struct list* list = &list_light_definition;
    u8* payload = (void*)0xdeadbeef;
    void* is_null[] = {
        list->peek(current),
        list->pop(current)
    };
    RX_ASSERT(0 == is_null[0]);
    RX_ASSERT(0 == is_null[1]);
    list->push(current, payload);
#ifdef USE_MEMORY_DEBUG_INFO
    list->print_head(current);
#endif
    list->push(current, ++payload);
#ifdef USE_MEMORY_DEBUG_INFO
    list->print_head(current);
#endif
    list->push(current, ++payload);
#ifdef USE_MEMORY_DEBUG_INFO
    list->print_head(current);
#endif
    list->push(current, ++payload);
#ifdef USE_MEMORY_DEBUG_INFO
    list->print_head(current);
#endif
    list->push(current, ++payload);
#ifdef USE_MEMORY_DEBUG_INFO
    list->print_head(current);
#endif
#ifdef USE_MEMORY_DEBUG_INFO
    list->print(current);
#endif
    void* q_peek0 = list->peek(current);
    ZEROPTR(q_peek0)
    void* q_pop0 = list->pop(current);
#ifdef USE_MEMORY_DEBUG_INFO
    list->print(current);
#endif
    void* q_pop1 = list->pop(current);
    ZEROPTR(q_pop1)
#ifdef USE_MEMORY_DEBUG_INFO
    list->print(current);
#endif
    void* q_pop2 = list->pop(current);
    ZEROPTR(q_pop2)
#ifdef USE_MEMORY_DEBUG_INFO
    list->print(current);
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
    list->print(current);
#endif
    void* q_pop5 = list->pop(current);
    ZEROPTR(q_pop5)
#ifdef USE_MEMORY_DEBUG_INFO
    list->print(current);
#endif
    void* q_peek4 = list->peek(current);
    list->push(current, q_pop0);
    ZEROPTR(q_peek4)
#ifdef USE_MEMORY_DEBUG_INFO
    list->print(current);
#endif
    void* q_pop6 = list->pop(current);
    ZEROPTR(q_pop6)
#ifdef USE_MEMORY_DEBUG_INFO
    list->print(current);
#endif
    void* q_pop7 = list->pop(current);
    ZEROPTR(q_pop7)
#ifdef USE_MEMORY_DEBUG_INFO
    list->print(current);
#endif
    void* q_peek5 = list->peek(current);
    ZEROPTR(q_peek5)
#ifdef USE_MEMORY_DEBUG_INFO
    list->print(current);
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
    // declares pointer to list functions definitions
    const struct list* list = &list_light_definition;
    // initializes to 0
    *ctx = 0;
    // initialize list
    list->init(ctx);
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // declares pointer to list functions definitions
    const struct list* list = &list_light_definition;
    // destroy list
    list->destroy(ctx);
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
    // declares pointer to list functions definitions
    const struct list* list = &list_light_definition;
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
    const struct list* list = &list_light_definition;
    // pops from the list
    struct list_data* head = list->pop(&ctx);
    // ensures head is not initialized
    RX_ASSERT(head == 0);
}

/* test pop from 0 pointer */
RX_TEST_CASE(myTestSuite, test_empty_list_peek_equals_0, .fixture = test_fixture) {
    struct list_data* ctx = 0;
    // declares pointer to list functions definitions
    const struct list* list = &list_light_definition;
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
    const struct list* list = &list_light_definition;
    // prepares the payload
    u8* payload = (void*)0xdeadbeef;
    // pushes to the list
    list->push(ctx, payload);
    // ensures data is added to the list
    RX_ASSERT(*ctx != 0);
}

RX_TEST_CASE(myTestSuite, test_list_alloc_payload, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list_data** ctx = &rx->ctx;
    // declares pointer to list functions definitions
    const struct list* list = &list_light_definition;
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
    const struct list* list = &list_light_definition;
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
    const struct list* list = &list_light_definition;
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
    const struct list* list = &list_light_definition;
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
    const struct list* list = &list_light_definition;
    // pops from the list
    void* head = list->pop(ctx);
    // ensures head is not initialized
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
