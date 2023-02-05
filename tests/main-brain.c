#include "playground/pointer/pointer.h"

#include "playground/brain/brain.h"

extern struct pointer_methods pointer_methods_definition;

const struct pointer_methods* pointer = &pointer_methods_definition;

struct pointer* copy(const char* data);
void open_file();
void read_file();

extern inline void process() {
    struct pointer* data_ptr = pointer->pop();
    char* data = pointer->data(data_ptr);
    printf("%s\n", data);
    pointer->free(data_ptr);
}

extern inline struct pointer* get_full_path() {
    struct pointer* data_ptr = pointer->alloc(PATH_MAX);
    struct pointer* argv_ptr = pointer->pop(); // NOLINT
    pointer->strcpy(data_ptr, argv_ptr);
    struct pointer* pattern_ptr = copy("/");
    struct pointer* last_match_ptr = pointer->match_last(data_ptr, pattern_ptr);
    pointer->free(pattern_ptr);
    char* data = pointer->data(last_match_ptr);
    if (data != 0) {
        *data = 0;
    }
    pointer->free(last_match_ptr);
    struct pointer* file_name_ptr = pointer->pop(); // NOLINT
    pointer->strcat(data_ptr, file_name_ptr);
    pointer->free(argv_ptr);
    pointer->free(file_name_ptr);
    return data_ptr;
}

struct data {
    char* file;
};

struct pointer* copy(const char* data) {
    u64 size = strlen(data) + 1;
    struct pointer* data_ptr = pointer->alloc(size);
    memcpy(pointer->data(data_ptr), data, size); // NOLINT
    return data_ptr;
}

void open_file() {
    struct pointer* file_path_ptr = pointer->pop();
    struct pointer* mode_ptr = pointer->pop();
    const char* file_path = pointer->data(file_path_ptr);
    const char* mode = pointer->data(mode_ptr);
    FILE* f = fopen(file_path, mode); // NOLINT
    struct pointer* f_ptr = pointer->new_file(f);
    pointer->push(f_ptr);
    pointer->free(mode_ptr);
    pointer->free(file_path_ptr);
}

void read_file() {
    struct pointer* f_ptr = pointer->pop();
    struct pointer* data_ptr = pointer->data(f_ptr);
    pointer->free(f_ptr);
    pointer->push(data_ptr);
}

int main(int argc, char** argv) {
    if (argc > 0) {
        pointer_init();
        struct pointer* file_name_ptr = copy("/input.txt");
        struct pointer* argv_ptr = copy(argv[0]);
        pointer->push(file_name_ptr);
        pointer->push(argv_ptr);
        struct pointer* cwd_ptr = get_full_path();
        struct pointer* mode_ptr = copy("rb");
        pointer->push(mode_ptr);
        pointer->push(cwd_ptr);
        open_file();
        read_file();
        process();
        pointer_destroy();
    }
    return 0;
}
