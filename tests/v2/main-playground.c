#include "playground/class/v2/class.h"
#include "playground/list/v2/list.h"
#include "rexo/include/rexo.h"
#include "std/common.h"
#ifdef USE_MEMORY_ALLOC
#include "playground/memory/memory.h"
#endif

/* externally visible class definition API */
extern const struct class class_definition_v2;
/* list definition */
extern const struct list list_v2;
/* externally visible class data definition API */
extern struct class_data class_data;

/* list definition */
static const struct list* list = &list_v2;
static struct list_data* list_data;

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    void* ptr;
} * TEST_DATA;

/* Initialize the data structure. Its allocation is handled by Rexo. */
RX_SET_UP(test_set_up) {
    list_data = list->new();
    list->init(list_data);
    // success
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    list->destroy(list_data);
    list->delete(list_data);
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

// test context
RX_TEST_CASE(myTestSuite, test_context_enter_leave_v2, .fixture = test_fixture) {
    const struct class* context = &class_definition_v2;
    struct class_data* data = context->new();
    context->push(list_data, data);
    RX_ASSERT(context->pop(list_data) == data);
    context->delete(data);
}

// test context
RX_TEST_CASE(myTestSuite, test_class_get_set_data_v2, .fixture = test_fixture) {
    const struct class* context = &class_definition_v2;
    struct class_data* data = context->new();
    void* payload = (void(*))0xdeadbeef;
    context->push(list_data, data);
    context->set(list_data, payload);
    RX_ASSERT(context->get(list_data) == payload);
    RX_ASSERT(context->pop(list_data) == data);
    context->delete(data);
}

// test context
RX_TEST_CASE(myTestSuite, test_class_push_pop_get_set_data_v2, .fixture = test_fixture) {
    const struct class* context = &class_definition_v2;
    struct class_data* data1 = context->new();
    struct class_data* data2 = context->new();

    void* payload1 = (void(*))0xdeadbeef;
    void* payload2 = (void(*))0xbebebebe;

    context->push(list_data, data1);
    context->set(list_data, payload1);

    context->push(list_data, data2);
    context->set(list_data, payload2);

    RX_ASSERT(context->get(list_data) == payload2);
    RX_ASSERT(context->pop(list_data) == data2);

    RX_ASSERT(context->get(list_data) == payload1);
    RX_ASSERT(context->pop(list_data) == data1);

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