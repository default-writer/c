/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   11 December 2023 at 9:15:15 GMT+3
 *
 */
/*
    Copyright (C) 2022-2047 Artur Mustafin (artur.mustafin@gmail.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _REXO_TESTS_
#define _REXO_TESTS_

#define RXP_DEBUG_TESTS

#include "../.deps/rexo/include/rexo.h"
#endif

#include "common/memory.h"
#include "list-micro/data.h"
#include "playground/brain/brain.h"
#include "playground/hashtable/v2/hashtable_v2.h"
#include "pointer/v2/pointer_v2.h"

#define DEFAULT_SIZE 0x8

/* definition */
extern const struct virtual_methods virtual_methods_definition;
extern const struct list list_micro_definition;

/* definition */
#if defined(VM_MEMORY_DEBUG_INFO)
extern const struct debug_methods debug_methods_definition;
#endif

/* definition */
#if defined(VM_MEMORY_DEBUG_INFO)
static const struct debug_methods* debug = &debug_methods_definition;
#endif

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
    char* dest = memory->alloc(size);
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
    char* buffer = memory->alloc(size);
    for (u64 i = 0; i < size - 1; i++) {
        u64 ch0 = list->peek(list_ptr);
        const char* ch = pointer->unsafe(ch0);
        *(buffer + i) = *ch;
        u64 ch_ptr = list->pop(list_ptr);
#ifdef USE_GC
        CLEAN(ch_ptr)
#endif
#ifndef USE_GC
        pointer->free(ch_ptr);
#endif
    }
    printf("   .: %s\n", buffer);
    memory->free(buffer, size);
    memory->free(dest, size);
    list->free(list_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_list_push_list_peek_list_pop_free, .fixture = test_fixture) {
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
        u64 data = pointer->load(ptr);
        *tmp = ch;
        list->push(list_ptr, data);
    }
    char* buffer = memory->alloc(size);
    for (u64 i = 0; i < size - 1; i++) {
        u64 ch0 = list->peek(list_ptr);
        const char* ch = pointer->unsafe(ch0);
        *(buffer + i) = *ch;
        u64 ch_ptr = list->pop(list_ptr);
#ifdef USE_GC
        CLEAN(ch_ptr)
#endif
#ifndef USE_GC
        pointer->free(ch_ptr);
#endif
    }
#ifndef USE_GC
    pointer->free(list_ptr);
#endif
    printf("   .: %s\n", buffer);
    memory->free(buffer, size);
    memory->free(dest, size);
    list->free(list_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_list_peek_0, .fixture = test_fixture) {
    const char* source = "Hello, world! A very long string do not fit in 8 bytes.";
    u64 size = strlen(source) + 1;
    char* dest = memory->alloc(size);
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
    char* buffer = memory->alloc(size);
    for (u64 i = 0; i < size - 1; i++) {
        const char* ch = pointer->unsafe(i + 1);
        *(buffer + i) = *ch;
#ifndef USE_GC
        pointer->free(i + 1);
#endif
    }
    printf("   .: %s\n", buffer);
    memory->free(buffer, size);
    memory->free(dest, size);
}

int main(int argc, char** argv) {
#ifdef USE_MEMORY_DEBUG_INFO
#if defined(VM_GLOBAL_DEBUG_INFO)
    global_statistics();
#endif
#endif
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
#if defined(VM_GLOBAL_DEBUG_INFO)
    global_statistics();
#endif
#endif
    return result;
}
