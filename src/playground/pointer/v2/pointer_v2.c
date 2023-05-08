#include "playground/pointer/v2/pointer_v2.h"
#include "common/alloc.h"
#include "list-micro/data.h"
#include "playground/vm/v2/vm_v2.h"

#define DEFAULT_SIZE 0xff

/* list definition */
extern const struct vm vm_definition;
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
static u64 pointer_pointer_copy(u64 ptr);
static u64 pointer_peek(void);
static u64 pointer_pop(void);
static u64 pointer_pointer_alloc(void);
#ifndef USE_GC
static void pointer_pointer_free(u64 ptr);
#endif
static u64 pointer_list_alloc(void);
static void pointer_list_free(u64 ptr);
static void pointer_list_push(u64 ptr, u64 data_ptr);
static u64 pointer_list_peek(u64 ptr);
static u64 pointer_list_pop(u64 ptr);
static char* pointer_unsafe(u64 ptr);
static u64 pointer_size(u64 ptr);
static void pointer_strcpy(u64 dest_ptr, u64 src_ptr);
static void pointer_strcat(u64 dest_ptr, u64 src_ptr);
static u64 pointer_match_last(u64 src_ptr, u64 match_ptr);
static u64 pointer_load(const char* data);
static u64 pointer_getcwd(void);
static u64 pointer_file_alloc(void);
static void pointer_file_open(u64 ptr, u64 file_path_ptr, u64 mode_ptr);
static u64 pointer_file_read(u64 ptr);
static void pointer_file_free(u64 ptr);
static void pointer_printf(u64 ptr);
static void pointer_put_char(u64 ptr, char value);
static u64 pointer_alloc(enum type type);
static void pointer_free(u64 ptr);

#ifdef USE_GC
static void pointer_gc(void);
#endif

/* internal */
static struct pointer* pointer_alloc_internal(u64 size, enum type type);
static void pointer_realloc_internal(struct pointer* ptr, u64 size);
static void pointer_list_free_internal(struct list_data** current);

static struct pointer* ptr_alloc_internal(void);
static struct pointer* file_alloc_internal(void);
static struct pointer* list_alloc_internal(void);

typedef void (*call_free_internal)(void*);
typedef struct pointer* (*call_alloc_internal)(void);

static void ptr_free_internal(void* ptr);
static void file_free_internal(void* ptr);
static void list_free_internal(void* ptr);

static call_alloc_internal alloc_internal[] = {
    [TYPE_VOID] = 0,
    [TYPE_PTR] = ptr_alloc_internal,
    [TYPE_FILE] = file_alloc_internal,
    [TYPE_LIST] = list_alloc_internal,
};
static call_free_internal free_internal[] = {
    [TYPE_VOID] = 0,
    [TYPE_PTR] = ptr_free_internal,
    [TYPE_FILE] = file_free_internal,
    [TYPE_LIST] = list_free_internal,
};

static struct pointer* ptr_alloc_internal(void) {
    return pointer_alloc_internal(0, TYPE_PTR);
}

static struct pointer* file_alloc_internal() {
    return pointer_alloc_internal(sizeof(struct file_handler), TYPE_FILE);
}

static struct pointer* list_alloc_internal(void) {
    struct pointer* ptr = pointer_alloc_internal(sizeof(struct list_handler), TYPE_LIST);
    struct list_handler* handler = ptr->ptr;
    list->init(&handler->list);
    return ptr;
}

static void ptr_free_internal(void* ptr) {
    CLEAN(ptr)
}

static void file_free_internal(void* ptr) {
    struct file_handler* handler = ptr;
    if (handler != 0 && handler->file != 0) {
        FILE* file = handler->file;
        fclose(file);
#ifdef USE_MEMORY_CLEANUP
        handler->file = 0;
#endif
    }
}

static void list_free_internal(void* ptr) {
    struct list_handler* handler = ptr;
    if (handler != 0 && handler->list != 0) {
        pointer_list_free_internal(&handler->list);
        list->destroy(&handler->list);
#ifdef USE_MEMORY_CLEANUP
        handler->list = 0;
#endif
    }
}

static struct pointer* pointer_alloc_internal(u64 size, enum type type) {
    struct pointer* ptr = _list_alloc(sizeof(struct pointer));
    if (size != 0) {
        ptr->ptr = _list_alloc(size);
        ptr->size = size;
    }
    ptr->type = type;
    u64 data = vm->write(&base->vm, ptr);
#ifdef USE_GC
    list->push(&base->gc, (void*)data);
#endif
    ptr->address = data;
    return ptr;
}

static void pointer_realloc_internal(struct pointer* ptr, u64 size) {
    if (ptr != 0) {
        ptr->ptr = _list_realloc(ptr->ptr, size);
        ptr->size = size;
    }
}

static u64 pointer_alloc(enum type type) {
    const struct pointer* ptr = alloc_internal[type]();
    return ptr->address;
}

static void pointer_free(u64 ptr) {
    struct pointer* data_ptr = vm->free(&base->vm, ptr);
    if (data_ptr == 0) {
        return;
    }
    if (data_ptr->ptr == 0) {
        return;
    }
    void* any = data_ptr->ptr;
    u64 size = data_ptr->size;
    enum type type = data_ptr->type;
#ifdef USE_MEMORY_CLEANUP
    data_ptr->type = TYPE_VOID;
    data_ptr->size = 0;
    data_ptr->address = 0;
    data_ptr->ptr = 0;
#endif
    free_internal[type](any);
    if (any != 0 && size != 0) {
        _list_free(any, size);
    }
    _list_free(data_ptr, sizeof(struct pointer));
}

static void pointer_list_free_internal(struct list_data** current) {
    u64 ptr = 0;
    while ((ptr = (u64)list->pop(current)) != 0) {
        pointer_free(ptr);
    }
}

/* implementation*/

void pointer_ctx_init(struct pointer_data** ctx, u64 size) {
    /* ctx */
    *ctx = _list_alloc(sizeof(struct pointer_data));
    struct pointer_data* ptr = *ctx;
    /* vm */
    vm->init(&ptr->vm, size);
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

void pointer_ctx_destroy(struct pointer_data** ctx) {
#ifdef USE_MEMORY_DEBUG_INFO
    vm->memory_dump(base->vm);
#endif
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
    vm->destroy(&ptr->vm);
    /* ctx */
    _list_free(*ctx, sizeof(struct pointer_data));
}

extern const struct list list_micro_definition;

void pointer_init(u64 size) {
    vm->init(&base->vm, size);
    list->init(&base->list);
#ifdef USE_GC
    list->init(&base->gc);
#endif
}

void pointer_destroy(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    vm->memory_dump(base->vm);
#endif
#ifdef USE_GC
    pointer_gc();
    list->destroy(&base->gc);
#endif
    list->destroy(&base->list);
    vm->destroy(&base->vm);
}

#ifdef USE_GC
static void pointer_gc(void) {
    pointer_list_free_internal(&base->gc);
}
#endif

static void pointer_push(u64 ptr) {
    if (ptr != 0) {
        list->push(&base->list, (void*)ptr);
    }
}

static u64 pointer_pointer_copy(u64 ptr) {
    u64 data = 0;
    if (ptr == 0) {
        return 0;
    }
    const struct pointer* data_ptr = vm->read(&base->vm, ptr);
    if (data_ptr == 0) {
        return 0;
    }
    if (data_ptr->type != TYPE_PTR) {
        return 0;
    }
    if (data_ptr->size != 0) {
        struct pointer* copy_ptr = pointer_alloc_internal(data_ptr->size, data_ptr->type);
        memcpy(copy_ptr->ptr, data_ptr->ptr, copy_ptr->size); /* NOLINT */
        data = copy_ptr->address;
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
    return pointer_alloc(TYPE_LIST);
}

static void pointer_list_free(u64 ptr) {
    if (ptr != 0) {
        pointer_free(ptr);
    }
}

static void pointer_list_push(u64 ptr_list, u64 ptr) {
    if (ptr_list != 0 && ptr != 0) {
        struct pointer* data_ptr = vm->read(&base->vm, ptr_list);
        if (data_ptr == 0) {
            return;
        }
        if (data_ptr->type != TYPE_LIST) {
            return;
        }
        struct list_handler* handler = data_ptr->ptr;
        list->push(&handler->list, (void*)ptr);
    }
}

static u64 pointer_list_peek(u64 ptr) {
    u64 data = 0;
    if (ptr == 0) {
        return 0;
    }
    struct pointer* data_ptr = vm->read(&base->vm, ptr);
    if (data_ptr == 0) {
        return 0;
    }
    if (data_ptr->type != TYPE_LIST) {
        return 0;
    }
    if (data_ptr->size != 0) {
        struct list_handler* handler = data_ptr->ptr;
        data = (u64)list->peek(&handler->list);
    }
    return data;
}

static u64 pointer_list_pop(u64 ptr) {
    u64 data = 0;
    if (ptr == 0) {
        return 0;
    }
    struct pointer* data_ptr = vm->read(&base->vm, ptr);
    if (data_ptr == 0) {
        return 0;
    }
    if (data_ptr->type != TYPE_LIST) {
        return 0;
    }
    if (data_ptr->size != 0) {
        struct list_handler* handler = data_ptr->ptr;
        data = (u64)list->pop(&handler->list);
    }
    return data;
}

#ifndef USE_GC
static void pointer_pointer_free(u64 ptr) {
    if (ptr != 0) {
        pointer_free(ptr);
    }
}
#endif

static char* pointer_unsafe(u64 ptr) {
    char* data = 0;
    if (ptr != 0) {
        struct pointer* data_ptr = vm->read(&base->vm, ptr);
        data = data_ptr->ptr;
    }
    return data;
}

static u64 pointer_size(u64 ptr) {
    u64 size = 0;
    if (ptr != 0) {
        const struct pointer* data_ptr = vm->read(&base->vm, ptr);
        size = data_ptr->size;
    }
    return size;
}

static void pointer_strcpy(u64 dest, u64 src) {
    struct pointer* dest_ptr = vm->read(&base->vm, dest);
    const struct pointer* src_ptr = vm->read(&base->vm, src);
    if (src_ptr == 0) {
        return;
    }
    if (dest_ptr == 0) {
        return;
    }
    if (src_ptr->size != 0) {
        if (dest_ptr->size == 0) {
            dest_ptr->ptr = _list_alloc(src_ptr->size);
            dest_ptr->size = src_ptr->size;
        } else {
            u64 size = src_ptr->size + 1;
            if (dest_ptr->size < size) {
                pointer_realloc_internal(dest_ptr, size);
            }
        }
        char* data_dest = dest_ptr->ptr;
        const char* data_src = src_ptr->ptr; /* NOLINT */
        strcpy(data_dest, data_src); /* NOLINT */
    }
}

static void pointer_strcat(u64 dest, u64 src) {
    struct pointer* dest_ptr = vm->read(&base->vm, dest);
    const struct pointer* src_ptr = vm->read(&base->vm, src);
    if (src_ptr == 0) {
        return;
    }
    if (dest_ptr == 0) {
        return;
    }
    if (src_ptr->size != 0) {
        if (dest_ptr->size == 0) {
            dest_ptr->ptr = _list_alloc(src_ptr->size);
            dest_ptr->size = src_ptr->size;
        } else {
            u64 size = dest_ptr->size + src_ptr->size - 1;
            if (dest_ptr->size < size) {
                pointer_realloc_internal(dest_ptr, size);
            }
        }
        char* data_dest = dest_ptr->ptr;
        const char* data_src = src_ptr->ptr; /* NOLINT */
        strcat(data_dest, data_src); /* NOLINT */
    }
}

static u64 pointer_match_last(u64 src, u64 match) {
    u64 data = 0;
    const struct pointer* src_ptr = vm->read(&base->vm, src);
    const struct pointer* match_ptr = vm->read(&base->vm, match);
    if (src_ptr == 0) {
        return 0;
    }
    if (match_ptr == 0) {
        return 0;
    }
    const char* data_src = src_ptr->ptr;
    const char* data_match = match_ptr->ptr;
    if (data_match == 0) {
        return 0;
    }
    if (*data_match == 0) {
        return 0;
    }
    char* data_last = strrchr(data_src, *data_match);
    while (data_last != 0 && *data_last != 0 && *data_match != 0 && *data_last == *data_match) {
        data_last++;
        data_match++;
    }
    if (data_last != 0 && *data_match == 0) {
        struct pointer* last_match_ptr = pointer_alloc_internal(0, TYPE_PTR);
        last_match_ptr->ptr = --data_last;
        data = last_match_ptr->address;
    }
    return data;
}

static u64 pointer_load(const char* src_data) {
    u64 data = 0;
    if (src_data != 0) {
        u64 size = strlen(src_data) + 1;
        if (size != 0) {
            struct pointer* data_ptr = pointer_alloc_internal(size, TYPE_PTR);
            memcpy(data_ptr->ptr, src_data, size); /* NOLINT */
            data = data_ptr->address;
        }
    }
    return data;
}

static u64 pointer_getcwd(void) {
    char cwd[PATH_MAX];
    u64 data_ptr;
    getcwd(cwd, sizeof(cwd));
    u64 size = strlen(cwd) + 1;
    char* data = _list_alloc(size);
    strcpy(data, cwd); /* NOLINT */
    data_ptr = pointer_load(data);
    _list_free(data, size);
    return data_ptr;
}

static u64 pointer_file_alloc() {
    return pointer_alloc(TYPE_FILE);
}

static u64 pointer_pointer_alloc(void) {
    return pointer_alloc(TYPE_PTR);
}

static void pointer_file_open(u64 ptr, u64 file_path, u64 mode) {
    struct pointer* file_path_ptr = vm->read(&base->vm, file_path);
    const struct pointer* mode_ptr = vm->read(&base->vm, mode);
    if (file_path_ptr != 0 && mode_ptr != 0) {
        struct pointer* f_ptr = vm->read(&base->vm, ptr);
        struct file_handler* handler = f_ptr->ptr;
        const char* file_path_data = file_path_ptr->ptr;
        const char* mode_data = mode_ptr->ptr;
        FILE* file = fopen(file_path_data, mode_data); /* NOLINT */
        if (file != 0) {
            handler->file = file;
#ifdef USE_MEMORY_DEBUG_INFO
            handler->path = file_path_ptr->ptr;
#endif
        }
    }
}

static u64 pointer_file_read(u64 ptr) {
    u64 data = 0;
    if (ptr == 0) {
        return 0;
    }
    struct pointer* data_ptr = vm->read(&base->vm, ptr);
    if (data_ptr == 0) {
        return 0;
    }
    if (data_ptr->type != TYPE_FILE) {
        return 0;
    }
    struct file_handler* handler = data_ptr->ptr;
    FILE* file = handler->file;
    if (file != 0) {
        fseek(file, 0, SEEK_END); /* NOLINT */
        u64 size = (u64)ftell(file);
        fseek(file, 0, SEEK_SET);
        u64 data_size = size + 1;
        data_ptr = pointer_alloc_internal(data_size, TYPE_PTR);
        fread(data_ptr->ptr, 1, size, handler->file);
        data = data_ptr->address;
    }
    return data;
}

static void pointer_file_free(u64 ptr) {
    if (ptr != 0) {
        pointer_free(ptr);
    }
}

static void pointer_printf(u64 ptr) {
    struct pointer* data_ptr = vm->read(&base->vm, ptr);
    if (data_ptr != 0) {
        const char* data = data_ptr->ptr;
#ifdef USE_MEMORY_DEBUG_INFO
        void* ptr_data = data_ptr->ptr;
        printf("   .: 0x%016llx >0x%016llx\n", (u64)data_ptr, (u64)ptr_data);
#endif
        if (data != 0) {
            puts(data);
        }
    }
}

static void pointer_put_char(u64 ptr, char value) {
    struct pointer* data_ptr = vm->read(&base->vm, ptr);
    if (data_ptr != 0) {
        char* data = data_ptr->ptr;
        if (data != 0) {
            *data = value;
        }
    }
}

/* public */

const struct pointer_file_methods pointer_file_methods_definition = {
    .alloc = pointer_file_alloc,
    .open = pointer_file_open,
    .read = pointer_file_read,
    .free = pointer_file_free,
};

const struct pointer_list_methods pointer_list_methods_definition = {
    .alloc = pointer_list_alloc,
    .free = pointer_list_free,
    .peek = pointer_list_peek,
    .pop = pointer_list_pop,
    .push = pointer_list_push
};

const struct pointer_methods pointer_methods_definition = {
    .init = pointer_init,
    .destroy = pointer_destroy,
    .alloc = pointer_pointer_alloc,
    .copy = pointer_pointer_copy,
    .peek = pointer_peek,
    .push = pointer_push,
    .pop = pointer_pop,
    .strcpy = pointer_strcpy,
    .strcat = pointer_strcat,
    .match_last = pointer_match_last,
    .load = pointer_load,
    .getcwd = pointer_getcwd,
    .printf = pointer_printf,
    .put_char = pointer_put_char,
    .unsafe = pointer_unsafe,
    .size = pointer_size,
#ifndef USE_GC
    .free = pointer_pointer_free,
#else
    .gc = pointer_gc
#endif
};
