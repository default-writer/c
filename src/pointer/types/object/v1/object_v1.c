#include "pointer/types/object/v1/object_v1.h"
#include "common/alloc.h"
#include "list-micro/data.h"
#include "pointer/v1/pointer_v1.h"
#include "vm/v1/vm_v1.h"

#define DEFAULT_SIZE 0x100

/* api */
const struct object_methods object_methods_definition;
void object_init();

/* definition */
extern void pointer_vm_register_type(const struct vm_type* type);
extern struct pointer_data vm_pointer;
extern const struct vm_methods vm_methods_definition;
extern const struct list list_micro_definition;
extern const struct pointer_vm_methods pointer_vm_methods_definition;

/* definition */
static const struct vm_methods* vm = &vm_methods_definition;
static const struct list* list = &list_micro_definition;
static const struct pointer_vm_methods* pointer = &pointer_vm_methods_definition;
static struct pointer_data* base = &vm_pointer;
static const struct vm_type type_definition;

/* internal */
static u64 object_alloc(u64 size);
static void object_free(u64 ptr);
static u64 object_load(const void* data, u64 size);
static void* object_unsafe(u64 ptr);
static u64 object_size(u64 ptr);

/* implementation*/
static u64 object_alloc(u64 size) {
    if (size == 0) {
        return 0;
    }
    struct pointer* ptr = pointer->alloc(size, TYPE_OBJECT);
    u64 data = vm->write(ptr);
#ifdef USE_GC
    list->push(&base->gc, (void*)data);
#endif
    return data;
}

static void object_free(u64 ptr) {
    if (ptr == 0) {
        return;
    }
    struct pointer* data_ptr = vm->read(ptr);
    if (data_ptr == 0) {
        return;
    }
    if (data_ptr->type != TYPE_OBJECT) {
        return;
    }
    // ptr is already a valid address because of previous vm->read check
    data_ptr = vm->free(ptr);
    pointer->free(data_ptr);
}

static u64 object_load(const void* src_data, u64 size) {
    if (src_data == 0) {
        return 0;
    }
    if (size == 0) {
        return 0;
    }
    struct pointer* data_ptr = pointer->alloc(size, TYPE_OBJECT);
    memcpy(data_ptr->data, src_data, size); /* NOLINT */
    u64 data = vm->write(data_ptr);
#ifdef USE_GC
    list->push(&base->gc, (void*)data);
#endif
    return data;
}

static void* object_unsafe(u64 ptr) {
    if (ptr == 0) {
        return 0;
    }
    struct pointer* data_ptr = vm->read(ptr);
    if (data_ptr == 0) {
        return 0;
    }
    if (data_ptr->type != TYPE_OBJECT) {
        return 0;
    }
    char* data = data_ptr->data;
    return data;
}

static u64 object_size(u64 ptr) {
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
    .free = object_free
};

static void INIT init() {
    const struct vm_type* type = &type_definition;
    pointer_vm_register_type(type);
}

/* public */
const struct object_methods object_methods_definition = {
    .alloc = object_alloc,
    .free = object_free,
    .load = object_load,
    .unsafe = object_unsafe,
    .size = object_size
};

#ifndef ATTRIBUTE
void object_init() {
    init();
}
#endif
