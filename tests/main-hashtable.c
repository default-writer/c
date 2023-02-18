#include "common/alloc.h"

#include "playground/brain/brain.h"
#include "playground/hashtable/hashtable.h"

#include "list-micro/data.h"
#include "playground/pointer/pointer.h"
#include "playground/virtual/vm.h"

#include "rexo/include/rexo.h"

#define HASHTABLE_SIZE 101

/* list definition */
extern const struct vm vm_definition;
extern const struct list list_micro_definition;
extern struct hashtable hashtable_definition;

const struct hashtable* ht = &hashtable_definition;

typedef struct test_data {
    struct pointer_data* ctx;
}* TEST_DATA;

RX_SET_UP(test_set_up) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct pointer_data** ctx = &rx->ctx;
    pointer_set(ctx);
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct pointer_data** ctx = &rx->ctx;
    pointer_get(ctx);
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

// test init
RX_TEST_CASE(myTestSuite, test_hashtable_init_destroy, .fixture = test_fixture) {
    ht->init(HASHTABLE_SIZE);
    ht->destroy();
    RX_ASSERT(0 == 0);
}

// test init
RX_TEST_CASE(myTestSuite, test_hashtable_alloc_free, .fixture = test_fixture) {
    ht->init(HASHTABLE_SIZE);
    char* key = calloc(1, sizeof(char) + 1);
    char* value = calloc(1, sizeof(char) + 1);
    memcpy(key, "1", 2); // NOLINT
    memcpy(value, "a", 2); // NOLINT
    struct hashtable_data* tmp = ht->alloc(key, value);
    ht->free(tmp);
    ht->destroy();
    free(key);
    free(value);
    RX_ASSERT(0 == 0);
}

// test init
RX_TEST_CASE(myTestSuite, test_hashtable_alloc, .fixture = test_fixture) {
    ht->init(HASHTABLE_SIZE);
    char* key = calloc(1, sizeof(char) + 1);
    char* value = calloc(1, sizeof(char) + 1);
    memcpy(key, "1", 2); // NOLINT
    memcpy(value, "a", 2); // NOLINT
    ht->alloc(key, value); // case when user forget to free, we care about our users, really
    ht->destroy();
    free(key);
    free(value);
    RX_ASSERT(0 == 0);
}

// test init
RX_TEST_CASE(myTestSuite, test_hashtable_alloc_alloc, .fixture = test_fixture) {
    ht->init(HASHTABLE_SIZE);
    char* key = calloc(1, sizeof(char) + 1);
    char* value = calloc(1, sizeof(char) + 1);
    memcpy(key, "1", 2); // NOLINT
    memcpy(value, "a", 2); // NOLINT
    ht->alloc(key, value); // case when user forget to free, we care about our users, really
    ht->alloc(key, value); // case when user forget to free, we care about our users, really
    ht->destroy();
    free(key);
    free(value);
    RX_ASSERT(0 == 0);
}

int main(int argc, char** argv) {
    CLEAN(argc)
    CLEAN(argv)
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- acceptance test code\n");
#endif
    ht->init(HASHTABLE_SIZE);
    ht->destroy();
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- rexo unit test code\n");
#endif
    /* Execute the main function that runs the test cases found. */
    return rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;
}
