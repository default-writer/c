#include "common/alloc.h"

#include "playground/brain/brain.h"
#include "playground/hashtable/hashtable.h"
#include "playground/list/v2/list.h"
#include "playground/pointer/pointer.h"
#include "playground/virtual/vm.h"

#include "rexo/include/rexo.h"

#define DEFAULT_SIZE 0x100

/* list definition */
extern const struct vm vm_definition;
// extern const struct hashtable hashtable_definition;
extern const struct list list_v2;

/* list definition */
static const struct list* list = &list_v2;
static const struct vm* vm = &vm_definition;
// static const struct hashtable* hashtable = &hashtable_definition;

extern struct pointer_methods pointer_methods_definition;
const struct pointer_methods* pointer = &pointer_methods_definition;

typedef struct test_data {
    struct init_data* ctx;
}* TEST_DATA;

RX_SET_UP(test_set_up) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct init_data** ctx = &rx->ctx;
    *ctx = calloc(1, sizeof(struct init_data));
    struct init_data* init = *ctx;
    init->vm = vm->init(DEFAULT_SIZE);
    init->list = list->alloc(DEFAULT_SIZE);
#ifdef USE_GC
    init->gc_list = list->alloc(DEFAULT_SIZE);
#endif
    pointer_set(*ctx);
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct init_data** ctx = &rx->ctx;
    struct init_data* init = *ctx;
#ifdef USE_GC
    pointer->gc(void);
#endif
    pointer_get(init);
    vm->destroy(init->vm);
    list->free(init->list);
#ifdef USE_GC
    list->free(init->gc_list);
#endif
    free(*ctx);
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

// test init
RX_TEST_CASE(myTestSuite, test_print_0, .fixture = test_fixture) {
#ifdef USE_MEMORY_DEBUG_INFO
    debug("TEST %s\n", "test_print_0");
#endif
    pointer->printf(0);
}

// test init
RX_TEST_CASE(myTestSuite, test_load_0, .fixture = test_fixture) {
#ifdef USE_MEMORY_DEBUG_INFO
    debug("TEST %s\n", "test_load_0");
#endif
    u64 empty_ptr = pointer->load(0);
    RX_ASSERT(empty_ptr == 0);
}

// test init
RX_TEST_CASE(myTestSuite, test_load_empty, .fixture = test_fixture) {
#ifdef USE_MEMORY_DEBUG_INFO
    debug("TEST %s\n", "test_load_empty");
#endif
    u64 empty_ptr = pointer->load("");
    RX_ASSERT(empty_ptr != 0);
#ifndef USE_GC
    pointer->free(empty_ptr);
#endif
}

// test init
RX_TEST_CASE(myTestSuite, test_print_load_empty, .fixture = test_fixture) {
#ifdef USE_MEMORY_DEBUG_INFO
    debug("TEST %s\n", "test_print_load_empty");
#endif
    u64 empty_ptr = pointer->load("");
    pointer->printf(empty_ptr);
    RX_ASSERT(empty_ptr != 0);
#ifndef USE_GC
    pointer->free(empty_ptr);
#endif
}

// test init
RX_TEST_CASE(myTestSuite, test_load_copy, .fixture = test_fixture) {
#ifdef USE_MEMORY_DEBUG_INFO
    debug("TEST %s\n", "test_load_copy");
#endif
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

// test init
RX_TEST_CASE(myTestSuite, test_load_push_peek_pop, .fixture = test_fixture) {
#ifdef USE_MEMORY_DEBUG_INFO
    debug("TEST %s\n", "test_load_push_peek_pop");
#endif
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

RX_TEST_CASE(myTestSuite, test_load_free_free, .fixture = test_fixture) {
#ifdef USE_MEMORY_DEBUG_INFO
    debug("TEST %s\n", "test_load_free_free");
#endif
    u64 char_ptr = pointer->load("/");
    RX_ASSERT(char_ptr != 0);
#ifndef USE_GC
    pointer->free(char_ptr);
    pointer->free(char_ptr);
#endif
}

// test init
RX_TEST_CASE(myTestSuite, test_strcat_load_alloc, .fixture = test_fixture) {
#ifdef USE_MEMORY_DEBUG_INFO
    debug("TEST %s\n", "test_strcat_load_alloc");
#endif
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

// test init
RX_TEST_CASE(myTestSuite, test_strcat_alloc_load, .fixture = test_fixture) {
#ifdef USE_MEMORY_DEBUG_INFO
    debug("TEST %s\n", "test_strcat_alloc_load");
#endif
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

// test init
RX_TEST_CASE(myTestSuite, test_strcat_load_alloc_alloc, .fixture = test_fixture) {
#ifdef USE_MEMORY_DEBUG_INFO
    debug("TEST %s\n", "test_strcat_load_alloc_alloc");
#endif
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

// test init
RX_TEST_CASE(myTestSuite, test_strcat_load_load, .fixture = test_fixture) {
#ifdef USE_MEMORY_DEBUG_INFO
    debug("TEST %s\n", "test_strcat_load_load");
#endif
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

// test init
RX_TEST_CASE(myTestSuite, test_strcpy_load_alloc, .fixture = test_fixture) {
#ifdef USE_MEMORY_DEBUG_INFO
    debug("TEST %s\n", "test_strcpy_load_alloc");
#endif
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

// test init
RX_TEST_CASE(myTestSuite, test_strcpy_load_load, .fixture = test_fixture) {
#ifdef USE_MEMORY_DEBUG_INFO
    debug("TEST %s\n", "test_strcpy_load_load");
#endif
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

// test init
RX_TEST_CASE(myTestSuite, test_load_open_file_close_file, .fixture = test_fixture) {
#ifdef USE_MEMORY_DEBUG_INFO
    debug("TEST %s\n", "test_load_open_file_close_file");
#endif
    u64 file_path_ptr = pointer->getcwd();
    u64 file_name_ptr = pointer->load("/nonexistent.txt");
    pointer->strcat(file_path_ptr, file_name_ptr);
#ifndef USE_GC
    pointer->free(file_name_ptr);
#endif
    u64 mode_ptr = pointer->load("rb");
    u64 f_ptr = pointer->open_file(file_path_ptr, mode_ptr);
    u64 data_ptr = pointer->read_file(f_ptr);
    RX_ASSERT(f_ptr == 0);
    RX_ASSERT(data_ptr == 0);
    pointer->close_file(f_ptr);
#ifndef USE_GC
    pointer->free(data_ptr);
    pointer->free(mode_ptr);
    pointer->free(file_name_ptr);
    pointer->free(file_path_ptr);
#endif
}
// test init

RX_TEST_CASE(myTestSuite, test_load_open_file_unsafe_hashtable, .fixture = test_fixture) {
#ifdef USE_MEMORY_DEBUG_INFO
    debug("TEST %s\n", "test_load_open_file_unsafe_hashtable");
#endif
    u64 file_path_ptr = pointer->getcwd();
    u64 file_name_ptr = pointer->load("/all_english_words.txt");
    pointer->strcat(file_path_ptr, file_name_ptr);
#ifndef USE_GC
    pointer->free(file_name_ptr);
#endif
    u64 mode_ptr = pointer->load("rb");
    u64 f_ptr = pointer->open_file(file_path_ptr, mode_ptr);
    u64 data_ptr = pointer->read_file(f_ptr);
    if (f_ptr != 0) {
        pointer->close_file(f_ptr);
        char* file_data = pointer->unsafe(data_ptr);
        for (int i = 0; i < 100; i++) {
            char* tmp = file_data;
            while (*tmp != 0 && *tmp != '\n') {
                tmp++;
            }
            *tmp++ = '\0';
            printf("%s\n", file_data);
            file_data = tmp;
        }
    }
//     u64 word_ptr = pointer->load(file_data);
//     // char* word = pointer->unsafe(word_ptr);
//     u64 value_ptr = pointer->load("value");
//     // char* value = pointer->unsafe(value_ptr);
//     //  struct hashtable_data* record = hashtable->get(word, value);
#ifndef USE_GC
    //     // hashtable->free(record);
    //     pointer->free(word_ptr);
    //     pointer->free(value_ptr);
    pointer->free(data_ptr);
    pointer->free(mode_ptr);
    pointer->free(file_name_ptr);
    pointer->free(file_path_ptr);
#endif
}

extern inline void source(void) {
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
    u64 f_ptr = pointer->open_file(file_path_ptr, mode_ptr);
#ifndef USE_GC
    pointer->free(file_path_ptr);
    pointer->free(mode_ptr);
#endif
    u64 data_ptr = pointer->read_file(f_ptr);
    pointer->close_file(f_ptr);
    pointer->printf(data_ptr);
#ifndef USE_GC
    pointer->free(data_ptr);
#endif
}

int main(int argc, char** argv) {
    ZEROPTR(argc)
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- acceptance test code\n");
#endif
    pointer_init(DEFAULT_SIZE);
    u64 argv_ptr = pointer->load(argv[0]);
    pointer->push(argv_ptr);
    source();
#ifndef USE_GC
    pointer->free(argv_ptr);
#endif
#ifdef USE_GC
    pointer->gc(void);
#endif
    pointer_destroy();
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- rexo unit test code\n");
#endif
    /* Execute the main function that runs the test cases found. */
    return rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;
}
