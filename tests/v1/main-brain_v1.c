#include "common/alloc.h"
#include "list-micro/data.h"
#include "playground/brain/brain.h"
#include "playground/hashtable/v1/hashtable_v1.h"
#include "playground/pointer/types/file/v1/file_v1.h"
#include "playground/pointer/types/list/v1/list_v1.h"
#include "playground/pointer/types/string/v1/string_v1.h"
#include "playground/pointer/v1/pointer_v1.h"

#include <rexo/include/rexo.h>

#define DEFAULT_SIZE 0x100

/* definition */
extern void pointer_ctx_init(struct pointer_data** ctx, u64 size);
extern void pointer_ctx_destroy(struct pointer_data** ctx);

extern const struct vm vm_definition;

extern const struct pointer_methods pointer_methods_definition;
extern const struct list_methods list_methods_definition;
extern const struct file_methods file_methods_definition;
extern const struct string_methods string_methods_definition;

const struct pointer_methods* pointer = &pointer_methods_definition;
const struct list_methods* list = &list_methods_definition;
const struct file_methods* file = &file_methods_definition;
const struct string_methods* string = &string_methods_definition;

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
RX_TEST_CASE(tests, test_print_0, .fixture = test_fixture) {
    string->printf(0);
}

/* test init */
RX_TEST_CASE(tests, test_load_0, .fixture = test_fixture) {
    u64 empty_ptr = string->load(0);
    RX_ASSERT(empty_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests, test_load_empty, .fixture = test_fixture) {
    u64 empty_ptr = string->load("");
    RX_ASSERT(empty_ptr != 0);
#ifndef USE_GC
    string->free(empty_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests, test_print_load_empty, .fixture = test_fixture) {
    u64 empty_ptr = string->load("");
    string->printf(empty_ptr);
    RX_ASSERT(empty_ptr != 0);
#ifndef USE_GC
    string->free(empty_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests, test_load_copy, .fixture = test_fixture) {
    u64 char_ptr = string->load("/");
    u64 copy_ptr = string->copy(char_ptr);
    RX_ASSERT(char_ptr != 0);
    RX_ASSERT(copy_ptr != 0);
    RX_ASSERT(char_ptr != copy_ptr);
#ifndef USE_GC
    string->free(char_ptr);
    string->free(copy_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests, test_load_push_peek_pop, .fixture = test_fixture) {
    u64 char_ptr = string->load("/");
    pointer->push(char_ptr);
    u64 peek_ptr = pointer->peek();
    u64 pop_ptr = pointer->pop();
    RX_ASSERT(char_ptr != 0);
    RX_ASSERT(peek_ptr != 0);
    RX_ASSERT(pop_ptr != 0);
#ifndef USE_GC
    string->free(char_ptr);
    string->free(peek_ptr);
    string->free(pop_ptr);
#endif
}

RX_TEST_CASE(tests, test_load_free_free, .fixture = test_fixture) {
    u64 char_ptr = string->load("/");
    RX_ASSERT(char_ptr != 0);
#ifndef USE_GC
    string->free(char_ptr);
    string->free(char_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests, test_strcat_load_alloc, .fixture = test_fixture) {
    u64 char_ptr = string->load("/");
    u64 pattern_ptr = string->alloc();
    string->strcat(pattern_ptr, char_ptr);
    RX_ASSERT(char_ptr != 0);
    RX_ASSERT(pattern_ptr != 0);
#ifndef USE_GC
    string->free(char_ptr);
    string->free(pattern_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests, test_strcat_alloc_load, .fixture = test_fixture) {
    u64 zero_ptr = string->alloc();
    u64 char_ptr = string->load("/");
    string->strcat(zero_ptr, char_ptr);
    RX_ASSERT(zero_ptr != 0);
    RX_ASSERT(char_ptr != 0);
#ifndef USE_GC
    string->free(zero_ptr);
    string->free(char_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests, test_strcat_load_alloc_alloc, .fixture = test_fixture) {
    u64 char_ptr = string->load("/");
    u64 pattern_ptr = string->alloc();
    u64 empty_ptr = string->alloc();
    string->strcat(pattern_ptr, char_ptr);
    string->strcpy(empty_ptr, pattern_ptr);
    RX_ASSERT(char_ptr != 0);
    RX_ASSERT(pattern_ptr != 0);
    RX_ASSERT(empty_ptr != 0);
#ifndef USE_GC
    string->free(empty_ptr);
    string->free(pattern_ptr);
    string->free(char_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests, test_strcat_load_load, .fixture = test_fixture) {
    u64 char_ptr = string->load("/");
    u64 pattern_ptr = string->load("*");
    string->strcat(pattern_ptr, char_ptr);
    RX_ASSERT(pattern_ptr != 0);
    RX_ASSERT(char_ptr != 0);
#ifndef USE_GC
    string->free(pattern_ptr);
    string->free(char_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests, test_strcpy_load_alloc, .fixture = test_fixture) {
    u64 pattern_ptr = string->load("/");
    u64 empty_ptr = string->alloc();
    string->strcpy(empty_ptr, pattern_ptr);
    RX_ASSERT(pattern_ptr != 0);
    RX_ASSERT(empty_ptr != 0);
#ifndef USE_GC
    string->free(empty_ptr);
    string->free(pattern_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests, test_strcpy_load_load, .fixture = test_fixture) {
    u64 char_ptr = string->load("/input.txt");
    u64 pattern_ptr = string->load("*");
    string->strcpy(pattern_ptr, char_ptr);
    RX_ASSERT(pattern_ptr != 0);
    RX_ASSERT(char_ptr != 0);
#ifndef USE_GC
    string->free(pattern_ptr);
    string->free(char_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests, test_load_open_file_close_file, .fixture = test_fixture) {
    u64 file_path_ptr = string->getcwd();
    u64 file_name_ptr = string->load("/nonexistent.txt");
    string->strcat(file_path_ptr, file_name_ptr);
#ifndef USE_GC
    string->free(file_name_ptr);
#endif
    u64 mode_ptr = string->load("rb");
    u64 f_ptr = file->alloc(file_path_ptr, mode_ptr);
    u64 data_ptr = file->read(f_ptr);
    RX_ASSERT(f_ptr == 0);
    RX_ASSERT(data_ptr == 0);
    file->free(f_ptr);
#ifndef USE_GC
    string->free(data_ptr);
    string->free(mode_ptr);
    string->free(file_name_ptr);
    string->free(file_path_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests, test_file_read_invalid_type, .fixture = test_fixture) {
    u64 list_ptr = list->alloc();
    u64 data_ptr = file->read(list_ptr);
    list->free(data_ptr);
    file->free(list_ptr);
    list->free(list_ptr);
#ifndef USE_GC
    string->free(data_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests, test_load_open_file_unsafe_hashtable, .fixture = test_fixture) {
    u64 file_path_ptr = string->getcwd();
    u64 file_name_ptr = string->load("/all_english_words.txt");
    string->strcat(file_path_ptr, file_name_ptr);
#ifndef USE_GC
    string->free(file_name_ptr);
#endif
    u64 mode_ptr = string->load("rb");
    u64 f_ptr = file->alloc(file_path_ptr, mode_ptr);
    if (f_ptr != 0) {
        u64 data_ptr = file->read(f_ptr);
        u64 list_ptr = list->alloc();
        file->free(f_ptr);
        u8* file_data = file->unsafe(data_ptr);
        for (int i = 0; i < 100; i++) {
            u8* tmp = file_data;
            while (*tmp != 0 && *tmp != '\n') {
                tmp++;
            }
            *tmp++ = '\0';
            u64 data = string->load((char*)file_data);
            list->push(list_ptr, data);
            char* unsafe = string->unsafe(data);
            printf("%s\n", unsafe);
            file_data = tmp;
        }
        list->free(list_ptr);
#ifndef USE_GC
        string->free(data_ptr);
#endif
    }
#ifndef USE_GC
    string->free(mode_ptr);
    string->free(file_name_ptr);
    string->free(file_path_ptr);
#endif
}

extern inline void source1(void) {
    u64 file_path_ptr = pointer->pop();
    u64 file_name_ptr = string->load("/input.txt");
    u64 pattern_ptr = string->load("/");
    u64 last_match_ptr = string->match_last(file_path_ptr, pattern_ptr);
#ifndef USE_GC
    string->free(pattern_ptr);
#endif
    string->put_char(last_match_ptr, '\0');
#ifndef USE_GC
    string->free(last_match_ptr);
#endif
    string->strcat(file_path_ptr, file_name_ptr);
#ifndef USE_GC
    string->free(file_name_ptr);
#endif
    u64 mode_ptr = string->load("rb");
    u64 f_ptr = file->alloc(file_path_ptr, mode_ptr);
#ifndef USE_GC
    string->free(file_path_ptr);
    string->free(mode_ptr);
#endif
    u64 data_ptr = file->read(f_ptr);
    file->free(f_ptr);
    string->printf(data_ptr);
#ifndef USE_GC
    string->free(data_ptr);
#endif
}

extern inline void source2(void) {
    u64 file_path_ptr = string->getcwd();
    u64 file_name_ptr = string->load("/all_english_words.txt");
    string->strcat(file_path_ptr, file_name_ptr);
#ifndef USE_GC
    string->free(file_name_ptr);
#endif
    u64 mode_ptr = string->load("rb");
    u64 f_ptr = file->alloc(file_path_ptr, mode_ptr);
    if (f_ptr != 0) {
        u64 data_ptr = file->read(f_ptr);
        u64 size = pointer->size(data_ptr);
        if (size > 100) {
            size = 100;
        }
        u64 list_ptr = list->alloc();
        file->free(f_ptr);
        u8* file_data = file->unsafe(data_ptr);
        for (u64 i = 0; i < size; i++) {
            u8* tmp = file_data;
            while (*tmp != 0 && *tmp != '\n') {
                tmp++;
            }
            *tmp++ = '\0';
            u64 data = string->load((char*)file_data);
            list->push(list_ptr, data);
            char* unsafe = string->unsafe(data);
            printf("%s\n", unsafe);
            file_data = tmp;
        }
        list->free(list_ptr);
#ifndef USE_GC
        string->free(data_ptr);
#endif
    }
#ifndef USE_GC
    string->free(mode_ptr);
    string->free(file_name_ptr);
    string->free(file_path_ptr);
#endif
}

int main(int argc, char** argv) {
    CLEAN(argc)
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- acceptance test code\n");
#endif
    pointer->init(DEFAULT_SIZE);
    u64 argv_ptr = string->load(argv[0]);
    pointer->push(argv_ptr);
    source1();
    source2();
#ifndef USE_GC
    string->free(argv_ptr);
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
