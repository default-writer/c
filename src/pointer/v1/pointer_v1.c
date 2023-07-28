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
#include "common/alloc.h"
#include "list-micro/data.h"
#include "vm/v1/vm_v1.h"

#define DEFAULT_SIZE 0x100
#define POINTER_SIZE sizeof(struct pointer)
#define POINTER_DATA_SIZE sizeof(struct pointer_data)

/* public */
void pointer_vm_register_type(struct vm_type* ptr);

/* private */
struct vm_types {
    struct vm_types* next;
    struct vm_type* type;
};
struct pointer_data vm_pointer;

static struct vm_types vm_types_definition;
static struct vm_types* vm_types = &vm_types_definition;
static struct vm_type* types;

/* definition */
extern const struct vm_methods vm_methods_definition;
extern const struct list list_micro_definition;

extern void void_init(void);
extern void data_init(void);
extern void string_init(void);
extern void file_init(void);
extern void list_init(void);
extern void object_init(void);

/* definition */
static struct pointer_data* base = &vm_pointer;
static const struct vm_methods* vm = &vm_methods_definition;
static const struct list* list = &list_micro_definition;
static struct vm* vm_ptr;

/* api */
const struct pointer_vm_methods pointer_vm_methods_definition;
struct pointer_data* pointer_data_init(u64 size);
void pointer_data_destroy(struct pointer_data** ctx);

/* internal */
static struct pointer* pointer_vm_alloc(u64 size, u64 id);
static void pointer_vm_realloc(struct pointer* ptr, u64 size);
static void pointer_vm_free(struct pointer* ptr);
static void pointer_vm_cleanup(struct list_data** current);

static void pointer_push(u64 ptr);
static u64 pointer_peek(void);
static u64 pointer_pop(void);

#ifdef VM_DEBUG_INFO
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

static void pointer_init_internal(struct pointer_data* ptr, u64 size);
static void pointer_destroy_internal(struct pointer_data* ptr);
static void pointer_data_init_internal(struct pointer_data* ptr, u64 size);
static void pointer_data_destroy_internal(struct pointer_data* ptr);
static void copy_internal(struct pointer_data* dest, struct pointer_data* src);

/* free */
static void vm_types_init(struct vm_type* ptr);
static void vm_types_destroy(void);
static void pointer_free_internal(u64 ptr);

/* internal */
void pointer_vm_register_type(struct vm_type* ptr) {
    vm_types_init(ptr);
}

static const struct pointer_vm_methods* virtual = &pointer_vm_methods_definition;

static u64 type_count = TYPE_OBJECT;

static void vm_types_init(struct vm_type* ptr) {
    struct vm_types* next = global_alloc(sizeof(struct vm_types));
    next->type = ptr;
    next->next = vm_types;
    if (ptr->id == 0) {
        ptr->id = ++type_count;
    }
    vm_types = next;
}

static void DESTROY vm_types_destroy() {
    while (vm_types->next != 0) {
        struct vm_types* prev = vm_types->next;
        global_free(vm_types, sizeof(struct vm_types));
        vm_types = prev;
    }
}

static struct pointer* pointer_vm_alloc(u64 size, u64 id) {
    struct pointer* ptr = global_alloc(POINTER_SIZE);
    if (size != 0) {
        ptr->data = global_alloc(size);
        ptr->size = size;
    }
    ptr->id = id;
    return ptr;
}

static void pointer_vm_realloc(struct pointer* ptr, u64 size) {
    if (ptr != 0 && ptr->data != 0) {
        ptr->data = global_realloc(ptr->data, ptr->size, size);
        ptr->size = size;
    }
}

static void pointer_vm_free(struct pointer* ptr) {
    if (ptr == 0) {
        return;
    }
    if (ptr->data != 0 && ptr->size != 0) {
        global_free(ptr->data, ptr->size);
    }
    vm->free(ptr);
    global_free(ptr, POINTER_SIZE);
}

static void pointer_free_internal(u64 ptr) {
    struct pointer* data_ptr = vm->read(ptr);
    if (data_ptr == 0) {
        return;
    }
    const struct vm_type* type_ptr = types + data_ptr->id;
    if (type_ptr->free) {
        type_ptr->free(data_ptr);
    }
}

static void pointer_vm_cleanup(struct list_data** current) {
    u64 ptr = 0;
    while ((ptr = (u64)list->pop(current)) != 0) {
        pointer_free_internal(ptr);
    }
}

/* implementation*/
static void pointer_init_internal(struct pointer_data* ptr, u64 size) {
    vm_ptr = vm->init(size);
    list->init(&ptr->list);
#ifdef USE_GC
    list->init(&ptr->gc);
#endif
#ifndef ATTRIBUTE
    void_init();
    data_init(void);
    string_init(void);
    file_init(void);
    list_init(void);
    object_init(void);
#endif
    types = global_alloc((type_count + 1) * sizeof(struct vm_type));
    struct vm_types* current = vm_types;
    while (current->next != 0) {
        struct vm_types* prev = current->next;
        types[current->type->id] = *(current->type);
        current = prev;
    }
}

static void pointer_destroy_internal(struct pointer_data* ptr) {
    global_free(types, type_count * sizeof(struct vm_type));
#ifdef USE_GC
    list->destroy(&ptr->gc);
#endif
    list->destroy(&ptr->list);
    vm->destroy(&vm_ptr);
#ifndef ATTRIBUTE
    vm_types_destroy();
#endif
}

static void copy_internal(struct pointer_data* dest, struct pointer_data* src) {
    dest->list = src->list;
#ifdef USE_GC
    /* gc */
    dest->gc = src->gc;
#endif
}

static void pointer_data_init_internal(struct pointer_data* ptr, u64 size) {
    copy_internal(base, ptr);
    pointer_init_internal(base, size);
}

static void pointer_data_destroy_internal(struct pointer_data* ptr) {
    copy_internal(ptr, base);
    pointer_destroy_internal(base);
}

struct pointer_data* pointer_data_init(u64 size) {
    struct pointer_data* context = global_alloc(POINTER_DATA_SIZE);
    pointer_data_init_internal(context, size);
    return context;
}

void pointer_data_destroy(struct pointer_data** ctx) {
#ifdef VM_DEBUG_INFO
    vm->dump_ref();
    vm->dump();
#endif
#ifdef USE_GC
    pointer_gc();
#endif
    pointer_data_destroy_internal(*ctx);
    global_free(*ctx, POINTER_DATA_SIZE);
}

static void pointer_init(u64 size) {
    pointer_init_internal(base, size);
}

static void pointer_destroy(void) {
#ifdef VM_DEBUG_INFO
    vm->dump_ref();
    vm->dump();
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

#ifdef VM_DEBUG_INFO
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

const struct pointer_vm_methods pointer_vm_methods_definition = {
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
#ifdef VM_DEBUG_INFO
    .dump = pointer_dump,
    .dump_ref = pointer_dump_ref,
#endif
#ifdef USE_GC
    .gc = pointer_gc
#endif
};
