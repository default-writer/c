#include "playground/pointer/pointer.h"

#include "playground/brain/brain.h"

extern struct pointer_methods pointer_methods_definition;

const struct pointer_methods* pointer = &pointer_methods_definition;

extern inline void source() {
    struct pointer* file_name_ptr = pointer->load("/input.txt");
    struct pointer* argv_ptr = pointer->pop();
    struct pointer* head_ptr = pointer->peek();
    pointer->printf(head_ptr);
    struct pointer* data_ptr = pointer->alloc(PATH_MAX);
    pointer->strcpy(data_ptr, argv_ptr);
#ifndef USE_GC
    pointer->free(argv_ptr);
#endif
    struct pointer* pattern_ptr = pointer->load("/");
    struct pointer* last_match_ptr = pointer->match_last(data_ptr, pattern_ptr);
#ifndef USE_GC
    pointer->free(pattern_ptr);
#endif
    pointer->put_char(last_match_ptr, '\0');
#ifndef USE_GC
    pointer->free(last_match_ptr);
#endif
    pointer->strcat(data_ptr, file_name_ptr);
#ifndef USE_GC
    pointer->free(file_name_ptr);
#endif
    struct pointer* mode_ptr = pointer->load("rb");
    struct pointer* f_ptr = pointer->open_file(data_ptr, mode_ptr);
#ifndef USE_GC
    pointer->free(mode_ptr);
    pointer->free(data_ptr);
#endif
    struct pointer* file_data_ptr = pointer->read_file(f_ptr);
    pointer->close_file(f_ptr);
#ifndef USE_GC
    pointer->free(f_ptr);
#endif
    pointer->printf(file_data_ptr);
#ifndef USE_GC
    pointer->free(file_data_ptr);
#endif
}

int main(int argc, char** argv) {
    ZEROPTR(argc)
    pointer_init();
    struct pointer* argv_ptr = pointer->load(argv[0]);
    pointer->push(argv_ptr);
    source();
    pointer_destroy();
    return 0;
}
