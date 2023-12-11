/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   11 December 2023 at 9:15:15 GMT+3
 *
 */
/*
    Copyright (C) 2022-2047 Artur Mustafin (artur.mustafin@gmail.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#define RXP_DEBUG_TESTS

#include "../tests/src/test.h"

#define INLINE

#include "common/memory.h"
#include "list-micro/data.h"

#include "playground/brain/brain.h"

#include "pointer/types/data/v1/data_v1.h"
#include "pointer/types/file/v1/file_v1.h"
#include "pointer/types/list/v1/list_v1.h"
#include "pointer/types/string/v1/string_v1.h"
#include "pointer/v1/pointer_v1.h"

#include "pointer/os/v1/os_v1.h"

extern const struct test_suite vm_v1_test_suite_definition;
static const struct test_suite* vm_v1_tests = &vm_v1_test_suite_definition;

extern const struct test_suite list_micro_test_suite_definition;
static const struct test_suite* list_micro_tests = &list_micro_test_suite_definition;

extern const struct test_suite list_alloc_test_suite_definition;
static const struct test_suite* list_alloc_tests = &list_alloc_test_suite_definition;

#define DEFAULT_SIZE 0x100

typedef struct test_data {
    struct pointer_data* ctx;
}* TEST_DATA;

u64 _list_ptr;

extern inline void source1(void) {
    u64 file_path_ptr = list->pop(_list_ptr);
    u64 file_name_ptr = string->load("/input.txt");
    u64 pattern_ptr = string->load("/");
    u64 last_match_ptr = string->offset(file_path_ptr, pattern_ptr);
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
    os->putc(data_ptr);
    data->free(data_ptr);
}

extern inline void source2(void) {
    u64 file_path_ptr = os->getcwd();
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
            printf("   .: %s\n", unsafe);
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
#ifdef USE_MEMORY_DEBUG_INFO
#if defined(VM_GLOBAL_DEBUG_INFO)
    global_statistics();
#endif
#endif
    TEST_RUN(alloc, list_alloc_tests);
    TEST_RUN(micro, list_micro_tests);
    TEST_RUN(vm_v1, vm_v1_tests);
    CLEAN(argc)
    pointer->init(DEFAULT_SIZE);
    _list_ptr = list->alloc();
    u64 argv_ptr = string->load(argv[0]);
    list->push(_list_ptr, argv_ptr);
    source1();
    source2();
    string->free(argv_ptr);
    list->free(_list_ptr);
    pointer->destroy();
#ifdef USE_MEMORY_DEBUG_INFO
#if defined(VM_GLOBAL_DEBUG_INFO)
    global_statistics();
#endif
#endif
    return alloc | micro | vm_v1;
}
