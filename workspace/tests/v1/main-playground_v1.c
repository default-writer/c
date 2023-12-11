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
#include "playground/class/v1/class_v1.h"
#include "playground/list/v1/list_v1.h"

#ifdef USE_MEMORY_ALLOC
#include "playground/memory/memory.h"
#endif

/* externally visible class definition API */
extern const struct class class_definition_v1;
/* definition */
extern const struct list list_methods_v1;
/* externally visible class data definition API */
extern struct class_data class_data;

/* definition */
static const struct list* list = &list_methods_v1;

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    struct list_data* ctx;
}* TEST_DATA;

/* Initialize the data structure. Its allocation is handled by Rexo. */
RX_SET_UP(test_set_up) {
    list->init();
    /* success */
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    list->destroy();
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

/* test context */
RX_TEST_CASE(tests, test_list_push_v1, .fixture = test_fixture) {
    /* prepares the payload */
    u8* payload = (void*)0x7bde8421;
    /* pushes to the list multiple times */
    list->push(payload);
    list->push(payload);
    list->push(payload);
    list->push(payload);
    /* pops from the list */
    list->pop();
    list->pop();
    list->pop();
    /* peeks from the list */
    const void* head = list->peek();
    /* ensures data is added to the list */
    RX_ASSERT(head == payload);
}

RX_TEST_CASE(tests, test_context_enter_leave_v1, .fixture = test_fixture) {
    const struct class* context = &class_definition_v1;
    struct class_data* data = context->new ();
    context->push(data);
    RX_ASSERT(context->pop() == data);
    context->delete (data);
}

/* test context */
RX_TEST_CASE(tests, test_class_get_set_data_v1, .fixture = test_fixture) {
    const struct class* context = &class_definition_v1;
    struct class_data* data = context->new ();
    void* payload = (void(*))0x7bde8421;
    context->push(data);
    context->set(payload);
    RX_ASSERT(context->get() == payload);
    RX_ASSERT(context->pop() == data);
    context->delete (data);
}

/* test context */
RX_TEST_CASE(tests, test_class_push_pop_get_set_data_v1, .fixture = test_fixture) {
    const struct class* context = &class_definition_v1;
    struct class_data* data1 = context->new ();
    struct class_data* data2 = context->new ();

    void* payload1 = (void(*))0x7bde8421;
    void* payload2 = (void(*))0xbebebebe;

    context->push(data1);
    context->set(payload1);

    context->push(data2);
    context->set(payload2);

    RX_ASSERT(context->get() == payload2);
    RX_ASSERT(context->pop() == data2);

    RX_ASSERT(context->get() == payload1);
    RX_ASSERT(context->pop() == data1);

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
