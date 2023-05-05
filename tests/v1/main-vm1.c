#include "common/alloc.h"
#include "list-micro/data.h"
#include "playground/brain/brain.h"
#include "playground/hashtable/hashtable.h"
#include "playground/pointer/pointer.h"

#define DEFAULT_SIZE 0x100

/* list definition */
extern const struct vm vm_definition;
extern const struct list list_micro_definition;
extern struct pointer_methods pointer_methods_definition;
const struct pointer_methods* pointer = &pointer_methods_definition;

typedef struct test_data {
    struct pointer_data* ctx;
}* TEST_DATA;

extern inline void source(void) {
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
    u64 f_ptr = pointer->file_alloc(file_path_ptr, mode_ptr);
#ifndef USE_GC
    pointer->free(file_path_ptr);
    pointer->free(mode_ptr);
#endif
    u64 data_ptr = pointer->file_read(f_ptr);
    pointer->file_free(f_ptr);
    pointer->printf(data_ptr);
#ifndef USE_GC
    pointer->free(data_ptr);
#endif
}

int main(int argc, char** argv) {
    CLEAN(argc)
    pointer->init(DEFAULT_SIZE);
    u64 argv_ptr = pointer->load(argv[0]);
    pointer->push(argv_ptr);
    source();
#ifndef USE_GC
    pointer->free(argv_ptr);
#endif
    pointer->destroy();
    return 0;
}
