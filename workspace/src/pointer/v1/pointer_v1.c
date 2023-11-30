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
#include "common/memory.h"

#include "pointer/types/types.h"
#include "pointer/v1/pointer_v1.h"
#include "pointer/v1/virtual_v1.h"

#if defined(USE_MEMORY_DEBUG_INFO)
#define VM_GLOBAL_DEBUG_INFO
#endif

#define DEFAULT_SIZE 0x100
#define POINTER_SIZE sizeof(struct pointer)

/* private */
struct vm_data;
struct list_data;

struct pointer {
    struct vm_data* vm;
    void* data;
    u64 size;
    u64 address;
    u64 id;
};

/* private */
struct vm_types {
    struct vm_types* next;
    u64 id;
    void (*free)(struct pointer* ptr);
};

/* definition */
static struct vm_types vm_types_definition;

/* definition */
static struct vm_types* vm_types = &vm_types_definition;

#ifndef ATTRIBUTE
extern void data_init(void);
extern void string_init(void);
extern void string_pointer_init(void);
extern void file_init(void);
extern void list_init(void);
extern void object_init(void);
#endif

static struct vm* vm;
static struct vm_types* types;

/* api */

/* internal */
static void pointer_push(u64 ptr);
static u64 pointer_peek(void);
static u64 pointer_pop(void);

#ifdef USE_VM_DEBUG_INFO
static void pointer_dump(struct pointer* ptr);
static void pointer_dump_ref(void** ptr);
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
static void pointer_gc(void);

static struct pointer* pointer_alloc(u64 size, u64 id);
static void pointer_realloc(struct pointer* ptr, u64 size);
static void pointer_free(u64 ptr);
static void pointer_release(struct pointer* ptr);
static u64 pointer_address(const struct pointer* ptr);
static u64 pointer_size(const struct pointer* ptr);
static void* pointer_read(const struct pointer* ptr);
static void pointer_write(struct pointer* ptr, struct vm_data* vm_data_ptr, u64 address);
static u64 pointer_read_type(const struct pointer* ptr, u64 type);

static void pointer_free_internal(struct pointer* ptr);

static void* _alloc(u64 size);
static void _free(void* ptr, u64 size);

/* free */
static u64 vm_types_init(u64 id, const struct vm_type* type);
static void vm_init(void);
static void vm_destroy(void);

/* internal */

static u64 type_count = TYPE_USER;

static u64 vm_types_init(u64 id, const struct vm_type* type) {
    struct vm_types* next = memory->alloc(sizeof(struct vm_types));
    next->id = id == TYPE_NULL || id >= TYPE_USER ? type_count++ : id;
    next->free = type->free;
    next->next = vm_types;
    vm_types = next;
    return vm_types->id;
}

static void INIT vm_init(void) {
#if defined(VM_GLOBAL_DEBUG_INFO)
    global_statistics();
#endif
}

static void DESTROY vm_destroy(void) {
    while (vm_types->next != 0) {
        struct vm_types* prev = vm_types->next;
        memory->free(vm_types, sizeof(struct vm_types));
        vm_types = prev;
    }
#if defined(VM_GLOBAL_DEBUG_INFO)
    global_statistics();
#endif
}

static void pointer_free_internal(struct pointer* ptr) {
    const struct vm_types* type_ptr = types + ptr->id;
    if (type_ptr->free) {
        type_ptr->free(ptr);
    }
}

static struct pointer* pointer_alloc(u64 size, u64 id) {
    struct pointer* ptr = memory->alloc(POINTER_SIZE);
    if (size != 0) {
        ptr->data = memory->alloc(size);
        ptr->size = size;
    }
    ptr->id = id;
    return ptr;
}

static void pointer_realloc(struct pointer* ptr, u64 size) {
    if (ptr != 0 && ptr->data != 0) {
        ptr->data = memory->realloc(ptr->data, ptr->size, size);
        ptr->size = size;
    }
}

u64 pointer_register_type(u64 id, const struct vm_type* type) {
    return vm_types_init(id, type);
}

static void pointer_free(u64 ptr) {
    if (ptr == 0) {
        return;
    }
    struct pointer* data_ptr = virtual->read(ptr);
    if (data_ptr == 0) {
        return;
    }
    pointer_free_internal(data_ptr);
}

static void pointer_release(struct pointer* ptr) {
    if (ptr == 0) {
        return;
    }
    void* data = ptr->data;
    u64 size = ptr->size;
    if (data != 0 && size != 0) {
        memory->free(data, size);
    }
    virtual->free(ptr);
    memory->free(ptr, POINTER_SIZE);
}

static u64 pointer_address(const struct pointer* ptr) {
    u64 address = 0;
    if (ptr) {
        address = ptr->address;
    }
    return address;
}

static struct vm_data* pointer_vm(const struct pointer* ptr) {
    struct vm_data* vm_data_ptr = 0;
    if (ptr) {
        vm_data_ptr = ptr->vm;
    }
    return vm_data_ptr;
}

static u64 pointer_size(const struct pointer* ptr) {
    u64 size = 0;
    if (ptr) {
        size = ptr->size;
    }
    return size;
}

static void* pointer_read(const struct pointer* ptr) {
    void* data = 0;
    if (ptr) {
        data = ptr->data;
    }
    return data;
}

static void pointer_write(struct pointer* ptr, struct vm_data* vm_data_ptr, u64 address) {
    if (ptr) {
        ptr->vm = vm_data_ptr;
        ptr->address = address;
    }
}

static u64 pointer_read_type(const struct pointer* ptr, u64 type) {
    u64 id = 0;
    if (ptr && ptr->id == type) {
        id = type;
    }
    return id;
}

/* implementation */
static void pointer_init(u64 size) {
    virtual->init(&vm, size);
    types = memory->alloc(type_count * sizeof(struct vm_types));
#ifndef ATTRIBUTE
    data_init();
    string_init();
    string_pointer_init();
    string_ref_init();
    file_init();
    list_init();
    object_init();
#endif
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

static void pointer_destroy(void) {
    memory->free(types, type_count * sizeof(struct vm_types));
    virtual->destroy(&vm);
}

static void pointer_gc(void) {
#ifdef USE_VM_DEBUG_INFO
    virtual->dump_ref();
    virtual->dump();
#endif
    virtual->enumerator_init();
    u64 ptr = 0;
    while ((ptr = virtual->enumerator_next()) != 0) {
        struct pointer* data_ptr = virtual->read(ptr);
        pointer_free_internal(data_ptr);
    }
    virtual->enumerator_destroy();
}

#ifdef USE_VM_DEBUG_INFO
static void pointer_dump(struct pointer* ptr) {
#if defined(VM_MEMORY_DEBUG_INFO)
    printf("   ^: %016llx > %016llx\n", (u64)ptr, (u64)ptr->data);
#endif
}

static void pointer_dump_ref(void** ptr) {
    if (*ptr == 0) {
        return;
    }
#if defined(VM_MEMORY_DEBUG_INFO)
    printf("   &: %016llx > %016llx\n", (u64)ptr, (u64)*ptr);
#endif
}
#endif

/* public */

const struct pointer_methods pointer_methods_definition = {
    .init = pointer_init,
    .destroy = pointer_destroy,
    .gc = pointer_gc,
    .alloc = pointer_alloc,
    .realloc = pointer_realloc,
    .register_type = pointer_register_type,
    .free = pointer_free,
    .release = pointer_release,
    .address = pointer_address,
    .vm = pointer_vm,
    .size = pointer_size,
    .read = pointer_read,
    .read_type = pointer_read_type,
    .write = pointer_write,
#ifdef USE_VM_DEBUG_INFO
    .dump = pointer_dump,
    .dump_ref = pointer_dump_ref,
#endif
};
