#include "common/alloc.h"
#include "list-micro/data.h"
#include "playground/brain/brain.h"
#include "playground/hashtable/v2/hashtable_v2.h"
#include "playground/pointer/pointer.h"

#include <rexo/include/rexo.h>

#define HASHTABLE_SIZE 101
#define DEFAULT_SIZE 0xffff

/* list definition */

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
RX_TEST_CASE(myTestSuite, test_hashtable_init_destroy, .fixture = test_fixture) {
    hashtable->init(HASHTABLE_SIZE);
    hashtable->destroy();
    RX_ASSERT(0 == 0);
}

/* test init */
RX_TEST_CASE(myTestSuite, test_hashtable_alloc_free, .fixture = test_fixture) {
    hashtable->init(HASHTABLE_SIZE);
    char* key = _list_alloc(sizeof(char) + 1);
    char* value = _list_alloc(sizeof(char) + 1);
    memcpy(key, "1", 2); /* NOLINT */
    memcpy(value, "a", 2); /* NOLINT */
    struct hashtable_data* tmp = hashtable->alloc(key, value);
    hashtable->free(tmp);
    hashtable->destroy();
    _list_free(key, 0);
    _list_free(value, 0);
    RX_ASSERT(0 == 0);
}

/* test init */
RX_TEST_CASE(myTestSuite, test_hashtable_alloc, .fixture = test_fixture) {
    hashtable->init(HASHTABLE_SIZE);
    char* key = _list_alloc(sizeof(char) + 1);
    char* value = _list_alloc(sizeof(char) + 1);
    memcpy(key, "1", 2); /* NOLINT */
    memcpy(value, "a", 2); /* NOLINT */
    hashtable->alloc(key, value);
    hashtable->destroy();
    _list_free(key, 0);
    _list_free(value, 0);
    RX_ASSERT(0 == 0);
}

/* test init */
RX_TEST_CASE(myTestSuite, test_hashtable_alloc_alloc, .fixture = test_fixture) {
    hashtable->init(HASHTABLE_SIZE);
    char* key = _list_alloc(sizeof(char) + 1);
    char* value = _list_alloc(sizeof(char) + 1);
    memcpy(key, "1", 2); /* NOLINT */
    memcpy(value, "a", 2); /* NOLINT */
    hashtable->alloc(key, value);
    hashtable->alloc(key, value);
    hashtable->destroy();
    _list_free(key, 0);
    _list_free(value, 0);
    RX_ASSERT(0 == 0);
}

/* test init */
RX_TEST_CASE(myTestSuite, test_hashtable_alloc_alloc_alloc, .fixture = test_fixture) {
    hashtable->init(HASHTABLE_SIZE);
    char* key = _list_alloc(sizeof(char) + 1);
    char* value = _list_alloc(sizeof(char) + 1);
    memcpy(key, "1", 2); /* NOLINT */
    memcpy(value, "a", 2); /* NOLINT */
    hashtable->alloc(key, value);
    hashtable->alloc(key, value);
    hashtable->alloc(key, value);
    hashtable->destroy();
    _list_free(key, 0);
    _list_free(value, 0);
    RX_ASSERT(0 == 0);
}

/* test init */
RX_TEST_CASE(myTestSuite, test_hashtable_alloc_free_alloc, .fixture = test_fixture) {
    hashtable->init(HASHTABLE_SIZE);
    char* key = _list_alloc(sizeof(char) + 1);
    char* value = _list_alloc(sizeof(char) + 1);
    memcpy(key, "1", 2); /* NOLINT */
    memcpy(value, "a", 2); /* NOLINT */
    struct hashtable_data* temp = hashtable->alloc(key, value);
    hashtable->free(temp);
    hashtable->alloc(key, value);
    hashtable->destroy();
    _list_free(key, 0);
    _list_free(value, 0);
    RX_ASSERT(0 == 0);
}

/* test init */
RX_TEST_CASE(myTestSuite, test_hashtable_alloc_alloc_free, .fixture = test_fixture) {
    hashtable->init(HASHTABLE_SIZE);
    char* key = _list_alloc(sizeof(char) + 1);
    char* value = _list_alloc(sizeof(char) + 1);
    memcpy(key, "1", 2); /* NOLINT */
    memcpy(value, "a", 2); /* NOLINT */
    struct hashtable_data* temp = hashtable->alloc(key, value);
    hashtable->alloc(key, value);
    hashtable->free(temp);
    hashtable->destroy();
    _list_free(key, 0);
    _list_free(value, 0);
    RX_ASSERT(0 == 0);
}

/* test init */
RX_TEST_CASE(myTestSuite, test_hashtable_alloc_free_alloc_free, .fixture = test_fixture) {
    hashtable->init(HASHTABLE_SIZE);
    char* key = _list_alloc(sizeof(char) + 1);
    char* value = _list_alloc(sizeof(char) + 1);
    memcpy(key, "1", 2); /* NOLINT */
    memcpy(value, "a", 2); /* NOLINT */
    struct hashtable_data* temp = hashtable->alloc(key, value);
    hashtable->free(temp);
    temp = hashtable->alloc(key, value);
    hashtable->free(temp);
    hashtable->destroy();
    _list_free(key, 0);
    _list_free(value, 0);
    RX_ASSERT(0 == 0);
}

/* test init */
RX_TEST_CASE(myTestSuite, test_hashtable_alloc_alloc_temp_alloc_free_temp_alloc_alloc, .fixture = test_fixture) {
    hashtable->init(HASHTABLE_SIZE);
    char* key = _list_alloc(sizeof(char) + 1);
    char* value = _list_alloc(sizeof(char) + 1);
    memcpy(key, "1", 2); /* NOLINT */
    memcpy(value, "a", 2); /* NOLINT */
    hashtable->alloc(key, value);
    struct hashtable_data* temp = hashtable->alloc(key, value);
    hashtable->alloc(key, value);
    hashtable->free(temp);
    hashtable->alloc(key, value);
    hashtable->alloc(key, value);
    hashtable->destroy();
    _list_free(key, 0);
    _list_free(value, 0);
    RX_ASSERT(0 == 0);
}

/* test init */
RX_TEST_CASE(myTestSuite, test_hashtable_alloc_set_get, .fixture = test_fixture) {
    hashtable->init(HASHTABLE_SIZE);
    char* key = _list_alloc(6 * sizeof(char));
    char* key1 = _list_alloc(2 * sizeof(char));
    char* key2 = _list_alloc(2 * sizeof(char));
    char* key3 = _list_alloc(2 * sizeof(char));
    char* value1 = _list_alloc(2 * sizeof(char));
    char* value2 = _list_alloc(2 * sizeof(char));
    char* value3 = _list_alloc(2 * sizeof(char));
    char* value = _list_alloc(5 * sizeof(char));
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
    const struct hashtable_data* data = hashtable->find(temp->key);
    RX_ASSERT(data->value == value1);
    const struct hashtable_data* key_value1 = hashtable->value(hashtable->get(key1));
    RX_ASSERT(key_value1 != 0);
    RX_ASSERT(strcmp(key_value1->value, "c") == 0);
    hashtable->set(key1, value);
    const struct hashtable_data* key_value2 = hashtable->value(hashtable->get(key1));
    RX_ASSERT(strcmp(key_value2->value, value) == 0);
    RX_ASSERT(key_value1 != 0);
    hashtable->set(key, value3);
    const struct hashtable_data* key_value3 = hashtable->value(hashtable->get(key));
    RX_ASSERT(strcmp(key_value3->key, key) == 0);
    RX_ASSERT(strcmp(key_value3->value, value3) == 0);
    hashtable->set(key, value);
    const struct hashtable_data* key_value = hashtable->value(hashtable->get(key));
    RX_ASSERT(strcmp(key_value->key, key) == 0);
    RX_ASSERT(strcmp(key_value->value, value) == 0);
    hashtable->free(temp);
    hashtable->destroy();
    _list_free(key, 0);
    _list_free(key1, 0);
    _list_free(key2, 0);
    _list_free(key3, 0);
    _list_free(value1, 0);
    _list_free(value2, 0);
    _list_free(value3, 0);
    _list_free(value, 0);
    RX_ASSERT(0 == 0);
}

/* test init */
RX_TEST_CASE(myTestSuite, test_load_open_file_unsafe_hashtable, .fixture = test_fixture) {
    hashtable->init(0xffff);
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
        u64 size = 0xfffff;
#ifdef USE_MEMORY_DEBUG_INFO
        printf("data size: %16lld\n", size);
#endif
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
            printf("  .#: 0x%016llx !0x%08lx (%6ld): %18s\n", (u64)unsafe, hash % HASHTABLE_DEFAULT_SIZE, count, unsafe);
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
RX_TEST_CASE(myTestSuite, test_list_push, .fixture = test_fixture) {
    // u64 mode_ptr = pointer->load("rb");
    u64 list_ptr = pointer->list_alloc();
    // pointer->list_push(list_ptr, mode_ptr);
    // u64 m_ptr = pointer->list_pop(list_ptr);
    pointer->list_free(list_ptr);
#ifndef USE_GC
    // pointer->free(mode_ptr);
    // pointer->free(list_ptr);
#else
    pointer->gc();
#endif
}

/* test init */
RX_TEST_CASE(myTestSuite, test_list_free_list_free, .fixture = test_fixture) {
    u64 list_ptr = pointer->list_alloc();
    pointer->list_free(list_ptr);
    pointer->list_free(list_ptr);
}

/* test init */
RX_TEST_CASE(myTestSuite, test_improper_use_of_different_calls, .fixture = test_fixture) {
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
        pointer->file_free(f_ptr);
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
RX_TEST_CASE(myTestSuite, test_list_alloc_list_free, .fixture = test_fixture) {
    u64 list_ptr = pointer->list_alloc();
    u64 size = pointer->size(list_ptr);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("data size: %16lld\n", size);
#endif
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
RX_TEST_CASE(myTestSuite, test_load_open_file_unsafe_hashtable_default_hash, .fixture = test_fixture) {
    hashtable->init(0xffff);
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
        u64 size = 0xfffff;
#ifdef USE_MEMORY_DEBUG_INFO
        printf("data size: %16lld\n", size);
#endif
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
            printf("  .#: 0x%016llx !0x%08lx (%6ld): %18s\n", (u64)unsafe, hash % HASHTABLE_DEFAULT_SIZE, count, unsafe);
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
RX_TEST_CASE(myTestSuite, test_load_open_file_unsafe_hashtable_murmurhash3_hash, .fixture = test_fixture) {
    hashtable->init(0xffff);
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
        u64 size = 0xfffff;
#ifdef USE_MEMORY_DEBUG_INFO
        printf("data size: %16lld\n", size);
#endif
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
            printf("  .#: 0x%016llx !0x%08lx (%6ld): %18s\n", (u64)unsafe, hash % HASHTABLE_DEFAULT_SIZE, count, unsafe);
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
    return rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;
}
