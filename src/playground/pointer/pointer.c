#include "playground/pointer/pointer.h"
#include "common/alloc.h"
#include "playground/list/v2/list.h"

#define DEFAULT_SIZE 0xffff // 64K bytes

/* list definition */
extern const struct list list_v2;

/* private */
struct pointer_data {
    struct list_data* list;
};

static struct pointer_data pointer;

static struct pointer_data* base = &pointer;

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

void pointer_init();
void pointer_destroy();

static void* pointer_data(struct pointer* ptr);
static u64 pointer_size(struct pointer* ptr);
static void pointer_push(struct pointer* ptr);
static struct pointer* pointer_peek();
static struct pointer* pointer_pop();
static struct pointer* pointer_alloc(u64 size);
static void pointer_free(struct pointer* ptr);
static void pointer_strcpy(struct pointer* dest_ptr, struct pointer* src_ptr);
static void pointer_strcat(struct pointer* dest_ptr, struct pointer* src_ptr);
static struct pointer* pointer_match_last(struct pointer* src_ptr, struct pointer* match_ptr);
static struct pointer* pointer_load(const char* data);
static struct pointer* pointer_open_file(struct pointer* file_path_ptr, struct pointer* mode_ptr);
static void pointer_printf(struct pointer* ptr);

void pointer_init() {
    base->list = list->alloc(DEFAULT_SIZE);
}

void pointer_destroy() {
    struct pointer* ptr;
    while ((ptr = list->pop(base->list)) != 0) {
        pointer_free(ptr);
    }
    list->free(base->list);
}

static void* pointer_data(struct pointer* ptr) {
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

static u64 pointer_size(struct pointer* ptr) {
    if (ptr->type == TYPE_FILE) {
        struct file_handler* handler = ptr->data;
        FILE* file = handler->file;
        fseek(file, 0, SEEK_END); // NOLINT
        long size = ftell(file);
        fseek(file, 0, SEEK_SET);
        return (u64)size;
    }
    return ptr->size;
}

static void pointer_push(struct pointer* ptr) {
    list->push(base->list, ptr);
}

static struct pointer* pointer_peek() {
    return list->peek(base->list);
}

static struct pointer* pointer_pop() {
    return list->pop(base->list);
}

static struct pointer* pointer_alloc(u64 size) {
    struct pointer* ptr = _list_alloc(1, sizeof(struct pointer));
    if (size != 0) {
        ptr->data = _list_alloc(1, size);
        ptr->size = size;
    }
    return ptr;
}

static void pointer_free(struct pointer* ptr) {
    if (ptr->type == TYPE_FILE) {
        struct file_handler* handler = ptr->data;
        FILE* file = handler->file;
        fclose(file);
    }
    if (ptr->size != 0) {
        _list_free(ptr->data, ptr->size);
        ptr->data = 0;
        ptr->size = 0;
    }
    _list_free(ptr, sizeof(struct pointer));
    ptr = 0;
}

static void pointer_strcpy(struct pointer* dest_ptr, struct pointer* src_ptr) {
    char* dest = pointer_data(dest_ptr);
    const char* src = pointer_data(src_ptr); // NOLINT
    strcpy(dest, src); // NOLINT
}

static void pointer_strcat(struct pointer* dest_ptr, struct pointer* src_ptr) {
    char* dest = pointer_data(dest_ptr);
    const char* src = pointer_data(src_ptr); // NOLINT
    strcat(dest, src); // NOLINT
}

static struct pointer* pointer_match_last(struct pointer* src_ptr, struct pointer* match_ptr) {
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

static struct pointer* pointer_load(const char* data) {
    u64 size = strlen(data) + 1;
    struct pointer* data_ptr = pointer_alloc(size);
    memcpy(pointer_data(data_ptr), data, size); // NOLINT
    return data_ptr;
}

static struct pointer* pointer_open_file(struct pointer* file_path_ptr, struct pointer* mode_ptr) {
    const char* file_path = pointer_data(file_path_ptr);
    const char* mode = pointer_data(mode_ptr);
    FILE* file = fopen(file_path, mode); // NOLINT
    struct pointer* f_ptr = pointer_alloc(sizeof(struct file_handler));
    f_ptr->type = TYPE_FILE;
    struct file_handler* handler = f_ptr->data;
    handler->file = file;
    return f_ptr;
}

static void pointer_printf(struct pointer* ptr) {
    const char* data = pointer_data(ptr);
#ifdef USE_MEMORY_DEBUG_INFO
    void* ptr_data = ptr->data;
    printf("   .: 0x%016llx >0x%016llx\n", (u64)ptr, (u64)ptr_data);
#endif
    puts(data);
}

const struct pointer_methods pointer_methods_definition = {
    .alloc = pointer_alloc,
    .free = pointer_free,
    .push = pointer_push,
    .peek = pointer_peek,
    .pop = pointer_pop,
    .data = pointer_data,
    .size = pointer_size,
    .strcpy = pointer_strcpy,
    .strcat = pointer_strcat,
    .match_last = pointer_match_last,
    .load = pointer_load,
    .open_file = pointer_open_file,
    .printf = pointer_printf
};
