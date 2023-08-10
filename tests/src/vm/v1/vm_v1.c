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
#include "vm_v1.h"
#include "vm/v1/vm_v1.h"

#include "pointer/types/list/v1/list_v1.h"
#include "pointer/types/user/v1/user_v1.h"
#include "pointer/v1/pointer_v1.h"

#define DEFAULT_SIZE 0x100

/* definition */
extern const struct vm_methods vm_methods_definition;
extern const struct vm_default_options vm_default_options_definition;
extern const struct pointer_vm_methods pointer_vm_methods_definition;

/* definition */
static const struct vm_methods* vm = &vm_methods_definition;
static const struct vm_default_options* vm_options = &vm_default_options_definition;
static const struct pointer_vm_methods* virtual = &pointer_vm_methods_definition;

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    struct vm* ctx;
}* TEST_DATA;

RX_SET_UP(test_set_up) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct vm** ctx = &rx->ctx;
    *ctx = vm->init(0);
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct vm** ctx = &rx->ctx;
    vm->destroy(ctx);
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

/* test init */
RX_TEST_CASE(tests, test_vm_read_type_0, .fixture = test_fixture) {
    const struct pointer* ptr = vm->read_type(0, TYPE_DATA);
    RX_ASSERT(ptr == 0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_alloc_user, .fixture = test_fixture) {
    struct pointer* ptr = virtual->alloc(0, TYPE_NULL);
    u64 data_ptr = vm->alloc(ptr);
    const struct pointer* vm_ptr = vm->read(data_ptr);
    RX_ASSERT(ptr != 0);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(vm_ptr == ptr);
    virtual->free(ptr);
}

/* test init */
RX_TEST_CASE(tests, test_vm_read_data, .fixture = test_fixture) {
    struct pointer* ptr = virtual->alloc(0, TYPE_DATA);
    u64 data_ptr = vm->alloc(ptr);
    const struct pointer* vm_ptr = vm->read(data_ptr);
    RX_ASSERT(ptr != 0);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(vm_ptr == ptr);
    virtual->free(ptr);
}

/* test init */
RX_TEST_CASE(tests, test_vm_read_1, .fixture = test_fixture) {
    struct pointer* ptr = virtual->alloc(0, TYPE_DATA);
    u64 data_ptr = vm->alloc(ptr);
    const struct pointer* vm_ptr = vm->read(1);
    RX_ASSERT(ptr != 0);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(vm_ptr == ptr);
    virtual->free(ptr);
}

/* test init */
RX_TEST_CASE(tests, test_vm_read_9, .fixture = test_fixture) {
    struct pointer* ptr = virtual->alloc(0, TYPE_DATA);
    u64 data_ptr = vm->alloc(ptr);
    const struct pointer* vm_ptr = vm->read(vm_options->size + 1);
    RX_ASSERT(ptr != 0);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(vm_ptr == 0);
    virtual->free(ptr);
}

/* test init */
RX_TEST_CASE(tests, test_pointer_vm_free_0, .fixture = test_fixture) {
    virtual->free(0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_free_0, .fixture = test_fixture) {
    vm->free(0);
    RX_ASSERT(0 == 0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_read_0, .fixture = test_fixture) {
    const struct pointer* ptr = vm->read(0);
    RX_ASSERT(ptr == 0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_init_0, .fixture = test_fixture) {
    RX_ASSERT(0 == 0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_read_type_1, .fixture = test_fixture) {
    const struct pointer* ptr = vm->read_type(1, TYPE_DATA);
    RX_ASSERT(ptr == 0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_write_0, .fixture = test_fixture) {
    u64 ptr = vm->alloc(0);
    RX_ASSERT(ptr == 0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_read_type_write_1_read_1, .fixture = test_fixture) {
    struct pointer* ptr = virtual->alloc(0, TYPE_DATA);
    u64 data_ptr = vm->alloc(ptr);
    const struct pointer* vm_ptr = vm->read_type(data_ptr, TYPE_DATA);
    RX_ASSERT(ptr != 0);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(vm_ptr == ptr);
    virtual->free(ptr);
}

/* test init */
RX_TEST_CASE(tests, test_vm_read_type_write_1_read_2, .fixture = test_fixture) {
    struct pointer* ptr = virtual->alloc(0, TYPE_DATA);
    u64 data_ptr = vm->alloc(ptr);
    const struct pointer* vm_ptr = vm->read_type(data_ptr + 1, TYPE_DATA);
    RX_ASSERT(ptr != 0);
    RX_ASSERT(data_ptr != 0);
    RX_ASSERT(vm_ptr == 0);
    virtual->free(ptr);
}

/* -------------------------------------------------------- *
 *                                                          *
 *                                                          *
 * test pointer                                             *
 *                                                          *
 *                                                          *
 * -------------------------------------------------------- */

/* definition */
extern struct pointer_data* pointer_data_init(u64 size);
extern void pointer_data_destroy(struct pointer_data** ctx);

/* definition */
extern const struct user_methods user_methods_definition;
extern const struct list_methods list_methods_definition;
extern const struct pointer_methods pointer_methods_definition;

/* definition */
static const struct user_methods* user = &user_methods_definition;
static const struct list_methods* list = &list_methods_definition;
static const struct pointer_methods* pointer = &pointer_methods_definition;

typedef struct test_pointer_data {
    struct pointer_data* ctx;
}* TEST_POINTER_DATA;

RX_SET_UP(test_pointer_set_up) {
    TEST_POINTER_DATA rx = (TEST_POINTER_DATA)RX_DATA;
    struct pointer_data** ctx = &rx->ctx;
    *ctx = pointer_data_init(8);
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_pointer_tear_down) {
    TEST_POINTER_DATA rx = (TEST_POINTER_DATA)RX_DATA;
    struct pointer_data** ctx = &rx->ctx;
    pointer_data_destroy(ctx);
}

/* Define the fixture. */
RX_FIXTURE(test_pointer_fixture, TEST_POINTER_DATA, .set_up = test_pointer_set_up, .tear_down = test_pointer_tear_down);

/* test init */
RX_TEST_CASE(tests, test_list, .fixture = test_pointer_fixture) {
    u64 list_ptr = list->alloc();
    list->free(list_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_list_user, .fixture = test_pointer_fixture) {
    u64 list_ptr = list->alloc();
    u64 user_ptr = user->alloc();
    list->push(list_ptr, user_ptr);
    user->free(user_ptr);
    list->free(list_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_list_free_user_free, .fixture = test_pointer_fixture) {
    u64 list_ptr = list->alloc();
    u64 user_ptr = user->alloc();
    list->push(list_ptr, user_ptr);
    list->free(list_ptr);
    user->free(user_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_user_free_0, .fixture = test_pointer_fixture) {
    user->free(0);
}

static int run(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- acceptance test code\n");
#endif
    /* safe */
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- rexo unit test code\n");
#endif
    /* Execute the main function that runs the test cases found. */
    int result = rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;
    return result;
}

const struct test_suite vm_v1_test_suite_definition = {
    .run = run
};