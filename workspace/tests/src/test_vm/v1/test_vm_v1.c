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
 * including prohibiting or restricive purpouses against Russians for any EU citizens
 * or other person with USA citizenship, origin or background including work permit
 * or locations from selected territories or any territory or any other country except
 * Russia considered as breaking basic human rights, freedom of speesh or involved in
 * acts of terrorism in a territory owned, occupied or managed by another country.
 *
 */
#include "../tests/src/test.h"

#include "test_vm_v1.h"

#include "pointer/types/data/v1/data_v1.h"
#include "pointer/types/list/v1/list_v1.h"
#include "pointer/types/types.h"
#include "pointer/types/user/v1/user_v1.h"
#include "pointer/v1/pointer_v1.h"
#include "pointer/v1/virtual_v1.h"

#define DEFAULT_SIZE 0x100

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    struct vm* ctx;
}* TEST_DATA;

RX_SET_UP(test_set_up) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct vm** ctx = &rx->ctx;
    virtual->init(ctx, 0);
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct vm** ctx = &rx->ctx;
    virtual->destroy(ctx);
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

/* test init */
RX_TEST_CASE(tests, test_vm_read_type_0, .fixture = test_fixture) {
    struct pointer* ptr = virtual->read_type(0, TYPE_DATA);
    RX_ASSERT(ptr == 0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_destroy_0, .fixture = test_fixture) {
    struct vm** ptr = 0;
    virtual->destroy(ptr);
    RX_ASSERT(ptr == 0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_destroy_null, .fixture = test_fixture) {
    struct vm* vm_ptr = 0;
    struct vm** ptr = &vm_ptr;
    virtual->destroy(ptr);
    RX_ASSERT(vm_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_alloc_user, .fixture = test_fixture) {
    struct pointer* ptr = pointer->alloc(0, TYPE_NULL);
    u64 virtual_ptr = virtual->alloc(ptr);
    const struct pointer* vm_ptr = virtual->read(virtual_ptr);
    RX_ASSERT(ptr != 0);
    RX_ASSERT(virtual_ptr != 0);
    RX_ASSERT(vm_ptr == ptr);
    pointer->release(ptr);
}

/* test init */
RX_TEST_CASE(tests, test_vm_read_data, .fixture = test_fixture) {
    struct pointer* ptr = pointer->alloc(0, TYPE_DATA);
    u64 virtual_ptr = virtual->alloc(ptr);
    const struct pointer* vm_ptr = virtual->read(virtual_ptr);
    RX_ASSERT(ptr != 0);
    RX_ASSERT(virtual_ptr != 0);
    RX_ASSERT(vm_ptr == ptr);
    pointer->release(ptr);
}

/* test init */
RX_TEST_CASE(tests, test_vm_read_1, .fixture = test_fixture) {
    struct pointer* ptr = pointer->alloc(0, TYPE_DATA);
    u64 virtual_ptr = virtual->alloc(ptr);
    const struct pointer* vm_ptr = virtual->read(1);
    RX_ASSERT(ptr != 0);
    RX_ASSERT(virtual_ptr != 0);
    RX_ASSERT(vm_ptr == ptr);
    pointer->release(ptr);
}

/* test init */
RX_TEST_CASE(tests, test_vm_read_9, .fixture = test_fixture) {
    struct pointer* ptr = pointer->alloc(0, TYPE_DATA);
    u64 virtual_ptr = virtual->alloc(ptr);
    const struct pointer* vm_ptr = virtual->read(vm_options->size + 1);
    RX_ASSERT(ptr != 0);
    RX_ASSERT(virtual_ptr != 0);
    RX_ASSERT(vm_ptr == 0);
    pointer->release(ptr);
}

/* test init */
RX_TEST_CASE(tests, test_virtual_free_0, .fixture = test_fixture) {
    pointer->release(0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_free_0, .fixture = test_fixture) {
    virtual->free(0);
    RX_ASSERT(0 == 0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_read_0, .fixture = test_fixture) {
    const struct pointer* ptr = virtual->read(0);
    RX_ASSERT(ptr == 0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_init_0, .fixture = test_fixture) {
    RX_ASSERT(0 == 0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_read_type_1, .fixture = test_fixture) {
    struct pointer* ptr = virtual->read_type(1, TYPE_DATA);
    RX_ASSERT(ptr == 0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_write_0, .fixture = test_fixture) {
    u64 virtual_ptr = virtual->alloc(0);
    RX_ASSERT(virtual_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests, test_vm_read_type_write_1_read_1, .fixture = test_fixture) {
    struct pointer* ptr = pointer->alloc(0, TYPE_DATA);
    u64 virtual_ptr = virtual->alloc(ptr);
    struct pointer* vm_ptr = virtual->read_type(virtual_ptr, TYPE_DATA);
    RX_ASSERT(ptr != 0);
    RX_ASSERT(virtual_ptr != 0);
    RX_ASSERT(vm_ptr == ptr);
    pointer->release(ptr);
}

/* test init */
RX_TEST_CASE(tests, test_vm_read_type_write_1_read_2, .fixture = test_fixture) {
    struct pointer* ptr = pointer->alloc(0, TYPE_DATA);
    u64 virtual_ptr = virtual->alloc(ptr);
    struct pointer* vm_ptr = virtual->read_type(virtual_ptr + 1, TYPE_DATA);
    RX_ASSERT(ptr != 0);
    RX_ASSERT(virtual_ptr != 0);
    RX_ASSERT(vm_ptr == 0);
    pointer->release(ptr);
}

/* -------------------------------------------------------- *
 *                                                          *
 *                                                          *
 * test pointer                                             *
 *                                                          *
 *                                                          *
 * -------------------------------------------------------- */

typedef struct test_pointer_data {
    void* ptr;
}* TEST_POINTER_DATA;

RX_SET_UP(test_pointer_set_up) {
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_pointer_tear_down) {
    /* nothing */
}

/* Define the fixture. */
RX_FIXTURE(test_pointer_fixture, TEST_POINTER_DATA, .set_up = test_pointer_set_up, .tear_down = test_pointer_tear_down);

/* test init */
RX_TEST_CASE(tests, test_list, .fixture = test_pointer_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    list->free(list_ptr);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests, test_list_user, .fixture = test_pointer_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    u64 user_ptr = user->alloc();
    list->push(list_ptr, user_ptr);
    user->free(user_ptr);
    list->free(list_ptr);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests, test_list_data, .fixture = test_pointer_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    u64 data_ptr = data->alloc(1024);
    list->push(list_ptr, data_ptr);
    data->free(data_ptr);
    list->free(list_ptr);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests, test_list_free_user_free, .fixture = test_pointer_fixture) {
    pointer->init(8);
    u64 list_ptr = list->alloc();
    u64 user_ptr = user->alloc();
    list->push(list_ptr, user_ptr);
    list->free(list_ptr);
    user->free(user_ptr);
    pointer->gc();
    pointer->destroy();
}

/* test init */
RX_TEST_CASE(tests, test_user_free_0, .fixture = test_pointer_fixture) {
    pointer->init(8);
    user->free(0);
    pointer->gc();
    pointer->destroy();
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
