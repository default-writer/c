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
#include "common/alloc.h"
#include "playground/class/v1/class_v1.h"
#include "playground/list/v1/list_v1.h"
#define RXP_DEBUG_TESTS

#include "rexo/include/rexo.h"

#ifdef USE_MEMORY_ALLOC
#include "playground/memory/memory.h"
#endif

/* externally visible class definition API */
extern const struct class class_definition_v1;
/* definition */
extern const struct list list_v1;
/* externally visible class data definition API */
extern struct class_data class_data;

/* definition */
static const struct list* list = &list_v1;

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
    u8* payload = (void*)0xdeadbeef;
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
    void* payload = (void(*))0xdeadbeef;
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

    void* payload1 = (void(*))0xdeadbeef;
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

static void INIT init() {
#ifdef USE_MEMORY_DEBUG_INFO
    global_statistics();
#endif
}

static void DESTROY destroy() {
#ifdef USE_MEMORY_DEBUG_INFO
    global_statistics();
#endif
}

int main(void) {
#ifndef ATTRIBUTE
    init();
#endif
#ifdef USE_MEMORY_ALLOC
    memory_init(void);
    memory_destroy(void);
#endif
    /* Execute the main function that runs the test cases found. */
    int result = rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;
#ifndef ATTRIBUTE
    destroy();
#endif
    return result;
}
