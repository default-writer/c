#include "common/alloc.h"
#include "list-micro/data.h"
#include "playground/brain/brain.h"
#include "playground/hashtable/v1/hashtable_v1.h"
#include "pointer/types/data/v1/data_v1.h"
#include "pointer/types/file/v1/file_v1.h"
#include "pointer/types/list/v1/list_v1.h"
#include "pointer/types/string/v1/string_v1.h"
#include "pointer/v1/pointer_v1.h"

#include "rexo/include/rexo.h"

#define DEFAULT_SIZE 0x8

/* definition */
extern void pointer_ctx_init(struct pointer_data** ctx, u64 size);
extern void pointer_ctx_destroy(struct pointer_data** ctx);

extern const struct vm_methods vm_methods_definition;

extern const struct pointer_methods pointer_methods_definition;
extern const struct list_methods list_methods_definition;
extern const struct string_methods string_methods_definition;
extern const struct file_methods file_methods_definition;
extern const struct data_methods data_methods_definition;

static const struct pointer_methods* pointer = &pointer_methods_definition;
static const struct list_methods* list = &list_methods_definition;
static const struct string_methods* string = &string_methods_definition;
static const struct file_methods* file = &file_methods_definition;
static const struct data_methods* data = &data_methods_definition;

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
        u64 string_ptr = string->load(ptr);
        *tmp = ch;
        list->push(list_ptr, string_ptr);
    }
    char* buffer = global_alloc(size);
    for (u64 i = 0; i < size - 1; i++) {
        u64 ch0 = list->peek(list_ptr);
        const char* string_ptr = string->unsafe(ch0);
        *(buffer + i) = *string_ptr;
        u64 ch = list->pop(list_ptr);
        string->free(ch);
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
        u64 string_ptr = string->load(ptr);
        *tmp = ch;
        list->push(list_ptr, string_ptr);
    }
    char* buffer = global_alloc(size);
    for (u64 i = 0; i < size - 1; i++) {
        u64 ch0 = list->peek(list_ptr);
        const char* string_ptr = string->unsafe(ch0);
        *(buffer + i) = *string_ptr;
        u64 ch = list->pop(list_ptr);
        string->free(ch);
    }
    string->free(list_ptr);
    printf("%s\n", buffer);
    global_free(buffer, size);
    global_free(dest, size);
    list->free(list_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_list_push_0_list_peek_0_list_pop_0_list_free_0, .fixture = test_fixture) {
    u64 list_ptr = list->alloc();
    list->push(list_ptr, 0);
    list->push(0, 0);
    list->peek(0);
    list->pop(0);
    u64 str1 = string->load("a");
    list->push(str1, str1);
    list->peek(str1);
    list->pop(str1);
    const char* data1 = string->unsafe(list_ptr);
    const char* data2 = string->unsafe(0);
    file->alloc(list_ptr, 0);
    file->alloc(list_ptr, str1);
    file->alloc(str1, list_ptr);
    file->free(list_ptr);
    file->free(0);
    u64 s1 = file->alloc(str1, 0);
    u64 s2 = file->alloc(str1, list_ptr);
    u64 s3 = file->alloc(0, 0);
    u64 s4 = file->data(0);
    u64 s5 = file->data(s1);
    u64 s6 = file->data(list_ptr);
    data->unsafe(0);
    data->unsafe(str1);
    data->unsafe(list_ptr);
    RX_ASSERT(data1 == 0);
    RX_ASSERT(data2 == 0);
    RX_ASSERT(s1 == 0);
    RX_ASSERT(s2 == 0);
    RX_ASSERT(s3 == 0);
    RX_ASSERT(s4 == 0);
    RX_ASSERT(s5 == 0);
    RX_ASSERT(s6 == 0);
    string->free(0);
    string->free(list_ptr);
    string->free(str1);
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
        u64 string_ptr = string->load(ptr);
        *tmp = ch;
        pointer->push(string_ptr);
    }
    char* buffer = global_alloc(size);
    for (u64 i = 0; i < size - 1; i++) {
        const char* string_ptr = string->unsafe(i + 1);
        *(buffer + i) = *string_ptr;
        string->free(i + 1);
    }
    printf("%s\n", buffer);
    global_free(buffer, size);
    global_free(dest, size);
}

static void INIT init() {
#ifdef USE_MEMORY_DEBUG_INFO
    global_statistics();
#endif
}

static void DESTROY destroy() {
#ifdef USE_MEMORY_DEBUG_INFO
    global_statistics();
#endif
}

int main(int argc, char** argv) {
#ifndef ATTRIBUTE
    init();
#endif
    CLEAN(argc)
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- acceptance test code\n");
#endif
    pointer->init(DEFAULT_SIZE);
    u64 argv_ptr = string->load(argv[0]);
    pointer->push(argv_ptr);
    string->free(argv_ptr);
    pointer->destroy();
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- rexo unit test code\n");
#endif
    /* Execute the main function that runs the test cases found. */
    int result = rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;
#ifndef ATTRIBUTE
    destroy();
#endif
    return result;
}
