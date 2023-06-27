#include "playground/pointer/v1/pointer_v1.h"
#include "common/alloc.h"
#include "list-micro/data.h"
#include "playground/vm/v1/vm_v1.h"

#define DEFAULT_SIZE 0x100
#define POINTER_SIZE sizeof(struct pointer)
#define POINTER_DATA_SIZE sizeof(struct pointer_data)

/* public data */
struct pointer_data vm_pointer;

/*private data */
static struct list_data* vm_enumerator_state;

/* extern definition */
extern const struct vm vm_definition;
extern const struct list list_micro_definition;
extern void list_init(void);
extern void file_init(void);
extern void string_init(void);

/* definition */
static struct pointer_data* base = &vm_pointer;
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

static void pointer_init(u64 size);
static void pointer_destroy(void);

/* api */
void pointer_vm_register_free(function free);
void pointer_ctx_init(struct pointer_data** ctx, u64 size);
void pointer_ctx_destroy(struct pointer_data** ctx);

static struct pointer* pointer_vm_alloc(u64 size, enum type type);
static void pointer_vm_realloc(struct pointer* ptr, u64 size);
static void pointer_vm_free(struct pointer* ptr);
static void pointer_vm_cleanup(struct list_data** current);

static void pointer_push(u64 ptr);
static u64 pointer_peek(void);
static u64 pointer_pop(void);
static u64 pointer_size(u64 ptr);

#ifdef USE_MEMORY_DEBUG_INFO
static void pointer_dump(struct pointer* ptr);
static void pointer_dump_ref(void** ptr);
#endif

#ifdef USE_GC
static void pointer_gc(void);
#endif

/* internal */
static void pointer_init_internal(struct pointer_data* ptr, u64 size);
static void pointer_destroy_internal(struct pointer_data* ptr);
static void pointer_ctx_init_internal(struct pointer_data* ptr, u64 size);
static void pointer_ctx_destroy_internal(struct pointer_data* ptr);

/* free */
static void pointer_vm_register_free_internal(struct list_data** current, function free);
static void pointer_free_internal(u64 ptr);

/* enumerator */
static void pointer_vm_enumerator_init_internal(struct list_data** current);
static function pointer_vm_enumerator_next(void);
static void pointer_vm_enumerator_destroy_internal(void);

/* internal */
static void pointer_vm_enumerator_init_internal(struct list_data** current) {
    vm_enumerator_state = *current;
}

static function pointer_vm_enumerator_next(void) {
    function free = 0;
    struct list_data* current = vm_enumerator_state;
    if (current == 0) {
        return 0;
    }
    vm_enumerator_state = current->next;
    while (current->data == 0) {
        current = current->next;
        if (current == 0) {
            return 0;
        }
    }
    free = current->data;
    return free;
}

static void pointer_vm_enumerator_destroy_internal(void) {
    vm_enumerator_state = 0;
}

void pointer_vm_register_free(function free) {
    pointer_vm_register_free_internal(&base->free, free);
}

static void pointer_vm_register_free_internal(struct list_data** current, function free) {
    list->push(current, free);
}

static struct pointer* pointer_vm_alloc(u64 size, enum type type) {
    struct pointer* ptr = global_alloc(POINTER_SIZE);
    if (size != 0) {
        ptr->data = global_alloc(size);
        ptr->size = size;
    }
    ptr->type = type;
    return ptr;
}

static void pointer_vm_realloc(struct pointer* ptr, u64 size) {
    if (ptr != 0 && ptr->data != 0) {
        ptr->data = global_realloc(ptr->data, ptr->size, size);
        ptr->size = size;
    }
}

static void pointer_vm_free(struct pointer* ptr) {
    if (ptr != 0) {
        if (ptr->data != 0 && ptr->size != 0) {
            global_free(ptr->data, ptr->size);
        }
#ifdef USE_MEMORY_CLEANUP
        ptr->data = 0;
        ptr->size = 0;
#endif
    }
    global_free(ptr, POINTER_SIZE);
}

static void pointer_free_internal(u64 ptr) {
    pointer_vm_enumerator_init_internal(&base->free);
    function free;
    while ((free = pointer_vm_enumerator_next()) != 0) {
        free(ptr);
    }
    pointer_vm_enumerator_destroy_internal();
}

static void pointer_vm_cleanup(struct list_data** current) {
    u64 ptr = 0;
    while ((ptr = (u64)list->pop(current)) != 0) {
        pointer_free_internal(ptr);
    }
}

/* implementation*/

static void pointer_init_internal(struct pointer_data* ptr, u64 size) {
    vm->init(&ptr->vm, size);
    list->init(&ptr->list);
    list->init(&ptr->free);
    list_init();
    file_init();
    string_init();
#ifdef USE_GC
    list->init(&ptr->gc);
#endif
}

static void pointer_destroy_internal(struct pointer_data* ptr) {
#ifdef USE_GC
    list->destroy(&ptr->gc);
#endif
    list->destroy(&ptr->list);
    list->destroy(&ptr->free);
    vm->destroy(&ptr->vm);
}

static void copy_internal(struct pointer_data* dest, struct pointer_data* src) {
    dest->vm = src->vm;
    dest->list = src->list;
    dest->free = src->free;
#ifdef USE_GC
    /* gc */
    dest->gc = src->gc;
#endif
}

static void pointer_ctx_init_internal(struct pointer_data* ptr, u64 size) {
    copy_internal(base, ptr);
    pointer_init_internal(base, size);
}

static void pointer_ctx_destroy_internal(struct pointer_data* ptr) {
    copy_internal(ptr, base);
    pointer_destroy_internal(base);
}

void pointer_ctx_init(struct pointer_data** ctx, u64 size) {
    *ctx = global_alloc(POINTER_DATA_SIZE);
    pointer_ctx_init_internal(*ctx, size);
}

void pointer_ctx_destroy(struct pointer_data** ctx) {
#ifdef USE_MEMORY_DEBUG_INFO
    vm->dump_ref(&base->vm);
    vm->dump(&base->vm);
#endif
#ifdef USE_GC
    pointer_gc();
#endif
    pointer_ctx_destroy_internal(*ctx);
    global_free(*ctx, POINTER_DATA_SIZE);
}

static void pointer_init(u64 size) {
    pointer_init_internal(base, size);
}

static void pointer_destroy(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    vm->dump_ref(&base->vm);
    vm->dump(&base->vm);
#endif
#ifdef USE_GC
    pointer_gc();
#endif
    pointer_destroy_internal(base);
}

#ifdef USE_GC
static void pointer_gc(void) {
    pointer_vm_cleanup(&base->gc);
}
#endif

static void pointer_push(u64 ptr) {
    if (ptr != 0) {
        list->push(&base->list, (void*)ptr);
    }
}

static u64 pointer_peek(void) {
    return (u64)list->peek(&base->list);
}

static u64 pointer_pop(void) {
    return (u64)list->pop(&base->list);
}

static u64 pointer_size(u64 ptr) {
    if (ptr == 0) {
        return 0;
    }
    const struct pointer* data_ptr = vm->read(&base->vm, ptr);
    if (data_ptr == 0) {
        return 0;
    }
    u64 size = data_ptr->size;
    return size;
}

#ifdef USE_MEMORY_DEBUG_INFO
static void pointer_dump(struct pointer* ptr) {
    if (ptr == 0) {
        return;
    }
    if (ptr->data == 0) {
        return;
    }
    printf("   ^: %016llx > %016llx\n", (u64)ptr, (u64)ptr->data);
}

static void pointer_dump_ref(void** ptr) {
    if (ptr == 0) {
        return;
    }
    if (*ptr == 0) {
        return;
    }
    printf("   &: %016llx > %016llx\n", (u64)ptr, (u64)*ptr);
}
#endif

/* public */

const struct pointer_vm_methods vm_methods_definition = {
    .alloc = pointer_vm_alloc,
    .realloc = pointer_vm_realloc,
    .free = pointer_vm_free,
    .cleanup = pointer_vm_cleanup
};

const struct pointer_methods pointer_methods_definition = {
    .init = pointer_init,
    .destroy = pointer_destroy,
    .peek = pointer_peek,
    .push = pointer_push,
    .pop = pointer_pop,
#ifdef USE_MEMORY_DEBUG_INFO
    .dump = pointer_dump,
    .dump_ref = pointer_dump_ref,
#endif
    .size = pointer_size,
#ifdef USE_GC
    .gc = pointer_gc
#endif
};
