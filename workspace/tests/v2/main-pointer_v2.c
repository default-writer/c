/*
 *
 * Russian's IP Protection License
 *
 * Copyright (c) 2023 Artur Mustafin
 *
 * Permission is hereby granted, free of charge, to any person with citizenship
 * and location in Russia including Crimea and all occupations obtaining a copy
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
 * For the rest of the world it is an order to pay royalties by agreement to the
 * author of the code base for ability to use any part of the project for any
 * purpouse including but not limited to the creative ideas or technologies are
 * being used in this owned intellectual property.
 *
 * It is strictly prohibited to use this code base or any part of it for any purpouse
 * including prohibiting or restricive purpouses agains Russians for any EU citizens
 * or other person with USA citizenship, origin or background including work permit
 * or locations from selected territories or any territory or any other country except
 * Russia considered as breaking basic human rights, freedom of speesh or involved in
 * acts of terrorism in a territory owned, occupied or managed by another country.
 *
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
