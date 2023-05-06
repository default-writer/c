#include "common/alloc.h"
#include "list-micro/data.h"
#include "playground/hashtable/v2/hashtable_v2.h"
#include "playground/pointer/v2/pointer_v2.h"

#define DEFAULT_SIZE 0x100

/* list definition */
extern const struct vm vm_definition;
extern const struct list list_micro_definition;

extern struct pointer_methods pointer_methods_definition;
extern struct pointer_list_methods pointer_list_methods_definition;
extern struct pointer_file_methods pointer_file_methods_definition;

const struct pointer_methods* pointer = &pointer_methods_definition;
const struct pointer_list_methods* pointer_list = &pointer_list_methods_definition;
const struct pointer_file_methods* pointer_file = &pointer_file_methods_definition;

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
    u64 f_ptr = pointer_file->alloc();
    pointer_file->open(f_ptr, file_path_ptr, mode_ptr);
#ifndef USE_GC
    pointer->free(file_path_ptr);
    pointer->free(mode_ptr);
#endif
    u64 data_ptr = pointer_file->read(f_ptr);
    pointer_file->free(f_ptr);
    pointer->printf(data_ptr);
#ifndef USE_GC
    pointer->free(data_ptr);
#else
    pointer->gc();
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
    u64 f_ptr = pointer_file->alloc();
    pointer_file->open(f_ptr, file_path_ptr, mode_ptr);
    if (f_ptr != 0) {
        u64 data_ptr = pointer_file->read(f_ptr);
        u64 size = pointer->size(data_ptr);
        if (size > 100) {
            size = 100;
        }
        u64 list_ptr = pointer_list->alloc();
        pointer_file->free(f_ptr);
        char* file_data = pointer->unsafe(data_ptr);
        for (u64 i = 0; i < size; i++) {
            char* tmp = file_data;
            while (*tmp != 0 && *tmp != '\n') {
                tmp++;
            }
            *tmp++ = '\0';
            u64 data = pointer->load(file_data);
            pointer_list->push(list_ptr, data);
            char* unsafe = pointer->unsafe(data);
            printf("%s\n", unsafe);
            file_data = tmp;
        }
        pointer_list->free(list_ptr);
#ifndef USE_GC
        pointer->free(data_ptr);
#endif
    }
#ifndef USE_GC
    pointer->free(mode_ptr);
    pointer->free(file_name_ptr);
    pointer->free(file_path_ptr);
#else
    pointer->gc();
#endif
}

int main(int argc, char** argv) {
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
    return 0;
}
