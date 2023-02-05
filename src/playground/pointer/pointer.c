#include "common/alloc.h"

#include "playground/list/v2/list.h"
#include "playground/pointer/pointer.h"

/* list definition */
extern const struct list list_v2;

/* private */
struct pointer_data {
    struct list_data* args;
};

struct pointer_data root_pointer;

struct pointer_data* root = &root_pointer;

/* list definition */
static const struct list* list = &list_v2;

enum type {
    TYPE_PTR = 0,
    TYPE_FILE = 1,
};

struct pointer {
    void* data;
    u64 size;
    enum type type;
};

struct file_handler {
    FILE* file;
};

void* pointer_data(struct pointer* ptr);
u64 pointer_size(struct pointer* ptr);
void pointer_push(struct pointer* ptr);
struct pointer* pointer_peek();
struct pointer* pointer_pop();

void pointer_init();
void pointer_destroy();

struct pointer* pointer_alloc(u64 size);
void pointer_free(struct pointer* ptr);

struct pointer* pointer_new_file(FILE* file);

const struct pointer_methods pointer_methods_definition;

void* pointer_data(struct pointer* ptr) {
    if (ptr->type == TYPE_FILE) {
        // unpack inner file handler
        struct file_handler* handler = ptr->data;
        u64 data_size = pointer_size(ptr) + 1;
        struct pointer* data_ptr = pointer_alloc(data_size);
        fread(data_ptr->data, 1, data_ptr->size, handler->file);
        return data_ptr;
    }
    return ptr->data;
}

u64 pointer_size(struct pointer* ptr) {
    if (ptr->type == TYPE_FILE) {
        struct file_handler* handler = ptr->data;
        FILE* f = handler->file;
        fseek(f, 0, SEEK_END); // NOLINT
        long size = ftell(f);
        fseek(f, 0, SEEK_SET);
        return (u64)size;
    }
    return ptr->size;
}

void pointer_push(struct pointer* ptr) {
    list->push(root->args, ptr);
}

struct pointer* pointer_peek() {
    return list->peek(root->args);
}

struct pointer* pointer_pop() {
    return list->pop(root->args);
}

void pointer_init() {
    root->args = list->new ();
}

void pointer_destroy() {
    struct pointer* ptr;
    while ((ptr = list->pop(root->args)) != 0) {
        pointer_free(ptr);
    }
    list->delete (root->args);
}

struct pointer* pointer_alloc(u64 size) {
    struct pointer* ptr = _list_alloc(1, sizeof(struct pointer));
    if (size != 0) {
        ptr->data = _list_alloc(1, size);
        ptr->size = size;
    }
    return ptr;
}

struct pointer* pointer_new_file(FILE* file) {
    struct pointer* ptr = _list_alloc(1, sizeof(struct pointer));
    ptr->data = _list_alloc(1, sizeof(struct file_handler));
    struct file_handler* handler = ptr->data;
    handler->file = file;
    ptr->size = sizeof(struct file_handler);
    ptr->type = TYPE_FILE;
    return ptr;
}

void pointer_free(struct pointer* ptr) {
    if (ptr->type == TYPE_FILE) {
        struct file_handler* handler = ptr->data;
        FILE* file = handler->file;
        fclose(file);
    }
    if (ptr->size != 0) {
        _list_free(ptr->data, (size_t)ptr->size);
        ptr->data = 0;
        ptr->size = 0;
    }
    _list_free(ptr, sizeof(struct pointer));
    ptr = 0;
}

void pointer_strcpy(struct pointer* dest_ptr, struct pointer* src_ptr) {
    char* dest = pointer_data(dest_ptr);
    const char* src = pointer_data(src_ptr); // NOLINT
    strcpy(dest, src); // NOLINT
}

void pointer_strcat(struct pointer* dest_ptr, struct pointer* src_ptr) {
    char* dest = pointer_data(dest_ptr);
    const char* src = pointer_data(src_ptr); // NOLINT
    strcat(dest, src); // NOLINT
}

struct pointer* pointer_match_last(struct pointer* src_ptr, struct pointer* match_ptr) {
    const char* src = pointer_data(src_ptr);
    const char* match = pointer_data(match_ptr);
    if (match != 0 && *match != 0) {
        char* last = strrchr(src, *match);
        while (last != 0 && *last != 0 && *match != 0 && *last == *match) {
            last++;
            match++;
        }
        if (last != 0 && *match == 0) {
            struct pointer* last_match_ptr = pointer_alloc(0);
            last_match_ptr->data = --last;
            return last_match_ptr;
        }
    }
    return 0;
}

const struct pointer_methods pointer_methods_definition = {
    .alloc = pointer_alloc,
    .free = pointer_free,
    .new_file = pointer_new_file,
    .push = pointer_push,
    .peek = pointer_peek,
    .pop = pointer_pop,
    .data = pointer_data,
    .size = pointer_size,
    .strcpy = pointer_strcpy,
    .strcat = pointer_strcat,
    .match_last = pointer_match_last
};
