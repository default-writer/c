#include "playground/class/v1/class.h"
#include "playground/list/v1/list.h"

#include "rexo/include/rexo.h"

#ifdef USE_MEMORY_ALLOC
#include "playground/memory/memory.h"
#endif

/* externally visible class definition API */
extern const struct class class_definition_v1;
/* list definition */
extern const struct list list_v1;
/* externally visible class data definition API */
extern struct class_data class_data;

/* list definition */
static const struct list* list = &list_v1;

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    struct list_data* ctx;
} * TEST_DATA;

/* Initialize the data structure. Its allocation is handled by Rexo. */
RX_SET_UP(test_set_up) {
    list->init();
    // success
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    list->destroy();
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

// test context
RX_TEST_CASE(myTestSuite, test_list_push_v1, .fixture = test_fixture) {
    // prepares the payload
    u8* payload = (void*)0xdeadbeef;
    // pushes to the list multiple times
    list->push(payload);
    list->push(payload);
    list->push(payload);
    list->push(payload);
    // pops from the list
    list->pop();
    list->pop();
    list->pop();
    // peeks from the list
    const void* head = list->peek();
    // ensures data is added to the list
    RX_ASSERT(head == payload);
}

// test context
RX_TEST_CASE(myTestSuite, test_context_enter_leave_v1, .fixture = test_fixture) {
    const struct class* context = &class_definition_v1;
    struct class_data* data = context->new();
    context->push(data);
    RX_ASSERT(context->pop() == data);
    context->delete(data);
}

// test context
RX_TEST_CASE(myTestSuite, test_class_get_set_data_v1, .fixture = test_fixture) {
    const struct class* context = &class_definition_v1;
    struct class_data* data = context->new();
    void* payload = (void(*))0xdeadbeef;
    context->push(data);
    context->set(payload);
    RX_ASSERT(context->get() == payload);
    RX_ASSERT(context->pop() == data);
    context->delete(data);
}

// test context
RX_TEST_CASE(myTestSuite, test_class_push_pop_get_set_data_v1, .fixture = test_fixture) {
    const struct class* context = &class_definition_v1;
    struct class_data* data1 = context->new();
    struct class_data* data2 = context->new();

    void* payload1 = (void(*))0xdeadbeef;
    void* payload2 = (void(*))0xbebebebe;

    context->push(data1);
    context->set(payload1);

    context->push(data2);
    context->set(payload2);

    RX_ASSERT(context->get() == payload2);
    RX_ASSERT(context->pop() == data2);

    RX_ASSERT(context->get() == payload1);
    RX_ASSERT(context->pop() == data1);

    context->delete(data1);
    context->delete(data2);
}

int main() {
#ifdef USE_MEMORY_ALLOC
    memory_init();
    memory_destroy();
#endif
    /* Execute the main function that runs the test cases found. */
    return rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;
}