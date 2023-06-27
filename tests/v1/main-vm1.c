#include "common/alloc.h"
#include "list-micro/data.h"
#include "playground/brain/brain.h"
#include "playground/hashtable/v1/hashtable_v1.h"
#include "playground/pointer/types/file/v1/file_v1.h"
#include "playground/pointer/types/list/v1/list_v1.h"
#include "playground/pointer/types/string/v1/string_v1.h"
#include "playground/pointer/v1/pointer_v1.h"

#define DEFAULT_SIZE 0x100

/* definition */
extern const struct vm vm_definition;

extern const struct pointer_methods pointer_methods_definition;
extern const struct pointer_list_methods pointer_list_methods_definition;
extern const struct pointer_file_methods pointer_file_methods_definition;
extern const struct pointer_string_methods pointer_string_methods_definition;

const struct pointer_methods* pointer = &pointer_methods_definition;
const struct pointer_list_methods* list = &pointer_list_methods_definition;
const struct pointer_file_methods* file = &pointer_file_methods_definition;
const struct pointer_string_methods* string = &pointer_string_methods_definition;

typedef struct test_data {
    struct pointer_data* ctx;
}* TEST_DATA;

extern inline void source1(void) {
    u64 file_path_ptr = pointer->pop();
    u64 file_name_ptr = string->load("/input.txt");
    u64 pattern_ptr = string->load("/");
    u64 last_match_ptr = string->match_last(file_path_ptr, pattern_ptr);
#ifndef USE_GC
    string->free(pattern_ptr);
#endif
    string->put_char(last_match_ptr, '\0');
#ifndef USE_GC
    string->free(last_match_ptr);
#endif
    string->strcat(file_path_ptr, file_name_ptr);
#ifndef USE_GC
    string->free(file_name_ptr);
#endif
    u64 mode_ptr = string->load("rb");
    u64 f_ptr = file->alloc(file_path_ptr, mode_ptr);
#ifndef USE_GC
    string->free(file_path_ptr);
    string->free(mode_ptr);
#endif
    u64 data_ptr = file->read(f_ptr);
    file->free(f_ptr);
    string->printf(data_ptr);
#ifndef USE_GC
    string->free(data_ptr);
#endif
}

extern inline void source2(void) {
    u64 file_path_ptr = string->getcwd();
    u64 file_name_ptr = string->load("/all_english_words.txt");
    string->strcat(file_path_ptr, file_name_ptr);
#ifndef USE_GC
    string->free(file_name_ptr);
#endif
    u64 mode_ptr = string->load("rb");
    u64 f_ptr = file->alloc(file_path_ptr, mode_ptr);
    if (f_ptr != 0) {
        u64 data_ptr = file->read(f_ptr);
        u64 size = pointer->size(data_ptr);
        if (size > 100) {
            size = 100;
        }
        u64 list_ptr = list->alloc();
        file->free(f_ptr);
        char* file_data = string->unsafe(data_ptr);
        for (u64 i = 0; i < size; i++) {
            char* tmp = file_data;
            while (*tmp != 0 && *tmp != '\n') {
                tmp++;
            }
            *tmp++ = '\0';
            u64 data = string->load(file_data);
            list->push(list_ptr, data);
            char* unsafe = string->unsafe(data);
            printf("%s\n", unsafe);
            file_data = tmp;
        }
        list->free(list_ptr);
#ifndef USE_GC
        string->free(data_ptr);
#endif
    }
#ifndef USE_GC
    string->free(mode_ptr);
    string->free(file_name_ptr);
    string->free(file_path_ptr);
#endif
}

int main(int argc, char** argv) {
    CLEAN(argc)
    pointer->init(DEFAULT_SIZE);
    u64 argv_ptr = string->load(argv[0]);
    pointer->push(argv_ptr);
    source1();
    source2();
#ifndef USE_GC
    string->free(argv_ptr);
#endif
    pointer->destroy();
    global_statistics();
    return 0;
}
