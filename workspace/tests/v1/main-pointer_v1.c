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
#ifndef _REXO_TESTS_
#define _REXO_TESTS_
//#define RXP_DEBUG_TESTS
#include "../.deps/rexo/include/rexo.h"
#endif

#include "common/memory.h"
#include "list-micro/data.h"

#include "playground/brain/brain.h"
#include "playground/hashtable/v1/hashtable_v1.h"

#include "pointer/v1/pointer_v1.h"
#include "pointer/types/data/v1/data_v1.h"
#include "pointer/types/file/v1/file_v1.h"
#include "pointer/types/list/v1/list_v1.h"
#include "pointer/types/string/v1/string_v1.h"

#define DEFAULT_SIZE 0x8

/* definition */
extern const struct virtual_methods virtual_methods_definition;

typedef struct test_data {
    struct pointer_data* ctx;
}* TEST_DATA;

RX_SET_UP(test_set_up) {
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    // nothing to cleanup
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

/* test init */
RX_TEST_CASE(tests, test_list_unsafe, .fixture = test_fixture) {
    pointer->init(8);
    u64 data_ptr = data->alloc(256);
    u64 data_size = data->size(data_ptr);
    RX_ASSERT(data_size == 256);
    data->free(data_ptr);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests, test_list_push_list_peek_list_pop, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    const char* source = "Hello, world!";
    u64 size = strlen(source) + 1;
    char* dest = memory->alloc(size);
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
    char* buffer = memory->alloc(size);
    for (u64 i = 0; i < size - 1; i++) {
        u64 ch0 = list->peek(list_ptr);
        const char* string_ptr = string->unsafe(ch0);
        *(buffer + i) = *string_ptr;
        u64 ch = list->pop(list_ptr);
        string->free(ch);
    }
    printf("   .: %s\n", buffer);
    memory->free(buffer, size);
    memory->free(dest, size);
    list->free(list_ptr);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests, test_list_push_list_peek_list_pop_free, .fixture = test_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    const char* source = "Hello, world!";
    u64 size = strlen(source) + 1;
    char* dest = memory->alloc(size);
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
    char* buffer = memory->alloc(size);
    for (u64 i = 0; i < size - 1; i++) {
        u64 ch0 = list->peek(list_ptr);
        const char* string_ptr = string->unsafe(ch0);
        *(buffer + i) = *string_ptr;
        u64 ch = list->pop(list_ptr);
        string->free(ch);
    }
    string->free(list_ptr);
    printf("   .: %s\n", buffer);
    memory->free(buffer, size);
    memory->free(dest, size);
    list->free(list_ptr);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests, test_list_push_0_list_peek_0_list_pop_0_list_free_0, .fixture = test_fixture) {
    pointer->init(8);
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
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests, test_list_peek_0, .fixture = test_fixture) {
    pointer->init(8);
    const char* source = "Hello, world! A very long string do not fit in 8 bytes.";
    u64 size = strlen(source) + 1;
    char* dest = memory->alloc(size);
    memcpy(dest, source, size); /* NOLINT */
    for (u64 i = 0; i < size - 1; i++) {
        char* ptr = dest + i;
        char* tmp = ptr + 1;
        char ch = *tmp;
        *tmp = 0;
        string->load(ptr);
        *tmp = ch;
    }
    char* buffer = memory->alloc(size);
    for (u64 i = 0; i < size - 1; i++) {
        const char* string_ptr = string->unsafe(i + 1);
        *(buffer + i) = *string_ptr;
        string->free(i + 1);
    }
    printf("   .: %s\n", buffer);
    memory->free(buffer, size);
    memory->free(dest, size);
    pointer->gc();
    pointer->destroy();
}

int main(int argc, char** argv) {
#ifdef USE_MEMORY_DEBUG_INFO
#if defined(VM_GLOBAL_DEBUG_INFO)
    global_statistics();
#endif
#endif
    CLEAN(argc)
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- rexo unit test code\n");
#endif
    /* Execute the main function that runs the test cases found. */
    int result = rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;
#ifdef USE_MEMORY_DEBUG_INFO
#if defined(VM_GLOBAL_DEBUG_INFO)
    global_statistics();
#endif
#endif
    return result;
}
