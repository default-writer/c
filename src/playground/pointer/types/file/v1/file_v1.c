#include "playground/pointer/types/file/v1/file_v1.h"
#include "common/alloc.h"
#include "list-micro/data.h"
#include "playground/pointer/types/virtual/v1/virtual_v1.h"
#include "playground/pointer/v1/pointer_v1.h"
#include "playground/vm/v1/vm_v1.h"

#define DEFAULT_SIZE 0x100

/* definition */
extern void pointer_vm_register_free(function function);

extern struct pointer_data vm_pointer;
static struct pointer_data* base = &vm_pointer;

/* definition */
extern const struct vm vm_definition;
extern const struct list list_micro_definition;
extern const struct pointer_vm_methods vm_methods_definition;
extern const struct virtual_methods virtual_methods_definition;

static const struct vm* vm = &vm_definition;
static const struct list* list = &list_micro_definition;
static const struct pointer_vm_methods* pointer = &vm_methods_definition;
static const struct virtual_methods* virtual = &virtual_methods_definition;

struct file_handler {
    FILE* file;
#ifdef USE_MEMORY_DEBUG_INFO
    char* path;
#endif
};

/* api */
static u64 file_alloc(u64 file_path_ptr, u64 mode_ptr);
static u64 file_read(u64 ptr);
static void file_free(u64 ptr);

/* internal */

/* implementation*/

static u64 file_alloc(u64 file_path, u64 mode) {
    if (file_path == 0) {
        return 0;
    }
    if (mode == 0) {
        return 0;
    }
    struct pointer* file_path_ptr = vm->read(&base->vm, file_path);
    if (file_path_ptr == 0) {
        return 0;
    }
    if (file_path_ptr->type != TYPE_STRING) {
        return 0;
    }
    const struct pointer* mode_ptr = vm->read(&base->vm, mode);
    if (mode_ptr == 0) {
        return 0;
    }
    if (mode_ptr->type != TYPE_STRING) {
        return 0;
    }
    const char* file_path_data = file_path_ptr->data;
    if (file_path_data == 0) {
        return 0;
    }
    const char* mode_data = mode_ptr->data;
    if (mode_data == 0) {
        return 0;
    }
    FILE* file = fopen(file_path_data, mode_data); /* NOLINT */
    if (file == 0) {
        return 0;
    }
    struct pointer* f_ptr = pointer->alloc(sizeof(struct file_handler), TYPE_FILE);
    struct file_handler* handler = f_ptr->data;
    if (handler == 0) {
        fclose(file);
        return 0;
    }
    handler->file = file;
#ifdef USE_MEMORY_DEBUG_INFO
    handler->path = file_path_ptr->data;
#endif
    u64 data = vm->write(&base->vm, f_ptr);
#ifdef USE_GC
    list->push(&base->gc, (void*)data);
#endif
    return data;
}

static u64 file_read(u64 ptr) {
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
    struct file_handler* handler = data_ptr->data;
    if (handler == 0) {
        return 0;
    }
    FILE* file = handler->file;
    if (file == 0) {
        return 0;
    }
    fseek(file, 0, SEEK_END); /* NOLINT */
    u64 size = (u64)ftell(file);
    fseek(file, 0, SEEK_SET);
    u64 data_size = size + 1;
    u64 void_data = virtual->alloc(data_size);
    void* file_data = virtual->unsafe(void_data);
    fread(file_data, 1, size, handler->file);
#ifdef USE_GC
    list->push(&base->gc, (void*)void_data);
#endif
    return void_data;
}

static void file_free(u64 ptr) {
    if (ptr == 0) {
        return;
    }
    struct pointer* data_ptr = vm->read(&base->vm, ptr);
    if (data_ptr == 0) {
        return;
    }
    if (data_ptr->type != TYPE_FILE) {
        return;
    }
    struct file_handler* handler = data_ptr->data;
    if (handler == 0) {
        return;
    }
    FILE* file = handler->file;
    if (file != 0) {
        fclose(file);
        handler->file = 0;
    }
    data_ptr = vm->free(&base->vm, ptr);
    if (data_ptr == 0) {
        return;
    }
    pointer->free(data_ptr);
}

/* public */

void file_init() {
    pointer_vm_register_free(file_free);
}

const struct file_methods file_methods_definition = {
    .alloc = file_alloc,
    .read = file_read,
    .free = file_free
};
