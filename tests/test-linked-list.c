
#include "stl/linked_list.h"
#include <rexo/include/rexo.h>

/* definition */
extern const struct linked_list_methods linked_list_methods_definition;
static const struct linked_list_methods* list = &linked_list_methods_definition;

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    struct linked_list* ctx;
}* TEST_DATA;

/* list parameters definition */

/* private */

static struct linked_list* new_list(void);
static void delete_list(struct linked_list** ctx);

/* Initialize the data structure. Its allocation is handled by Rexo. */
RX_SET_UP(test_set_up) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list** ctx = &rx->ctx;
    /* initialize list */
    *ctx = new_list();
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list** ctx = &rx->ctx;
    /* gets the current memory pointer */
    struct linked_list* ptr = *ctx;
    /* cleans up */
    delete_list(&ptr);
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

/* test init */
RX_TEST_CASE(tests, test_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct linked_list** ctx = &rx->ctx;
    struct linked_list* node;

    node = *ctx;
    for (int i = 0; i < 10; i++) {
        struct linked_list* new_item = list->new ();
        new_item->data = node;
        node->next = new_item;
        node = node->next;
    }

    /* assign to ctx cause overwise it will be a memory leak */
    *ctx = list->reverse_list(*ctx);

    node = *ctx;
    for (int i = 0; i < 10; i++) {
        RX_ASSERT(node->data == node->next);
        node = node->next;
    }
}

/* allocates memory pointer for list object */
static struct linked_list* new_list(void) {
    /* returns list object */
    return calloc(1, sizeof(struct linked_list));
}

/* releases memory pointer for list object */
static void delete_list(struct linked_list** ctx) {
    list->delete (*ctx);
    /* cleans up */
    *ctx = 0;
}

int main(void) {
    /* Execute the main function that runs the test cases found. */
    int result = rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;
    return result;
}
