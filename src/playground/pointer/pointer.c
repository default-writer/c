#include "common/alloc.h"

// #include "playground/list/v2/list.h"

#include "list-micro/data.h"
#include "playground/pointer/pointer.h"
#include "playground/virtual/vm.h"

#define DEFAULT_SIZE 0x100

/* list definition */
extern const struct vm vm_definition;

// extern const struct list list_v2;
extern const struct list list_micro_definition;

/* private */

struct pointer_data {
    struct vm_data* vm;
    struct list_data* list;
#ifdef USE_GC
    struct list_data* gc;
#endif
};

static struct pointer_data pointer;
static struct pointer_data* base = &pointer;

/* list definition */
static const struct vm* vm = &vm_definition;
static const struct list* list = &list_micro_definition;

enum type {
    TYPE_PTR = 0, // pointer type
    TYPE_FILE = 1, // file type
    TYPE_LIST = 2
};

struct pointer {
    void* data;
    u64 size;
    enum type type;
};

struct file_handler {
    FILE* file;
#ifdef USE_MEMORY_DEBUG_INFO
    char* path;
#endif
};

struct list_handler {
    struct list_data* list;
};

void pointer_init(u64 size);
void pointer_destroy(void);

/* api */

static void pointer_push(u64 ptr);
static u64 pointer_copy(u64 ptr);
static u64 pointer_peek(void);
static u64 pointer_pop(void);
static u64 pointer_alloc(void);
static void pointer_free(u64 ptr);
static u64 pointer_list_alloc(void);
static void pointer_list_free(u64 ptr);
static void pointer_list_push(u64 ptr, u64 data_ptr);
static u64 pointer_list_peek(u64 ptr);
static u64 pointer_list_pop(u64 ptr);
static char* pointer_unsafe(u64 ptr);
static void pointer_strcpy(u64 dest_ptr, u64 src_ptr);
static void pointer_strcat(u64 dest_ptr, u64 src_ptr);
static u64 pointer_match_last(u64 src_ptr, u64 match_ptr);
static u64 pointer_load(const char* data);
static u64 pointer_getcwd(void);
static u64 pointer_open_file(u64 file_path_ptr, u64 mode_ptr);
static u64 pointer_read_file(u64 ptr);
static void pointer_close_file(u64 ptr);
static void pointer_printf(u64 ptr);
static void pointer_put_char(u64 ptr, char value);
static char* char_copy(char* s, u64 size);

#ifdef USE_GC
static void pointer_gc(void);
#endif

/* internal */
static struct pointer* pointer_alloc_internal(u64 size, enum type type);
static void pointer_realloc_internal(struct pointer* ptr, u64 size);
static void pointer_free_internal(struct pointer* ptr);

/* implementation*/

void pointer_set(struct pointer_data** ctx) {
    /* ctx */
    *ctx = calloc(1, sizeof(struct pointer_data));
    struct pointer_data* ptr = *ctx;
    /* vm */
    ptr->vm = vm->init(DEFAULT_SIZE);
    base->vm = ptr->vm;
    /* list */
    list->init(&ptr->list);
    base->list = ptr->list;
#ifdef USE_GC
    /* gc */
    list->init(&ptr->gc);
    base->gc = ptr->gc;
#endif
}

void pointer_get(struct pointer_data** ctx) {
    struct pointer_data* ptr = *ctx;
#ifdef USE_GC
    pointer_gc();
    /* gc */
    ptr->gc = base->gc;
    list->destroy(&ptr->gc);
#endif
    /* list */
    ptr->list = base->list;
    list->destroy(&ptr->list);
    /* vm */
    ptr->vm = base->vm;
    vm->destroy(ptr->vm);
    /* ctx */
    free(*ctx);
}

extern const struct list list_micro_definition;

void pointer_init(u64 size) {
    base->vm = vm->init(size);
    list->init(&base->list);
#ifdef USE_GC
    list->init(&base->gc);
#endif
}

void pointer_destroy(void) {
#ifdef USE_GC
    pointer_gc();
    list->destroy(&base->gc);
#endif
    list->destroy(&base->list);
    vm->destroy(base->vm);
}

#ifdef USE_GC
static void pointer_gc(void) {
    u64 ptr = 0;
    while ((ptr = (u64)list->pop(&base->gc)) != 0) {
        pointer_free(ptr);
    }
}
#endif

static void pointer_push(u64 ptr) {
    if (ptr != 0) {
        list->push(&base->list, (void*)ptr);
    }
}

static u64 pointer_copy(u64 ptr) {
    u64 data = 0;
    if (ptr != 0) {
        struct pointer* data_ptr = vm->read(base->vm, ptr);
        if (data_ptr != 0 && data_ptr->size > 0 && data_ptr->type == TYPE_PTR) {
            struct pointer* copy_ptr = pointer_alloc_internal(data_ptr->size, data_ptr->type);
            memcpy(copy_ptr->data, data_ptr->data, copy_ptr->size); // NOLINT
            data = vm->alloc(base->vm);
            vm->write(base->vm, data, copy_ptr);
#ifdef USE_GC
            list->push(&base->gc, (void*)data);
#endif
        }
    }
    return data;
}

static u64 pointer_peek(void) {
    return (u64)list->peek(&base->list);
}

static u64 pointer_pop(void) {
    return (u64)list->pop(&base->list);
}

static u64 pointer_list_alloc(void) {
    u64 data = 0;
    struct pointer* ptr = pointer_alloc_internal(sizeof(struct list_handler), TYPE_LIST);
    struct list_handler* handler = ptr->data;
    list->init(&handler->list);
    data = vm->alloc(base->vm);
    vm->write(base->vm, data, ptr);
#ifdef USE_GC
    list->push(&base->gc, (void*)data);
#endif
    return data;
}

static void pointer_list_free(u64 ptr) {
    if (ptr != 0) {
        struct pointer* data_ptr = vm->read(base->vm, ptr);
        vm->free(base->vm, ptr);
        if (data_ptr != 0 && data_ptr->size > 0 && data_ptr->type == TYPE_LIST) {
            if (data_ptr->data != 0) {
                struct list_handler* handler = data_ptr->data;
                while ((ptr = (u64)list->pop(&handler->list)) != 0) {
                    pointer_free(ptr);
                }
                list->destroy(&handler->list);
                _list_free(data_ptr->data, data_ptr->size);
#ifdef USE_MEMORY_CLEANUP
                data_ptr->data = 0;
                data_ptr->size = 0;
#endif
            }
            pointer_free_internal(data_ptr);
        }
    }
}

static void pointer_list_push(u64 ptr_list, u64 ptr) {
    if (ptr_list != 0 && ptr != 0) {
        struct pointer* list_ptr = vm->read(base->vm, ptr_list);
        struct list_handler* handler = list_ptr->data;
        if (list_ptr->size > 0 && list_ptr->type == TYPE_LIST) {
            list->push(&handler->list, (void*)ptr);
#ifdef USE_GC
            list->push(&base->gc, (void*)ptr);
#endif
        }
    }
}

static u64 pointer_list_peek(u64 ptr) {
    u64 data = 0;
    if (ptr != 0) {
        struct pointer* data_ptr = vm->read(base->vm, ptr);
        if (data_ptr != 0 && data_ptr->size > 0 && data_ptr->type == TYPE_LIST) {
            struct list_handler* handler = data_ptr->data;
            data = (u64)list->peek(&handler->list);
        }
    }
    return data;
}

static u64 pointer_list_pop(u64 ptr) {
    u64 data = 0;
    if (ptr != 0) {
        struct pointer* data_ptr = vm->read(base->vm, ptr);
        if (data_ptr != 0 && data_ptr->size > 0 && data_ptr->type == TYPE_LIST) {
            struct list_handler* handler = data_ptr->data;
            data = (u64)list->pop(&handler->list);
        }
    }
    return data;
}

static u64 pointer_alloc(void) {
    u64 data = 0;
    struct pointer* ptr = pointer_alloc_internal(0, TYPE_PTR);
    data = vm->alloc(base->vm);
    vm->write(base->vm, data, ptr);
#ifdef USE_GC
    list->push(&base->gc, (void*)data);
#endif
    return data;
}

static struct pointer* pointer_alloc_internal(u64 size, enum type type) {
    struct pointer* ptr = _list_alloc(1, sizeof(struct pointer));
    if (size != 0) {
        ptr->data = _list_alloc(1, size);
        ptr->type = type;
        ptr->size = size;
    }
    return ptr;
}

static void pointer_free_internal(struct pointer* ptr) {
    if (ptr != 0) {
        if (ptr->data != 0 && ptr->size > 0) {
            _list_free(ptr->data, ptr->size);
        }
#ifdef USE_MEMORY_CLEANUP
        ptr->data = 0;
        ptr->size = 0;
#endif
    }
    _list_free(ptr, sizeof(struct pointer));
}

static void pointer_free(u64 ptr) {
    if (ptr != 0) {
        struct pointer* data_ptr = vm->read(base->vm, ptr);
        vm->free(base->vm, ptr);
        if (data_ptr != 0) {
            pointer_free_internal(data_ptr);
        }
    }
}

static char* pointer_unsafe(u64 ptr) {
    char* data = 0;
    if (ptr != 0) {
        struct pointer* data_ptr = vm->read(base->vm, ptr);
        data = data_ptr->data;
    }
    return data;
}

static void pointer_strcpy(u64 dest, u64 src) {
    struct pointer* dest_ptr = vm->read(base->vm, dest);
    struct pointer* src_ptr = vm->read(base->vm, src);
    if (dest_ptr != 0 && src_ptr != 0 && src_ptr->size > 0) {
        if (dest_ptr->size == 0) {
            dest_ptr->data = _list_alloc(1, src_ptr->size);
            dest_ptr->size = src_ptr->size;
        } else {
            u64 size = src_ptr->size + 1;
            if (dest_ptr->size < size) {
                pointer_realloc_internal(dest_ptr, size);
            }
        }
        char* data_dest = dest_ptr->data;
        const char* data_src = src_ptr->data; // NOLINT
        strcpy(data_dest, data_src); // NOLINT
    }
}

static void pointer_strcat(u64 dest, u64 src) {
    struct pointer* dest_ptr = vm->read(base->vm, dest);
    struct pointer* src_ptr = vm->read(base->vm, src);
    if (dest_ptr != 0 && src_ptr != 0 && src_ptr->size > 0) {
        if (dest_ptr->size == 0) {
            dest_ptr->data = _list_alloc(1, src_ptr->size);
            dest_ptr->size = src_ptr->size;
        } else {
            u64 size = dest_ptr->size + src_ptr->size - 1;
            if (dest_ptr->size < size) {
                pointer_realloc_internal(dest_ptr, size);
            }
        }
        char* data_dest = dest_ptr->data;
        const char* data_src = src_ptr->data; // NOLINT
        strcat(data_dest, data_src); // NOLINT
    }
}

static u64 pointer_match_last(u64 src, u64 match) {
    u64 data = 0;
    struct pointer* src_ptr = vm->read(base->vm, src);
    struct pointer* match_ptr = vm->read(base->vm, match);
    if (src_ptr != 0 && match_ptr != 0) {
        const char* data_src = src_ptr->data;
        const char* data_match = match_ptr->data;
        if (data_match != 0 && *data_match != 0) {
            char* data_last = strrchr(data_src, *data_match);
            while (data_last != 0 && *data_last != 0 && *data_match != 0 && *data_last == *data_match) {
                data_last++;
                data_match++;
            }
            if (data_last != 0 && *data_match == 0) {
                struct pointer* last_match_ptr = pointer_alloc_internal(0, TYPE_PTR);
                last_match_ptr->data = --data_last;
                data = vm->alloc(base->vm);
                vm->write(base->vm, data, last_match_ptr);
#ifdef USE_GC
                list->push(&base->gc, (void*)data);
#endif
            }
        }
    }
    return data;
}

static u64 pointer_load(const char* src_data) {
    u64 data = 0;
    if (src_data != 0) {
        u64 size = strlen(src_data) + 1;
        if (size != 0) {
            struct pointer* data_ptr = pointer_alloc_internal(size, TYPE_PTR);
            memcpy(data_ptr->data, src_data, size); // NOLINT
            data = vm->alloc(base->vm);
            vm->write(base->vm, data, data_ptr);
#ifdef USE_GC
            list->push(&base->gc, (void*)data);
#endif
        }
    }
    return data;
}

static u64 pointer_getcwd(void) {
    char cwd[PATH_MAX];
    u64 data_ptr;
    getcwd(cwd, sizeof(cwd));
    char* data = _list_alloc(1, strlen(cwd) + 1);
    strcpy(data, cwd); // NOLINT
    data_ptr = pointer_load(data);
    free(data);
    return data_ptr;
}

static u64 pointer_open_file(u64 file_path, u64 mode) {
    u64 data = 0;
    struct pointer* file_path_ptr = vm->read(base->vm, file_path);
    struct pointer* mode_ptr = vm->read(base->vm, mode);
    if (file_path_ptr != 0 && mode_ptr != 0) {
        const char* file_path_data = file_path_ptr->data;
        const char* mode_data = mode_ptr->data;
        FILE* file = fopen(file_path_data, mode_data); // NOLINT
#ifdef USE_MEMORY_DEBUG_INFO
        if (file != 0) {
            debug("file exists: %s\n", file_path_data);
        } else {
            debug("file does not exist: %s\n", file_path_data);
        }
#endif
        if (file != 0) {
            struct pointer* f_ptr = pointer_alloc_internal(sizeof(struct file_handler), TYPE_FILE);
            struct file_handler* handler = f_ptr->data;
            handler->file = file;
#ifdef USE_MEMORY_DEBUG_INFO
            handler->path = char_copy(file_path_ptr->data, file_path_ptr->size);
#endif
            data = vm->alloc(base->vm);
            vm->write(base->vm, data, f_ptr);
#ifdef USE_GC
            list->push(&base->gc, (void*)data);
#endif
        }
    }
    return data;
}

static u64 pointer_read_file(u64 ptr) {
    u64 data = 0;
    if (ptr != 0) {
        struct pointer* file_ptr = vm->read(base->vm, ptr);
        if (file_ptr->type == TYPE_FILE) {
            struct file_handler* handler = file_ptr->data;
            FILE* file = handler->file;
#ifdef USE_MEMORY_DEBUG_INFO
            debug("file handler: 0x%016llx\n", (u64)handler->file);
            debug("file name: %s\n", handler->path);
#endif
            if (file != 0) {
                fseek(file, 0, SEEK_END); // NOLINT
                u64 size = (u64)ftell(file);
                fseek(file, 0, SEEK_SET);
                u64 data_size = size + 1;
                struct pointer* data_ptr = pointer_alloc_internal(data_size, TYPE_PTR);
#ifdef USE_MEMORY_DEBUG_INFO
                debug("file size: %lld\n", size);
#endif
                fread(data_ptr->data, 1, size, handler->file);
                data = vm->alloc(base->vm);
                vm->write(base->vm, data, data_ptr);
            }
#ifdef USE_GC
            list->push(&base->gc, (void*)data);
#endif
        }
    }
    return data;
}

static void pointer_close_file(u64 ptr) {
    if (ptr != 0) {
        struct pointer* file_ptr = vm->read(base->vm, ptr);
        if (file_ptr->type == TYPE_FILE) {
            struct file_handler* handler = file_ptr->data;
#ifdef USE_MEMORY_DEBUG_INFO
            free(handler->path);
#endif
            FILE* file = handler->file;
            if (file != 0) {
                fclose(file);
            }
#ifndef USE_GC
            pointer_free(ptr);
#endif
        }
    }
}

static void pointer_printf(u64 ptr) {
    struct pointer* data_ptr = vm->read(base->vm, ptr);
    if (data_ptr != 0) {
        const char* data = data_ptr->data;
#ifdef USE_MEMORY_DEBUG_INFO
        void* ptr_data = data_ptr->data;
        printf("   .: 0x%016llx >0x%016llx\n", (u64)data_ptr, (u64)ptr_data);
#endif
        if (data != 0) {
            puts(data);
        }
    }
}

static void pointer_put_char(u64 ptr, char value) {
    struct pointer* data_ptr = vm->read(base->vm, ptr);
    if (data_ptr != 0) {
        char* data = data_ptr->data;
        if (data != 0) {
            *data = value;
        }
    }
}

static char* char_copy(char* s, u64 len) /* make a duplicate of s */
{
    char* p = calloc(1, len); /* +1 for ’\0’ */
    memcpy(p, s, len); // NOLINT
    return p;
}

static void pointer_realloc_internal(struct pointer* ptr, u64 size) {
    if (ptr != 0 && ptr->data != 0) {
#ifdef USE_MEMORY_DEBUG_INFO
        printf("   &: 0x%016llx !  %16lld\n", (u64)ptr->data, ptr->size);
#endif
        ptr->data = _list_realloc(ptr->data, size);
        ptr->size = size;
    }
}

const struct pointer_methods pointer_methods_definition = {
    .init = pointer_init,
    .destroy = pointer_destroy,
    .list_alloc = pointer_list_alloc,
    .list_free = pointer_list_free,
    .list_peek = pointer_list_peek,
    .list_pop = pointer_list_pop,
    .list_push = pointer_list_push,
    .alloc = pointer_alloc,
    .copy = pointer_copy,
    .peek = pointer_peek,
    .push = pointer_push,
    .pop = pointer_pop,
    .strcpy = pointer_strcpy,
    .strcat = pointer_strcat,
    .match_last = pointer_match_last,
    .load = pointer_load,
    .getcwd = pointer_getcwd,
    .open_file = pointer_open_file,
    .read_file = pointer_read_file,
    .close_file = pointer_close_file,
    .printf = pointer_printf,
    .put_char = pointer_put_char,
    .unsafe = pointer_unsafe,
#ifndef USE_GC
    .free = pointer_free,
#else
    .gc = pointer_gc
#endif
};
