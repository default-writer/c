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

const struct hashtable* hashtable = &hashtable_definition;

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
    hashtable->init(HASHTABLE_SIZE);
    hashtable->destroy();
    RX_ASSERT(0 == 0);
}

// test init
RX_TEST_CASE(myTestSuite, test_hashtable_alloc_free, .fixture = test_fixture) {
    hashtable->init(HASHTABLE_SIZE);
    char* key = calloc(1, sizeof(char) + 1);
    char* value = calloc(1, sizeof(char) + 1);
    memcpy(key, "1", 2); // NOLINT
    memcpy(value, "a", 2); // NOLINT
    struct hashtable_data* tmp = hashtable->alloc(key, value);
    hashtable->free(tmp);
    hashtable->destroy();
    free(key);
    free(value);
    RX_ASSERT(0 == 0);
}

// test init
RX_TEST_CASE(myTestSuite, test_hashtable_alloc, .fixture = test_fixture) {
    hashtable->init(HASHTABLE_SIZE);
    char* key = calloc(1, sizeof(char) + 1);
    char* value = calloc(1, sizeof(char) + 1);
    memcpy(key, "1", 2); // NOLINT
    memcpy(value, "a", 2); // NOLINT
    hashtable->alloc(key, value); // case when user forget to free, we care about our users, really
    hashtable->destroy();
    free(key);
    free(value);
    RX_ASSERT(0 == 0);
}

// test init
RX_TEST_CASE(myTestSuite, test_hashtable_alloc_alloc, .fixture = test_fixture) {
    hashtable->init(HASHTABLE_SIZE);
    char* key = calloc(1, sizeof(char) + 1);
    char* value = calloc(1, sizeof(char) + 1);
    memcpy(key, "1", 2); // NOLINT
    memcpy(value, "a", 2); // NOLINT
    hashtable->alloc(key, value);
    hashtable->alloc(key, value);
    hashtable->destroy();
    free(key);
    free(value);
    RX_ASSERT(0 == 0);
}

// test init
RX_TEST_CASE(myTestSuite, test_hashtable_alloc_alloc_alloc, .fixture = test_fixture) {
    hashtable->init(HASHTABLE_SIZE);
    char* key = calloc(1, sizeof(char) + 1);
    char* value = calloc(1, sizeof(char) + 1);
    memcpy(key, "1", 2); // NOLINT
    memcpy(value, "a", 2); // NOLINT
    hashtable->alloc(key, value);
    hashtable->alloc(key, value);
    hashtable->alloc(key, value);
    hashtable->destroy();
    free(key);
    free(value);
    RX_ASSERT(0 == 0);
}

// test init
RX_TEST_CASE(myTestSuite, test_hashtable_alloc_free_alloc, .fixture = test_fixture) {
    hashtable->init(HASHTABLE_SIZE);
    char* key = calloc(1, sizeof(char) + 1);
    char* value = calloc(1, sizeof(char) + 1);
    memcpy(key, "1", 2); // NOLINT
    memcpy(value, "a", 2); // NOLINT
    struct hashtable_data* temp = hashtable->alloc(key, value);
    hashtable->free(temp);
    hashtable->alloc(key, value);
    hashtable->destroy();
    free(key);
    free(value);
    RX_ASSERT(0 == 0);
}

// test init
RX_TEST_CASE(myTestSuite, test_hashtable_alloc_alloc_free, .fixture = test_fixture) {
    hashtable->init(HASHTABLE_SIZE);
    char* key = calloc(1, sizeof(char) + 1);
    char* value = calloc(1, sizeof(char) + 1);
    memcpy(key, "1", 2); // NOLINT
    memcpy(value, "a", 2); // NOLINT
    struct hashtable_data* temp = hashtable->alloc(key, value);
    hashtable->alloc(key, value);
    hashtable->free(temp);
    hashtable->destroy();
    free(key);
    free(value);
    RX_ASSERT(0 == 0);
}

// test init
RX_TEST_CASE(myTestSuite, test_hashtable_alloc_free_alloc_free, .fixture = test_fixture) {
    hashtable->init(HASHTABLE_SIZE);
    char* key = calloc(1, sizeof(char) + 1);
    char* value = calloc(1, sizeof(char) + 1);
    memcpy(key, "1", 2); // NOLINT
    memcpy(value, "a", 2); // NOLINT
    struct hashtable_data* temp = hashtable->alloc(key, value);
    hashtable->free(temp);
    temp = hashtable->alloc(key, value);
    hashtable->free(temp);
    hashtable->destroy();
    free(key);
    free(value);
    RX_ASSERT(0 == 0);
}

// test init
RX_TEST_CASE(myTestSuite, test_hashtable_alloc_alloc_temp_alloc_free_temp_alloc_alloc, .fixture = test_fixture) {
    hashtable->init(HASHTABLE_SIZE);
    char* key = calloc(1, sizeof(char) + 1);
    char* value = calloc(1, sizeof(char) + 1);
    memcpy(key, "1", 2); // NOLINT
    memcpy(value, "a", 2); // NOLINT
    hashtable->alloc(key, value);
    struct hashtable_data* temp = hashtable->alloc(key, value);
    hashtable->alloc(key, value);
    hashtable->free(temp);
    hashtable->alloc(key, value);
    hashtable->alloc(key, value);
    hashtable->destroy();
    free(key);
    free(value);
    RX_ASSERT(0 == 0);
}

// test init
RX_TEST_CASE(myTestSuite, test_hashtable_alloc_alloc_alloc_temp_alloc_alloc_free_temp, .fixture = test_fixture) {
    hashtable->init(HASHTABLE_SIZE);
    char* key1 = calloc(1, sizeof(char) + 1);
    char* key2 = calloc(1, sizeof(char) + 1);
    char* key3 = calloc(1, sizeof(char) + 1);
    char* key4 = calloc(1, sizeof(char) + 1);
    char* key5 = calloc(1, sizeof(char) + 1);
    char* value = calloc(1, sizeof(char) + 1);
    memcpy(key1, "1", 2); // NOLINT
    memcpy(key2, "1", 2); // NOLINT
    memcpy(key3, "1", 2); // NOLINT
    memcpy(key4, "1", 2); // NOLINT
    memcpy(key5, "1", 2); // NOLINT
    memcpy(value, "a", 2); // NOLINT
    hashtable->alloc(key1, value);
    hashtable->alloc(key2, value);
    struct hashtable_data* temp = hashtable->alloc(key3, value);
    RX_ASSERT(temp != 0);
    struct hashtable_data* values[2] = {
        hashtable->alloc(key4, value),
        hashtable->alloc(key5, value)
    };
    RX_ASSERT(values[0] != 0);
    RX_ASSERT(values[1] != 0);
    RX_ASSERT(values[0] != values[1]);
    hashtable->free(temp);
    // we need to be sure that values are not erased in hashtable list during freeing element
    struct hashtable_data* value0 = hashtable->find(values[0]->name);
    struct hashtable_data* value1 = hashtable->find(values[1]->name);
    RX_ASSERT(value0 != 0);
    RX_ASSERT(value1 != 0);
    hashtable->destroy();
    free(key1);
    free(key2);
    free(key3);
    free(key4);
    free(key5);
    free(value);
    RX_ASSERT(0 == 0);
}

int main(int argc, char** argv) {
    CLEAN(argc)
    CLEAN(argv)
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- acceptance test code\n");
#endif
    hashtable->init(HASHTABLE_SIZE);
    hashtable->destroy();
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- rexo unit test code\n");
#endif
    /* Execute the main function that runs the test cases found. */
    return rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;
}
