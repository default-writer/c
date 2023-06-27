#include "common/alloc.h"
#include "list-micro/data.h"
#include "playground/brain/brain.h"
#include "playground/hashtable/v1/hashtable_v1.h"
#include "playground/pointer/types/file/v1/file_v1.h"
#include "playground/pointer/types/list/v1/list_v1.h"
#include "playground/pointer/v1/pointer_v1.h"
#include <rexo/include/rexo.h>

#define DEFAULT_SIZE 0x8

/* list definition */
extern const struct vm vm_definition;

extern struct pointer_methods pointer_methods_definition;
extern struct pointer_list_methods pointer_list_methods_definition;

const struct pointer_methods* pointer = &pointer_methods_definition;
const struct pointer_list_methods* list = &pointer_list_methods_definition;

typedef struct test_data {
    struct pointer_data* ctx;
}* TEST_DATA;

RX_SET_UP(test_set_up) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct pointer_data** ctx = &rx->ctx;
    pointer_ctx_init(ctx, 8);
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct pointer_data** ctx = &rx->ctx;
    pointer_ctx_destroy(ctx);
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

/* test init */
RX_TEST_CASE(tests, test_list_push_list_peek_list_pop, .fixture = test_fixture) {
    u64 list_ptr = list->alloc();
    const char* source = "Hello, world!";
    u64 size = strlen(source) + 1;
    char* dest = global_alloc(size);
    memcpy(dest, source, size); /* NOLINT */
    for (u64 i = 0; i < size - 1; i++) {
        char* ptr = dest + i;
        char* tmp = ptr + 1;
        char ch = *tmp;
        *tmp = 0;
        u64 data = pointer->load(ptr);
        *tmp = ch;
        list->push(list_ptr, data);
    }
    char* buffer = global_alloc(size);
    for (u64 i = 0; i < size - 1; i++) {
        u64 ch0 = list->peek(list_ptr);
        const char* data = pointer->unsafe(ch0);
        *(buffer + i) = *data;
        u64 ch = list->pop(list_ptr);
#ifdef USE_GC
        CLEAN(ch)
#endif
#ifndef USE_GC
        pointer->free(ch);
#endif
    }
    printf("%s\n", buffer);
    global_free(buffer, size);
    global_free(dest, size);
    list->free(list_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_list_push_list_peek_list_pop_free, .fixture = test_fixture) {
    u64 list_ptr = list->alloc();
    const char* source = "Hello, world!";
    u64 size = strlen(source) + 1;
    char* dest = global_alloc(size);
    memcpy(dest, source, size); /* NOLINT */
    for (u64 i = 0; i < size - 1; i++) {
        char* ptr = dest + i;
        char* tmp = ptr + 1;
        char ch = *tmp;
        *tmp = 0;
        u64 data = pointer->load(ptr);
        *tmp = ch;
        list->push(list_ptr, data);
    }
    char* buffer = global_alloc(size);
    for (u64 i = 0; i < size - 1; i++) {
        u64 ch0 = list->peek(list_ptr);
        const char* data = pointer->unsafe(ch0);
        *(buffer + i) = *data;
        u64 ch = list->pop(list_ptr);
#ifdef USE_GC
        CLEAN(ch)
#endif
#ifndef USE_GC
        pointer->free(ch);
#endif
    }
#ifndef USE_GC
    pointer->free(list_ptr);
#endif
    printf("%s\n", buffer);
    global_free(buffer, size);
    global_free(dest, size);
    list->free(list_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_list_peek_0, .fixture = test_fixture) {
    const char* source = "Hello, world! A very long string do not fit in 8 bytes.";
    u64 size = strlen(source) + 1;
    char* dest = global_alloc(size);
    memcpy(dest, source, size); /* NOLINT */
    for (u64 i = 0; i < size - 1; i++) {
        char* ptr = dest + i;
        char* tmp = ptr + 1;
        char ch = *tmp;
        *tmp = 0;
        u64 data = pointer->load(ptr);
        *tmp = ch;
        pointer->push(data);
    }
    char* buffer = global_alloc(size);
    for (u64 i = 0; i < size - 1; i++) {
        const char* data = pointer->unsafe(i + 1);
        *(buffer + i) = *data;
#ifndef USE_GC
        pointer->free(i + 1);
#endif
    }
    printf("%s\n", buffer);
    global_free(buffer, size);
    global_free(dest, size);
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
    int result = rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;
#ifdef USE_MEMORY_DEBUG_INFO
    global_statistics();
#endif
    return result;
}
