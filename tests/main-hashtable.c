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
extern struct pointer_methods pointer_methods_definition;

const struct hashtable* hashtable = &hashtable_definition;
const struct pointer_methods* pointer = &pointer_methods_definition;

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
    char* key = calloc(6, sizeof(char));
    char* key1 = calloc(2, sizeof(char));
    char* key2 = calloc(2, sizeof(char));
    char* key3 = calloc(2, sizeof(char));
    char* key4 = calloc(2, sizeof(char));
    char* key5 = calloc(2, sizeof(char));
    char* value = calloc(2, sizeof(char));
    memcpy(key, "12345", 6); // NOLINT
    memcpy(key1, "1", 2); // NOLINT
    memcpy(key2, "2", 2); // NOLINT
    memcpy(key3, "1", 2); // NOLINT
    memcpy(key4, "1", 2); // NOLINT
    memcpy(key5, "2", 2); // NOLINT
    memcpy(value, "a", 2); // NOLINT
    memcpy(value, "a", 2); // NOLINT
    struct hashtable_data* values1[2] = {
        hashtable->alloc(key1, value),
        hashtable->alloc(key2, value)
    };
    struct hashtable_data* temp = hashtable->alloc(key3, value);
    RX_ASSERT(temp != 0);
    struct hashtable_data* values2[2] = {
        hashtable->alloc(key4, value),
        hashtable->alloc(key5, value)
    };
    RX_ASSERT(values2[0] != 0);
    RX_ASSERT(values2[1] != 0);
    RX_ASSERT(values2[0] != values2[1]);
    hashtable->free(temp);
    struct hashtable_data* get_values1_0 = hashtable->get(values1[0]->key);
    RX_ASSERT(get_values1_0 != values1[0]);
    struct hashtable_data* get_values1_1 = hashtable->get(values1[1]->key);
    RX_ASSERT(get_values1_1 != values1[1]);
    struct hashtable_data* values1_0 = hashtable->find(values1[0]->key);
    RX_ASSERT(values1_0 == values1[0]);
    struct hashtable_data* values1_1 = hashtable->find(values1[1]->key);
    RX_ASSERT(values1_1 == values1[1]);
    struct hashtable_data* values2_0 = hashtable->find(values2[0]->key);
    RX_ASSERT(values2_0 != values2[0]);
    struct hashtable_data* values2_1 = hashtable->find(values2[1]->key);
    RX_ASSERT(values2_1 != values2[1]);
    struct hashtable_data* node_key = hashtable->get(key1);
    struct hashtable_data* nonexistent_key = hashtable->get(key);
    RX_ASSERT(get_values1_0 != 0);
    RX_ASSERT(get_values1_1 != 0);
    RX_ASSERT(values1_0 != 0);
    RX_ASSERT(values1_1 != 0);
    RX_ASSERT(values2_0 != 0);
    RX_ASSERT(values2_1 != 0);
    RX_ASSERT(node_key != 0);
    RX_ASSERT(nonexistent_key == 0);
    hashtable->destroy();
    free(key);
    free(key1);
    free(key2);
    free(key3);
    free(key4);
    free(key5);
    free(value);
    RX_ASSERT(0 == 0);
}

// test init
RX_TEST_CASE(myTestSuite, test_hashtable_alloc_set_get, .fixture = test_fixture) {
    hashtable->init(HASHTABLE_SIZE);
    char* key = calloc(6, sizeof(char));
    char* key1 = calloc(2, sizeof(char));
    char* key2 = calloc(2, sizeof(char));
    char* key3 = calloc(2, sizeof(char));
    char* value1 = calloc(2, sizeof(char));
    char* value2 = calloc(2, sizeof(char));
    char* value3 = calloc(2, sizeof(char));
    char* value = calloc(5, sizeof(char));
    memcpy(key, "12345", 6); // NOLINT
    memcpy(key1, "1", 2); // NOLINT
    memcpy(key2, "1", 2); // NOLINT
    memcpy(key3, "1", 2); // NOLINT
    memcpy(value1, "a", 2); // NOLINT
    memcpy(value2, "b", 2); // NOLINT
    memcpy(value3, "c", 2); // NOLINT
    memcpy(value, "abcd", 5); // NOLINT
    struct hashtable_data* values[2] = {
        hashtable->alloc(key1, value1),
        hashtable->alloc(key2, value2)
    };
    struct hashtable_data* temp = hashtable->alloc(key3, value3);
    RX_ASSERT(temp != 0);
    RX_ASSERT(values[0] != 0);
    RX_ASSERT(values[1] != 0);
    struct hashtable_data* key_value1 = hashtable->get(key1);
    RX_ASSERT(key_value1 != 0);
    RX_ASSERT(strcmp(key_value1->value, "c") == 0);
    hashtable->set(key1, value);
    struct hashtable_data* key_value2 = hashtable->get(key1);
    RX_ASSERT(strcmp(key_value2->value, value) == 0);
    RX_ASSERT(key_value1 != 0);
    hashtable->set(key, value3);
    struct hashtable_data* key_value3 = hashtable->get(key);
    RX_ASSERT(strcmp(key_value3->key, key) == 0);
    RX_ASSERT(strcmp(key_value3->value, value3) == 0);
    hashtable->set(key, value);
    struct hashtable_data* key_value = hashtable->get(key);
    RX_ASSERT(strcmp(key_value->key, key) == 0);
    RX_ASSERT(strcmp(key_value->value, value) == 0);
    hashtable->free(temp);
    hashtable->destroy();
    free(key);
    free(key1);
    free(key2);
    free(key3);
    free(value1);
    free(value2);
    free(value3);
    free(value);
    RX_ASSERT(0 == 0);
}

// test init
RX_TEST_CASE(myTestSuite, test_load_open_file_unsafe_hashtable, .fixture = test_fixture) {
#ifdef DEBUG
    debug("TEST %s\n", "test_load_open_file_unsafe_hashtable");
#endif
    u64 file_path_ptr = pointer->getcwd();
    u64 file_name_ptr = pointer->load("/all_english_words.txt");
    pointer->strcat(file_path_ptr, file_name_ptr);
#ifndef USE_GC
    pointer->free(file_name_ptr);
#endif
    u64 mode_ptr = pointer->load("rb");
    u64 f_ptr = pointer->open_file(file_path_ptr, mode_ptr);
    if (f_ptr != 0) {
        u64 data_ptr = pointer->read_file(f_ptr);
        u64 list_ptr = pointer->list_alloc();
        pointer->close_file(f_ptr);
        char* file_data = pointer->unsafe(data_ptr);
        for (int i = 0; i < 100; i++) {
            char* tmp = file_data;
            while (*tmp != 0 && *tmp != '\n') {
                tmp++;
            }
            *tmp++ = '\0';
            u64 data = pointer->load(file_data);
            pointer->list_push(list_ptr, data);
            char* unsafe = pointer->unsafe(data);
            printf("%s\n", unsafe);
            file_data = tmp;
        }
        pointer->list_free(list_ptr);
#ifndef USE_GC
        pointer->free(data_ptr);
#endif
    }
#ifndef USE_GC
    pointer->free(mode_ptr);
    pointer->free(file_name_ptr);
    pointer->free(file_path_ptr);
#endif
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
