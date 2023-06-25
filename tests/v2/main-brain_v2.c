#include "common/alloc.h"
#include "list-micro/data.h"
#include "playground/brain/brain.h"
#include "playground/hashtable/v2/hashtable_v2.h"
#include "playground/pointer/v2/pointer_v2.h"
#include <rexo/include/rexo.h>

#define DEFAULT_SIZE 0x100

/* list definition */
extern const struct vm vm_definition;

extern const struct pointer_methods pointer_methods_definition;
extern const struct list_methods list_methods_definition;
extern const struct file_methods file_methods_definition;
extern const struct memory_methods memory_methods_definition;
const struct pointer_methods* pointer = &pointer_methods_definition;
const struct list_methods* list = &list_methods_definition;
const struct file_methods* file = &file_methods_definition;
static const struct memory_methods* memory = &memory_methods_definition;

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
    pointer->printf(0);
}

/* test init */
RX_TEST_CASE(tests, test_load_0, .fixture = test_fixture) {
    u64 empty_ptr = pointer->load(0);
    RX_ASSERT(empty_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests, test_load_empty, .fixture = test_fixture) {
    u64 empty_ptr = pointer->load("");
    RX_ASSERT(empty_ptr != 0);
#ifndef USE_GC
    pointer->free(empty_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests, test_print_load_empty, .fixture = test_fixture) {
    u64 empty_ptr = pointer->load("");
    pointer->printf(empty_ptr);
    RX_ASSERT(empty_ptr != 0);
#ifndef USE_GC
    pointer->free(empty_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests, test_load_copy, .fixture = test_fixture) {
    u64 char_ptr = pointer->load("/");
    u64 copy_ptr = pointer->copy(char_ptr);
    RX_ASSERT(char_ptr != 0);
    RX_ASSERT(copy_ptr != 0);
    RX_ASSERT(char_ptr != copy_ptr);
#ifndef USE_GC
    pointer->free(char_ptr);
    pointer->free(copy_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests, test_load_push_peek_pop, .fixture = test_fixture) {
    u64 char_ptr = pointer->load("/");
    pointer->push(char_ptr);
    u64 peek_ptr = pointer->peek();
    u64 pop_ptr = pointer->pop();
    RX_ASSERT(char_ptr != 0);
    RX_ASSERT(peek_ptr != 0);
    RX_ASSERT(pop_ptr != 0);
#ifndef USE_GC
    pointer->free(char_ptr);
    pointer->free(peek_ptr);
    pointer->free(pop_ptr);
#endif
}

RX_TEST_CASE(tests, test_load_free_free, .fixture = test_fixture) {
    u64 char_ptr = pointer->load("/");
    RX_ASSERT(char_ptr != 0);
#ifndef USE_GC
    pointer->free(char_ptr);
    pointer->free(char_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests, test_strcat_load_alloc, .fixture = test_fixture) {
    u64 char_ptr = pointer->load("/");
    u64 pattern_ptr = pointer->alloc();
    pointer->strcat(pattern_ptr, char_ptr);
    RX_ASSERT(char_ptr != 0);
    RX_ASSERT(pattern_ptr != 0);
#ifndef USE_GC
    pointer->free(char_ptr);
    pointer->free(pattern_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests, test_strcat_alloc_load, .fixture = test_fixture) {
    u64 zero_ptr = pointer->alloc();
    u64 char_ptr = pointer->load("/");
    pointer->strcat(zero_ptr, char_ptr);
    RX_ASSERT(zero_ptr != 0);
    RX_ASSERT(char_ptr != 0);
#ifndef USE_GC
    pointer->free(zero_ptr);
    pointer->free(char_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests, test_strcat_load_alloc_alloc, .fixture = test_fixture) {
    u64 char_ptr = pointer->load("/");
    u64 pattern_ptr = pointer->alloc();
    u64 empty_ptr = pointer->alloc();
    pointer->strcat(pattern_ptr, char_ptr);
    pointer->strcpy(empty_ptr, pattern_ptr);
    RX_ASSERT(char_ptr != 0);
    RX_ASSERT(pattern_ptr != 0);
    RX_ASSERT(empty_ptr != 0);
#ifndef USE_GC
    pointer->free(empty_ptr);
    pointer->free(pattern_ptr);
    pointer->free(char_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests, test_strcat_load_load, .fixture = test_fixture) {
    u64 char_ptr = pointer->load("/");
    u64 pattern_ptr = pointer->load("*");
    pointer->strcat(pattern_ptr, char_ptr);
    RX_ASSERT(pattern_ptr != 0);
    RX_ASSERT(char_ptr != 0);
#ifndef USE_GC
    pointer->free(pattern_ptr);
    pointer->free(char_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests, test_strcpy_load_alloc, .fixture = test_fixture) {
    u64 pattern_ptr = pointer->load("/");
    u64 empty_ptr = pointer->alloc();
    pointer->strcpy(empty_ptr, pattern_ptr);
    RX_ASSERT(pattern_ptr != 0);
    RX_ASSERT(empty_ptr != 0);
#ifndef USE_GC
    pointer->free(empty_ptr);
    pointer->free(pattern_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests, test_strcpy_load_load, .fixture = test_fixture) {
    u64 char_ptr = pointer->load("/input.txt");
    u64 pattern_ptr = pointer->load("*");
    pointer->strcpy(pattern_ptr, char_ptr);
    RX_ASSERT(pattern_ptr != 0);
    RX_ASSERT(char_ptr != 0);
#ifndef USE_GC
    pointer->free(pattern_ptr);
    pointer->free(char_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests, test_load_open_file_close_file, .fixture = test_fixture) {
    u64 file_path_ptr = pointer->getcwd();
    u64 file_name_ptr = pointer->load("/nonexistent.txt");
    pointer->strcat(file_path_ptr, file_name_ptr);
#ifndef USE_GC
    pointer->free(file_name_ptr);
#endif
    u64 mode_ptr = pointer->load("rb");
    u64 f_ptr = file->file_alloc(file_path_ptr, mode_ptr);
    u64 data_ptr = file->file_read(f_ptr);
    RX_ASSERT(f_ptr == 0);
    RX_ASSERT(data_ptr == 0);
    file->file_free(f_ptr);
#ifndef USE_GC
    pointer->free(data_ptr);
    pointer->free(mode_ptr);
    pointer->free(file_name_ptr);
    pointer->free(file_path_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests, test_file_read_invalid_type, .fixture = test_fixture) {
    u64 list_ptr = list->list_alloc();
    u64 data_ptr = file->file_read(list_ptr);
    list->list_free(data_ptr);
    file->file_free(list_ptr);
    list->list_free(list_ptr);
#ifndef USE_GC
    pointer->free(data_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests, test_load_open_file_unsafe_hashtable, .fixture = test_fixture) {
    u64 file_path_ptr = pointer->getcwd();
    u64 file_name_ptr = pointer->load("/all_english_words.txt");
    pointer->strcat(file_path_ptr, file_name_ptr);
#ifndef USE_GC
    pointer->free(file_name_ptr);
#endif
    u64 mode_ptr = pointer->load("rb");
    u64 f_ptr = file->file_alloc(file_path_ptr, mode_ptr);
    if (f_ptr != 0) {
        u64 data_ptr = file->file_read(f_ptr);
        u64 list_ptr = list->list_alloc();
        file->file_free(f_ptr);
        char* file_data = pointer->unsafe(data_ptr);
        for (int i = 0; i < 100; i++) {
            char* tmp = file_data;
            while (*tmp != 0 && *tmp != '\n') {
                tmp++;
            }
            *tmp++ = '\0';
            u64 data = pointer->load(file_data);
            list->list_push(list_ptr, data);
            char* unsafe = pointer->unsafe(data);
            printf("%s\n", unsafe);
            file_data = tmp;
        }
        list->list_free(list_ptr);
#ifndef USE_GC
        pointer->free(data_ptr);
#endif
    }
#ifndef USE_GC
    pointer->free(mode_ptr);
    pointer->free(file_name_ptr);
    pointer->free(file_path_ptr);
#endif
}

extern inline void source1(void) {
    u64 file_path_ptr = pointer->pop();
    u64 file_name_ptr = pointer->load("/input.txt");
    u64 pattern_ptr = pointer->load("/");
    u64 last_match_ptr = pointer->match_last(file_path_ptr, pattern_ptr);
#ifndef USE_GC
    pointer->free(pattern_ptr);
#endif
    pointer->put_char(last_match_ptr, '\0');
#ifndef USE_GC
    pointer->free(last_match_ptr);
#endif
    pointer->strcat(file_path_ptr, file_name_ptr);
#ifndef USE_GC
    pointer->free(file_name_ptr);
#endif
    u64 mode_ptr = pointer->load("rb");
    u64 f_ptr = file->file_alloc(file_path_ptr, mode_ptr);
#ifndef USE_GC
    pointer->free(file_path_ptr);
    pointer->free(mode_ptr);
#endif
    u64 data_ptr = file->file_read(f_ptr);
    file->file_free(f_ptr);
    pointer->printf(data_ptr);
#ifndef USE_GC
    pointer->free(data_ptr);
#endif
}

extern inline void source2(void) {
    u64 file_path_ptr = pointer->getcwd();
    u64 file_name_ptr = pointer->load("/all_english_words.txt");
    pointer->strcat(file_path_ptr, file_name_ptr);
#ifndef USE_GC
    pointer->free(file_name_ptr);
#endif
    u64 mode_ptr = pointer->load("rb");
    u64 f_ptr = file->file_alloc(file_path_ptr, mode_ptr);
    if (f_ptr != 0) {
        u64 data_ptr = file->file_read(f_ptr);
        u64 size = pointer->size(data_ptr);
        if (size > 100) {
            size = 100;
        }
        u64 list_ptr = list->list_alloc();
        file->file_free(f_ptr);
        char* file_data = pointer->unsafe(data_ptr);
        for (u64 i = 0; i < size; i++) {
            char* tmp = file_data;
            while (*tmp != 0 && *tmp != '\n') {
                tmp++;
            }
            *tmp++ = '\0';
            u64 data = pointer->load(file_data);
            list->list_push(list_ptr, data);
            char* unsafe = pointer->unsafe(data);
            printf("%s\n", unsafe);
            file_data = tmp;
        }
        list->list_free(list_ptr);
#ifndef USE_GC
        pointer->free(data_ptr);
#endif
    }
#ifndef USE_GC
    pointer->free(mode_ptr);
    pointer->free(file_name_ptr);
    pointer->free(file_path_ptr);
#endif
}

int main(int argc, char** argv) {
    CLEAN(argc)
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- acceptance test code\n");
#endif
    pointer->init(DEFAULT_SIZE);
    u64 argv_ptr = pointer->load(argv[0]);
    pointer->push(argv_ptr);
    source1();
    source2();
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
