#include "rexo/include/rexo.h"
#include "std/common.h"
#include "classes/data.h"

/*

C type system

Any data struct will have last for bytes as pointer to its type methods
and any type will contain method of_type(...) which will check validity of data

struct some_data
{
    const struct some_data_methods* type;
}

struct some_data_methods
{
    int of_type(void* ptr);
}

extern const struct some_data_methods some_data_methods_type;

int of_type(void* ptr)
{
    const struct some_data_methods* type = &some_data_methods_type;
    struct some_data* ptr = (struct some_data*)ptr;
    return ptr->type == type;
}

const struct some_data_methods some_data_methods_type =
{
    .of_type = of_type
};

*/

extern const struct class_methods class_definition;

struct type
{
    // type ptr
    void* ptr;
};

struct some_base_class;
struct class_methods;

union class_methods_interface
{
    const struct some_base_class* base;
    const struct class_methods* class;
    void* ptr;
};

struct some_base_class
{
    LPTR (*get_type)();
};

const union class_methods_interface class_methods_interface_type = {
    .class = &class_definition
};

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    struct class_methods* ctx;
} *TEST_DATA;


/* Initialize the data structure. Its allocation is handled by Rexo. */
RX_SET_UP(test_set_up) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct class_methods** ctx = &rx->ctx;
    // access context's functions pointer
    const struct class_methods* class = &class_definition;
    // initilize list
    class->init(ctx);
    // success
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct class_methods** ctx = &rx->ctx;
    // access context's functions pointer
    const struct class_methods* class = &class_definition;
    // destroy list
    class->destroy(ctx);
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

// test pop
RX_TEST_CASE(myTestSuite, test_get_type, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct class_methods** ctx = &rx->ctx;
    const struct class_methods* class = *ctx;
    printf("i is of type class at %llx\n", (LPTR)class);
    // ensure that data being added to list
    RX_ASSERT(class->get_type() == (LPTR)&class_definition);
}

int main() {
    /* Execute the main function that runs the test cases found. */
    return rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;
}