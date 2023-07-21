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

#include "rexo/include/rexo.h"

#define DEFAULT_SIZE 0x100

/* definition */
extern void pointer_ctx_init(struct pointer_data** ctx, u64 size);
extern void pointer_ctx_destroy(struct pointer_data** ctx);

extern const struct vm_methods vm_methods_definition;

extern const struct pointer_methods pointer_methods_definition;
extern const struct list_methods list_methods_definition;
extern const struct file_methods file_methods_definition;
extern const struct string_methods string_methods_definition;
extern const struct data_methods data_methods_definition;
extern const struct object_methods object_methods_definition;

const struct pointer_methods* pointer = &pointer_methods_definition;
const struct list_methods* list = &list_methods_definition;
const struct file_methods* file = &file_methods_definition;
const struct string_methods* string = &string_methods_definition;
const struct data_methods* data = &data_methods_definition;
const struct object_methods* object = &object_methods_definition;

typedef struct test_data {
    struct pointer_data* ctx;
}* TEST_DATA;

RX_SET_UP(test_set_up) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct pointer_data** ctx = &rx->ctx;
    pointer_ctx_init(ctx, 8);
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
RX_TEST_CASE(tests, test_print_0, .fixture = test_fixture) {
    string->printf(0);
}

/* test init */
RX_TEST_CASE(tests, test_load_0, .fixture = test_fixture) {
    u64 empty_ptr = string->load(0);
    RX_ASSERT(empty_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests, test_load_empty, .fixture = test_fixture) {
    u64 empty_ptr = string->load("");
    RX_ASSERT(empty_ptr == 0);
    string->free(empty_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_print_load_empty, .fixture = test_fixture) {
    u64 empty_ptr = string->load("");
    string->printf(empty_ptr);
    RX_ASSERT(empty_ptr == 0);
    string->free(empty_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_load_copy, .fixture = test_fixture) {
    u64 char_ptr = string->load("/");
    u64 copy_ptr = string->copy(char_ptr);
    RX_ASSERT(char_ptr != 0);
    RX_ASSERT(copy_ptr != 0);
    RX_ASSERT(char_ptr != copy_ptr);
    string->free(char_ptr);
    string->free(copy_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_load_push_peek_pop, .fixture = test_fixture) {
    u64 char_ptr = string->load("/");
    pointer->push(char_ptr);
    u64 peek_ptr = pointer->peek();
    u64 pop_ptr = pointer->pop();
    RX_ASSERT(char_ptr != 0);
    RX_ASSERT(peek_ptr != 0);
    RX_ASSERT(pop_ptr != 0);
    string->free(char_ptr);
    string->free(peek_ptr);
    string->free(pop_ptr);
}

RX_TEST_CASE(tests, test_load_free_free, .fixture = test_fixture) {
    u64 char_ptr = string->load("/");
    RX_ASSERT(char_ptr != 0);
    string->free(char_ptr);
    string->free(char_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_strcat_load_alloc, .fixture = test_fixture) {
    u64 char_ptr = string->load("/");
    u64 pattern_ptr = string->alloc();
    string->strcat(pattern_ptr, char_ptr);
    RX_ASSERT(char_ptr != 0);
    RX_ASSERT(pattern_ptr != 0);
    string->free(char_ptr);
    string->free(pattern_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_string_size, .fixture = test_fixture) {
    u64 char_ptr = string->load("/");
    u64 pattern_ptr = string->alloc();
    u64 string_size = data->size(pattern_ptr);
    RX_ASSERT(string_size == 0);
    RX_ASSERT(char_ptr != 0);
    RX_ASSERT(pattern_ptr != 0);
    string->free(char_ptr);
    string->free(pattern_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_pointer_size_0, .fixture = test_fixture) {
    u64 char_ptr = string->load("/");
    u64 pattern_ptr = string->alloc();
    u64 string_size = string->size(0);
    RX_ASSERT(string_size == 0);
    RX_ASSERT(char_ptr != 0);
    RX_ASSERT(pattern_ptr != 0);
    string->free(char_ptr);
    string->free(pattern_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_pointer_size_empty, .fixture = test_fixture) {
    u64 char_ptr = string->load(" ");
    u64 pattern_ptr = string->alloc();
    u64 string_size = string->size(char_ptr);
    RX_ASSERT(string_size == 2);
    RX_ASSERT(char_ptr != 0);
    RX_ASSERT(pattern_ptr != 0);
    string->free(char_ptr);
    string->free(pattern_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_pointer_size_object, .fixture = test_fixture) {
    u64 object_ptr = object->alloc(1);
    u64 pattern_ptr = string->alloc();
    u64 object_size = object->size(object_ptr);
    RX_ASSERT(object_size == 1);
    RX_ASSERT(object_ptr != 0);
    RX_ASSERT(pattern_ptr != 0);
    string->free(object_ptr);
    string->free(pattern_ptr);
    object->free(object_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_object_0, .fixture = test_fixture) {
    u64 object_ptr = object->alloc(0);
    u64 pattern_ptr = string->alloc();
    u64 object_size = object->size(object_ptr);
    RX_ASSERT(object_size == 0);
    RX_ASSERT(object_ptr == 0);
    RX_ASSERT(pattern_ptr != 0);
    string->free(object_ptr);
    string->free(pattern_ptr);
    object->free(object_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_object_alloc_0, .fixture = test_fixture) {
    u64 object_ptr = object->alloc(0);
    u64 pattern_ptr = string->alloc();
    u64 object_size = object->size(object_ptr);
    RX_ASSERT(object_size == 0);
    RX_ASSERT(object_ptr == 0);
    RX_ASSERT(pattern_ptr != 0);
    string->free(object_ptr);
    string->free(pattern_ptr);
    object->free(object_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_object_load_0, .fixture = test_fixture) {
    u64 object_ptr = object->load(0, 0x01234567);
    u64 pattern_ptr = string->alloc();
    u64 object_size = object->size(object_ptr);
    RX_ASSERT(object_size == 0);
    RX_ASSERT(object_ptr == 0);
    RX_ASSERT(pattern_ptr != 0);
    string->free(object_ptr);
    string->free(pattern_ptr);
    object->free(object_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_object_load_string_0, .fixture = test_fixture) {
    const char* test_data = "Hello, world!";
    const void* ptr = (const void*)test_data;
    u64 object_ptr = object->load(ptr, 0);
    u64 pattern_ptr = string->alloc();
    u64 object_size = object->size(object_ptr);
    RX_ASSERT(object_size == 0);
    RX_ASSERT(object_ptr == 0);
    RX_ASSERT(pattern_ptr != 0);
    string->free(object_ptr);
    string->free(pattern_ptr);
    object->free(object_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_object_load_string, .fixture = test_fixture) {
    const char* test_data = "Hello, world!";
    const void* ptr = (const void*)test_data;
    u64 object_ptr = object->load(ptr, strlen(ptr));
    u64 pattern_ptr = string->alloc();
    u64 object_size = object->size(object_ptr);
    RX_ASSERT(object_size == strlen(ptr));
    RX_ASSERT(object_ptr != 0);
    RX_ASSERT(pattern_ptr != 0);
    string->free(object_ptr);
    string->free(pattern_ptr);
    object->free(object_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_object_load_string_unsafe_alloc_0, .fixture = test_fixture) {
    u64 object_ptr = object->alloc(0);
    u64 pattern_ptr = string->alloc();
    u64 object_size = object->size(object_ptr);
    const char* ch = (char*)object->unsafe(object_ptr);
    RX_ASSERT(ch == 0);
    RX_ASSERT(object_size == 0);
    RX_ASSERT(object_ptr == 0);
    RX_ASSERT(pattern_ptr != 0);
    string->free(object_ptr);
    string->free(pattern_ptr);
    object->free(object_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_object_load_string_unsafe_string, .fixture = test_fixture) {
    u64 object_ptr = object->alloc(0);
    u64 pattern_ptr = string->alloc();
    u64 object_size = object->size(object_ptr);
    const char* ch = (char*)object->unsafe(pattern_ptr);
    RX_ASSERT(ch == 0);
    RX_ASSERT(object_size == 0);
    RX_ASSERT(object_ptr == 0);
    RX_ASSERT(pattern_ptr != 0);
    string->free(object_ptr);
    string->free(pattern_ptr);
    object->free(object_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_object_load_string_unsafe_0, .fixture = test_fixture) {
    const char* test_data = "Hello, world!";
    const void* ptr = (const void*)test_data;
    u64 object_ptr = object->load(ptr, strlen(ptr));
    u64 pattern_ptr = string->alloc();
    u64 object_size = object->size(object_ptr);
    const char* ch = (char*)object->unsafe(0);
    RX_ASSERT(ch == 0);
    RX_ASSERT(object_size == strlen(ptr));
    RX_ASSERT(object_ptr != 0);
    RX_ASSERT(pattern_ptr != 0);
    string->free(object_ptr);
    string->free(pattern_ptr);
    object->free(object_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_object_load_string_unsafe, .fixture = test_fixture) {
    const char* test_data = "Hello, world!";
    const void* ptr = (const void*)test_data;
    u64 object_ptr = object->load(ptr, strlen(ptr));
    u64 pattern_ptr = string->alloc();
    u64 object_size = object->size(object_ptr);
    char* ch = (char*)object->unsafe(object_ptr);
    *ch = 'Z';
    RX_ASSERT(object_size == strlen(ptr));
    RX_ASSERT(object_ptr != 0);
    RX_ASSERT(pattern_ptr != 0);
    string->free(object_ptr);
    string->free(pattern_ptr);
    object->free(object_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_strcat_alloc_alloc, .fixture = test_fixture) {
    u64 char_ptr = string->alloc();
    u64 pattern_ptr = string->alloc();
    string->strcat(pattern_ptr, char_ptr);
    RX_ASSERT(char_ptr != 0);
    RX_ASSERT(pattern_ptr != 0);
    string->free(char_ptr);
    string->free(pattern_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_strcat_load_alloc_copy, .fixture = test_fixture) {
    u64 string_ptr = string->load("/all_english_words.txt");
    u64 zero_ptr = string->alloc();
    u64 data_ptr = data->alloc(1);
    u64 list_ptr = list->alloc();
    u64 empty_ptr = string->load("\0");

    u64 null_ptr = 0;
    u64 none_ptr = 0xfffffffff;

    u64 data_ptr1 = string->copy(null_ptr);
    u64 data_ptr2 = string->copy(zero_ptr);
    u64 data_ptr3 = string->copy(string_ptr);
    u64 data_ptr4 = string->copy(none_ptr);

    string->printf(string_ptr);
    string->printf(zero_ptr);
    string->printf(data_ptr);
    string->printf(list_ptr);
    string->printf(empty_ptr);
    string->printf(null_ptr);
    string->printf(none_ptr);

    string->put_char(string_ptr, 'a');
    string->put_char(zero_ptr, 'a');
    string->put_char(data_ptr, 'a');
    string->put_char(list_ptr, 'a');
    string->put_char(empty_ptr, 'a');
    string->put_char(null_ptr, 'a');
    string->put_char(none_ptr, 'a');

    string->put_char(string_ptr, '\0');
    string->put_char(zero_ptr, '\0');
    string->put_char(data_ptr, '\0');
    string->put_char(list_ptr, '\0');
    string->put_char(empty_ptr, '\0');
    string->put_char(null_ptr, '\0');
    string->put_char(none_ptr, '\0');

    string->put_char(string_ptr, '/');
    string->strcpy(string_ptr, string_ptr);
    string->strcpy(string_ptr, zero_ptr);
    string->strcpy(zero_ptr, string_ptr);
    string->strcpy(data_ptr, null_ptr);
    string->strcpy(list_ptr, null_ptr);
    string->strcpy(string_ptr, data_ptr);
    string->strcpy(string_ptr, empty_ptr);
    string->strcpy(data_ptr, null_ptr);
    string->strcpy(list_ptr, string_ptr);
    string->strcpy(data_ptr, null_ptr);
    string->strcpy(list_ptr, null_ptr);
    string->strcpy(string_ptr, data_ptr);
    string->strcpy(data_ptr, null_ptr);
    string->strcpy(list_ptr, string_ptr);
    string->strcpy(null_ptr, null_ptr);
    string->strcpy(string_ptr, null_ptr);
    string->strcpy(string_ptr, none_ptr);
    string->strcpy(none_ptr, string_ptr);

    string->strcat(string_ptr, string_ptr);
    string->strcat(string_ptr, zero_ptr);
    string->strcat(zero_ptr, string_ptr);
    string->strcat(data_ptr, null_ptr);
    string->strcat(list_ptr, null_ptr);
    string->strcat(string_ptr, data_ptr);
    string->strcat(string_ptr, empty_ptr);
    string->strcat(data_ptr, null_ptr);
    string->strcat(list_ptr, string_ptr);
    string->strcat(data_ptr, null_ptr);
    string->strcat(list_ptr, null_ptr);
    string->strcat(string_ptr, data_ptr);
    string->strcat(data_ptr, null_ptr);
    string->strcat(list_ptr, string_ptr);
    string->strcat(null_ptr, null_ptr);
    string->strcat(string_ptr, null_ptr);
    string->strcat(string_ptr, none_ptr);
    string->strcat(none_ptr, string_ptr);

    string->match_last(string_ptr, string_ptr);
    string->match_last(data_ptr, null_ptr);
    string->match_last(list_ptr, null_ptr);
    string->match_last(string_ptr, data_ptr);
    string->match_last(string_ptr, empty_ptr);
    string->match_last(data_ptr, null_ptr);
    string->match_last(list_ptr, string_ptr);
    string->match_last(data_ptr, null_ptr);
    string->match_last(list_ptr, null_ptr);
    string->match_last(string_ptr, data_ptr);
    string->match_last(data_ptr, null_ptr);
    string->match_last(list_ptr, string_ptr);
    string->match_last(null_ptr, null_ptr);
    string->match_last(string_ptr, null_ptr);
    string->match_last(string_ptr, string_ptr);
    string->match_last(string_ptr, zero_ptr);
    string->match_last(data_ptr, zero_ptr);
    string->match_last(list_ptr, zero_ptr);
    string->match_last(string_ptr, data_ptr);
    string->match_last(zero_ptr, zero_ptr);
    string->match_last(zero_ptr, data_ptr);
    string->match_last(zero_ptr, list_ptr);
    string->match_last(zero_ptr, string_ptr);
    string->match_last(empty_ptr, empty_ptr);
    string->match_last(empty_ptr, string_ptr);
    string->match_last(empty_ptr, zero_ptr);

    string->match_last(null_ptr, null_ptr);
    string->match_last(null_ptr, empty_ptr);
    string->match_last(null_ptr, string_ptr);
    string->match_last(null_ptr, data_ptr);
    string->match_last(null_ptr, list_ptr);
    string->match_last(empty_ptr, null_ptr);
    string->match_last(empty_ptr, empty_ptr);
    string->match_last(empty_ptr, string_ptr);
    string->match_last(empty_ptr, data_ptr);
    string->match_last(empty_ptr, list_ptr);
    string->match_last(string_ptr, null_ptr);
    string->match_last(string_ptr, empty_ptr);
    string->match_last(string_ptr, string_ptr);
    string->match_last(string_ptr, data_ptr);
    string->match_last(string_ptr, list_ptr);
    string->match_last(data_ptr, null_ptr);
    string->match_last(data_ptr, empty_ptr);
    string->match_last(data_ptr, string_ptr);
    string->match_last(data_ptr, data_ptr);
    string->match_last(data_ptr, list_ptr);
    string->match_last(list_ptr, null_ptr);
    string->match_last(list_ptr, empty_ptr);
    string->match_last(list_ptr, string_ptr);
    string->match_last(list_ptr, data_ptr);
    string->match_last(list_ptr, list_ptr);
    string->match_last(string_ptr, none_ptr);
    string->match_last(none_ptr, string_ptr);

    string->unsafe(null_ptr);
    string->unsafe(empty_ptr);
    string->unsafe(string_ptr);
    string->unsafe(data_ptr);
    string->unsafe(list_ptr);
    string->unsafe(none_ptr);

    file->alloc(zero_ptr, zero_ptr);
    file->alloc(string_ptr, zero_ptr);
    file->alloc(data_ptr, zero_ptr);
    file->alloc(list_ptr, zero_ptr);
    file->alloc(zero_ptr, string_ptr);
    file->alloc(zero_ptr, data_ptr);
    file->alloc(zero_ptr, list_ptr);
    file->alloc(string_ptr, string_ptr);
    file->alloc(data_ptr, null_ptr);
    file->alloc(list_ptr, null_ptr);
    file->alloc(string_ptr, data_ptr);
    file->alloc(string_ptr, empty_ptr);
    file->alloc(data_ptr, null_ptr);
    file->alloc(list_ptr, string_ptr);
    file->alloc(data_ptr, null_ptr);
    file->alloc(list_ptr, null_ptr);
    file->alloc(string_ptr, data_ptr);
    file->alloc(data_ptr, null_ptr);
    file->alloc(list_ptr, string_ptr);
    file->alloc(null_ptr, null_ptr);
    file->alloc(string_ptr, null_ptr);
    file->alloc(string_ptr, none_ptr);
    file->alloc(none_ptr, string_ptr);

    const u64* data_unsafe_ptr1 = data->unsafe(empty_ptr);
    const u64* data_unsafe_ptr2 = data->unsafe(null_ptr);
    const u64* data_unsafe_ptr3 = data->unsafe(string_ptr);
    const u64* data_unsafe_ptr4 = data->unsafe(list_ptr);
    const u64* data_unsafe_ptr5 = data->unsafe(none_ptr);

    RX_ASSERT(data_unsafe_ptr1 == 0);
    RX_ASSERT(data_unsafe_ptr2 == 0);
    RX_ASSERT(data_unsafe_ptr3 == 0);
    RX_ASSERT(data_unsafe_ptr4 == 0);
    RX_ASSERT(data_unsafe_ptr5 == 0);

    RX_ASSERT(string_ptr != 0);
    RX_ASSERT(data_ptr1 == 0);
    RX_ASSERT(data_ptr2 == 0);
    RX_ASSERT(data_ptr3 != 0);
    RX_ASSERT(data_ptr3 != 0);
    RX_ASSERT(data_ptr4 == 0);

    u64 undefined_ptr = string->copy(list_ptr);

    RX_ASSERT(undefined_ptr == 0);

    string->free(null_ptr);
    string->free(zero_ptr);
    string->free(string_ptr);
    string->free(data_ptr3);
    string->free(list_ptr);
    string->free(empty_ptr);
    string->free(data_ptr);
    string->free(string_ptr);
    string->free(data_ptr3);
    string->free(empty_ptr);
    string->free(data_ptr);

    data->free(zero_ptr);
    data->free(data_ptr);
    data->free(data_ptr3);
    data->free(empty_ptr);
    data->free(null_ptr);
    data->free(data_ptr);
    data->free(list_ptr);
    data->free(data_ptr);
    data->free(data_ptr3);
    data->free(none_ptr);
    list->free(zero_ptr);
    list->free(data_ptr);
    list->free(empty_ptr);
    list->free(null_ptr);
    list->free(data_ptr);
    list->free(list_ptr);
    list->free(data_ptr);
    list->free(list_ptr);
    list->free(none_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_strcat_alloc_load, .fixture = test_fixture) {
    u64 zero_ptr = string->alloc();
    u64 char_ptr = string->load("/");
    string->strcat(zero_ptr, char_ptr);
    RX_ASSERT(zero_ptr != 0);
    RX_ASSERT(char_ptr != 0);
    string->free(zero_ptr);
    string->free(char_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_strcat_load_alloc_alloc, .fixture = test_fixture) {
    u64 char_ptr = string->load("/");
    u64 pattern_ptr = string->alloc();
    u64 empty_ptr = string->alloc();
    string->strcat(pattern_ptr, char_ptr);
    string->strcpy(empty_ptr, pattern_ptr);
    RX_ASSERT(char_ptr != 0);
    RX_ASSERT(pattern_ptr != 0);
    RX_ASSERT(empty_ptr != 0);
    string->free(empty_ptr);
    string->free(pattern_ptr);
    string->free(char_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_strcat_load_load, .fixture = test_fixture) {
    u64 char_ptr = string->load("/");
    u64 pattern_ptr = string->load("*");
    string->strcat(pattern_ptr, char_ptr);
    RX_ASSERT(pattern_ptr != 0);
    RX_ASSERT(char_ptr != 0);
    string->free(pattern_ptr);
    string->free(char_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_strcpy_load_alloc, .fixture = test_fixture) {
    u64 pattern_ptr = string->load("/");
    u64 empty_ptr = string->alloc();
    string->strcpy(empty_ptr, pattern_ptr);
    RX_ASSERT(pattern_ptr != 0);
    RX_ASSERT(empty_ptr != 0);
    string->free(empty_ptr);
    string->free(pattern_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_strcpy_load_load, .fixture = test_fixture) {
    u64 char_ptr = string->load("/input.txt");
    u64 pattern_ptr = string->load("*");
    string->strcpy(pattern_ptr, char_ptr);
    RX_ASSERT(pattern_ptr != 0);
    RX_ASSERT(char_ptr != 0);
    string->free(pattern_ptr);
    string->free(char_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_load_open_file_close_file, .fixture = test_fixture) {
    u64 file_path_ptr = string->getcwd();
    u64 file_name_ptr = string->load("/nonexistent.txt");
    string->strcat(file_path_ptr, file_name_ptr);
    string->free(file_name_ptr);
    u64 mode_ptr = string->load("rb");
    u64 f_ptr = file->alloc(file_path_ptr, mode_ptr);
    u64 data_ptr = file->data(f_ptr);
    RX_ASSERT(f_ptr == 0);
    RX_ASSERT(data_ptr == 0);
    file->free(f_ptr);
    data->free(data_ptr);
    string->free(mode_ptr);
    string->free(file_name_ptr);
    string->free(file_path_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_file_read_invalid_type, .fixture = test_fixture) {
    u64 list_ptr = list->alloc();
    u64 data_ptr = file->data(list_ptr);
    data->free(data_ptr);
    file->free(list_ptr);
    list->free(list_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_load_open_match_last_unsafe_free, .fixture = test_fixture) {
    u64 file_path_ptr = string->getcwd();
    u64 file_name_ptr = string->load("/all_english_words.txt//");
    string->strcat(file_path_ptr, file_name_ptr);
    u64 pattern_ptr = string->load("//");
    u64 last_match_ptr = string->match_last(file_path_ptr, pattern_ptr);
    string->free(pattern_ptr);
    string->put_char(last_match_ptr, '\0');
    string->free(last_match_ptr);
    string->strcat(file_path_ptr, file_name_ptr);
    string->free(file_name_ptr);
    u64 mode_ptr = string->load("rb");
    u64 f_ptr = file->alloc(file_path_ptr, mode_ptr);
    string->free(file_path_ptr);
    string->free(mode_ptr);
    u64 data_ptr = file->data(f_ptr);
    file->free(f_ptr);
    string->printf(data_ptr);
    data->free(data_ptr);
}

/* test init */
RX_TEST_CASE(tests, test_load_open_file_unsafe_hashtable, .fixture = test_fixture) {
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
        u8* file_data = data->unsafe(data_ptr);
        for (int i = 0; i < 100; i++) {
            u8* tmp = file_data;
            while (*tmp != 0 && *tmp != '\n') {
                tmp++;
            }
            *tmp++ = '\0';
            u64 string_ptr = string->load((char*)file_data);
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

RX_TEST_CASE(tests, test_load_load_match_last, .fixture = test_fixture) {
    u64 str_ptr = string->load("Hello, world!");
    u64 ch_ptr = string->load("z");
    u64 last_match_ptr = string->match_last(str_ptr, ch_ptr);
    string->free(last_match_ptr);
    string->free(str_ptr);
    string->free(ch_ptr);
    RX_ASSERT(last_match_ptr == 0);
}

extern inline void source1(void) {
    u64 file_path_ptr = pointer->pop();
    u64 file_name_ptr = string->load("/input.txt");
    u64 pattern_ptr = string->load("/");
    u64 last_match_ptr = string->match_last(file_path_ptr, pattern_ptr);
    string->free(pattern_ptr);
    string->put_char(last_match_ptr, '\0');
    string->free(last_match_ptr);
    string->strcat(file_path_ptr, file_name_ptr);
    string->free(file_name_ptr);
    u64 mode_ptr = string->load("rb");
    u64 f_ptr = file->alloc(file_path_ptr, mode_ptr);
    string->free(file_path_ptr);
    string->free(mode_ptr);
    u64 data_ptr = file->data(f_ptr);
    file->free(f_ptr);
    string->printf(data_ptr);
    data->free(data_ptr);
}

extern inline void source2(void) {
    u64 file_path_ptr = string->getcwd();
    u64 file_name_ptr = string->load("/all_english_words.txt");
    string->strcat(file_path_ptr, file_name_ptr);
    string->free(file_name_ptr);
    u64 mode_ptr = string->load("rb");
    u64 f_ptr = file->alloc(file_path_ptr, mode_ptr);
    if (f_ptr != 0) {
        u64 data_ptr = file->data(f_ptr);
        u64 size = data->size(data_ptr);
        if (size > 100) {
            size = 100;
        }
        u64 list_ptr = list->alloc();
        file->free(f_ptr);
        u8* file_data = data->unsafe(data_ptr);
        for (u64 i = 0; i < size; i++) {
            u8* tmp = file_data;
            while (*tmp != 0 && *tmp != '\n') {
                tmp++;
            }
            *tmp++ = '\0';
            u64 string_ptr = string->load((char*)file_data);
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
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- acceptance test code\n");
#endif
    pointer->init(DEFAULT_SIZE);
    u64 argv_ptr = string->load(argv[0]);
    pointer->push(argv_ptr);
    source1();
    source2();
    string->free(argv_ptr);
    pointer->destroy();
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
