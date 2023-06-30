#include "playground/pointer/types/string/v1/string_v1.h"
#include "common/alloc.h"
#include "list-micro/data.h"
#include "playground/pointer/v1/pointer_v1.h"
#include "playground/vm/v1/vm_v1.h"

#define DEFAULT_SIZE 0x100

/* definition */
extern const struct vm vm_definition;
extern const struct list list_micro_definition;
extern const struct pointer_vm_methods vm_methods_definition;
extern void pointer_vm_register_free(function function);

extern struct pointer_data vm_pointer;
static struct pointer_data* base = &vm_pointer;

/* definition */
static const struct vm* vm = &vm_definition;
static const struct list* list = &list_micro_definition;
static const struct pointer_vm_methods* pointer = &vm_methods_definition;

struct list_handler {
    struct list_data* list;
};

/* api */

static u64 string_alloc(void);
static void string_free(u64 ptr);
static u64 string_copy(u64 ptr);
static void string_strcpy(u64 dest_ptr, u64 src_ptr);
static void string_strcat(u64 dest_ptr, u64 src_ptr);
static u64 string_match_last(u64 src_ptr, u64 match_ptr);
static u64 string_load(const char* data);
static u64 string_getcwd(void);
static void string_printf(u64 ptr);
static void string_put_char(u64 ptr, char value);
static char* string_unsafe(u64 ptr);

/* internal */

/* implementation*/

static u64 string_alloc(void) {
    struct pointer* ptr = pointer->alloc(0, TYPE_STRING);
    u64 data = vm->write(base->vm, ptr);
#ifdef USE_GC
    list->push(&base->gc, (void*)data);
#endif
    return data;
}

static void string_free(u64 ptr) {
    if (ptr == 0) {
        return;
    }
    struct pointer* data_ptr = vm->read(base->vm, ptr);
    if (data_ptr == 0) {
        return;
    }
    if (data_ptr->type != TYPE_STRING) {
        return;
    }
    data_ptr = vm->free(base->vm, ptr);
    if (data_ptr == 0) {
        return;
    }
    pointer->free(data_ptr);
}

static u64 string_copy(u64 ptr) {
    if (ptr == 0) {
        return 0;
    }
    const struct pointer* data_ptr = vm->read(base->vm, ptr);
    if (data_ptr == 0) {
        return 0;
    }
    if (data_ptr->type != TYPE_STRING) {
        return 0;
    }
    if (data_ptr->size == 0) {
        return 0;
    }
    struct pointer* copy_ptr = pointer->alloc(data_ptr->size, data_ptr->type);
    memcpy(copy_ptr->data, data_ptr->data, copy_ptr->size); /* NOLINT */
    u64 data = vm->write(base->vm, copy_ptr);
#ifdef USE_GC
    list->push(&base->gc, (void*)data);
#endif
    return data;
}

static void string_strcpy(u64 dest, u64 src) {
    if (src == dest) {
        return;
    }
    struct pointer* dest_ptr = vm->read(base->vm, dest);
    if (dest_ptr == 0) {
        return;
    }
    if (dest_ptr->type != TYPE_STRING) {
        return;
    }
    const struct pointer* src_ptr = vm->read(base->vm, src);
    if (src_ptr == 0) {
        return;
    }
    if (src_ptr->type != TYPE_STRING) {
        return;
    }
    if (src_ptr->size == 0) {
        return;
    }
    if (dest_ptr->size == 0) {
        dest_ptr->data = global_alloc(src_ptr->size);
        dest_ptr->size = src_ptr->size;
    } else {
        u64 size = src_ptr->size + 1;
        if (dest_ptr->size < size) {
            pointer->realloc(dest_ptr, size);
        }
    }
    char* data_dest = dest_ptr->data;
    const char* data_src = src_ptr->data; /* NOLINT */
    strcpy(data_dest, data_src); /* NOLINT */
}

static void string_strcat(u64 dest, u64 src) {
    if (src == dest) {
        return;
    }
    struct pointer* dest_ptr = vm->read(base->vm, dest);
    if (dest_ptr == 0) {
        return;
    }
    if (dest_ptr->type != TYPE_STRING) {
        return;
    }
    const struct pointer* src_ptr = vm->read(base->vm, src);
    if (src_ptr == 0) {
        return;
    }
    if (src_ptr->type != TYPE_STRING) {
        return;
    }
    if (src_ptr->size == 0) {
        return;
    }
    if (dest_ptr->size == 0) {
        dest_ptr->data = global_alloc(src_ptr->size);
        dest_ptr->size = src_ptr->size;
    } else {
        u64 size = dest_ptr->size + src_ptr->size - 1;
        if (dest_ptr->size < size) {
            pointer->realloc(dest_ptr, size);
        }
    }
    char* data_dest = dest_ptr->data;
    const char* data_src = src_ptr->data; /* NOLINT */
    strcat(data_dest, data_src); /* NOLINT */
}

static u64 string_match_last(u64 src, u64 match) {
    const struct pointer* src_ptr = vm->read(base->vm, src);
    if (src_ptr == 0) {
        return 0;
    }
    if (src_ptr->type != TYPE_STRING) {
        return 0;
    }
    const struct pointer* match_ptr = vm->read(base->vm, match);
    if (match_ptr == 0) {
        return 0;
    }
    if (match_ptr->type != TYPE_STRING) {
        return 0;
    }
    const char* data_src = src_ptr->data;
    if (data_src == 0) {
        return 0;
    }
    const char* data_match = match_ptr->data;
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
    if (data_last == 0) {
        return 0;
    }
    if (*data_match != 0) {
        return 0;
    }
    struct pointer* last_match_ptr = pointer->alloc(0, TYPE_STRING);
    last_match_ptr->data = --data_last;
    u64 data = vm->write(base->vm, last_match_ptr);
#ifdef USE_GC
    list->push(&base->gc, (void*)data);
#endif
    return data;
}

static u64 string_load(const char* src_data) {
    if (src_data == 0) {
        return 0;
    }
    u64 size = strlen(src_data) + 1;
    if (size == 0) {
        return 0;
    }
    struct vm_data* current = base->vm;
    struct pointer* data_ptr = pointer->alloc(size, TYPE_STRING);
    memcpy(data_ptr->data, src_data, size); /* NOLINT */
    u64 data = vm->write(current, data_ptr);
#ifdef USE_GC
    list->push(&base->gc, (void*)data);
#endif
    return data;
}

static u64 string_getcwd(void) {
    char cwd[PATH_MAX];
    u64 data_ptr;
    getcwd(cwd, sizeof(cwd));
    u64 size = strlen(cwd) + 1;
    char* data = global_alloc(size);
    strcpy(data, cwd); /* NOLINT */
    data_ptr = string_load(data);
    global_free(data, size);
    return data_ptr;
}

static void string_printf(u64 ptr) {
    struct pointer* data_ptr = vm->read(base->vm, ptr);
    if (data_ptr == 0) {
        return;
    }
    if (data_ptr->type != TYPE_STRING) {
        return;
    }
    const char* data = data_ptr->data;
    if (data == 0) {
        return;
    }
#ifdef USE_MEMORY_DEBUG_INFO
    void* ptr_data = data_ptr->data;
    printf("   .: %016llx > %016llx\n", (u64)data_ptr, (u64)ptr_data);
#endif
    puts(data);
}

static void string_put_char(u64 ptr, char value) {
    struct pointer* data_ptr = vm->read(base->vm, ptr);
    if (data_ptr == 0) {
        return;
    }
    if (data_ptr->type != TYPE_STRING) {
        return;
    }
    char* data = data_ptr->data;
    if (data == 0) {
        return;
    }
    *data = value;
}

static char* string_unsafe(u64 ptr) {
    if (ptr == 0) {
        return 0;
    }
    struct pointer* data_ptr = vm->read(base->vm, ptr);
    if (data_ptr == 0) {
        return 0;
    }
    if (data_ptr->type != TYPE_STRING) {
        return 0;
    }
    char* data = data_ptr->data;
    return data;
}

/* public */

void string_init() {
    pointer_vm_register_free(string_free);
}

const struct string_methods string_methods_definition = {
    .alloc = string_alloc,
    .copy = string_copy,
    .strcpy = string_strcpy,
    .strcat = string_strcat,
    .match_last = string_match_last,
    .load = string_load,
    .getcwd = string_getcwd,
    .printf = string_printf,
    .put_char = string_put_char,
    .unsafe = string_unsafe,
#ifndef USE_GC
    .free = string_free
#endif
};
