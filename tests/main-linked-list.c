/*
 *
 *  MIT License
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

#include "rexo/include/rexo.h"

#include "stl/linked_list.h"
#include "stl/linked_list_enumerator.h"

/* definition */
extern const struct linked_list_methods linked_list_methods_definition;
extern const struct linked_list_enumerator_methods linked_list_enumerator_methods_definition;
static const struct linked_list_methods* list = &linked_list_methods_definition;
static const struct linked_list_enumerator_methods* enumerator = &linked_list_enumerator_methods_definition;

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    struct linked_list* ctx;
}* TEST_DATA;

/* list parameters definition */

/* private */

/* Initialize the data structure. Its allocation is handled by Rexo. */
RX_SET_UP(test_set_up) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list** ctx = &rx->ctx;
    /* initialize list */
    *ctx = list->new ();
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list** ctx = &rx->ctx;
    /* gets the current memory pointer */
    struct linked_list* ptr = *ctx;
    /* cleans up */
    list->delete (&ptr);
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

/* test init */
RX_TEST_CASE(tests, test_list_enumerator_new_0, .fixture = test_fixture) {
    const struct linked_list_enumerator* list_enumerator = enumerator->new (0);

    RX_ASSERT(list_enumerator == 0);
}

RX_TEST_CASE(tests, test_list_new_0, .fixture = test_fixture) {
    struct linked_list* list_new = list->new ();

    RX_ASSERT(list_new != 0);

    list->delete (&list_new);
}

/* test init */
RX_TEST_CASE(tests, test_list_append_head_0_count, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;

    struct linked_list* ctx = rx->ctx;

    list->append_head(ctx, 0);

    u64 count = list->count(ctx);

    RX_ASSERT(count == 0);
}

/* test init */
RX_TEST_CASE(tests, test_list_data, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    const struct linked_list_node* node = enumerator->next(list_enumerator);

    RX_ASSERT(node == 0);

    enumerator->delete (&list_enumerator);
}

/* test init */
RX_TEST_CASE(tests, test_list_count, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    u64 count = list->count(ctx);

    RX_ASSERT(count == 0);
}

/* test init */
RX_TEST_CASE(tests, test_list_head, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    const struct linked_list_node* node = list->head(ctx);

    RX_ASSERT(node == 0);
}

/* test init */
RX_TEST_CASE(tests, test_list_tail, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    const struct linked_list_node* node = list->tail(ctx);

    RX_ASSERT(node == 0);
}

/* test init */
RX_TEST_CASE(tests, test_list_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    u64 i = 0;

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    RX_ASSERT(enumerator->next(list_enumerator) == 0);

    enumerator->delete (&list_enumerator);

    RX_ASSERT(i == 0);
}

/* test init */
RX_TEST_CASE(tests, test_list_0_reverse, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->reverse(ctx);

    u64 i = 0;

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    RX_ASSERT(enumerator->next(list_enumerator) == 0);

    enumerator->delete (&list_enumerator);

    RX_ASSERT(i == 0);
}

/* test init */
RX_TEST_CASE(tests, test_list_reverse_until_0, .fixture = test_fixture) {
    list->reverse_until(0, 0);

    RX_ASSERT(0 == 0);
}

/* test init */
RX_TEST_CASE(tests, test_list_reverse_until, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->reverse_until(ctx, 0);

    RX_ASSERT(0 == 0);
}

/* test init */
RX_TEST_CASE(tests, test_list_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    for (u64 i = 0; i < 1; i++) {
        list->append_head(ctx, (void*)(i + 1));
    }

    u64 i = 0;

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    struct linked_list_node* node = 0;
    while ((node = enumerator->next(list_enumerator)) != 0) {
        RX_ASSERT((u64)list->data(node) == (1 - i));
        i++;
    }
    enumerator->delete (&list_enumerator);

    RX_ASSERT(i == 1);
}

/* test init */
RX_TEST_CASE(tests, test_list_1_reverse, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    for (u64 i = 0; i < 1; i++) {
        list->append_head(ctx, (void*)(i + 1));
    }

    list->reverse(ctx);

    u64 i = 0;

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    struct linked_list_node* node = 0;
    while ((node = enumerator->next(list_enumerator)) != 0) {
        RX_ASSERT((u64)list->data(node) == (i + 1));
        i++;
    }
    enumerator->delete (&list_enumerator);

    RX_ASSERT(i == 1);
}

/* test init */
RX_TEST_CASE(tests, test_list_12_reverse, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->append_head(ctx, (void*)(2));
    list->append_head(ctx, (void*)(1));

    list->reverse(ctx);

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    struct linked_list_node* node = 0;

    node = enumerator->next(list_enumerator);
    RX_ASSERT((u64)list->data(node) == 2);

    node = enumerator->next(list_enumerator);
    RX_ASSERT((u64)list->data(node) == 1);

    enumerator->delete (&list_enumerator);
}

/* test init */
RX_TEST_CASE(tests, test_list_123_reverse, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->append_head(ctx, (void*)(3));
    list->append_head(ctx, (void*)(2));
    list->append_head(ctx, (void*)(1));

    list->reverse(ctx);

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    struct linked_list_node* node = 0;

    node = enumerator->next(list_enumerator);
    RX_ASSERT((u64)list->data(node) == 3);

    node = enumerator->next(list_enumerator);
    RX_ASSERT((u64)list->data(node) == 2);

    node = enumerator->next(list_enumerator);
    RX_ASSERT((u64)list->data(node) == 1);

    enumerator->delete (&list_enumerator);
}

/* test init */
RX_TEST_CASE(tests, test_list_21_reverse, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    for (u64 i = 0; i < 2; i++) {
        list->append_head(ctx, (void*)(i + 1));
    }

    list->reverse(ctx);

    u64 i = 0;

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    struct linked_list_node* node = 0;
    while ((node = enumerator->next(list_enumerator)) != 0) {
        RX_ASSERT((u64)list->data(node) == (i + 1));
        i++;
    }
    enumerator->delete (&list_enumerator);

    RX_ASSERT(i == 2);
}

/* test init */
RX_TEST_CASE(tests, test_list_321_reverse, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    for (u64 i = 0; i < 3; i++) {
        list->append_head(ctx, (void*)(i + 1));
    }

    list->reverse(ctx);

    u64 i = 0;

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    struct linked_list_node* node = 0;
    while ((node = enumerator->next(list_enumerator)) != 0) {
        RX_ASSERT((u64)list->data(node) == (i + 1));
        i++;
    }
    enumerator->delete (&list_enumerator);

    RX_ASSERT(i == 3);
}

/* test init */
RX_TEST_CASE(tests, test_list_10, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    for (u64 i = 0; i < 10; i++) {
        list->append_head(ctx, (void*)(i + 1));
    }

    u64 i = 0;

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    struct linked_list_node* node = 0;
    while ((node = enumerator->next(list_enumerator)) != 0) {
        RX_ASSERT((u64)list->data(node) == (10 - i));
        i++;
    }
    enumerator->delete (&list_enumerator);

    RX_ASSERT(i == 10);
}

/* test init */
RX_TEST_CASE(tests, test_list_append_tail_10, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    u64 i = 0;

    list->append_tail(ctx, (void*)(i++ + 1));

    const struct linked_list_node* tail = list->tail(ctx);

    for (; i < 10; i++) {
        list->append_tail(ctx, (void*)(i + 1));
    }

    const struct linked_list_node* head = list->head(ctx);

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    struct linked_list_node* node = 0;

    i = 0;
    while ((node = enumerator->next(list_enumerator)) != 0) {
        RX_ASSERT((u64)list->data(node) == (i + 1));
        i++;
    }
    enumerator->delete (&list_enumerator);

    RX_ASSERT(i == 10);

    RX_ASSERT(list->head(ctx) == head);
    RX_ASSERT(list->tail(ctx) != tail);
}

/* test init */
RX_TEST_CASE(tests, test_list_append_head_10, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    u64 i = 0;

    list->append_head(ctx, (void*)(i++ + 1));

    const struct linked_list_node* tail = list->head(ctx);

    for (; i < 10; i++) {
        list->append_head(ctx, (void*)(i + 1));
    }

    const struct linked_list_node* head = list->tail(ctx);

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    struct linked_list_node* node = 0;

    i = 0;
    while ((node = enumerator->next(list_enumerator)) != 0) {
        RX_ASSERT((u64)list->data(node) == (10 - i));
        i++;
    }
    enumerator->delete (&list_enumerator);

    RX_ASSERT(i == 10);

    RX_ASSERT(list->head(ctx) != head);
    RX_ASSERT(list->tail(ctx) == tail);
}

/* test init */
RX_TEST_CASE(tests, test_list_10_reverse, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    for (u64 i = 0; i < 10; i++) {
        list->append_head(ctx, (void*)(i + 1));
    }

    const struct linked_list_node* head = list->head(ctx);
    const struct linked_list_node* tail = list->tail(ctx);

    list->reverse(ctx);

    u64 i = 0;

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    struct linked_list_node* node = 0;
    while ((node = enumerator->next(list_enumerator)) != 0) {
        RX_ASSERT((u64)list->data(node) == (i + 1));
        i++;
    }
    enumerator->delete (&list_enumerator);

    RX_ASSERT(i == 10);

    RX_ASSERT(list->head(ctx) == tail);
    RX_ASSERT(list->tail(ctx) == head);
}

/* test init */
RX_TEST_CASE(tests, test_list_10_reverse_reverse, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    for (u64 i = 0; i < 10; i++) {
        list->append_head(ctx, (void*)(i + 1));
    }

    const struct linked_list_node* head = list->head(ctx);
    const struct linked_list_node* tail = list->tail(ctx);

    list->reverse(ctx);
    list->reverse(ctx);

    u64 i = 0;

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    struct linked_list_node* node = 0;
    while ((node = enumerator->next(list_enumerator)) != 0) {
        RX_ASSERT((u64)list->data(node) == (10 - i));
        i++;
    }
    enumerator->delete (&list_enumerator);

    RX_ASSERT(i == 10);

    RX_ASSERT(list->head(ctx) == head);
    RX_ASSERT(list->tail(ctx) == tail);
}

/* test init */
RX_TEST_CASE(tests, test_list_enumerator_delete, .fixture = test_fixture) {
    enumerator->delete (0);

    RX_ASSERT(0 == 0);
}

/* test init */
RX_TEST_CASE(tests, test_list_enumerator_next, .fixture = test_fixture) {
    const void* result = enumerator->next(0);

    RX_ASSERT(result == 0);
}

/* test init */
RX_TEST_CASE(tests, test_list_reverse_list_0, .fixture = test_fixture) {
    list->reverse(0);

    RX_ASSERT(0 == 0);
}

/* test init */
RX_TEST_CASE(tests, test_list_append_head_0, .fixture = test_fixture) {
    list->append_head(0, 0);

    RX_ASSERT(0 == 0);
}

/* test init */
RX_TEST_CASE(tests, test_list_append_head_ctx_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    const struct linked_list_node* head = list->head(ctx);
    const struct linked_list_node* tail = list->tail(ctx);

    list->append_head(ctx, 0);

    RX_ASSERT(list->head(ctx) == head);
    RX_ASSERT(list->tail(ctx) == tail);
}

/* test init */
RX_TEST_CASE(tests, test_list_append_tail_0, .fixture = test_fixture) {
    list->append_tail(0, 0);

    RX_ASSERT(0 == 0);
}

/* test init */
RX_TEST_CASE(tests, test_list_head_ctx_0, .fixture = test_fixture) {
    const void* data = list->head(0);

    RX_ASSERT(data == 0);
}

/* test init */
RX_TEST_CASE(tests, test_list_head_ctx, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    const struct linked_list_node* head = list->head(ctx);
    const struct linked_list_node* tail = list->tail(ctx);

    const void* data = list->head(ctx);

    RX_ASSERT(data == 0);

    RX_ASSERT(list->head(ctx) == head);
    RX_ASSERT(list->tail(ctx) == tail);
}

/* test init */
RX_TEST_CASE(tests, test_list_tail_ctx_0, .fixture = test_fixture) {
    const void* data = list->tail(0);

    RX_ASSERT(data == 0);
}

/* test init */
RX_TEST_CASE(tests, test_list_next_ctx_0, .fixture = test_fixture) {
    const void* data = list->next(0);

    RX_ASSERT(data == 0);
}

/* test init */
RX_TEST_CASE(tests, test_list_count_0, .fixture = test_fixture) {
    u64 count = list->count(0);

    RX_ASSERT(count == 0);
}

/* test init */
RX_TEST_CASE(tests, test_list_delete_0, .fixture = test_fixture) {
    list->delete (0);

    RX_ASSERT(0 == 0);
}

/* test init */
RX_TEST_CASE(tests, test_list_data_0, .fixture = test_fixture) {
    const void* data = list->data(0);

    RX_ASSERT(data == 0);
}

/* test init */
RX_TEST_CASE(tests, test_list_tail_ctx, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    const struct linked_list_node* head = list->head(ctx);
    const struct linked_list_node* tail = list->tail(ctx);

    const void* data = list->tail(ctx);

    RX_ASSERT(data == 0);

    RX_ASSERT(list->head(ctx) == head);
    RX_ASSERT(list->tail(ctx) == tail);
}

/* test init */
RX_TEST_CASE(tests, test_list_append_tail_ctx_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    const struct linked_list_node* head = list->head(ctx);
    const struct linked_list_node* tail = list->tail(ctx);

    list->append_tail(ctx, 0);

    RX_ASSERT(list->head(ctx) == head);
    RX_ASSERT(list->tail(ctx) == tail);
}

/* test init */
RX_TEST_CASE(tests, test_list_1_reverse_until_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->append_head(ctx, (void*)(1));

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    struct linked_list_node* node = enumerator->find(list_enumerator, (void*)1);
    enumerator->delete (&list_enumerator);

    const struct linked_list_node* head = list->head(ctx);
    const struct linked_list_node* tail = list->tail(ctx);

    list->reverse_until(ctx, node);

    RX_ASSERT(list->head(ctx) == head);
    RX_ASSERT(list->tail(ctx) == tail);
}

/* test init */
RX_TEST_CASE(tests, test_list_12_reverse_until_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->append_head(ctx, (void*)(2));
    list->append_head(ctx, (void*)(1));

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    struct linked_list_node* node = enumerator->find(list_enumerator, (void*)1);
    enumerator->delete (&list_enumerator);

    const struct linked_list_node* head = list->head(ctx);
    const struct linked_list_node* tail = list->tail(ctx);

    list->reverse_until(ctx, node);

    RX_ASSERT(list->head(ctx) == head);
    RX_ASSERT(list->tail(ctx) == tail);
}

/* test init */
RX_TEST_CASE(tests, test_list_12_reverse_until_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->append_head(ctx, (void*)(2));
    list->append_head(ctx, (void*)(1));

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    struct linked_list_node* node = enumerator->find(list_enumerator, (void*)2);
    enumerator->delete (&list_enumerator);

    const struct linked_list_node* head = list->head(ctx);
    const struct linked_list_node* tail = list->tail(ctx);

    list->reverse_until(ctx, node);

    RX_ASSERT(list->head(ctx) == tail);
    RX_ASSERT(list->tail(ctx) == head);
}

/* test init */
RX_TEST_CASE(tests, test_list_1_reverse_until_2, .fixture = test_fixture) {
    struct linked_list* list_new = list->new ();

    list->append_head(list_new, (void*)(1));

    struct linked_list_enumerator* list_enumerator = enumerator->new (list_new);
    struct linked_list_node* another_list_node = enumerator->find(list_enumerator, (void*)1);
    enumerator->delete (&list_enumerator);

    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->append_head(ctx, (void*)(1));

    const struct linked_list_node* head = list->head(ctx);
    const struct linked_list_node* tail = list->tail(ctx);

    list->reverse_until(ctx, another_list_node);

    RX_ASSERT(list_new != 0);

    list->delete (&list_new);

    RX_ASSERT(list->head(ctx) == tail);
    RX_ASSERT(list->tail(ctx) == head);
}

/* test init */
RX_TEST_CASE(tests, test_list_123_reverse_until_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->append_head(ctx, (void*)(3));
    list->append_head(ctx, (void*)(2));
    list->append_head(ctx, (void*)(1));

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    struct linked_list_node* node = enumerator->find(list_enumerator, (void*)1);
    enumerator->delete (&list_enumerator);

    list->reverse_until(ctx, node);

    RX_ASSERT(list->head(ctx) == list->head(ctx));
}

/* test init */
RX_TEST_CASE(tests, test_list_123_reverse_until_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->append_head(ctx, (void*)(3));
    list->append_head(ctx, (void*)(2));

    const struct linked_list_node* tail = list->head(ctx);

    list->append_head(ctx, (void*)(1));

    const struct linked_list_node* head = list->tail(ctx);

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    struct linked_list_node* node = enumerator->find(list_enumerator, (void*)2);
    enumerator->delete (&list_enumerator);

    list->reverse_until(ctx, node);

    RX_ASSERT(list->head(ctx) == tail);
    RX_ASSERT(list->tail(ctx) == head);
}

/* test init */
RX_TEST_CASE(tests, test_list_123_reverse_until_3, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->append_head(ctx, (void*)(3));
    list->append_head(ctx, (void*)(2));
    list->append_head(ctx, (void*)(1));

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    struct linked_list_node* node = enumerator->find(list_enumerator, (void*)3);
    enumerator->delete (&list_enumerator);

    list->reverse_until(ctx, node);

    RX_ASSERT(list->tail(ctx) == list->tail(ctx));
}

/* test init */
RX_TEST_CASE(tests, test_list_1234_reverse_until_4, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->append_head(ctx, (void*)(4));
    list->append_head(ctx, (void*)(3));
    list->append_head(ctx, (void*)(2));
    list->append_head(ctx, (void*)(1));

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    struct linked_list_node* node = enumerator->find(list_enumerator, (void*)4);
    enumerator->delete (&list_enumerator);

    list->reverse_until(ctx, node);

    RX_ASSERT(list->head(ctx) == node);
}

/* test init */
RX_TEST_CASE(tests, test_list_1234_reverse_until_3, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->append_head(ctx, (void*)(4));
    list->append_head(ctx, (void*)(3));

    const struct linked_list_node* tail = list->head(ctx);

    list->append_head(ctx, (void*)(2));
    list->append_head(ctx, (void*)(1));

    const struct linked_list_node* head = list->tail(ctx);

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    struct linked_list_node* node = enumerator->find(list_enumerator, (void*)3);
    enumerator->delete (&list_enumerator);

    list->reverse_until(ctx, node);

    RX_ASSERT(list->head(ctx) == tail);
    RX_ASSERT(list->tail(ctx) == head);
}

/* test init */
RX_TEST_CASE(tests, test_list_set_next_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->append_tail(ctx, (void*)(2));
    list->append_tail(ctx, (void*)(1));

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    struct linked_list_node* node = enumerator->find(list_enumerator, (void*)2);
    struct linked_list_node* node_prev = enumerator->find_prev(list_enumerator, (void*)2);
    list->set_next(node, node_prev);
    enumerator->delete (&list_enumerator);

    RX_ASSERT(list->next(node) == node_prev);
}

/* test init */
RX_TEST_CASE(tests, test_list_set_next_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->append_tail(ctx, (void*)(2));
    list->append_tail(ctx, (void*)(1));

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    struct linked_list_node* node = enumerator->find(list_enumerator, (void*)2);
    const struct linked_list_node* node_prev = enumerator->find_prev(list_enumerator, (void*)2);
    list->set_next(0, 0);
    enumerator->delete (&list_enumerator);

    RX_ASSERT(list->next(node) == node_prev);
}

/* test init */
RX_TEST_CASE(tests, test_list_set_next_node, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->append_tail(ctx, (void*)(2));
    list->append_tail(ctx, (void*)(1));

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    struct linked_list_node* node = enumerator->find(list_enumerator, (void*)2);
    struct linked_list_node* node_prev = enumerator->find_prev(list_enumerator, (void*)2);
    list->set_next(0, node_prev);
    enumerator->delete (&list_enumerator);

    RX_ASSERT(list->next(node) == node_prev);
}

/* test init */
RX_TEST_CASE(tests, test_list_set, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->append_tail(ctx, (void*)(2));
    list->append_tail(ctx, (void*)(1));

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    struct linked_list_node* node = enumerator->find(list_enumerator, (void*)2);
    struct linked_list_node* node_prev = enumerator->find_prev(list_enumerator, (void*)2);
    list->set(ctx, node, node_prev);
    enumerator->delete (&list_enumerator);

    RX_ASSERT(list->head(ctx) == node);
    RX_ASSERT(list->tail(ctx) == node_prev);
}

/* test init */
RX_TEST_CASE(tests, test_list_set_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->append_tail(ctx, (void*)(2));
    list->append_tail(ctx, (void*)(1));

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    struct linked_list_node* node = enumerator->find(list_enumerator, (void*)2);
    struct linked_list_node* node_prev = enumerator->find_prev(list_enumerator, (void*)2);
    list->set(ctx, node_prev, 0);
    list->set(ctx, 0, node);
    list->set(0, node_prev, node);
    enumerator->delete (&list_enumerator);

    RX_ASSERT(list->head(ctx) == node);
    RX_ASSERT(list->tail(ctx) == node_prev);
}

static struct linked_list_node* context = 0;

static u64 match_function(struct linked_list_node* current) {
    return current == context;
}

/* test init */
RX_TEST_CASE(tests, test_list_1234_reverse_until_match_3, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->append_head(ctx, (void*)(4));
    list->append_head(ctx, (void*)(3));

    const struct linked_list_node* tail = list->head(ctx);

    list->append_head(ctx, (void*)(2));
    list->append_head(ctx, (void*)(1));

    const struct linked_list_node* head = list->tail(ctx);

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    struct linked_list_node* node = enumerator->find(list_enumerator, (void*)3);
    context = node;

    enumerator->delete (&list_enumerator);

    list->reverse_until_match(ctx, match_function);

    RX_ASSERT(list->head(ctx) == tail);
    RX_ASSERT(list->tail(ctx) == head);
}

/* test init */
RX_TEST_CASE(tests, test_list_reverse_until_match_0, .fixture = test_fixture) {
    list->reverse_until_match(0, 0);
}

/* test init */
RX_TEST_CASE(tests, test_list_reverse_until_match_function_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->reverse_until_match(ctx, 0);
}

/* test init */
RX_TEST_CASE(tests, test_list_reverse_until_match, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    const struct linked_list_node* tail = list->head(ctx);
    const struct linked_list_node* head = list->tail(ctx);

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    struct linked_list_node* node = enumerator->find(list_enumerator, (void*)3);
    context = node;

    enumerator->delete (&list_enumerator);

    list->reverse_until_match(ctx, match_function);

    RX_ASSERT(list->head(ctx) == tail);
    RX_ASSERT(list->tail(ctx) == head);
}

/* test init */
RX_TEST_CASE(tests, test_list_1234_reverse_until_match_function_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->append_head(ctx, (void*)(1));

    const struct linked_list_node* tail = list->head(ctx);
    const struct linked_list_node* head = list->tail(ctx);

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    struct linked_list_node* node = enumerator->find(list_enumerator, (void*)1);
    context = node;

    enumerator->delete (&list_enumerator);

    list->reverse_until_match(ctx, match_function);

    RX_ASSERT(list->head(ctx) == tail);
    RX_ASSERT(list->tail(ctx) == head);
}

/* test init */
RX_TEST_CASE(tests, test_list_1234_reverse_until_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->append_head(ctx, (void*)(4));
    list->append_head(ctx, (void*)(3));
    list->append_head(ctx, (void*)(2));
    list->append_head(ctx, (void*)(1));

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    struct linked_list_node* node = enumerator->find(list_enumerator, (void*)2);
    enumerator->delete (&list_enumerator);

    list->reverse_until(ctx, node);

    RX_ASSERT(list->head(ctx) == node);
}

/* test init */
RX_TEST_CASE(tests, test_list_1234_reverse_until_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->append_head(ctx, (void*)(4));
    list->append_head(ctx, (void*)(3));
    list->append_head(ctx, (void*)(2));
    list->append_head(ctx, (void*)(1));

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    struct linked_list_node* node = enumerator->find(list_enumerator, (void*)1);
    enumerator->delete (&list_enumerator);

    list->reverse_until(ctx, node);

    RX_ASSERT(list->head(ctx) == node);
}

/* test init */
RX_TEST_CASE(tests, test_list_12345_reverse_until_5, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->append_head(ctx, (void*)(5));
    list->append_head(ctx, (void*)(4));
    list->append_head(ctx, (void*)(3));
    list->append_head(ctx, (void*)(2));
    list->append_head(ctx, (void*)(1));

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    struct linked_list_node* node = enumerator->find(list_enumerator, (void*)5);
    enumerator->delete (&list_enumerator);

    list->reverse_until(ctx, node);

    RX_ASSERT(list->head(ctx) == node);
}

/* test init */
RX_TEST_CASE(tests, test_list_12345_enumerator_reverse, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->append_head(ctx, (void*)(5));
    list->append_head(ctx, (void*)(4));
    list->append_head(ctx, (void*)(3));
    list->append_head(ctx, (void*)(2));
    list->append_head(ctx, (void*)(1));

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    const struct linked_list_node* node = enumerator->find(list_enumerator, (void*)5);
    enumerator->reverse(list_enumerator);
    enumerator->delete (&list_enumerator);

    RX_ASSERT(list->head(ctx) == node);
}

/* test init */
RX_TEST_CASE(tests, test_list_12345_reverse_until_4, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->append_head(ctx, (void*)(5));
    list->append_head(ctx, (void*)(4));
    list->append_head(ctx, (void*)(3));
    list->append_head(ctx, (void*)(2));
    list->append_head(ctx, (void*)(1));

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    struct linked_list_node* node = enumerator->find(list_enumerator, (void*)4);
    enumerator->delete (&list_enumerator);

    list->reverse_until(ctx, node);

    RX_ASSERT(list->head(ctx) == node);
}

/* test init */
RX_TEST_CASE(tests, test_list_12345_reverse_until_3, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->append_head(ctx, (void*)(5));
    list->append_head(ctx, (void*)(4));
    list->append_head(ctx, (void*)(3));
    list->append_head(ctx, (void*)(2));
    list->append_head(ctx, (void*)(1));

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    struct linked_list_node* node = enumerator->find(list_enumerator, (void*)3);
    enumerator->delete (&list_enumerator);

    list->reverse_until(ctx, node);

    RX_ASSERT(list->head(ctx) == node);
}
/* test init */
RX_TEST_CASE(tests, test_list_12345_reverse_until_prev_3, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->append_head(ctx, (void*)(5));
    list->append_head(ctx, (void*)(4));
    list->append_head(ctx, (void*)(3));
    list->append_head(ctx, (void*)(2));
    list->append_head(ctx, (void*)(1));

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    struct linked_list_node* node = enumerator->find_prev(list_enumerator, (void*)4);
    enumerator->delete (&list_enumerator);

    list->reverse_until(ctx, node);

    RX_ASSERT(list->head(ctx) == node);
}

/* test init */
RX_TEST_CASE(tests, test_list_enumerator_reverse, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    enumerator->reverse(list_enumerator);
    enumerator->delete (&list_enumerator);

    RX_ASSERT(list->head(ctx) == 0);
}

/* test init */
RX_TEST_CASE(tests, test_list_enumerator_reverse_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->append_head(ctx, (void*)(1));

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    enumerator->reverse(0);
    enumerator->delete (&list_enumerator);

    RX_ASSERT(list->head(ctx) != 0);
}

/* test init */
RX_TEST_CASE(tests, test_list_0_find_prev_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    const struct linked_list_node* node = enumerator->find_prev(list_enumerator, (void*)1);
    enumerator->delete (&list_enumerator);

    RX_ASSERT(0 == node);
}

/* test init */
RX_TEST_CASE(tests, test_list_1_find_prev_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->append_head(ctx, (void*)(1));

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    const struct linked_list_node* node = enumerator->find_prev(list_enumerator, (void*)0);
    enumerator->delete (&list_enumerator);

    RX_ASSERT(0 == node);
}

/* test init */
RX_TEST_CASE(tests, test_list_1_find_prev_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->append_head(ctx, (void*)(1));

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    const struct linked_list_node* node = enumerator->find_prev(list_enumerator, (void*)1);
    enumerator->delete (&list_enumerator);

    RX_ASSERT(0 == node);
}

/* test init */
RX_TEST_CASE(tests, test_list_12345_reverse_until_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->append_head(ctx, (void*)(5));
    list->append_head(ctx, (void*)(4));
    list->append_head(ctx, (void*)(3));
    list->append_head(ctx, (void*)(2));
    list->append_head(ctx, (void*)(1));

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    struct linked_list_node* node = enumerator->find(list_enumerator, (void*)2);
    enumerator->delete (&list_enumerator);

    list->reverse_until(ctx, node);

    RX_ASSERT(list->head(ctx) == node);
}

/* test init */
RX_TEST_CASE(tests, test_list_12345_reverse_until_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->append_head(ctx, (void*)(5));
    list->append_head(ctx, (void*)(4));
    list->append_head(ctx, (void*)(3));
    list->append_head(ctx, (void*)(2));
    list->append_head(ctx, (void*)(1));

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    struct linked_list_node* node = enumerator->find(list_enumerator, (void*)1);
    enumerator->delete (&list_enumerator);

    list->reverse_until(ctx, node);

    RX_ASSERT(list->head(ctx) == node);
}

/* test init */
RX_TEST_CASE(tests, test_list_reverse_until_4, .fixture = test_fixture) {
    struct linked_list* list_new = list->new ();

    list->append_head(list_new, (void*)(4));

    struct linked_list_enumerator* list_enumerator = enumerator->new (list_new);
    struct linked_list_node* node = enumerator->find(list_enumerator, (void*)4);
    enumerator->delete (&list_enumerator);

    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->reverse_until(ctx, node);

    RX_ASSERT(list_new != 0);

    list->delete (&list_new);
}

/* test init */
RX_TEST_CASE(tests, test_list_123_reverse_until_out_4, .fixture = test_fixture) {
    struct linked_list* list_new = list->new ();

    list->append_head(list_new, (void*)(4));
    struct linked_list_enumerator* list_enumerator_new = enumerator->new (list_new);
    struct linked_list_node* another_list_node = enumerator->find(list_enumerator_new, (void*)4);
    enumerator->delete (&list_enumerator_new);

    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->append_head(ctx, (void*)(3));
    list->append_head(ctx, (void*)(2));
    list->append_head(ctx, (void*)(1));

    list->reverse_until(ctx, another_list_node);

    RX_ASSERT(list->head(ctx) != another_list_node);

    RX_ASSERT(list_new != 0);

    list->delete (&list_new);
}

/* test init */
RX_TEST_CASE(tests, test_list_enumerator_find, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->append_head(ctx, (void*)(3));
    list->append_head(ctx, (void*)(2));
    list->append_head(ctx, (void*)(1));

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    const struct linked_list_node* node = enumerator->find(0, (void*)0);
    enumerator->delete (&list_enumerator);

    RX_ASSERT(0 == node);
}

/* test init */
RX_TEST_CASE(tests, test_list_enumerator_find_list, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    const struct linked_list_node* node = enumerator->find(list_enumerator, (void*)0);
    enumerator->delete (&list_enumerator);

    RX_ASSERT(0 == node);
}

/* test init */
RX_TEST_CASE(tests, test_list_enumerator_find_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->append_head(ctx, (void*)(1));

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    const struct linked_list_node* node = enumerator->find(list_enumerator, (void*)0);
    enumerator->delete (&list_enumerator);

    RX_ASSERT(0 == node);
}

int main(void) {
    /* Execute the main function that runs the test cases found. */
    int result = rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;
    return result;
}
