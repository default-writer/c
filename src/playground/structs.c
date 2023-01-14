#include "rexo/include/rexo.h"

#include "class/data.h"
#include "std/common.h"

/*

C type system

Any data struct will have last for bytes as pointer to its type methods
and any type will contain method of_type(...) which will check validity of data

struct some_data
{
    struct some_data* type;
}

struct some_data
{
    int of_type(void* ptr);
}

extern struct some_data some_data_type;

int of_type(void* ptr)
{
    struct some_data* type = &some_data_type;
    struct some_data* ptr = (struct some_data*)ptr;
    return ptr->type == type;
}

struct some_data some_data_type =
{
    .of_type = of_type
};

*/

/* externally visible class definition API */
extern struct class class_definition;
/* externally visible class context definition API */
extern struct context context_definition;
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
    // initialize list
    class_init(ctx);
    // success
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct class_data** ctx = &rx->ctx;
    // destroy list
    class_destroy(ctx);
    // initializes to 0
    *ctx = 0;
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

// test class get_type
RX_TEST_CASE(myTestSuite, test_init, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct class_data** ctx = &rx->ctx;
    struct class_data* data = *ctx;
    class_init(ctx);
    RX_ASSERT(*ctx == data);
}

// test class get_type
RX_TEST_CASE(myTestSuite, test_destroy, .fixture = test_fixture) {
    struct class_data* data = 0;
    struct class_data** ctx = &data;
    class_destroy(ctx);
    RX_ASSERT(*ctx == 0);
}

// test class get_type
RX_TEST_CASE(myTestSuite, test_get_type, .fixture = test_fixture) {
    struct class* context = &class_definition;
    // ensures data is added to the list
    RX_ASSERT(context->get_type() == (u64)&class_definition);
}

// test class get_type
RX_TEST_CASE(myTestSuite, test_class_uninitialized_is_zero, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct class_data** ctx = &rx->ctx;
    struct class_data* data = *ctx;
    // ensures data is added to the list
    RX_ASSERT(data->data != 0);
}

// test class get_type
RX_TEST_CASE(myTestSuite, test_class_data, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct class_data** ctx = &rx->ctx;
    struct class_data* class = *ctx;
    struct class* context = &class_definition;
    void* payload = (void*)0xdeadbeef;
    context->set_data(class, payload);
    // ensures data is added to the list
    RX_ASSERT(context->get_data(class) == payload);
}

// test context
RX_TEST_CASE(myTestSuite, test_context_enter_leave, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct class_data** ctx = &rx->ctx;
    struct class_data* data = *ctx;
    struct class* context = &class_definition;
    context->push(data);
    RX_ASSERT(context->pop() == data);
}

// test context
RX_TEST_CASE(myTestSuite, test_class_get_set_data, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct class_data** ctx = &rx->ctx;
    struct class_data* data = *ctx;
    struct class* context = &class_definition;
    void* payload = (void(*))0xdeadbeef;
    context->push(data);
    context->set(payload);
    RX_ASSERT(context->get() == payload);
    RX_ASSERT(context->pop() == data);
}

int main() {
    /* Execute the main function that runs the test cases found. */
    return rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;
}