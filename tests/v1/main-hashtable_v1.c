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
#include "common/alloc.h"
#include "list-micro/data.h"
#include "playground/brain/brain.h"
#include "playground/hashtable/v1/hashtable_v1.h"
#include "pointer/types/data/v1/data_v1.h"
#include "pointer/types/file/v1/file_v1.h"
#include "pointer/types/list/v1/list_v1.h"
#include "pointer/types/object/v1/object_v1.h"
#include "pointer/types/string/v1/string_v1.h"
#include "pointer/v1/pointer_v1.h"

#define RXP_DEBUG_TESTS

#include "rexo/include/rexo.h"

#define HASHTABLE_SIZE 101
#define DEFAULT_SIZE 0x100

/* definition */
extern void pointer_ctx_init(struct pointer_data** ctx, u64 size);
extern void pointer_ctx_destroy(struct pointer_data** ctx);

extern const struct vm_methods vm_methods_definition;

extern const struct hashtable hashtable_definition_v1;
extern const struct pointer_methods pointer_methods_definition;
extern const struct list_methods list_methods_definition;
extern const struct file_methods file_methods_definition;
extern const struct string_methods string_methods_definition;
extern const struct data_methods data_methods_definition;
extern const struct object_methods object_methods_definition;

static const struct hashtable* hashtable = &hashtable_definition_v1;
static const struct pointer_methods* pointer = &pointer_methods_definition;
static const struct list_methods* list = &list_methods_definition;
static const struct file_methods* file = &file_methods_definition;
static const struct string_methods* string = &string_methods_definition;
static const struct data_methods* data = &data_methods_definition;
static const struct object_methods* object = &object_methods_definition;

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
    struct hashtable_data* unsafe_tmp = hashtable->alloc(key, value);
    hashtable->free(unsafe_tmp);
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
    struct hashtable_data* unsafe_tmp = hashtable->alloc(key, value);
    const struct hashtable_data* unsafe_tmp1 = hashtable->alloc(key, value);
    RX_ASSERT(unsafe_tmp1 != 0);
    hashtable->free(unsafe_tmp);
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
    const struct hashtable_data* unsafe_tmp1 = hashtable->alloc(key, value);
    const struct hashtable_data* unsafe_tmp2 = hashtable->alloc(key, value);
    const struct hashtable_data* unsafe_tmp3 = hashtable->alloc(key, value);
    RX_ASSERT(unsafe_tmp1 != 0);
    RX_ASSERT(unsafe_tmp2 != 0);
    RX_ASSERT(unsafe_tmp3 != 0);
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
    const struct hashtable_data* unsafe_tmp1 = hashtable->alloc(key, value);
    RX_ASSERT(unsafe_tmp1 != 0);
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
    const struct hashtable_data* unsafe_tmp1 = hashtable->alloc(key, value);
    RX_ASSERT(unsafe_tmp1 != 0);
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
    const struct hashtable_data* unsafe_tmp1 = hashtable->alloc(key, value);
    struct hashtable_data* temp = hashtable->alloc(key, value);
    const struct hashtable_data* unsafe_tmp2 = hashtable->alloc(key, value);
    hashtable->free(temp);
    const struct hashtable_data* unsafe_tmp3 = hashtable->alloc(key, value);
    const struct hashtable_data* unsafe_tmp4 = hashtable->alloc(key, value);
    RX_ASSERT(unsafe_tmp1 != 0);
    RX_ASSERT(unsafe_tmp2 != 0);
    RX_ASSERT(unsafe_tmp3 != 0);
    RX_ASSERT(unsafe_tmp4 != 0);
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
    const struct hashtable_data* get_values1_0 = hashtable->get(values1[0]->key);
    RX_ASSERT(get_values1_0 != values1[0]);
    const struct hashtable_data* get_values1_1 = hashtable->get(values1[1]->key);
    RX_ASSERT(get_values1_1 != values1[1]);
    const struct hashtable_data* values1_0 = hashtable->find(values1[0]->key);
    RX_ASSERT(values1_0 == values1[0]);
    const struct hashtable_data* values1_1 = hashtable->find(values1[1]->key);
    RX_ASSERT(values1_1 == values1[1]);
    const struct hashtable_data* values2_0 = hashtable->find(values2[0]->key);
    RX_ASSERT(values2_0 != values2[0]);
    const struct hashtable_data* values2_1 = hashtable->find(values2[1]->key);
    RX_ASSERT(values2_1 != values2[1]);
    const struct hashtable_data* node_key = hashtable->get(key1);
    const struct hashtable_data* nonexistent_key = hashtable->get(key);
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
    const struct hashtable_data* key_value1 = hashtable->get(key1);
    RX_ASSERT(key_value1 != 0);
    RX_ASSERT(strcmp(key_value1->value, "c") == 0);
    hashtable->set(key1, value);
    const struct hashtable_data* key_value2 = hashtable->get(key1);
    RX_ASSERT(strcmp(key_value2->value, value) == 0);
    RX_ASSERT(key_value1 != 0);
    hashtable->set(key, value3);
    const struct hashtable_data* key_value3 = hashtable->get(key);
    RX_ASSERT(strcmp(key_value3->key, key) == 0);
    RX_ASSERT(strcmp(key_value3->value, value3) == 0);
    hashtable->set(key, value);
    const struct hashtable_data* key_value = hashtable->get(key);
    RX_ASSERT(strcmp(key_value->key, key) == 0);
    RX_ASSERT(strcmp(key_value->value, value) == 0);
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
RX_TEST_CASE(tests, test_load_open_file_unsafe_hashtable, .fixture = test_fixture) {
    hashtable->init(0xff);
    u64 file_path_ptr = string->getcwd();
    u64 file_name_ptr = string->load("/all_english_words.txt");
    string->strcat(file_path_ptr, file_name_ptr);
    string->free(file_name_ptr);
    u64 mode_ptr = string->load("rb");
    u64 f_ptr = file->alloc(file_path_ptr, mode_ptr);
    if (f_ptr != 0) {
        u64 data_ptr = file->data(f_ptr);
        u64 list_ptr = list->alloc();
        file->free(f_ptr);
        const u64 size = 0xfff;
        char* file_data;
        const char* file_end;
        file_data = data->unsafe(data_ptr);
        file_end = file_data + size;
        while (file_data < file_end) {
            char* tmp = file_data;
            while (*tmp != 0 && *tmp != '\n') {
                tmp++;
            }
            *tmp++ = '\0';
            u64 string_ptr = string->load(file_data);
            list->push(list_ptr, string_ptr);
            const char* unsafe = string->unsafe(string_ptr);
            string->printf(string_ptr);
            string->put_char(string_ptr, unsafe[0]);
            u64 pattern_ptr = string->load("b");
            u64 last_match_ptr = string->match_last(string_ptr, pattern_ptr);
            u64 leak_ptr = string->match_last(string_ptr, last_match_ptr);
            string->printf(leak_ptr);
            struct hashtable_data* unsafe_tmp = hashtable->alloc(unsafe, 0);
            hashtable->free(unsafe_tmp);
            string->free(pattern_ptr);
            string->free(last_match_ptr);
            string->free(leak_ptr);
            file_data = tmp;
        }
        file_data = data->unsafe(data_ptr);
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
        list->free(list_ptr);
        data->free(data_ptr);
    }
    string->free(mode_ptr);
    string->free(file_name_ptr);
    string->free(file_path_ptr);
    hashtable->destroy();
}

/* test init */
RX_TEST_CASE(tests, test_list_push, .fixture = test_fixture) {
    u64 list_ptr = list->alloc();
    RX_ASSERT(list_ptr != 0);
#ifndef USE_GC
    list->free(list_ptr);
#else
    pointer->gc();
#endif
}

/* test init */
RX_TEST_CASE(tests, test_list_size, .fixture = test_fixture) {
    u64 list_ptr = list->alloc();
    u64 size = list->size(list_ptr);
    RX_ASSERT(list_ptr != 0);
    RX_ASSERT(size == 8);
#ifndef USE_GC
    list->free(list_ptr);
#else
    pointer->gc();
#endif
}

/* test init */
RX_TEST_CASE(tests, test_list_size_null, .fixture = test_fixture) {
    u64 list_ptr = data->alloc(0);
    u64 size = list->size(list_ptr);
    data->free(list_ptr);
    RX_ASSERT(list_ptr != 0);
    RX_ASSERT(size == 0);
#ifndef USE_GC
    list->free(list_ptr);
#else
    pointer->gc();
#endif
}

/* test init */
RX_TEST_CASE(tests, test_list_size_0, .fixture = test_fixture) {
    u64 size = list->size(0);
    RX_ASSERT(size == 0);
}

/* test init */
RX_TEST_CASE(tests, test_free, .fixture = test_fixture) {
    u64 list_ptr = list->alloc();
    list->free(list_ptr);
    list->free(list_ptr);
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
    u64 file_path_ptr = string->getcwd();
    u64 file_name_ptr = string->load("/all_english_words.txt");
    string->strcat(file_path_ptr, file_name_ptr);
    u64 mode_ptr = string->load("rb");
    u64 f_ptr = file->alloc(file_path_ptr, mode_ptr);
    if (f_ptr != 0) {
        idx4 = file->data(file_name_ptr);
        idx5 = list->peek(file_name_ptr);
        idx6 = list->pop(file_name_ptr);
        list->push(file_name_ptr, f_ptr);
        u64 data_ptr = file->data(f_ptr);
        u64 list_ptr = list->alloc();
        list->push(list_ptr, mode_ptr);
        idx3 = string->copy(list_ptr);
        data->free(data_ptr);
        data->free(list_ptr);
        data->free(mode_ptr);
        file->free(list_ptr);
        list->peek(list_ptr);
        list->push(list_ptr, f_ptr);
        file->free(f_ptr);
        list->pop(0);
        list->pop(1);
        list->pop(2);
        list->pop(3);
        list->pop(4);
        list->pop(5);
        list->pop(6);
        list->pop(7);
        list->pop(8);
        list->pop(9);
        list->push(f_ptr, f_ptr);
        list->push(list_ptr, f_ptr);
        list->pop(0);
        list->pop(1);
        list->pop(2);
        list->pop(3);
        list->pop(4);
        list->pop(5);
        list->pop(6);
        list->pop(7);
        list->pop(8);
        list->pop(9);
        list->push(f_ptr, f_ptr);
        list->push(list_ptr, f_ptr);
        list->push(data_ptr, f_ptr);
        list->peek(f_ptr);
        list->peek(list_ptr);
        list->peek(data_ptr);
        list->pop(f_ptr);
        list->pop(list_ptr);
        list->pop(data_ptr);
        file->data(f_ptr);
        file->data(list_ptr);
        file->data(data_ptr);
        string->free(f_ptr);
        string->free(list_ptr);
        string->free(data_ptr);
        list->free(f_ptr);
        data->free(data_ptr);
        list->push(f_ptr, data_ptr);
        list->push(list_ptr, data_ptr);
        list->push(data_ptr, data_ptr);
        idx1 = string->copy(list_ptr);
        list->free(list_ptr);
        idx2 = string->copy(list_ptr);
        list->free(f_ptr);
        data->free(data_ptr);
        string->free(list_ptr);
        string->free(f_ptr);
    }
    file->free(f_ptr);
    list->peek(f_ptr);
    list->push(f_ptr, f_ptr);
    file->free(f_ptr);
    list->free(f_ptr);
    string->free(f_ptr);
    string->free(mode_ptr);
    string->free(file_name_ptr);
    string->free(file_path_ptr);
#ifdef USE_GC
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
    u64 list_ptr = list->alloc();
    u64 pattern_ptr = string->alloc();
    u64 list_size = list->size(list_ptr);
    u64 ptr_size = object->size(pattern_ptr);
    u64 null_size = string->size(0);
    list->free(pattern_ptr);
    list->free(list_ptr);
#ifndef USE_GC
    list->free(list_ptr);
    string->free(pattern_ptr);
#else
    pointer->gc();
#endif
    RX_ASSERT(list_size == 8);
    RX_ASSERT(ptr_size == 0);
    RX_ASSERT(null_size == 0);
}

/* test init */
RX_TEST_CASE(tests, test_alloc_free_list, .fixture = test_fixture) {
    u64 list_ptr = list->alloc();
    u64 size = list->size(list_ptr);
    RX_ASSERT(size == 8);
    u64 pattern_ptr = string->alloc();
    list->free(pattern_ptr);
    list->free(list_ptr);
    string->free(list_ptr);
    string->free(pattern_ptr);
#ifdef USE_GC
    pointer->gc();
#endif
}

/* test init */
RX_TEST_CASE(tests, test_load_open_file_unsafe_hashtable_default_hash, .fixture = test_fixture) {
    hashtable->init(0x3);
    hashtable->setup(murmurhash3);
    u64 file_path_ptr = string->getcwd();
    u64 file_name_ptr = string->load("/all_english_words.txt");
    string->strcat(file_path_ptr, file_name_ptr);
    string->free(file_name_ptr);
    u64 mode_ptr = string->load("rb");
    u64 f_ptr = file->alloc(file_path_ptr, mode_ptr);
    if (f_ptr != 0) {
        u64 data_ptr = file->data(f_ptr);
        u64 list_ptr = list->alloc();
        file->free(f_ptr);
        const u64 size = 0xfff;
        char* file_data;
        const char* file_end;
        file_data = data->unsafe(data_ptr);
        file_end = file_data + size;
        while (file_data < file_end) {
            char* tmp = file_data;
            while (*tmp != 0 && *tmp != '\n') {
                tmp++;
            }
            *tmp++ = '\0';
            u64 string_ptr = string->load(file_data);
            list->push(list_ptr, string_ptr);
            const char* unsafe = string->unsafe(string_ptr);
            struct hashtable_data* unsafe_tmp = hashtable->alloc(unsafe, 0);
            hashtable->free(unsafe_tmp);
            file_data = tmp;
        }
        file_data = data->unsafe(data_ptr);
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
        list->free(list_ptr);
        data->free(data_ptr);
    }
    string->free(mode_ptr);
    string->free(file_name_ptr);
    string->free(file_path_ptr);
    hashtable->destroy();
}

/* test init */
RX_TEST_CASE(tests, test_load_open_file_unsafe_hashtable_murmurhash3_hash, .fixture = test_fixture) {
    u64 file_path_ptr = string->getcwd();
    u64 file_name_ptr = string->load("/all_english_words.txt");
    string->strcat(file_path_ptr, file_name_ptr);
    string->free(file_name_ptr);
    u64 mode_ptr = string->load("rb");
    u64 f_ptr = file->alloc(file_path_ptr, mode_ptr);
    if (f_ptr != 0) {
        u64 data_ptr = file->data(f_ptr);
        u64 list_ptr = list->alloc();
        file->free(f_ptr);
        u64 size = data->size(data_ptr);
        CLEAN(size)
        char* file_data = data->unsafe(data_ptr);
        const char* file_end = file_data + 0xffff;
        while (file_data < file_end) {
            char* tmp = file_data;
            while (*tmp != 0 && *tmp != '\n') {
                tmp++;
            }
            *tmp++ = '\0';
            u64 string_ptr = string->load(file_data);
            list->push(list_ptr, string_ptr);
            char* unsafe = string->unsafe(string_ptr);
            printf("%s\n", unsafe);
            file_data = tmp;
        }
        list->free(list_ptr);
        data->free(data_ptr);
    }
    string->free(mode_ptr);
    string->free(file_name_ptr);
    string->free(file_path_ptr);
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
    const struct hashtable_data* key_value1 = hashtable->get(key1);
    RX_ASSERT(key_value1 != 0);
    RX_ASSERT(strcmp(key_value1->value, value3) == 0);
    hashtable->set(key1, value2);
    const struct hashtable_data* key_value2 = hashtable->get(key1);
    RX_ASSERT(key_value2 != 0);
    RX_ASSERT(strcmp(key_value2->value, value2) == 0);
    hashtable->set(key1, value1);
    const struct hashtable_data* key_value3 = hashtable->get(key1);
    RX_ASSERT(key_value3 != 0);
    RX_ASSERT(strcmp(key_value3->value, value1) == 0);
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

int main(int argc, char** argv) {
#ifndef ATTRIBUTE
    init();
#endif
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
#ifndef ATTRIBUTE
    destroy();
#endif
    return result;
}
