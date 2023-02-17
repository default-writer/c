#include "common/alloc.h"

#include "playground/brain/brain.h"
#include "playground/hashtable/hashtable.h"

// #include "playground/list/v2/list.h"
#include "list-micro/data.h"
#include "playground/pointer/pointer.h"
#include "playground/virtual/vm.h"

#include "rexo/include/rexo.h"

#define DEFAULT_SIZE 0x100

/* list definition */
extern const struct vm vm_definition;
extern const struct list list_micro_definition;
extern struct pointer_methods pointer_methods_definition;
const struct pointer_methods* pointer = &pointer_methods_definition;

typedef struct test_data {
    struct pointer_data* ctx;
}* TEST_DATA;

RX_SET_UP(test_set_up) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct pointer_data** ctx = &rx->ctx;
    pointer_set(ctx);
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct pointer_data** ctx = &rx->ctx;
    pointer_get(ctx);
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

// test init
RX_TEST_CASE(myTestSuite, test_list_push_list_peek_list_pop, .fixture = test_fixture) {
    u64 list_ptr = pointer->list_alloc();
    const char* source = "Hello, world!";
    u64 size = strlen(source);
    char* dest = calloc(1, size + 1);
    memcpy(dest, source, size + 1); // NOLINT
    for (u64 i = 0; i < size; i++) {
        char* ptr = dest + i;
        char* tmp = ptr + 1;
        char ch = *tmp;
        *tmp = 0;
        u64 data = pointer->load(ptr);
        *tmp = ch;
        pointer->list_push(list_ptr, data);
    }
    char* buffer = calloc(1, size + 1);
    for (u64 i = 0; i < size; i++) {
        u64 ch0 = pointer->list_peek(list_ptr);
        char* data = pointer->unsafe(ch0);
        *(buffer + i) = *data;
        u64 ch = pointer->list_pop(list_ptr);
#ifdef USE_GC
        CLEAN(ch)
#endif
#ifndef USE_GC
        pointer->free(ch);
#endif
    }
    printf("%s\n", buffer);
    free(buffer);
    free(dest);
    pointer->list_free(list_ptr);
}

int main(int argc, char** argv) {
    CLEAN(argc)
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- acceptance test code\n");
#endif
    pointer->init(DEFAULT_SIZE);
    u64 argv_ptr = pointer->load(argv[0]);
    pointer->push(argv_ptr);
#ifndef USE_GC
    pointer->free(argv_ptr);
#endif
    pointer->destroy();
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- rexo unit test code\n");
#endif
    /* Execute the main function that runs the test cases found. */
    return rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;
}
