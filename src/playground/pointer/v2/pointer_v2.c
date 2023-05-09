#include "common/alloc.h"
#include "list-micro/data.h"
#include "playground/pointer/pointer.h"
#include "playground/vm/vm.h"

#define DEFAULT_SIZE 0x100

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
static u64 pointer_file_alloc(u64 file_path_ptr, u64 mode_ptr);
static u64 pointer_file_read(u64 ptr);
static void pointer_file_free(u64 ptr);
static void pointer_printf(u64 ptr);
static void pointer_put_char(u64 ptr, char value);

#ifdef USE_GC
static void pointer_gc(void);
#endif

/* internal */

static struct pointer* pointer_alloc_internal(u64 size, enum type type);
static void pointer_realloc_internal(struct pointer* ptr, u64 size);
static void pointer_free_internal(struct pointer* ptr);
static void pointer_list_free_internal(struct list_data** current);
static struct pointer* list_alloc_internal(void);
static void list_free_internal(struct pointer* ptr);
static void file_close_internal(struct pointer* ptr);

static struct pointer* pointer_alloc_internal(u64 size, enum type type) {
    struct pointer* ptr = _list_alloc(sizeof(struct pointer));
    if (size != 0) {
        ptr->data = _list_alloc(size);
        ptr->size = size;
    }
    ptr->type = type;
    return ptr;
}

static void pointer_realloc_internal(struct pointer* ptr, u64 size) {
    if (ptr != 0 && ptr->data != 0 && ptr->size != 0) {
        ptr->data = _list_realloc(ptr->data, size);
        ptr->size = size;
    }
}

static void pointer_free_internal(struct pointer* ptr) {
    if (ptr != 0) {
        if (ptr->data != 0 && ptr->size != 0) {
            _list_free(ptr->data, ptr->size);
        }
#ifdef USE_MEMORY_CLEANUP
        ptr->data = 0;
        ptr->size = 0;
#endif
        _list_free(ptr, sizeof(struct pointer));
    }
}

static void pointer_list_free_internal(struct list_data** current) {
    u64 ptr = 0;
    while ((ptr = (u64)list->pop(current)) != 0) {
        pointer_list_free(ptr);
        pointer_file_free(ptr);
        struct pointer* data_ptr = vm->free(&base->vm, ptr);
        pointer_free_internal(data_ptr);
    }
}

static struct pointer* list_alloc_internal(void) {
    struct pointer* ptr = pointer_alloc_internal(sizeof(struct list_handler), TYPE_LIST);
    struct list_handler* handler = ptr->data;
    list->init(&handler->list);
    return ptr;
}

static void list_free_internal(struct pointer* ptr) {
    if (ptr->data != 0 && ptr->size != 0) {
        struct list_handler* handler = ptr->data;
        pointer_list_free_internal(&handler->list);
        list->destroy(&handler->list);
    }
}

static void file_close_internal(struct pointer* ptr) {
    struct file_handler* handler = ptr->data;
    FILE* file = handler->file;
    if (file != 0) {
        fclose(file);
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
    vm->memory_dump_ref(base->vm);
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
    vm->memory_dump_ref(base->vm);
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
    if (ptr != 0) {
        const struct pointer* data_ptr = vm->read(&base->vm, ptr);
        if (data_ptr == 0) {
            return data;
        }
        if (data_ptr->type != TYPE_PTR) {
            return data;
        }
        if (data_ptr->size != 0) {
            struct pointer* copy_ptr = pointer_alloc_internal(data_ptr->size, data_ptr->type);
            memcpy(copy_ptr->data, data_ptr->data, copy_ptr->size); /* NOLINT */
            data = vm->write(&base->vm, copy_ptr);
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
    struct pointer* ptr = list_alloc_internal();
    data = vm->write(&base->vm, ptr);
#ifdef USE_GC
    list->push(&base->gc, (void*)data);
#endif
    return data;
}

static void pointer_list_free(u64 ptr) {
    if (ptr != 0) {
        struct pointer* data_ptr = vm->read(&base->vm, ptr);
        if (data_ptr == 0) {
            return;
        }
        if (data_ptr->type != TYPE_LIST) {
            return;
        }
        list_free_internal(data_ptr);
        data_ptr = vm->free(&base->vm, ptr);
        pointer_free_internal(data_ptr);
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
        struct list_handler* handler = data_ptr->data;
        list->push(&handler->list, (void*)ptr);
#ifdef USE_GC
        list->push(&base->gc, (void*)ptr);
#endif
    }
}

static u64 pointer_list_peek(u64 ptr) {
    u64 data = 0;
    if (ptr != 0) {
        struct pointer* data_ptr = vm->read(&base->vm, ptr);
        if (data_ptr == 0) {
            return data;
        }
        if (data_ptr->type != TYPE_LIST) {
            return data;
        }
        if (data_ptr->size != 0) {
            struct list_handler* handler = data_ptr->data;
            data = (u64)list->peek(&handler->list);
        }
    }
    return data;
}

static u64 pointer_list_pop(u64 ptr) {
    u64 data = 0;
    if (ptr != 0) {
        struct pointer* data_ptr = vm->read(&base->vm, ptr);
        if (data_ptr == 0) {
            return data;
        }
        if (data_ptr->type != TYPE_LIST) {
            return data;
        }
        if (data_ptr->size != 0) {
            struct list_handler* handler = data_ptr->data;
            data = (u64)list->pop(&handler->list);
        }
    }
    return data;
}

static struct pointer* pointer_alloc(void) {
    struct pointer* ptr = pointer_alloc_internal(0, TYPE_PTR);
    return ptr;
}

static u64 pointer_pointer_alloc(void) {
    u64 data = 0;
    struct pointer* ptr = pointer_alloc();
    data = vm->write(&base->vm, ptr);
#ifdef USE_GC
    list->push(&base->gc, (void*)data);
#endif
    return data;
}

#ifndef USE_GC
static void pointer_pointer_free(u64 ptr) {
    if (ptr != 0) {
        struct pointer* data_ptr = vm->read(&base->vm, ptr);
        if (data_ptr == 0) {
            return;
        }
        if (data_ptr->type != TYPE_PTR) {
            return;
        }
        data_ptr = vm->free(&base->vm, ptr);
        pointer_free_internal(data_ptr);
    }
}
#endif

static char* pointer_unsafe(u64 ptr) {
    char* data = 0;
    if (ptr != 0) {
        struct pointer* data_ptr = vm->read(&base->vm, ptr);
        data = data_ptr->data;
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
    if (dest_ptr != 0 && src_ptr != 0 && src_ptr->size != 0) {
        if (dest_ptr->size == 0) {
            dest_ptr->data = _list_alloc(src_ptr->size);
            dest_ptr->size = src_ptr->size;
        } else {
            u64 size = src_ptr->size + 1;
            if (dest_ptr->size < size) {
                pointer_realloc_internal(dest_ptr, size);
            }
        }
        char* data_dest = dest_ptr->data;
        const char* data_src = src_ptr->data; /* NOLINT */
        strcpy(data_dest, data_src); /* NOLINT */
    }
}

static void pointer_strcat(u64 dest, u64 src) {
    struct pointer* dest_ptr = vm->read(&base->vm, dest);
    const struct pointer* src_ptr = vm->read(&base->vm, src);
    if (dest_ptr != 0 && src_ptr != 0 && src_ptr->size != 0) {
        if (dest_ptr->size == 0) {
            dest_ptr->data = _list_alloc(src_ptr->size);
            dest_ptr->size = src_ptr->size;
        } else {
            u64 size = dest_ptr->size + src_ptr->size - 1;
            if (dest_ptr->size < size) {
                pointer_realloc_internal(dest_ptr, size);
            }
        }
        char* data_dest = dest_ptr->data;
        const char* data_src = src_ptr->data; /* NOLINT */
        strcat(data_dest, data_src); /* NOLINT */
    }
}

static u64 pointer_match_last(u64 src, u64 match) {
    u64 data = 0;
    const struct pointer* src_ptr = vm->read(&base->vm, src);
    const struct pointer* match_ptr = vm->read(&base->vm, match);
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
                data = vm->write(&base->vm, last_match_ptr);
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
            memcpy(data_ptr->data, src_data, size); /* NOLINT */
            data = vm->write(&base->vm, data_ptr);
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
    u64 size = strlen(cwd) + 1;
    char* data = _list_alloc(size);
    strcpy(data, cwd); /* NOLINT */
    data_ptr = pointer_load(data);
    _list_free(data, size);
    return data_ptr;
}

static u64 pointer_file_alloc(u64 file_path, u64 mode) {
    u64 data = 0;
    struct pointer* file_path_ptr = vm->read(&base->vm, file_path);
    const struct pointer* mode_ptr = vm->read(&base->vm, mode);
    if (file_path_ptr != 0 && mode_ptr != 0) {
        const char* file_path_data = file_path_ptr->data;
        const char* mode_data = mode_ptr->data;
        FILE* file = fopen(file_path_data, mode_data); /* NOLINT */
        if (file != 0) {
            struct pointer* f_ptr = pointer_alloc_internal(sizeof(struct file_handler), TYPE_FILE);
            struct file_handler* handler = f_ptr->data;
            handler->file = file;
#ifdef USE_MEMORY_DEBUG_INFO
            handler->path = file_path_ptr->data;
#endif
            data = vm->write(&base->vm, f_ptr);
#ifdef USE_GC
            list->push(&base->gc, (void*)data);
#endif
        }
    }
    return data;
}

static u64 pointer_file_read(u64 ptr) {
    u64 data = 0;
    if (ptr != 0) {
        struct pointer* data_ptr = vm->read(&base->vm, ptr);
        if (data_ptr == 0) {
            return data;
        }
        if (data_ptr->type != TYPE_FILE) {
            return data;
        }
        struct file_handler* handler = data_ptr->data;
        FILE* file = handler->file;
        if (file != 0) {
            fseek(file, 0, SEEK_END); /* NOLINT */
            u64 size = (u64)ftell(file);
            fseek(file, 0, SEEK_SET);
            u64 data_size = size + 1;
            data_ptr = pointer_alloc_internal(data_size, TYPE_PTR);
            fread(data_ptr->data, 1, size, handler->file);
            data = vm->write(&base->vm, data_ptr);
        }
#ifdef USE_GC
        list->push(&base->gc, (void*)data);
#endif
    }
    return data;
}

static void pointer_file_free(u64 ptr) {
    if (ptr != 0) {
        struct pointer* data_ptr = vm->read(&base->vm, ptr);
        if (data_ptr == 0) {
            return;
        }
        if (data_ptr->type != TYPE_FILE) {
            return;
        }
        file_close_internal(data_ptr);
        data_ptr = vm->free(&base->vm, ptr);
        pointer_free_internal(data_ptr);
    }
}

static void pointer_printf(u64 ptr) {
    struct pointer* data_ptr = vm->read(&base->vm, ptr);
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
    struct pointer* data_ptr = vm->read(&base->vm, ptr);
    if (data_ptr != 0) {
        char* data = data_ptr->data;
        if (data != 0) {
            *data = value;
        }
    }
}

/* public */

const struct pointer_methods pointer_methods_definition = {
    .init = pointer_init,
    .destroy = pointer_destroy,
    .list_alloc = pointer_list_alloc,
    .list_free = pointer_list_free,
    .list_peek = pointer_list_peek,
    .list_pop = pointer_list_pop,
    .list_push = pointer_list_push,
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
    .file_alloc = pointer_file_alloc,
    .file_read = pointer_file_read,
    .file_free = pointer_file_free,
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
