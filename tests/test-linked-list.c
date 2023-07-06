
#include "stl/linked_list.h"
#include <rexo/include/rexo.h>

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
RX_TEST_CASE(tests, test_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    u64 i = 0;

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    while (enumerator->next(list_enumerator) != 0) {
        i++;
    }
    enumerator->delete (&list_enumerator);

    RX_ASSERT(i == 0);
}

/* test init */
RX_TEST_CASE(tests, test_0_reverse, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    list->reverse_list(ctx);

    u64 i = 0;

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    while (enumerator->next(list_enumerator) != 0) {
        i++;
    }
    enumerator->delete (&list_enumerator);

    RX_ASSERT(i == 0);
}

/* test init */
RX_TEST_CASE(tests, test_1, .fixture = test_fixture) {
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
RX_TEST_CASE(tests, test_1_reverse, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    for (u64 i = 0; i < 1; i++) {
        list->append_head(ctx, (void*)(i + 1));
    }

    list->reverse_list(ctx);

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
RX_TEST_CASE(tests, test_2, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    for (u64 i = 0; i < 2; i++) {
        list->append_head(ctx, (void*)(i + 1));
    }

    u64 i = 0;

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    struct linked_list_node* node = 0;
    while ((node = enumerator->next(list_enumerator)) != 0) {
        RX_ASSERT((u64)list->data(node) == (2 - i));
        i++;
    }
    enumerator->delete (&list_enumerator);

    RX_ASSERT(i == 2);
}

/* test init */
RX_TEST_CASE(tests, test_2_reverse, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    for (u64 i = 0; i < 2; i++) {
        list->append_head(ctx, (void*)(i + 1));
    }

    list->reverse_list(ctx);

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
RX_TEST_CASE(tests, test_10, .fixture = test_fixture) {
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
RX_TEST_CASE(tests, test_10_reverse, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    for (u64 i = 0; i < 10; i++) {
        list->append_head(ctx, (void*)(i + 1));
    }

    list->reverse_list(ctx);

    u64 i = 0;

    struct linked_list_enumerator* list_enumerator = enumerator->new (ctx);
    struct linked_list_node* node = 0;
    while ((node = enumerator->next(list_enumerator)) != 0) {
        RX_ASSERT((u64)list->data(node) == (i + 1));
        i++;
    }
    enumerator->delete (&list_enumerator);

    RX_ASSERT(i == 10);
}

/* test init */
RX_TEST_CASE(tests, test_10_reverse_reverse, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list* ctx = rx->ctx;

    for (u64 i = 0; i < 10; i++) {
        list->append_head(ctx, (void*)(i + 1));
    }

    list->reverse_list(ctx);
    list->reverse_list(ctx);

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

int main(void) {
    /* Execute the main function that runs the test cases found. */
    int result = rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;
    return result;
}
