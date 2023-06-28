#include "playground/pointer/types/virtual/v1/virtual_v1.h"
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

/* api */
static u64 virtual_alloc(u64 size);
static void virtual_free(u64 ptr);
static void* virtual_unsafe(u64 ptr);

/* internal */

/* implementation*/

static u64 virtual_alloc(u64 size) {
    struct pointer* f_ptr = pointer->alloc(size, TYPE_PTR);
    u64 data = vm->write(&base->vm, f_ptr);
#ifdef USE_GC
    list->push(&base->gc, (void*)data);
#endif
    return data;
}

static void virtual_free(u64 ptr) {
    if (ptr == 0) {
        return;
    }
    struct pointer* data_ptr = vm->read(&base->vm, ptr);
    if (data_ptr == 0) {
        return;
    }
    if (data_ptr->type != TYPE_PTR) {
        return;
    }
    data_ptr = vm->free(&base->vm, ptr);
    if (data_ptr == 0) {
        return;
    }
    pointer->free(data_ptr);
}

static void* virtual_unsafe(u64 ptr) {
    if (ptr == 0) {
        return 0;
    }
    struct pointer* data_ptr = vm->read(&base->vm, ptr);
    if (data_ptr == 0) {
        return 0;
    }
    if (data_ptr->type != TYPE_PTR) {
        return 0;
    }
    void* data = data_ptr->data;
    return data;
}

/* public */

void virtual_init() {
    pointer_vm_register_free(virtual_free);
}

const struct virtual_methods virtual_methods_definition = {
    .alloc = virtual_alloc,
    .free = virtual_free,
    .unsafe = virtual_unsafe
};
