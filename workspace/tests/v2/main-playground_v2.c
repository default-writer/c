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
#ifndef _REXO_TESTS_
#define _REXO_TESTS_

#define RXP_DEBUG_TESTS

#include "../.deps/rexo/include/rexo.h"
#endif

#include "common/memory.h"
#include "playground/class/v2/class_v2.h"
#include "playground/list/v2/list_v2.h"

#define DEFAULT_SIZE 0xffff /* 64K bytes */

#ifdef USE_MEMORY_ALLOC
#include "playground/memory/memory.h"
#endif

/* externally visible class definition API */
extern const struct class class_definition_v2;
/* definition */
extern const struct list list_v2;
/* externally visible class data definition API */
extern struct class_data class_data;

/* definition */
static const struct list* list = &list_v2;
static struct list_data* list_data;

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    void* ptr;
}* TEST_DATA;

/* Initialize the data structure. Its allocation is handled by Rexo. */
RX_SET_UP(test_set_up) {
    list_data = list->alloc(DEFAULT_SIZE);
    /* success */
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    list->free(list_data);
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

/* test context */
RX_TEST_CASE(tests, test_list_push_v1, .fixture = test_fixture) {
    /* prepares the payload */
    u8* payload = (void*)0x7bde8421;
    /* pushes to the list multiple times */
    struct list_data* list_data_object = list->alloc(16);
    list->push(list_data_object, payload);
    list->push(list_data_object, payload);
    list->push(list_data_object, payload);
    list->push(list_data_object, payload);
    /* pops from the list */
    list->pop(list_data_object);
    list->pop(list_data_object);
    list->pop(list_data_object);
    /* peeks from the list */
    const void* head = list->peek(list_data_object);
    list->free(list_data_object);
    /* ensures data is added to the list */
    RX_ASSERT(head == payload);
}

RX_TEST_CASE(tests, test_context_enter_leave_v2, .fixture = test_fixture) {
    const struct class* context = &class_definition_v2;
    struct class_data* data = context->new (list_data);
    context->push(data);
    RX_ASSERT(context->pop(data) == data);
    context->delete (data);
}

/* test context */
RX_TEST_CASE(tests, test_class_get_set_data_v2, .fixture = test_fixture) {
    const struct class* context = &class_definition_v2;
    struct class_data* data = context->new (list_data);
    void* payload = (void(*))0x7bde8421;
    context->push(data);
    context->set(data, payload);
    RX_ASSERT(context->get(data) == payload);
    RX_ASSERT(context->pop(data) == data);
    context->delete (data);
}

/* test context */
RX_TEST_CASE(tests, test_class_push_pop_get_set_data_v2, .fixture = test_fixture) {
    const struct class* context = &class_definition_v2;
    struct class_data* data1 = context->new (list_data);
    struct class_data* data2 = context->new (list_data);

    void* payload1 = (void(*))0x7bde8421;
    void* payload2 = (void(*))0xbebebebe;

    context->push(data1);
    context->set(data1, payload1);

    context->push(data2);
    context->set(data2, payload2);

    RX_ASSERT(context->get(data2) == payload2);
    RX_ASSERT(context->pop(data2) == data2);

    RX_ASSERT(context->get(data1) == payload1);
    RX_ASSERT(context->pop(data1) == data1);

    context->delete (data1);
    context->delete (data2);
}

int main(void) {
#ifdef USE_MEMORY_DEBUG_INFO
#if defined(VM_GLOBAL_DEBUG_INFO)
    global_statistics();
#endif
#endif
#ifdef USE_MEMORY_ALLOC
    memory_init(void);
    memory_destroy(void);
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
