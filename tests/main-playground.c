#include "playground/class/class.h"
#include "rexo/include/rexo.h"
#include "std/common.h"
#ifdef USE_MEMORY_ALLOC
#include "playground/memory/memory.h"
#endif

/* externally visible class definition API */
extern const struct class class_definition;
/* externally visible class data definition API */
extern struct class_data class_data;

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    struct class_data* ctx;
} * TEST_DATA;

/* Initialize the data structure. Its allocation is handled by Rexo. */
RX_SET_UP(test_set_up) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct class_data** ctx = &rx->ctx;
    // initializes to 0
    *ctx = 0;
    // success
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct class_data** ctx = &rx->ctx;
    // initializes to 0
    *ctx = 0;
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

// test class get_type
RX_TEST_CASE(myTestSuite, test_class_uninitialized_is_zero, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct class_data** ctx = &rx->ctx;
    struct class_data* data = *ctx;
    // ensures data is added to the list
    RX_ASSERT(data == 0);
}

// test context
RX_TEST_CASE(myTestSuite, test_context_enter_leave, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const struct class* context = &class_definition;
    struct class_data** ctx = &rx->ctx;
    struct class_data* data = *ctx;
    context->push(data);
    RX_ASSERT(context->pop() == data);
}

// test context
RX_TEST_CASE(myTestSuite, test_class_get_set_data, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const struct class* context = &class_definition;
    struct class_data** ctx = &rx->ctx;
    struct class_data* data = *ctx;
    void* payload = (void(*))0xdeadbeef;
    context->init(&data);
    context->push(data);
    context->set(payload);
    RX_ASSERT(context->get() == payload);
    RX_ASSERT(context->pop() == data);
    context->destroy(&data);
}

// test context
RX_TEST_CASE(myTestSuite, test_class_push_pop_get_set_data, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const struct class* context = &class_definition;
    struct class_data** ctx = &rx->ctx;
    struct class_data* data1 = *ctx;
    struct class_data* data2 = 0;
    context->init(&data2);

    void* payload1 = (void(*))0xdeadbeef;
    void* payload2 = (void(*))0xbebebebe;

    context->init(&data1);
    context->push(data1);
    context->set(payload1);

    context->push(data2);
    context->set(payload2);

    RX_ASSERT(context->get() == payload2);
    RX_ASSERT(context->pop() == data2);

    RX_ASSERT(context->get() == payload1);
    RX_ASSERT(context->pop() == data1);

    context->destroy(&data1);
    context->destroy(&data2);
}

int main() {
#ifdef USE_MEMORY_ALLOC
    memory_init();
    memory_destroy();
#endif
    /* Execute the main function that runs the test cases found. */
    return rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;
}