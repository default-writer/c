/*
 *
 * MIT License
 *
 * Copyright (c) 2023 Artur Mustafin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */
#define RXP_DEBUG_TESTS
#include "../tests/src/test.h"

#include "test_vm_v2.h"

#include "common/memory.h"
#include "list-micro/data.h"
#include "playground/brain/brain.h"
#include "playground/hashtable/v2/hashtable_v2.h"
#include "pointer/v2/pointer_v2.h"
#include "vm/v2/vm_v2.h"

#define DEFAULT_SIZE 0x100

/* definition */
#ifdef USE_MEMORY_DEBUG_INFO
extern const struct debug_methods debug_methods_definition;
#endif
extern const struct test_suite list_micro_test_suite_definition;
extern const struct test_suite list_alloc_test_suite_definition;

/* definition */
#ifdef USE_MEMORY_DEBUG_INFO
static const struct debug_methods* debug = &debug_methods_definition;
#endif
static const struct test_suite* list_micro_tests = &list_micro_test_suite_definition;
static const struct test_suite* list_alloc_tests = &list_alloc_test_suite_definition;

struct pointer_data {
    struct vm_data* vm;
};

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
RX_TEST_CASE(tests_v2, test_print_0, .fixture = test_fixture) {
    pointer->printf(0);
}

/* test init */
RX_TEST_CASE(tests_v2, test_load_0, .fixture = test_fixture) {
    u64 empty_ptr = pointer->load(0);
    RX_ASSERT(empty_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_v2, test_load_null_ptr, .fixture = test_fixture) {
    u64 null_ptr = 0;
#ifndef USE_GC
    pointer->free(null_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_v2, test_load_empty, .fixture = test_fixture) {
    u64 empty_ptr = pointer->load("");
    RX_ASSERT(empty_ptr != 0);
#ifndef USE_GC
    pointer->free(empty_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_v2, test_print_load_empty, .fixture = test_fixture) {
    u64 empty_ptr = pointer->load("");
    pointer->printf(empty_ptr);
    RX_ASSERT(empty_ptr != 0);
#ifndef USE_GC
    pointer->free(empty_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_v2, test_load_copy, .fixture = test_fixture) {
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
RX_TEST_CASE(tests_v2, test_load_push_peek_pop, .fixture = test_fixture) {
    u64 char_ptr = pointer->load("/");
    u64 file_name_ptr = pointer->load("input.txt");
    u64 file_mode_ptr = pointer->load("rb");
    u64 file_data_ptr = file->file_alloc(file_name_ptr, file_mode_ptr);
    pointer->push(char_ptr);
    u64 peek_ptr = pointer->peek();
    u64 pop_ptr = pointer->pop();
    RX_ASSERT(char_ptr != 0);
    RX_ASSERT(peek_ptr != 0);
    RX_ASSERT(pop_ptr != 0);
#ifndef USE_GC
    pointer->free(file_name_ptr);
    pointer->free(file_mode_ptr);
    pointer->free(file_data_ptr);
    pointer->free(char_ptr);
    pointer->free(peek_ptr);
    pointer->free(pop_ptr);
#endif
    file->file_free(file_data_ptr);
}

RX_TEST_CASE(tests_v2, test_load_free_free, .fixture = test_fixture) {
    u64 char_ptr = pointer->load("/");
    RX_ASSERT(char_ptr != 0);
#ifndef USE_GC
    pointer->free(char_ptr);
    pointer->free(char_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_v2, test_strcat_load_alloc, .fixture = test_fixture) {
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
RX_TEST_CASE(tests_v2, test_push_pop_peek, .fixture = test_fixture) {
    u64 list_ptr = list->alloc();
    pointer->push(0);
    u64 value_ptr1 = pointer->peek();
    pointer->push(list_ptr);
    u64 value_ptr2 = pointer->peek();
    u64 value_ptr3 = pointer->pop();
    RX_ASSERT(value_ptr1 == 0);
    RX_ASSERT(value_ptr2 != 0);
    RX_ASSERT(value_ptr3 == value_ptr2);
    list->free(list_ptr);
}

/* test init */
RX_TEST_CASE(tests_v2, test_api_0, .fixture = test_fixture) {
    u64 value_ptr1 = pointer->peek();
    u64 value_ptr2 = pointer->pop();
    u64 value_ptr3 = pointer->copy(0);
    RX_ASSERT(value_ptr1 == 0);
    RX_ASSERT(value_ptr2 == 0);
    RX_ASSERT(value_ptr3 == 0);
}

/* test init */
RX_TEST_CASE(tests_v2, test_pointer_unsafe, .fixture = test_fixture) {
    u64 zero_ptr = pointer->alloc();
    u64 data_ptr1 = pointer->alloc();

    u64 char_ptr = pointer->load("/");
    pointer->strcat(zero_ptr, char_ptr);

    u64 list_ptr = list->alloc();

    const void* data1 = pointer->unsafe(0);
    const void* data2 = pointer->unsafe(0xffffffffffffffff);
    const void* data3 = pointer->unsafe(list_ptr);
    const void* data4 = pointer->unsafe(data_ptr1);
    const void* data5 = pointer->unsafe(zero_ptr);

    u64 value_ptr = pointer->copy(data_ptr1);

    RX_ASSERT(value_ptr == 0);
    RX_ASSERT(zero_ptr != 0);
    RX_ASSERT(char_ptr != 0);

    RX_ASSERT(data1 == 0);
    RX_ASSERT(data2 == 0);
    RX_ASSERT(data3 == 0);
    RX_ASSERT(data4 == 0);
    RX_ASSERT(data5 != 0);

    list->free(list_ptr);

#ifndef USE_GC
    pointer->free(zero_ptr);
    pointer->free(char_ptr);
    pointer->free(data_ptr1);
    pointer->free(value_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_v2, test_strcat_load_alloc_copy, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct pointer_data* ctx = rx->ctx;

    u64 file_name_ptr = pointer->load("input.txt");
    u64 mode_ptr = pointer->load("rb");

    u64 string_ptr = pointer->load("input.txt");
    u64 missing_ptr = pointer->load("ii");
    u64 list_ptr = list->alloc();
    u64 empty_ptr = pointer->load("\0");
    u64 zero_ptr = pointer->alloc();
    u64 null_ptr = 0;
    u64 void_ptr = 0xffffffffffffffff;

    u64 null_types[5] = { zero_ptr, string_ptr, empty_ptr, void_ptr, null_ptr };
    for (int i = 0; i < 5; i++) {
        pointer->printf(null_types[i % 7]);
    }

    pointer->printf(string_ptr);
    pointer->printf(list_ptr);
    pointer->printf(empty_ptr);
    pointer->printf(null_ptr);

    list->peek(string_ptr);
    list->peek(list_ptr);
    list->peek(empty_ptr);
    list->peek(null_ptr);

    list->push(string_ptr, string_ptr);
    list->push(string_ptr, list_ptr);
    list->push(string_ptr, empty_ptr);
    list->push(string_ptr, null_ptr);
    list->push(string_ptr, void_ptr);
    list->push(list_ptr, string_ptr);
    list->push(list_ptr, list_ptr);
    list->push(list_ptr, empty_ptr);
    list->push(list_ptr, null_ptr);
    list->push(list_ptr, void_ptr);
    list->push(empty_ptr, string_ptr);
    list->push(empty_ptr, list_ptr);
    list->push(empty_ptr, empty_ptr);
    list->push(empty_ptr, null_ptr);
    list->push(empty_ptr, void_ptr);
    list->push(null_ptr, string_ptr);
    list->push(null_ptr, list_ptr);
    list->push(null_ptr, empty_ptr);
    list->push(null_ptr, null_ptr);
    list->push(void_ptr, string_ptr);
    list->push(void_ptr, list_ptr);
    list->push(void_ptr, empty_ptr);
    list->push(void_ptr, null_ptr);

    list->pop(string_ptr);
    list->pop(list_ptr);
    list->pop(empty_ptr);
    list->pop(null_ptr);

    u64 path_error_ptr = pointer->load("\0");
    u64 result = file->file_alloc(path_error_ptr, mode_ptr);
    RX_ASSERT(result == 0);

    pointer->match_last(zero_ptr, path_error_ptr);
    pointer->match_last(path_error_ptr, zero_ptr);

    u64 file_data_ptr = file->file_alloc(file_name_ptr, mode_ptr);
    u64 data_ptr = file->file_read(file_data_ptr);

    const char* tmp = pointer->unsafe(data_ptr);
    u64 copy_ptr = pointer->load(tmp);
    file->file_free(data_ptr);

    pointer->strcpy(string_ptr, missing_ptr);
    pointer->strcpy(string_ptr, file_data_ptr);
    pointer->strcpy(string_ptr, data_ptr);
    pointer->strcpy(string_ptr, empty_ptr);
    pointer->strcpy(string_ptr, zero_ptr);
    pointer->strcpy(string_ptr, list_ptr);

    pointer->strcat(string_ptr, missing_ptr);
    pointer->strcat(string_ptr, file_data_ptr);
    pointer->strcat(string_ptr, data_ptr);
    pointer->strcat(string_ptr, copy_ptr);
    pointer->strcat(string_ptr, empty_ptr);
    pointer->strcat(string_ptr, zero_ptr);
    pointer->strcat(string_ptr, list_ptr);

    pointer->match_last(string_ptr, missing_ptr);

    u64 types[8] = { string_ptr, empty_ptr, zero_ptr, data_ptr, file_data_ptr, void_ptr, null_ptr };

    for (int i = 0; i < 49; i++) {
        pointer->strcpy(types[i % 7], types[i / 7]);
    }

    u64 pointer_match_last[49];
    for (int i = 0; i < 49; i++) {
        pointer_match_last[i] = pointer->match_last(types[i % 7], types[i / 7]);
    }

    u64 file_file_alloc[49];
    for (int i = 0; i < 49; i++) {
        file_file_alloc[i] = file->file_alloc(types[i % 7], types[i / 7]);
    }

    file->file_free(file_data_ptr);
    file->file_free(file_data_ptr);
    file->file_free(data_ptr);
    file->file_free(data_ptr);

    for (int i = 0; i < 49; i++) {
        file_file_alloc[i] = file->file_alloc(types[i % 7], types[i / 7]);
    }

    for (int i = 0; i < 49; i++) {
        RX_ASSERT(pointer_match_last[i] == 0);
    }

    for (int i = 0; i < 49; i++) {
        RX_ASSERT(file_file_alloc[i] == 0);
    }

    pointer->put_char(string_ptr, 'a');
    pointer->put_char(void_ptr, 'a');
    pointer->put_char(list_ptr, 'a');
    pointer->put_char(copy_ptr, 'a');
    pointer->put_char(data_ptr, 'a');
    pointer->put_char(file_data_ptr, 'a');

    pointer->put_char(string_ptr, 'a');
    pointer->put_char(empty_ptr, 'a');
    pointer->put_char(string_ptr, '\0');
    pointer->put_char(empty_ptr, '\0');
    pointer->put_char(string_ptr, '/');

    pointer->size(zero_ptr);
    pointer->size(string_ptr);
    pointer->size(null_ptr);
    pointer->size(void_ptr);

    file->file_read(string_ptr);
    file->file_read(null_ptr);
    file->file_read(void_ptr);
    file->file_read(empty_ptr);
    file->file_read(copy_ptr);

    pointer->printf(string_ptr);
    pointer->printf(null_ptr);
    pointer->printf(void_ptr);
    pointer->printf(zero_ptr);
    pointer->printf(copy_ptr);

    pointer->strcpy(string_ptr, null_ptr);
    pointer->strcpy(string_ptr, copy_ptr);
    pointer->strcpy(null_ptr, string_ptr);
    pointer->strcpy(string_ptr, void_ptr);
    pointer->strcpy(void_ptr, void_ptr);
    pointer->strcpy(string_ptr, empty_ptr);

    pointer->strcat(string_ptr, null_ptr);
    pointer->strcat(null_ptr, string_ptr);
    pointer->strcat(string_ptr, void_ptr);
    pointer->strcat(void_ptr, void_ptr);
    pointer->strcat(zero_ptr, void_ptr);
    pointer->strcat(string_ptr, string_ptr);
    pointer->strcat(string_ptr, empty_ptr);

    pointer->match_last(string_ptr, null_ptr);
    pointer->match_last(null_ptr, string_ptr);
    pointer->match_last(string_ptr, void_ptr);
    pointer->match_last(void_ptr, void_ptr);
    pointer->match_last(string_ptr, string_ptr);
    pointer->match_last(string_ptr, empty_ptr);
    pointer->match_last(string_ptr, string_ptr);
    pointer->match_last(empty_ptr, empty_ptr);
    pointer->match_last(empty_ptr, string_ptr);

    pointer->match_last(empty_ptr, empty_ptr);
    pointer->match_last(empty_ptr, string_ptr);
    pointer->match_last(string_ptr, empty_ptr);
    pointer->match_last(string_ptr, string_ptr);

    pointer->unsafe(null_ptr);
    pointer->unsafe(void_ptr);
    pointer->unsafe(empty_ptr);
    pointer->unsafe(string_ptr);

    file->file_alloc(string_ptr, null_ptr);
    file->file_alloc(string_ptr, void_ptr);
    file->file_alloc(string_ptr, string_ptr);
    file->file_alloc(string_ptr, empty_ptr);
    file->file_alloc(empty_ptr, mode_ptr);
    file->file_alloc(void_ptr, void_ptr);
    file->file_alloc(null_ptr, null_ptr);
    file->file_alloc(zero_ptr, mode_ptr);

    file->file_free(file_data_ptr);
    file->file_free(file_data_ptr);
    file->file_free(copy_ptr);
    file->file_free(copy_ptr);

    file->file_read(string_ptr);
    file->file_read(null_ptr);
    file->file_read(void_ptr);
    file->file_read(empty_ptr);
    file->file_read(copy_ptr);

    const char* data_unsafe_ptr1 = pointer->unsafe(empty_ptr);
    const char* data_unsafe_ptr2 = pointer->unsafe(string_ptr);
    const char* data_unsafe_ptr3 = pointer->unsafe(list_ptr);

    RX_ASSERT(data_unsafe_ptr1 != 0);
    RX_ASSERT(data_unsafe_ptr2 != 0);
    RX_ASSERT(data_unsafe_ptr3 == 0);

    RX_ASSERT(string_ptr != 0);

    u64 undefined_ptr = pointer->copy(list_ptr);

    RX_ASSERT(undefined_ptr == 0);

    list->free(list_ptr);

#ifdef USE_MEMORY_DEBUG_INFO
    virtual->dump(0);
    virtual->dump(ctx->vm);
    virtual->dump_ref(0);
    virtual->dump_ref(ctx->vm);
#endif

#ifndef USE_GC
    pointer->free(data_ptr);
    pointer->free(path_error_ptr);
    pointer->free(copy_ptr);
    pointer->free(file_name_ptr);
    pointer->free(mode_ptr);
    pointer->free(missing_ptr);
    pointer->free(string_ptr);
    pointer->free(empty_ptr);
    pointer->free(zero_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_v2, test_strcat_alloc_load, .fixture = test_fixture) {
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
RX_TEST_CASE(tests_v2, test_strcat_load_alloc_alloc, .fixture = test_fixture) {
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
RX_TEST_CASE(tests_v2, test_strcat_load_load, .fixture = test_fixture) {
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
RX_TEST_CASE(tests_v2, test_strcpy_load_alloc, .fixture = test_fixture) {
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
RX_TEST_CASE(tests_v2, test_strcpy_load_load, .fixture = test_fixture) {
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
RX_TEST_CASE(tests_v2, test_load_open_file_close_file, .fixture = test_fixture) {
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
RX_TEST_CASE(tests_v2, test_file_read_invalid_type, .fixture = test_fixture) {
    u64 list_ptr = list->alloc();
    u64 data_ptr = file->file_read(list_ptr);
    list->free(data_ptr);
    file->file_free(list_ptr);
    list->free(list_ptr);
#ifndef USE_GC
    pointer->free(data_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_v2, test_load_open_match_last_unsafe_free, .fixture = test_fixture) {
    u64 file_path_ptr = pointer->getcwd();
    u64 file_name_ptr = pointer->load("/all_english_words.txt//");
    pointer->strcat(file_path_ptr, file_name_ptr);
    u64 pattern_ptr = pointer->load("//");
    u64 last_match_ptr = pointer->match_last(file_path_ptr, pattern_ptr);
    pointer->put_char(last_match_ptr, '\0');
    pointer->strcat(file_path_ptr, file_name_ptr);
    u64 mode_ptr = pointer->load("rb");
    u64 f_ptr = file->file_alloc(file_path_ptr, mode_ptr);
    u64 data_ptr = file->file_read(f_ptr);
    pointer->printf(data_ptr);
#ifndef USE_GC
    pointer->free(file_path_ptr);
    pointer->free(file_name_ptr);
    pointer->free(pattern_ptr);
    pointer->free(last_match_ptr);
    pointer->free(mode_ptr);
    pointer->free(f_ptr);
    pointer->free(data_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_v2, test_load_open_file_unsafe_hashtable, .fixture = test_fixture) {
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
        u64 list_ptr = list->alloc();
        file->file_free(f_ptr);
        char* file_data = pointer->unsafe(data_ptr);
        for (int i = 0; i < 100; i++) {
            char* tmp = file_data;
            while (*tmp != 0 && *tmp != '\n') {
                tmp++;
            }
            *tmp++ = '\0';
            u64 data = pointer->load(file_data);
            list->push(list_ptr, data);
            char* unsafe = pointer->unsafe(data);
            printf("   .: %s\n", unsafe);
            file_data = tmp;
        }
        list->free(list_ptr);
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

static int run(void) {
    TEST_RUN(alloc, list_alloc_tests);
    TEST_RUN(micro, list_micro_tests);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- acceptance test code\n");
#endif
    /* safe */
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- rexo unit test code\n");
#endif
    /* Execute the main function that runs the test cases found. */
    int result = rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;
    return alloc | micro | result;
}

const struct test_suite vm_v2_test_suite_definition = {
    .run = run
};
