/*
 *
 * MIT License
 *
 * Copyright (c) 2023 Artur Mustafin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */
#include "pointer/v1/pointer_v1.h"
#include "common/memory.h"
#include "list-micro/data.h"
#include "vm/v1/vm_v1.h"

#define DEFAULT_SIZE 0x100
#define POINTER_SIZE sizeof(struct pointer)
#define POINTER_DATA_SIZE sizeof(struct pointer_data)

/* public */
u64 pointer_vm_register_type(u64 id, const struct vm_type* type);

/* private */

struct vm_types {
    struct vm_types* next;
    u64 id;
    void (*free)(struct pointer* ptr);
};
struct pointer_data vm_pointer;

static struct vm_types vm_types_definition;
static struct vm_types* vm_types = &vm_types_definition;
static struct vm_types* types;

extern void void_init(void);
extern void data_init(void);
extern void string_init(void);
extern void file_init(void);
extern void list_init(void);
extern void object_init(void);
extern void* memory_alloc(u64 size);
extern void memory_free(void* data, u64 size);

/* definition */
extern const struct vm_methods vm_methods_definition;
extern const struct list list_micro_definition;

/* definition */
static const struct vm_methods* vm = &vm_methods_definition;
static const struct list* list = &list_micro_definition;

/* definition */
static struct pointer_data* base = &vm_pointer;

/* api */
const struct pointer_vm_methods pointer_vm_methods_definition;

/* internal */
static struct pointer* pointer_vm_alloc(u64 size, u64 id);
static void pointer_vm_realloc(struct pointer* ptr, u64 size);
static void pointer_vm_free(struct pointer* ptr);
static void pointer_vm_release(struct list_data** current);

static void pointer_push(u64 ptr);
static u64 pointer_peek(void);
static u64 pointer_pop(void);

#ifdef USE_VM_DEBUG_INFO
static void pointer_dump(struct pointer* ptr);
static void pointer_dump_ref(void** ptr);
#endif

#ifdef USE_GC
static void pointer_gc(void);
#endif

/* internal */
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
static void pointer_release(void);

static void pointer_init_internal(struct pointer_data* ptr, u64 size);
static void pointer_destroy_internal(struct pointer_data* ptr);

/* free */
static u64 vm_types_init(u64 id, const struct vm_type* type);
static void vm_types_destroy(void);
static void pointer_free_internal(struct pointer* ptr);

/* internal */
u64 pointer_vm_register_type(u64 id, const struct vm_type* type) {
    return vm_types_init(id, type);
}

static const struct pointer_vm_methods* virtual = &pointer_vm_methods_definition;

static u64 type_count = TYPE_USER;

static u64 vm_types_init(u64 id, const struct vm_type* type) {
    struct vm_types* next = memory->alloc(sizeof(struct vm_types));
    next->id = id == TYPE_NULL || id >= TYPE_USER ? type_count++ : id;
    next->free = type->free;
    next->next = vm_types;
    vm_types = next;
    return vm_types->id;
}

static void DESTROY vm_types_destroy() {
    while (vm_types->next != 0) {
        struct vm_types* prev = vm_types->next;
        memory->free(vm_types, sizeof(struct vm_types));
        vm_types = prev;
    }
}

static struct pointer* pointer_vm_alloc(u64 size, u64 id) {
    struct pointer* ptr = memory->alloc(POINTER_SIZE);
    if (size != 0) {
        ptr->data = memory->alloc(size);
        ptr->size = size;
    }
    ptr->id = id;
    return ptr;
}

static void pointer_vm_realloc(struct pointer* ptr, u64 size) {
    if (ptr != 0 && ptr->data != 0) {
        ptr->data = memory->realloc(ptr->data, ptr->size, size);
        ptr->size = size;
    }
}

static void pointer_vm_free(struct pointer* ptr) {
    if (ptr == 0) {
        return;
    }
    if (ptr->data != 0 && ptr->size != 0) {
        memory->free(ptr->data, ptr->size);
    }
    vm->free(ptr);
    memory->free(ptr, POINTER_SIZE);
}

static void pointer_free_internal(struct pointer* ptr) {
    const struct vm_types* type_ptr = types + ptr->id;
    if (type_ptr->free) {
        type_ptr->free(ptr);
    }
}

static void pointer_vm_release(struct list_data** current) {
    u64 ptr = 0;
    while ((ptr = (u64)list->pop(current)) != 0) {
        struct pointer* data_ptr = vm->read(ptr);
        if (data_ptr == 0) {
            return;
        }
        pointer_free_internal(data_ptr);
    }
}

/* implementation */
static void pointer_init_internal(struct pointer_data* ptr, u64 size) {
    ptr->vm = vm->init(size);
#ifdef USE_GC
    list->init(&ptr->gc);
#endif
#ifndef ATTRIBUTE
    void_init();
    data_init(void);
    string_init(void);
    string_ref_init(void);
    file_init(void);
    list_init(void);
    object_init(void);
#endif
    types = memory->alloc(type_count * sizeof(struct vm_types));
    struct vm_types* current = vm_types;
    while (current->next != 0) {
        struct vm_types* prev = current->next;
        u64 index = current->id;
        if (index > 0 && index < type_count) {
            types[index] = *current;
        }
        current = prev;
    }
}

static void pointer_destroy_internal(struct pointer_data* ptr) {
    memory->free(types, type_count * sizeof(struct vm_types));
#ifdef USE_GC
    list->destroy(&ptr->gc);
#endif
    vm->destroy(&ptr->vm);
#ifndef ATTRIBUTE
    vm_types_destroy();
#endif
}

static void pointer_init(u64 size) {
    pointer_init_internal(base, size);
}

static void pointer_destroy(void) {
#ifdef USE_GC
    pointer_gc();
#endif
    pointer_destroy_internal(base);
}

static void pointer_release(void) {
#ifdef USE_VM_DEBUG_INFO
    vm->dump_ref();
    vm->dump();
#endif
    vm->enumerator_init();
    void** ptr = 0;
    while ((ptr = vm->enumerator_next()) != 0) {
        struct pointer* data_ptr = *ptr;
        if (data_ptr == 0) {
            continue;
        }
        pointer_free_internal(data_ptr);
    }
    vm->enumerator_destroy();
}

#ifdef USE_GC
static void pointer_gc(void) {
    pointer_vm_release(&base->gc);
}
#endif

#ifdef USE_VM_DEBUG_INFO
static void pointer_dump(struct pointer* ptr) {
    printf("   ^: %016llx > %016llx\n", (u64)ptr, (u64)ptr->data);
}

static void pointer_dump_ref(void** ptr) {
    if (*ptr == 0) {
        return;
    }
    printf("   &: %016llx > %016llx\n", (u64)ptr, (u64)*ptr);
}
#endif

/* public */

const struct pointer_vm_methods pointer_vm_methods_definition = {
    .alloc = pointer_vm_alloc,
    .realloc = pointer_vm_realloc,
    .free = pointer_vm_free,
    .release = pointer_vm_release
};

const struct pointer_methods pointer_methods_definition = {
    .init = pointer_init,
    .destroy = pointer_destroy,
    .release = pointer_release,
#ifdef USE_VM_DEBUG_INFO
    .dump = pointer_dump,
    .dump_ref = pointer_dump_ref,
#endif
#ifdef USE_GC
    .gc = pointer_gc
#endif
};
