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

#ifdef USE_GC
static struct pointer_data buffer;
static struct pointer_data* gc = &buffer;
#endif

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
static struct pointer* pointer_read_file(struct pointer* ptr);
static void pointer_close_file(struct pointer* ptr);
static void pointer_printf(struct pointer* ptr);
static void pointer_put_char(struct pointer* ptr, char value);

void pointer_init() {
    base->list = list->alloc(DEFAULT_SIZE);
#ifdef USE_GC
    gc->list = list->alloc(DEFAULT_SIZE);
#endif
}

void pointer_destroy() {
    list->free(base->list);
#ifdef USE_GC
    struct pointer* ptr;
    while ((ptr = list->pop(gc->list)) != 0) {
        pointer_free(ptr);
    }
    list->free(gc->list);
#endif
}

static struct pointer* pointer_read_file(struct pointer* ptr) {
    struct pointer* data_ptr = 0;
    if (ptr != 0 && ptr->type == TYPE_FILE) {
        // unpack inner file handler
        struct file_handler* handler = ptr->data;
        FILE* file = handler->file;
        fseek(file, 0, SEEK_END); // NOLINT
        u64 size = (u64)ftell(file);
        fseek(file, 0, SEEK_SET);
        u64 data_size = size + 1;
        data_ptr = pointer_alloc(data_size);
        fread(data_ptr->data, 1, data_ptr->size, handler->file);
    }
    return data_ptr;
}

static void pointer_push(struct pointer* ptr) {
    if (ptr != 0) {
        list->push(base->list, ptr);
    }
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
#ifdef USE_GC
    list->push(gc->list, ptr);
#endif
    return ptr;
}

static void pointer_free(struct pointer* ptr) {
    if (ptr != 0) {
        if (ptr->size != 0) {
            _list_free(ptr->data, ptr->size);
            ptr->data = 0;
            ptr->size = 0;
        }
        _list_free(ptr, sizeof(struct pointer));
        ptr = 0;
    }
}

static void pointer_close_file(struct pointer* ptr) {
    if (ptr != 0 && ptr->type == TYPE_FILE) {
        struct file_handler* handler = ptr->data;
        FILE* file = handler->file;
        fclose(file);
    }
}

static void pointer_strcpy(struct pointer* dest_ptr, struct pointer* src_ptr) {
    if (dest_ptr != 0 && src_ptr != 0) {
        char* dest = dest_ptr->data;
        const char* src = src_ptr->data; // NOLINT
        strcpy(dest, src); // NOLINT
    }
}

static void pointer_strcat(struct pointer* dest_ptr, struct pointer* src_ptr) {
    if (dest_ptr != 0 && src_ptr != 0) {
        char* dest = dest_ptr->data;
        const char* src = src_ptr->data; // NOLINT
        strcat(dest, src); // NOLINT
    }
}

static struct pointer* pointer_match_last(struct pointer* src_ptr, struct pointer* match_ptr) {
    struct pointer* last_match_ptr = 0;
    if (src_ptr != 0 && match_ptr != 0) {
        const char* src = src_ptr->data;
        const char* match = match_ptr->data;
        if (match != 0 && *match != 0) {
            char* last = strrchr(src, *match);
            while (last != 0 && *last != 0 && *match != 0 && *last == *match) {
                last++;
                match++;
            }
            if (last != 0 && *match == 0) {
                last_match_ptr = pointer_alloc(0);
                last_match_ptr->data = --last;
            }
        }
    }
    return last_match_ptr;
}

static struct pointer* pointer_load(const char* data) {
    struct pointer* data_ptr = 0;
    if (data != 0) {
        u64 size = strlen(data) + 1;
        data_ptr = pointer_alloc(size);
        memcpy(data_ptr->data, data, size); // NOLINT
    }
    return data_ptr;
}

static struct pointer* pointer_open_file(struct pointer* file_path_ptr, struct pointer* mode_ptr) {
    struct pointer* f_ptr = 0;
    if (file_path_ptr != 0 && mode_ptr != 0) {
        const char* file_path = file_path_ptr->data;
        const char* mode = mode_ptr->data;
        FILE* file = fopen(file_path, mode); // NOLINT
        f_ptr = pointer_alloc(sizeof(struct file_handler));
        f_ptr->type = TYPE_FILE;
        struct file_handler* handler = f_ptr->data;
        handler->file = file;
    }
    return f_ptr;
}

static void pointer_printf(struct pointer* ptr) {
    if (ptr != 0) {
        const char* data = ptr->data;
#ifdef USE_MEMORY_DEBUG_INFO
        void* ptr_data = ptr->data;
        printf("   .: 0x%016llx >0x%016llx\n", (u64)ptr, (u64)ptr_data);
#endif
        puts(data);
    }
}

static void pointer_put_char(struct pointer* ptr, char value) {
    if (ptr != 0) {
        char* data = ptr->data;
        if (data != 0) {
            *data = value;
        }
    }
}

const struct pointer_methods pointer_methods_definition = {
    .alloc = pointer_alloc,
#ifndef USE_GC
    .free = pointer_free,
#endif
    .push = pointer_push,
    .peek = pointer_peek,
    .pop = pointer_pop,
    .strcpy = pointer_strcpy,
    .strcat = pointer_strcat,
    .match_last = pointer_match_last,
    .load = pointer_load,
    .open_file = pointer_open_file,
    .read_file = pointer_read_file,
    .close_file = pointer_close_file,
    .printf = pointer_printf,
    .put_char = pointer_put_char
};
