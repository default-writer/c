/*
 *
 * MIT License
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
#include "pointer/types/string/v1/string_v1.h"
#include "pointer/types/string_pointer/v1/string_pointer_v1.h"
#include "pointer/v1/pointer_v1.h"

#include "../tests/src/test.h"

extern const struct test_suite vm_v1_test_suite_definition;
static const struct test_suite* vm_v1_tests = &vm_v1_test_suite_definition;

extern const struct test_suite list_micro_test_suite_definition;
static const struct test_suite* list_micro_tests = &list_micro_test_suite_definition;

extern const struct test_suite list_alloc_test_suite_definition;
static const struct test_suite* list_alloc_tests = &list_alloc_test_suite_definition;

#define DEFAULT_SIZE 0x100

/* definition */
extern const struct vm_methods vm_methods_definition;

extern const struct pointer_methods pointer_methods_definition;
extern const struct list_methods list_methods_definition;
extern const struct file_methods file_methods_definition;
extern const struct string_methods string_methods_definition;
extern const struct string_pointer_methods string_pointer_methods_definition;
extern const struct data_methods data_methods_definition;

const struct pointer_methods* pointer = &pointer_methods_definition;
const struct list_methods* list = &list_methods_definition;
const struct file_methods* file = &file_methods_definition;
const struct string_methods* string = &string_methods_definition;
const struct string_pointer_methods* string_pointer = &string_pointer_methods_definition;
const struct data_methods* data = &data_methods_definition;

typedef struct test_data {
    struct pointer_data* ctx;
}* TEST_DATA;

extern inline void source1(void) {
    u64 file_path_ptr = pointer->pop();
    u64 file_name_ptr = string->load("/input.txt");
    u64 pattern_ptr = string->load("/");
    u64 last_match_ptr = string->offset(file_path_ptr, pattern_ptr);
    string->free(pattern_ptr);
    string->put_char(last_match_ptr, '\0');
    string_pointer->free(last_match_ptr);
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
        char* file_data = data->unsafe(data_ptr);
        for (u64 i = 0; i < size; i++) {
            char* tmp = file_data;
            while (*tmp != 0 && *tmp != '\n') {
                tmp++;
            }
            *tmp++ = '\0';
            u64 string_ptr = string->load(file_data);
            list->push(list_ptr, string_ptr);
            char* unsafe = string->unsafe(string_ptr);
            printf("   +: %s\n", unsafe);
            file_data = tmp;
        }
        list->free(list_ptr);
        data->free(data_ptr);
    }
    string->free(mode_ptr);
    string->free(file_name_ptr);
    string->free(file_path_ptr);
}

int main(int argc, char** argv) {
    global_statistics();
    int alloc = list_alloc_tests->run();
    int micro = list_micro_tests->run();
    int vm_v1 = vm_v1_tests->run();
    CLEAN(argc)
    pointer->init(DEFAULT_SIZE);
    u64 argv_ptr = string->load(argv[0]);
    pointer->push(argv_ptr);
    source1();
    source2();
    string->free(argv_ptr);
    pointer->destroy();
    global_statistics();
    return alloc | micro | vm_v1;
}
