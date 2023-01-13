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

extern  struct some_data some_data_type;

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

/* class type */
struct type {
    // type ptr
    void* ptr;
};

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    struct class* ctx;
} * TEST_DATA;

/* Initialize the data structure. Its allocation is handled by Rexo. */
RX_SET_UP(test_set_up) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct class** ctx = &rx->ctx;
    // initializes to 0
    *ctx = 0;
    // initialize list
    class_init(ctx);
    // success
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct class** ctx = &rx->ctx;
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
    struct class** ctx = &rx->ctx;
    struct class* class = *ctx;
    class_init(ctx);
    RX_ASSERT(*ctx == class);
}

// test class get_type
RX_TEST_CASE(myTestSuite, test_destroy, .fixture = test_fixture) {
    struct class* ptr = 0;
    struct class** ctx = &ptr;
    class_destroy(ctx);
    RX_ASSERT(*ctx == 0);
}

// test class get_type
RX_TEST_CASE(myTestSuite, test_get_type, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct class** ctx = &rx->ctx;
    struct class* class = *ctx;
    __u_int64_t expected = (__u_int64_t)&class_definition;
    __u_int64_t actual = class->get_type();
    // ensures data is added to the list
    RX_ASSERT(actual == expected);
}

// test class get_type
RX_TEST_CASE(myTestSuite, test_class_uninitialized_is_zero, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct class** ctx = &rx->ctx;
    struct class* class = *ctx;
    struct data* actual = class->data;
    // ensures data is added to the list
    RX_ASSERT(actual != 0);
}

// test class get_type
RX_TEST_CASE(myTestSuite, test_class_data, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct class** ctx = &rx->ctx;
    struct class* class = *ctx;
    void* expected = (void*)0xdeadbeef;
    class->set_data(class, expected);
    void* actual = class->get_data(class);
    // ensures data is added to the list
    RX_ASSERT(actual == expected);
}

// test context
RX_TEST_CASE(myTestSuite, test_context_enter_leave, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct class** ctx = &rx->ctx;
    struct class* expected = *ctx;
    struct context* context = &context_definition;
    context->enter(expected);
    struct class* actual = context->leave();
    RX_ASSERT(actual == expected);
}

// test context
RX_TEST_CASE(myTestSuite, test_context_get_data_set_data, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct class** ctx = &rx->ctx;
    struct class* class = *ctx;
    struct context* context = &context_definition;
    void* expected = (void(*))0xdeadbeef;
    context->enter(class);
    context->set_data(expected);
    void* actual = context->get_data();
    struct class* ptr = context->leave();
    RX_ASSERT(actual == expected);
    RX_ASSERT(ptr == class);
}

int main() {
    /* Execute the main function that runs the test cases found. */
    return rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;
}