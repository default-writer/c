#include "vm_v1.h"
#include "vm/v1/vm_v1.h"

/* definition */
extern const struct vm_methods vm_methods_definition;
extern const struct pointer_vm_methods pointer_vm_methods_definition;

/* definition */
static const struct vm_methods* vm = &vm_methods_definition;
static const struct pointer_vm_methods* virtual = &pointer_vm_methods_definition;

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    struct vm* ctx;
}* TEST_DATA;

RX_SET_UP(test_set_up) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct vm** ctx = &rx->ctx;
    *ctx = vm->init(0);
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct vm** ctx = &rx->ctx;
    vm->destroy(ctx);
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

/* test init */
RX_TEST_CASE(tests, test_vm_read_0, .fixture = test_fixture) {
    struct pointer* ptr = vm->read(0, TYPE_PTR);
    RX_ASSERT(ptr == 0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_init_0, .fixture = test_fixture) {
    // vm->init(0);
    // vm->destroy();
    // RX_ASSERT(ptr == 0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_read_1, .fixture = test_fixture) {
    struct pointer* ptr = vm->read(1, TYPE_PTR);
    RX_ASSERT(ptr == 0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_write_0, .fixture = test_fixture) {
    u64 ptr = vm->alloc(0);
    RX_ASSERT(ptr == 0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_write_1_read_1, .fixture = test_fixture) {
    struct pointer* ptr = virtual->alloc(0, TYPE_PTR);
    u64 data_ptr = vm->alloc(ptr);
    struct pointer* vm_ptr = vm->read(data_ptr, TYPE_PTR);
    RX_ASSERT(ptr != 0);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(vm_ptr == ptr);
    virtual->free(ptr);
}

/* test init */
RX_TEST_CASE(tests, test_vm_write_1_read_2, .fixture = test_fixture) {
    struct pointer* ptr = virtual->alloc(0, TYPE_PTR);
    u64 data_ptr = vm->alloc(ptr);
    struct pointer* vm_ptr = vm->read(data_ptr + 1, TYPE_PTR);
    RX_ASSERT(ptr != 0);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(vm_ptr == 0);
    virtual->free(ptr);
}

static int run(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- acceptance test code\n");
#endif
    /* safe */
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- rexo unit test code\n");
#endif
    /* Execute the main function that runs the test cases found. */
    int result = rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;
    return result;
}

const struct test_suite vm_v1_test_suite_definition = {
    .run = run
};