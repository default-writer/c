#include "pointer/types/data/v1/data_v1.h"
#include "common/alloc.h"
#include "list-micro/data.h"
#include "pointer/v1/pointer_v1.h"
#include "vm/v1/vm_v1.h"

#define DEFAULT_SIZE 0x100

/* api */
const struct data_methods data_methods_definition;
void data_init();

/* definition */
extern void pointer_vm_register_type(const struct vm_type* type);
extern struct pointer_data vm_pointer;
extern const struct vm vm_definition;
extern const struct list list_micro_definition;
extern const struct pointer_vm_methods vm_methods_definition;

/* definition */
static const struct vm* vm = &vm_definition;
static const struct list* list = &list_micro_definition;
static const struct pointer_vm_methods* pointer = &vm_methods_definition;
static struct pointer_data* base = &vm_pointer;
static const struct vm_type type_definition;

/* internal */
static u64 data_alloc(u64 size);
static void data_free(u64 ptr);
static void* data_unsafe(u64 ptr);
static u64 data_size(u64 ptr);

/* implementation*/
static u64 data_alloc(u64 size) {
    struct pointer* f_ptr = pointer->alloc(size, TYPE_PTR);
    u64 data = vm->write(f_ptr);
#ifdef USE_GC
    list->push(&base->gc, (void*)data);
#endif
    return data;
}

static void data_free(u64 ptr) {
    if (ptr == 0) {
        return;
    }
    struct pointer* data_ptr = vm->read(ptr);
    if (data_ptr == 0) {
        return;
    }
    if (data_ptr->type != TYPE_PTR) {
        return;
    }
    // ptr is already a valid address because of previous vm->read check
    data_ptr = vm->free(ptr);
    pointer->free(data_ptr);
}

static void* data_unsafe(u64 ptr) {
    if (ptr == 0) {
        return 0;
    }
    struct pointer* data_ptr = vm->read(ptr);
    if (data_ptr == 0) {
        return 0;
    }
    if (data_ptr->type != TYPE_PTR) {
        return 0;
    }
    void* data = data_ptr->data;
    return data;
}

static u64 data_size(u64 ptr) {
    if (ptr == 0) {
        return 0;
    }
    const struct pointer* data_ptr = vm->read(ptr);
    if (data_ptr == 0) {
        return 0;
    }
    u64 size = data_ptr->size;
    return size;
}

static const struct vm_type type_definition = {
    .free = data_free
};

static void INIT init() {
    const struct vm_type* type = &type_definition;
    pointer_vm_register_type(type);
}

/* public */
const struct data_methods data_methods_definition = {
    .alloc = data_alloc,
    .free = data_free,
    .unsafe = data_unsafe,
    .size = data_size
};

#ifndef ATTRIBUTE
void data_init() {
    init();
}
#endif