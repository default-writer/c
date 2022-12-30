#include "rexo/include/rexo.h"
#include "std/common.h"
#include "classes/data.h"

/*

C type system

Any data struct will have last for bytes as pointer to its type methods
and any type will contain method of_type(...) which will check validity of data

struct some_data
{
    const struct some_data* type;
}

struct some_data
{
    int of_type(void* ptr);
}

extern const struct some_data some_data_type;

int of_type(void* ptr)
{
    const struct some_data* type = &some_data_type;
    struct some_data* ptr = (struct some_data*)ptr;
    return ptr->type == type;
}

const struct some_data some_data_type =
{
    .of_type = of_type
};

*/

extern const struct class class_definition;

struct type
{
    // type ptr
    void* ptr;
};

struct class;
struct derived_class;

struct derived_class
{
    const struct class* class;
    const struct derived_class* derived;
    LPTR (*get_type)();
};

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    struct class* ctx;
} *TEST_DATA;

/* initializes the new context's head element */
/* as a result, new memory block will be allocated */
/* current context pointer set to zero */
void class_init(struct class** const current) {
    /* sets current context's head element */
    *current = _new();
}

/* destroys the memory stack */
/* frees all memory elements */
/* as a result, memory will be freed */
void class_destroy(struct class** const current) {
    /* get current context's head */
    /* assigns currently selected item pointer to temporary */
    struct class* tmp = *current;
    /* if not already freed */
    if (tmp != 0) {
        /* gets temporary pointer value */
        struct class* ptr = tmp;
        /* gets prev pointer value */
        _delete(ptr);
        /* all stack items are processed */
        *current = 0;
    }
}

/* Initialize the data structure. Its allocation is handled by Rexo. */
RX_SET_UP(test_set_up) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct class** ctx = &rx->ctx;
    // initilize list
    class_init(ctx);
    // success
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct class** ctx = &rx->ctx;
    // destroy list
    class_destroy(ctx);
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

// test class get_type
RX_TEST_CASE(myTestSuite, test_get_type, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct class** ctx = &rx->ctx;
    const struct class* class = *ctx;
    printf("i is of type class at %llx\n", (LPTR)class);
    // ensure that data being added to list
    RX_ASSERT(class->get_type(class) == (LPTR)&class_definition);
}

// test class get_type
RX_TEST_CASE(myTestSuite, test_class_unintialized_is_zero, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct class** ctx = &rx->ctx;
    const struct class* class = *ctx;
    printf("i is of type class at %llx\n", (LPTR)class);
    // ensure that data being added to list
    RX_ASSERT(class->data != 0);
}

// test class get_type
RX_TEST_CASE(myTestSuite, test_class_data, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct class** ctx = &rx->ctx;
    const struct class* class = *ctx;
    class->set_data(class, (void*)*ctx);
    printf("i is of type class at %llx\n", (LPTR)class);
    // ensure that data being added to list
    RX_ASSERT(class->get_data(class) == (void*)*ctx);
}

int main() {
    /* Execute the main function that runs the test cases found. */
    return rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;
}