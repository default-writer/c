#include "common/alloc.h"

#include "playground/brain/brain.h"
#include "playground/list/v2/list.h"
#include "playground/pointer/pointer.h"
#include "playground/virtual/vm.h"

#include "rexo/include/rexo.h"

#define DEFAULT_SIZE 0xffff // 64K bytes

/* list definition */
extern const struct vm vm_definition;
extern const struct list list_v2;

/* list definition */
static const struct list* list = &list_v2;
static const struct vm* vm = &vm_definition;

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
    pointer->gc();
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
    pointer->printf(0);
}

// test init
RX_TEST_CASE(myTestSuite, test_load_0, .fixture = test_fixture) {
    u64 ptr = pointer->load(0);
    RX_ASSERT(ptr == 0);
}

// test init
RX_TEST_CASE(myTestSuite, test_load_1, .fixture = test_fixture) {
    u64 ptr = pointer->load("");
    RX_ASSERT(ptr == 0);
}

// test init
RX_TEST_CASE(myTestSuite, test_print_load_1, .fixture = test_fixture) {
    u64 empty_ptr = pointer->load("");
    pointer->printf(empty_ptr);
    RX_ASSERT(empty_ptr == 0);
}

// test init
RX_TEST_CASE(myTestSuite, test_strcpy_0, .fixture = test_fixture) {
    u64 pattern_ptr = pointer->load("/");
    u64 empty_ptr = pointer->alloc(0);
    pointer->strcpy(empty_ptr, pattern_ptr);
    RX_ASSERT(pattern_ptr != 0);
    RX_ASSERT(empty_ptr != 0);
#ifndef USE_GC
    pointer->free(empty_ptr);
    pointer->free(pattern_ptr);
#endif
}

// test init
RX_TEST_CASE(myTestSuite, test_strcat_0, .fixture = test_fixture) {
    u64 zero_ptr = pointer->alloc(0);
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
RX_TEST_CASE(myTestSuite, test_strcat_1, .fixture = test_fixture) {
    u64 char_ptr = pointer->load("/");
    u64 pattern_ptr = pointer->alloc(1);
    pointer->strcat(pattern_ptr, char_ptr);
    RX_ASSERT(char_ptr != 0);
    RX_ASSERT(pattern_ptr != 0);
#ifndef USE_GC
    pointer->free(char_ptr);
    pointer->free(pattern_ptr);
#endif
}

// test init
RX_TEST_CASE(myTestSuite, test_load_copy, .fixture = test_fixture) {
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
RX_TEST_CASE(myTestSuite, test_push_peek_pop, .fixture = test_fixture) {
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

RX_TEST_CASE(myTestSuite, test_double_free, .fixture = test_fixture) {
    u64 char_ptr = pointer->load("/");
    RX_ASSERT(char_ptr != 0);
#ifndef USE_GC
    pointer->free(char_ptr);
    pointer->free(char_ptr);
#endif
}

// test init
RX_TEST_CASE(myTestSuite, test_strcpy_load, .fixture = test_fixture) {
    u64 char_ptr = pointer->load("/");
    u64 pattern_ptr = pointer->alloc(1);
    u64 empty_ptr = pointer->alloc(0);
    pointer->strcat(pattern_ptr, char_ptr);
#ifndef USE_GC
    pointer->free(char_ptr);
#endif
    pointer->strcpy(empty_ptr, pattern_ptr);
    RX_ASSERT(pattern_ptr != 0);
    RX_ASSERT(empty_ptr != 0);
#ifndef USE_GC
    pointer->free(empty_ptr);
    pointer->free(pattern_ptr);
#endif
}

extern inline void source() {
    u64 data_ptr = pointer->pop();
    u64 file_name_ptr = pointer->load("/input.txt");
    u64 pattern_ptr = pointer->load("/");
    u64 last_match_ptr = pointer->match_last(data_ptr, pattern_ptr);
#ifndef USE_GC
    pointer->free(pattern_ptr);
#endif
    pointer->put_char(last_match_ptr, '\0');
#ifndef USE_GC
    pointer->free(last_match_ptr);
#endif
    pointer->strcat(data_ptr, file_name_ptr);
#ifndef USE_GC
    pointer->free(file_name_ptr);
#endif
    u64 mode_ptr = pointer->load("rb");
    u64 f_ptr = pointer->open_file(data_ptr, mode_ptr);
#ifndef USE_GC
    pointer->free(data_ptr);
    pointer->free(mode_ptr);
#endif
    u64 file_data_ptr = pointer->read_file(f_ptr);
    pointer->close_file(f_ptr);
#ifndef USE_GC
    pointer->free(f_ptr);
#endif
    pointer->printf(file_data_ptr);
#ifndef USE_GC
    pointer->free(file_data_ptr);
#endif
}

int main(int argc, char** argv) {
    ZEROPTR(argc)
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- acceptance test code\n");
#endif
    pointer_init();
    u64 argv_ptr = pointer->load(argv[0]);
    pointer->push(argv_ptr);
    source();
#ifdef USE_GC
    pointer->gc();
#endif
    pointer_destroy();
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- rexo unit test code\n");
#endif
    /* Execute the main function that runs the test cases found. */
    return rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;
}
