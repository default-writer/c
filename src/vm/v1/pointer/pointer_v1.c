/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   January 29, 2025 at 10:31:27 PM GMT+3
 *
 */
/*
    Copyright (C) 2022-2047 Artur Mustafin (artur.mustafin@gmail.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "generic/memory_v1.h"

#include "pointer_v1.h"

#include "std/macros.h"
#include "std/headers.h"

#include "vm/v1/virtual/virtual_v1.h"
#include "vm/v1/vm_type.h"
#include "vm/v1/vm_v1.h"
#include "vm/vm_type.h"

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
static struct vm_types* default_types;

#ifdef USE_MEMORY_DEBUG_INFO
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
static u64 pointer_read_type(const struct pointer* ptr, u64 type_id);

static void pointer_free_internal(struct pointer* ptr);

/* free */
static u64 vm_types_init(u64 id, const struct vm_type* data_type);
static void vm_init(void);
static void vm_destroy(void);

/* internal */

static u64 type_count = TYPE_USER;

static u64 vm_types_init(u64 id, const struct vm_type* data_type) {
    struct vm_types* next = generic_memory_v1->alloc(sizeof(struct vm_types));
    next->id = id == TYPE_NULL || id >= TYPE_USER ? type_count++ : id;
    next->free = data_type->free;
    next->next = vm_types;
    vm_types = next;
    return vm_types->id;
}

static void INIT vm_init(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    global_statistics();
#endif
}

static void DESTROY vm_destroy(void) {
    while (vm_types->next != 0) {
        struct vm_types* prev = vm_types->next;
        generic_memory_v1->free(vm_types, sizeof(struct vm_types));
        vm_types = prev;
    }
#ifdef USE_MEMORY_DEBUG_INFO
    global_statistics();
#endif
}

static void pointer_free_internal(struct pointer* ptr) {
    const struct vm_types* type_ptr = default_types + ptr->id;
    if (type_ptr->free) {
        type_ptr->free(ptr);
    }
}

static struct pointer* pointer_alloc(u64 size, u64 id) {
    struct pointer* ptr = generic_memory_v1->alloc(POINTER_SIZE);
    if (size != 0) {
        ptr->data = generic_memory_v1->alloc(size);
        ptr->size = size;
    }
    ptr->id = id;
    return ptr;
}

static void pointer_realloc(struct pointer* ptr, u64 size) {
    if (ptr != 0 && ptr->data != 0) {
        ptr->data = generic_memory_v1->realloc(ptr->data, ptr->size, size);
        ptr->size = size;
    }
}

u64 pointer_register_type(u64 id, const struct vm_type* data_type) {
    return vm_types_init(id, data_type);
}

static void pointer_free(u64 ptr) {
    if (ptr == 0) {
        return;
    }
    struct pointer* data_ptr = virtual_v1->read(ptr);
    if (data_ptr == 0) {
        return;
    }
    pointer_free_internal(data_ptr);
}

static void pointer_release(struct pointer* ptr) {
    if (ptr == 0) {
        return;
    }
    void* data_ptr = ptr->data;
    u64 size = ptr->size;
    if (data_ptr != 0 && size != 0) {
        generic_memory_v1->free(data_ptr, size);
    }
    virtual_v1->free(ptr);
    generic_memory_v1->free(ptr, POINTER_SIZE);
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
    void* data_ptr = 0;
    if (ptr) {
        data_ptr = ptr->data;
    }
    return data_ptr;
}

static void pointer_write(struct pointer* ptr, struct vm_data* vm_data_ptr, u64 address) {
    if (ptr) {
        ptr->vm = vm_data_ptr;
        ptr->address = address;
    }
}

static u64 pointer_read_type(const struct pointer* ptr, u64 type_id) {
    u64 id = 0;
    if (ptr && ptr->id == type_id) {
        id = type_id;
    }
    return id;
}

/* implementation */
static void pointer_init(u64 size) {
    virtual_v1->init(&vm, size);
    default_types = generic_memory_v1->alloc(type_count * sizeof(struct vm_types));
#ifndef ATTRIBUTE
    data_init();
    string_init();
    string_pointer_init();
    file_init();
    list_init();
    object_init();
#endif
    struct vm_types* current = vm_types;
    while (current->next != 0) {
        struct vm_types* prev = current->next;
        u64 index = current->id;
        if (index > 0 && index < type_count) {
            default_types[index] = *current;
        }
        current = prev;
    }
}

static void pointer_destroy(void) {
    generic_memory_v1->free(default_types, type_count * sizeof(struct vm_types));
    virtual_v1->destroy(&vm);
}

static void pointer_gc(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    virtual_v1->dump_ref();
    virtual_v1->dump();
#endif
    virtual_v1->enumerator_init();
    u64 ptr = 0;
    while ((ptr = virtual_v1->enumerator_next()) != 0) {
        struct pointer* data_ptr = virtual_v1->read(ptr);
        pointer_free_internal(data_ptr);
    }
    virtual_v1->enumerator_destroy();
}

#ifdef USE_MEMORY_DEBUG_INFO
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

const struct pointer_methods_v1 pointer_methods_definition_v1 = {
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
#ifdef USE_MEMORY_DEBUG_INFO
    .dump = pointer_dump,
    .dump_ref = pointer_dump_ref,
#endif
};
