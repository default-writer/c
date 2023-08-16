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
#include "playground/hashtable/v2/hashtable_v2.h"
#include "pointer/v2/pointer_v2.h"

#define DEFAULT_SIZE 0x100

/* definition */
extern const struct vm_methods vm_methods_definition;

extern const struct pointer_methods pointer_methods_definition;
extern const struct list_methods list_methods_definition;
extern const struct file_methods file_methods_definition;
extern const struct debug_methods debug_methods_definition;

static const struct pointer_methods* pointer = &pointer_methods_definition;
static const struct list_methods* list = &list_methods_definition;
static const struct file_methods* file = &file_methods_definition;
static const struct debug_methods* debug = &debug_methods_definition;

typedef struct test_data {
    struct pointer_data* ctx;
}* TEST_DATA;

static void source1(void) {
    u64 file_path_ptr = pointer->pop();
    u64 file_name_ptr = pointer->load("/input.txt");
    u64 pattern_ptr = pointer->load("/");
    u64 last_match_ptr = pointer->match_last(file_path_ptr, pattern_ptr);
#ifndef USE_GC
    pointer->free(pattern_ptr);
#endif
    pointer->put_char(last_match_ptr, '\0');
#ifndef USE_GC
    pointer->free(last_match_ptr);
#endif
    pointer->strcat(file_path_ptr, file_name_ptr);
#ifndef USE_GC
    pointer->free(file_name_ptr);
#endif
    u64 mode_ptr = pointer->load("rb");
    u64 f_ptr = file->file_alloc(file_path_ptr, mode_ptr);
#ifndef USE_GC
    pointer->free(file_path_ptr);
    pointer->free(mode_ptr);
#endif
    u64 data_ptr = file->file_read(f_ptr);
    file->file_free(f_ptr);
    pointer->printf(data_ptr);
#ifndef USE_GC
    pointer->free(data_ptr);
#endif
}

static void source2(void) {
    u64 file_path_ptr = pointer->getcwd();
    u64 file_name_ptr = pointer->load("/all_english_words.txt");
    pointer->strcat(file_path_ptr, file_name_ptr);
#ifndef USE_GC
    pointer->free(file_name_ptr);
#endif
    u64 mode_ptr = pointer->load("rb");
    u64 f_ptr = file->file_alloc(file_path_ptr, mode_ptr);
    if (f_ptr != 0) {
        u64 data_ptr = file->file_read(f_ptr);
        u64 size = pointer->size(data_ptr);
        if (size > 100) {
            size = 100;
        }
        u64 list_ptr = list->alloc();
        file->file_free(f_ptr);
        char* file_data = pointer->unsafe(data_ptr);
        for (u64 i = 0; i < size; i++) {
            char* tmp = file_data;
            while (*tmp != 0 && *tmp != '\n') {
                tmp++;
            }
            *tmp++ = '\0';
            u64 data = pointer->load(file_data);
            list->push(list_ptr, data);
            char* unsafe = pointer->unsafe(data);
            printf("   .: %s\n", unsafe);
            file_data = tmp;
        }
        list->free(list_ptr);
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
    global_statistics();
    CLEAN(argc)
    pointer->init(DEFAULT_SIZE);
    u64 argv_ptr = pointer->load(argv[0]);
    pointer->push(argv_ptr);
    source1();
    source2();
#ifndef USE_GC
    pointer->free(argv_ptr);
#endif
    pointer->destroy();
    global_statistics();
    return 0;
}
