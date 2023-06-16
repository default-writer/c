#include "common/alloc.h"
#include "list-micro/data.h"
#include "playground/brain/brain.h"
#include "playground/hashtable/v2/hashtable_v2.h"
#include "playground/pointer/pointer.h"

#include <rexo/include/rexo.h>

#define HASHTABLE_SIZE 101
#define DEFAULT_SIZE 0x1

/* list definition */

extern const struct list list_micro_definition;
extern struct hashtable hashtable_definition_v2;
extern struct pointer_methods pointer_methods_definition;

const struct hashtable* hashtable = &hashtable_definition_v2;
const struct pointer_methods* pointer = &pointer_methods_definition;

typedef struct test_data {
    struct pointer_data* ctx;
}* TEST_DATA;

RX_SET_UP(test_set_up) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct pointer_data** ctx = &rx->ctx;
    pointer_ctx_init(ctx, DEFAULT_SIZE);
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct pointer_data** ctx = &rx->ctx;
    pointer_ctx_destroy(ctx);
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

/* test init */
RX_TEST_CASE(tests, test_hashtable_init_destroy, .fixture = test_fixture) {
    hashtable->init(HASHTABLE_SIZE);
    hashtable->destroy();
}

/* test init */
RX_TEST_CASE(tests, test_hashtable_init_setup_destroy, .fixture = test_fixture) {
    hashtable->init(HASHTABLE_SIZE);
    hashtable->setup(murmurhash3);
    hashtable->destroy();
}

/* test init */
RX_TEST_CASE(tests, test_hashtable_alloc_free, .fixture = test_fixture) {
    hashtable->init(HASHTABLE_SIZE);
    const u64 size = 2;
    char* key = global_alloc(size);
    char* value = global_alloc(size);
    memcpy(key, "1", 2); /* NOLINT */
    memcpy(value, "a", 2); /* NOLINT */
    struct hashtable_data* tmp = hashtable->alloc(key, value);
    hashtable->free(tmp);
    hashtable->destroy();
    global_free(key, size);
    global_free(value, size);
}

/* test init */
RX_TEST_CASE(tests, test_hashtable_alloc_0_free, .fixture = test_fixture) {
    hashtable->init(HASHTABLE_SIZE);
    const u64 size = 5;
    char* value = global_alloc(size);
    memcpy(value, "asdf", 5); /* NOLINT */
    struct hashtable_data* tmp = hashtable->alloc(0, (void*)value);
    hashtable->free(tmp);
    hashtable->destroy();
    global_free(value, size);
}

/* test init */
RX_TEST_CASE(tests, test_hashtable_alloc_5, .fixture = test_fixture) {
    hashtable->init(HASHTABLE_SIZE);
    const u64 size = 5;
    char* key = global_alloc(size);
    char* value = global_alloc(size);
    memcpy(key, "1234", 3); /* NOLINT */
    memcpy(value, "abcd", 3); /* NOLINT */
    struct hashtable_data* tmp = hashtable->alloc(key, value);
    hashtable->free(tmp);
    hashtable->destroy();
    global_free(key, size);
    global_free(value, size);
}

/* test init */
RX_TEST_CASE(tests, test_hashtable_alloc_4, .fixture = test_fixture) {
    hashtable->init(HASHTABLE_SIZE);
    const u64 size = 4;
    char* key = global_alloc(size);
    char* value = global_alloc(size);
    memcpy(key, "123", 3); /* NOLINT */
    memcpy(value, "abc", 3); /* NOLINT */
    struct hashtable_data* tmp = hashtable->alloc(key, value);
    hashtable->free(tmp);
    hashtable->destroy();
    global_free(key, size);
    global_free(value, size);
}

/* test init */
RX_TEST_CASE(tests, test_hashtable_alloc_3, .fixture = test_fixture) {
    hashtable->init(HASHTABLE_SIZE);
    const u64 size = 3;
    char* key = global_alloc(size);
    char* value = global_alloc(size);
    memcpy(key, "12", 2); /* NOLINT */
    memcpy(value, "ab", 2); /* NOLINT */
    struct hashtable_data* tmp = hashtable->alloc(key, value);
    hashtable->free(tmp);
    hashtable->destroy();
    global_free(key, size);
    global_free(value, size);
}

/* test init */
RX_TEST_CASE(tests, test_hashtable_alloc_2, .fixture = test_fixture) {
    hashtable->init(HASHTABLE_SIZE);
    const u64 size = 2;
    char* key = global_alloc(size);
    char* value = global_alloc(size);
    memcpy(key, "1", 2); /* NOLINT */
    memcpy(value, "a", 2); /* NOLINT */
    hashtable->alloc(key, value);
    hashtable->destroy();
    global_free(key, size);
    global_free(value, size);
}

/* test init */
RX_TEST_CASE(tests, test_hashtable_alloc_alloc, .fixture = test_fixture) {
    hashtable->init(HASHTABLE_SIZE);
    const u64 size = 2;
    char* key = global_alloc(size);
    char* value = global_alloc(size);
    memcpy(key, "1", 2); /* NOLINT */
    memcpy(value, "a", 2); /* NOLINT */
    hashtable->alloc(key, value);
    hashtable->alloc(key, value);
    hashtable->destroy();
    global_free(key, size);
    global_free(value, size);
}

/* test init */
RX_TEST_CASE(tests, test_hashtable_alloc_alloc_alloc, .fixture = test_fixture) {
    hashtable->init(HASHTABLE_SIZE);
    const u64 size = 2;
    char* key = global_alloc(size);
    char* value = global_alloc(size);
    memcpy(key, "1", 2); /* NOLINT */
    memcpy(value, "a", 2); /* NOLINT */
    hashtable->alloc(key, value);
    hashtable->alloc(key, value);
    hashtable->alloc(key, value);
    hashtable->destroy();
    global_free(key, size);
    global_free(value, size);
}

/* test init */
RX_TEST_CASE(tests, test_hashtable_alloc_free_alloc, .fixture = test_fixture) {
    hashtable->init(HASHTABLE_SIZE);
    const u64 size = 2;
    char* key = global_alloc(size);
    char* value = global_alloc(size);
    memcpy(key, "1", 2); /* NOLINT */
    memcpy(value, "a", 2); /* NOLINT */
    struct hashtable_data* temp = hashtable->alloc(key, value);
    hashtable->free(temp);
    hashtable->alloc(key, value);
    hashtable->destroy();
    global_free(key, size);
    global_free(value, size);
}

/* test init */
RX_TEST_CASE(tests, test_hashtable_alloc_alloc_free, .fixture = test_fixture) {
    hashtable->init(HASHTABLE_SIZE);
    const u64 size = 2;
    char* key = global_alloc(size);
    char* value = global_alloc(size);
    memcpy(key, "1", 2); /* NOLINT */
    memcpy(value, "a", 2); /* NOLINT */
    struct hashtable_data* temp = hashtable->alloc(key, value);
    hashtable->alloc(key, value);
    hashtable->free(temp);
    hashtable->destroy();
    global_free(key, size);
    global_free(value, size);
}

/* test init */
RX_TEST_CASE(tests, test_hashtable_alloc_free_alloc_free, .fixture = test_fixture) {
    hashtable->init(HASHTABLE_SIZE);
    const u64 size = 2;
    char* key = global_alloc(size);
    char* value = global_alloc(size);
    memcpy(key, "1", 2); /* NOLINT */
    memcpy(value, "a", 2); /* NOLINT */
    struct hashtable_data* temp = hashtable->alloc(key, value);
    hashtable->free(temp);
    temp = hashtable->alloc(key, value);
    hashtable->free(temp);
    hashtable->destroy();
    global_free(key, size);
    global_free(value, size);
}

/* test init */
RX_TEST_CASE(tests, test_hashtable_alloc_alloc_temp_alloc_free_temp_alloc_alloc, .fixture = test_fixture) {
    hashtable->init(HASHTABLE_SIZE);
    const u64 size = 2;
    char* key = global_alloc(size);
    char* value = global_alloc(size);
    memcpy(key, "1", 2); /* NOLINT */
    memcpy(value, "a", 2); /* NOLINT */
    hashtable->alloc(key, value);
    struct hashtable_data* temp = hashtable->alloc(key, value);
    hashtable->alloc(key, value);
    hashtable->free(temp);
    hashtable->alloc(key, value);
    hashtable->alloc(key, value);
    hashtable->destroy();
    global_free(key, size);
    global_free(value, size);
}

/* test init */
RX_TEST_CASE(tests, test_hashtable_alloc_alloc_alloc_temp_alloc_alloc_free_temp, .fixture = test_fixture) {
    hashtable->init(HASHTABLE_SIZE);
    char* key = global_alloc(6);
    char* key1 = global_alloc(2);
    char* key2 = global_alloc(2);
    char* key3 = global_alloc(2);
    char* key4 = global_alloc(2);
    char* key5 = global_alloc(2);
    char* value = global_alloc(2);
    memcpy(key, "12345", 6); /* NOLINT */
    memcpy(key1, "1", 2); /* NOLINT */
    memcpy(key2, "2", 2); /* NOLINT */
    memcpy(key3, "1", 2); /* NOLINT */
    memcpy(key4, "1", 2); /* NOLINT */
    memcpy(key5, "2", 2); /* NOLINT */
    memcpy(value, "a", 2); /* NOLINT */
    memcpy(value, "a", 2); /* NOLINT */
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
    const struct hashtable_data* get_values1_0 = hashtable->value(hashtable->get(hashtable->key(values1[0])));
    RX_ASSERT(get_values1_0 != values1[0]);
    const struct hashtable_data* get_values1_1 = hashtable->value(hashtable->get(hashtable->key(values1[1])));
    RX_ASSERT(get_values1_1 != values1[1]);
    const struct hashtable_data* values1_0 = hashtable->find(hashtable->key(values1[0]));
    RX_ASSERT(values1_0 == values1[0]);
    const struct hashtable_data* values1_1 = hashtable->find(hashtable->key(values1[1]));
    RX_ASSERT(values1_1 == values1[1]);
    const struct hashtable_data* values2_0 = hashtable->find(hashtable->key(values2[0]));
    RX_ASSERT(values2_0 != values2[0]);
    const struct hashtable_data* values2_1 = hashtable->find(hashtable->key(values2[1]));
    RX_ASSERT(values2_1 != values2[1]);
    const struct hashtable_data* node_key = hashtable->value(hashtable->get(key1));
    const struct hashtable_data* nonexistent_key = hashtable->value(hashtable->get(key));
    RX_ASSERT(get_values1_0 != 0);
    RX_ASSERT(get_values1_1 != 0);
    RX_ASSERT(values1_0 != 0);
    RX_ASSERT(values1_1 != 0);
    RX_ASSERT(values2_0 != 0);
    RX_ASSERT(values2_1 != 0);
    RX_ASSERT(node_key != 0);
    RX_ASSERT(nonexistent_key == 0);
    hashtable->destroy();
    global_free(key, 6);
    global_free(key1, 2);
    global_free(key2, 2);
    global_free(key3, 2);
    global_free(key4, 2);
    global_free(key5, 2);
    global_free(value, 2);
}

/* test init */
RX_TEST_CASE(tests, test_hashtable_alloc_set_get, .fixture = test_fixture) {
    hashtable->init(HASHTABLE_SIZE);
    char* key = global_alloc(6);
    char* key1 = global_alloc(2);
    char* key2 = global_alloc(2);
    char* key3 = global_alloc(2);
    char* value1 = global_alloc(2);
    char* value2 = global_alloc(2);
    char* value3 = global_alloc(2);
    char* value = global_alloc(5);
    memcpy(key, "12345", 6); /* NOLINT */
    memcpy(key1, "1", 2); /* NOLINT */
    memcpy(key2, "1", 2); /* NOLINT */
    memcpy(key3, "1", 2); /* NOLINT */
    memcpy(value1, "a", 2); /* NOLINT */
    memcpy(value2, "b", 2); /* NOLINT */
    memcpy(value3, "c", 2); /* NOLINT */
    memcpy(value, "abcd", 5); /* NOLINT */
    struct hashtable_data* values[2] = {
        hashtable->alloc(key1, value1),
        hashtable->alloc(key2, value2)
    };
    struct hashtable_data* temp = hashtable->alloc(key3, value3);
    RX_ASSERT(temp != 0);
    RX_ASSERT(values[0] != 0);
    RX_ASSERT(values[1] != 0);
    const struct hashtable_data* data = hashtable->find(hashtable->key(temp));
    RX_ASSERT(hashtable->data(data) == value1);
    const struct hashtable_data* key_value1 = hashtable->value(hashtable->get(key1));
    RX_ASSERT(key_value1 != 0);
    RX_ASSERT(strcmp(hashtable->data(key_value1), "c") == 0);
    hashtable->set(key1, value);
    const struct hashtable_data* key_value2 = hashtable->value(hashtable->get(key1));
    RX_ASSERT(strcmp(hashtable->data(key_value2), value) == 0);
    RX_ASSERT(key_value1 != 0);
    hashtable->set(key, value3);
    const struct hashtable_data* key_value3 = hashtable->value(hashtable->get(key));
    RX_ASSERT(strcmp(hashtable->key(key_value3), key) == 0);
    RX_ASSERT(strcmp(hashtable->data(key_value3), value3) == 0);
    hashtable->set(key, value);
    const struct hashtable_data* key_value = hashtable->value(hashtable->get(key));
    RX_ASSERT(strcmp(hashtable->key(key_value), key) == 0);
    RX_ASSERT(strcmp(hashtable->data(key_value), value) == 0);
    hashtable->free(temp);
    hashtable->destroy();
    global_free(key, 6);
    global_free(key1, 2);
    global_free(key2, 2);
    global_free(key3, 2);
    global_free(value1, 2);
    global_free(value2, 2);
    global_free(value3, 2);
    global_free(value, 5);
}

/* test init */
RX_TEST_CASE(tests, test_hashtable_alloc_set_get_count, .fixture = test_fixture) {
    hashtable->init(HASHTABLE_SIZE);
    char* key1 = global_alloc(2);
    char* key2 = global_alloc(2);
    char* key3 = global_alloc(2);
    char* value1 = global_alloc(2);
    char* value2 = global_alloc(2);
    char* value3 = global_alloc(2);
    memcpy(key1, "1", 2); /* NOLINT */
    memcpy(key2, "1", 2); /* NOLINT */
    memcpy(key3, "1", 2); /* NOLINT */
    memcpy(value1, "a", 2); /* NOLINT */
    memcpy(value2, "b", 2); /* NOLINT */
    memcpy(value3, "c", 2); /* NOLINT */
    struct hashtable_data* values[3] = {
        hashtable->alloc(key1, value1),
        hashtable->alloc(key2, value2),
        hashtable->alloc(key3, value3)
    };
    RX_ASSERT(values[0] != 0);
    RX_ASSERT(values[1] != 0);
    RX_ASSERT(values[2] != 0);
    u32 key_value1 = hashtable->get(key1);
    RX_ASSERT(key_value1 != 0);
    hashtable->set(key1, value2);
    u32 key_value2 = hashtable->get(key1);
    RX_ASSERT(key_value1 != 0);
    hashtable->set(key1, value3);
    u32 key_value3 = hashtable->get(key1);
    RX_ASSERT(key_value1 != 0);
    RX_ASSERT(key_value3 == key_value2 && key_value2 == key_value1);
    u64 count = hashtable->count(key1);
    RX_ASSERT(count == 3);
    hashtable->destroy();
    global_free(key1, 2);
    global_free(key2, 2);
    global_free(key3, 2);
    global_free(value1, 2);
    global_free(value2, 2);
    global_free(value3, 2);
}

/* test init */
RX_TEST_CASE(tests, test_load_open_file_unsafe_hashtable, .fixture = test_fixture) {
    hashtable->init(0xff);
    u64 file_path_ptr = pointer->getcwd();
    u64 file_name_ptr = pointer->load("/all_english_words.txt");
    pointer->strcat(file_path_ptr, file_name_ptr);
#ifndef USE_GC
    pointer->free(file_name_ptr);
#endif
    u64 mode_ptr = pointer->load("rb");
    u64 f_ptr = pointer->file_alloc(file_path_ptr, mode_ptr);
    if (f_ptr != 0) {
        u64 data_ptr = pointer->file_read(f_ptr);
        u64 list_ptr = pointer->list_alloc();
        pointer->file_free(f_ptr);
        const u64 size = 0xfff;
        char* file_data;
        const char* file_end;
        file_data = pointer->unsafe(data_ptr);
        file_end = file_data + size;
        while (file_data < file_end) {
            char* tmp = file_data;
            while (*tmp != 0 && *tmp != '\n') {
                tmp++;
            }
            *tmp++ = '\0';
            u64 data = pointer->load(file_data);
            pointer->list_push(list_ptr, data);
            const char* unsafe = pointer->unsafe(data);
            pointer->printf(data);
            pointer->put_char(data, unsafe[0]);
            u64 pattern_ptr = pointer->load("b");
            u64 last_match_ptr = pointer->match_last(data, pattern_ptr);
            u64 leak_ptr = pointer->match_last(data, last_match_ptr);
            pointer->printf(leak_ptr);
            hashtable->alloc(unsafe, 0);
#ifndef USE_GC
            pointer->free(pattern_ptr);
            pointer->free(last_match_ptr);
            pointer->free(leak_ptr);
#endif
            file_data = tmp;
        }
        file_data = pointer->unsafe(data_ptr);
        file_end = file_data + size;
        while (file_data < file_end) {
            char* tmp = file_data;
            while (*tmp != 0 && *tmp != '\n') {
                tmp++;
            }
            *tmp++ = '\0';
            char* unsafe = file_data;
            u32 hash = hashtable->hash(unsafe);
            u32 count = hashtable->count(unsafe);
            printf("  .#: %016llx ! %08lx (%6ld): %16s\n", (u64)unsafe, hash % HASHTABLE_DEFAULT_SIZE, count, unsafe);
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
    hashtable->destroy();
}

/* test init */
RX_TEST_CASE(tests, test_list_push, .fixture = test_fixture) {
    u64 list_ptr = pointer->list_alloc();
    RX_ASSERT(list_ptr != 0);
#ifndef USE_GC
    pointer->list_free(list_ptr);
#else
    pointer->gc();
#endif
}

/* test init */
RX_TEST_CASE(tests, test_free, .fixture = test_fixture) {
    u64 list_ptr = pointer->list_alloc();
    pointer->list_free(list_ptr);
    pointer->list_free(list_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_improper_use_of_different_calls, .fixture = test_fixture) {
    u64 idx0 = 0;
    u64 idx1 = 0;
    u64 idx2 = 0;
    u64 idx3 = 0;
    u64 idx4 = 0;
    u64 idx5 = 0;
    u64 idx6 = 0;
    u64 idx7 = 0;
    u64 file_path_ptr = pointer->getcwd();
    u64 file_name_ptr = pointer->load("/all_english_words.txt");
    pointer->strcat(file_path_ptr, file_name_ptr);
    u64 mode_ptr = pointer->load("rb");
    u64 f_ptr = pointer->file_alloc(file_path_ptr, mode_ptr);
    if (f_ptr != 0) {
        idx4 = pointer->file_read(file_name_ptr);
        idx5 = pointer->list_peek(file_name_ptr);
        idx6 = pointer->list_pop(file_name_ptr);
        pointer->list_push(file_name_ptr, f_ptr);
        u64 data_ptr = pointer->file_read(f_ptr);
        u64 list_ptr = pointer->list_alloc();
        pointer->list_push(list_ptr, mode_ptr);
        idx3 = pointer->copy(list_ptr);
        pointer->file_free(data_ptr);
        pointer->file_free(list_ptr);
        pointer->list_peek(list_ptr);
        pointer->list_push(list_ptr, f_ptr);
        pointer->file_free(f_ptr);
        pointer->list_pop(0);
        pointer->list_pop(1);
        pointer->list_pop(2);
        pointer->list_pop(3);
        pointer->list_pop(4);
        pointer->list_pop(5);
        pointer->list_pop(6);
        pointer->list_pop(7);
        pointer->list_pop(8);
        pointer->list_pop(9);
        pointer->list_push(f_ptr, f_ptr);
        pointer->list_push(list_ptr, f_ptr);
        pointer->list_pop(0);
        pointer->list_pop(1);
        pointer->list_pop(2);
        pointer->list_pop(3);
        pointer->list_pop(4);
        pointer->list_pop(5);
        pointer->list_pop(6);
        pointer->list_pop(7);
        pointer->list_pop(8);
        pointer->list_pop(9);
        pointer->list_push(f_ptr, f_ptr);
        pointer->list_push(list_ptr, f_ptr);
        pointer->list_push(data_ptr, f_ptr);
        pointer->list_peek(f_ptr);
        pointer->list_peek(list_ptr);
        pointer->list_peek(data_ptr);
        pointer->list_pop(f_ptr);
        pointer->list_pop(list_ptr);
        pointer->list_pop(data_ptr);
        pointer->file_read(f_ptr);
        pointer->file_read(list_ptr);
        pointer->file_read(data_ptr);
        pointer->list_free(data_ptr);
        pointer->list_push(f_ptr, data_ptr);
        pointer->list_push(list_ptr, data_ptr);
        pointer->list_push(data_ptr, data_ptr);
        idx1 = pointer->copy(list_ptr);
        pointer->list_free(list_ptr);
        idx2 = pointer->copy(list_ptr);
        pointer->list_free(f_ptr);
#ifndef USE_GC
        pointer->free(data_ptr);
        pointer->free(list_ptr);
        pointer->free(f_ptr);
#endif
    }
    pointer->file_free(f_ptr);
    pointer->list_peek(f_ptr);
    pointer->list_push(f_ptr, f_ptr);
    pointer->file_free(f_ptr);
    pointer->list_free(f_ptr);
#ifndef USE_GC
    pointer->free(f_ptr);
    pointer->free(mode_ptr);
    pointer->free(file_name_ptr);
    pointer->free(file_path_ptr);
#else
    pointer->gc();
#endif
    RX_ASSERT(idx0 == 0);
    RX_ASSERT(idx1 == 0);
    RX_ASSERT(idx2 == 0);
    RX_ASSERT(idx3 == 0);
    RX_ASSERT(idx4 == 0);
    RX_ASSERT(idx5 == 0);
    RX_ASSERT(idx6 == 0);
    RX_ASSERT(idx7 == 0);
}

/* test init */
RX_TEST_CASE(tests, test_alloc_free, .fixture = test_fixture) {
    u64 list_ptr = pointer->list_alloc();
    u64 size = pointer->size(list_ptr);
    RX_ASSERT(size == 8);
    u64 pattern_ptr = pointer->alloc();
    pointer->list_free(pattern_ptr);
    pointer->list_free(list_ptr);
#ifndef USE_GC
    pointer->free(list_ptr);
    pointer->free(pattern_ptr);
#else
    pointer->gc();
#endif
}

/* test init */
RX_TEST_CASE(tests, test_load_open_file_unsafe_hashtable_default_hash, .fixture = test_fixture) {
    hashtable->init(0x3);
    hashtable->setup(default_hash);
    u64 file_path_ptr = pointer->getcwd();
    u64 file_name_ptr = pointer->load("/all_english_words.txt");
    pointer->strcat(file_path_ptr, file_name_ptr);
#ifndef USE_GC
    pointer->free(file_name_ptr);
#endif
    u64 mode_ptr = pointer->load("rb");
    u64 f_ptr = pointer->file_alloc(file_path_ptr, mode_ptr);
    if (f_ptr != 0) {
        u64 data_ptr = pointer->file_read(f_ptr);
        u64 list_ptr = pointer->list_alloc();
        pointer->file_free(f_ptr);
        u64 size = 0xfff;
        char* file_data;
        const char* file_end;
        file_data = pointer->unsafe(data_ptr);
        file_end = file_data + size;
        while (file_data < file_end) {
            char* tmp = file_data;
            while (*tmp != 0 && *tmp != '\n') {
                tmp++;
            }
            *tmp++ = '\0';
            u64 data = pointer->load(file_data);
            pointer->list_push(list_ptr, data);
            const char* unsafe = pointer->unsafe(data);
            hashtable->alloc(unsafe, 0);
            file_data = tmp;
        }
        file_data = pointer->unsafe(data_ptr);
        file_end = file_data + size;
        while (file_data < file_end) {
            char* tmp = file_data;
            while (*tmp != 0 && *tmp != '\n') {
                tmp++;
            }
            *tmp++ = '\0';
            char* unsafe = file_data;
            u32 hash = hashtable->hash(unsafe);
            u32 count = hashtable->count(unsafe);
            printf("  .#: %016llx ! %08lx (%6ld): %16s\n", (u64)unsafe, hash % HASHTABLE_DEFAULT_SIZE, count, unsafe);
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
    hashtable->destroy();
}

/* test init */
RX_TEST_CASE(tests, test_load_open_file_unsafe_hashtable_murmurhash3_hash, .fixture = test_fixture) {
    hashtable->init(0x3);
    hashtable->setup(murmurhash3);
    u64 file_path_ptr = pointer->getcwd();
    u64 file_name_ptr = pointer->load("/all_english_words.txt");
    pointer->strcat(file_path_ptr, file_name_ptr);
#ifndef USE_GC
    pointer->free(file_name_ptr);
#endif
    u64 mode_ptr = pointer->load("rb");
    u64 f_ptr = pointer->file_alloc(file_path_ptr, mode_ptr);
    if (f_ptr != 0) {
        u64 data_ptr = pointer->file_read(f_ptr);
        u64 list_ptr = pointer->list_alloc();
        pointer->file_free(f_ptr);
        const u64 size = 0xfff;
        char* file_data;
        const char* file_end;
        file_data = pointer->unsafe(data_ptr);
        file_end = file_data + size;
        while (file_data < file_end) {
            char* tmp = file_data;
            while (*tmp != 0 && *tmp != '\n') {
                tmp++;
            }
            *tmp++ = '\0';
            u64 data = pointer->load(file_data);
            pointer->list_push(list_ptr, data);
            const char* unsafe = pointer->unsafe(data);
            hashtable->alloc(unsafe, 0);
            file_data = tmp;
        }
        file_data = pointer->unsafe(data_ptr);
        file_end = file_data + size;
        while (file_data < file_end) {
            char* tmp = file_data;
            while (*tmp != 0 && *tmp != '\n') {
                tmp++;
            }
            *tmp++ = '\0';
            char* unsafe = file_data;
            u32 hash = hashtable->hash(unsafe);
            u32 count = hashtable->count(unsafe);
            printf("  .#: %016llx ! %08lx (%6ld): %16s\n", (u64)unsafe, hash % HASHTABLE_DEFAULT_SIZE, count, unsafe);
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
    hashtable->destroy();
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
    int result = rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;
#ifdef USE_MEMORY_DEBUG_INFO
    global_statistics();
#endif
    return result;
}
