/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   February 16, 2025 at 1:08:28 PM GMT+3
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

#include "std/data.h"

#include "pointer_v1.h"

#include "std/api.h"

#include "sys/memory/memory_v1.h"

#include "vm/virtual/virtual_v1.h"

#include <stdio.h>

#define DEFAULT_SIZE 0x100
#define POINTER_SIZE sizeof(struct pointer)

/* private */
struct pointer {
    virtual_pointer_ptr virtual_pointer;
    void* data;
    u64 size;
    u64 address;
    u64 id;
};

/* private */
typedef struct known_types* known_types_ptr;
typedef const struct known_types* const_known_types_ptr;

struct known_types {
    u64 id;
    known_types_ptr next;
    void (*free)(pointer_ptr ptr);
};

/* definition */
static struct known_types known_types_definition;

/* definition */
static known_types_ptr vm_types = &known_types_definition;

#ifndef ATTRIBUTE
extern void data_init(void);
extern void file_init(void);
extern void object_init(void);
extern void stack_init(void);
extern void string_init(void);
extern void string_pointer_init(void);
extern void user_init(void);
#endif

static struct vm* vm;
static known_types_ptr default_types;

#ifdef USE_MEMORY_DEBUG_INFO
static void pointer_dump(pointer_ptr ptr);
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
    stack_ptr list;
};

static void pointer_init(u64 size);
static void pointer_destroy(void);
static void pointer_gc(void);

static pointer_ptr pointer_alloc(u64 size, u64 id);
static void pointer_realloc(pointer_ptr ptr, u64 size);
static void pointer_free(u64 ptr);
static void pointer_release(pointer_ptr ptr);
static u64 pointer_address(const_pointer_ptr ptr);
static virtual_pointer_ptr pointer_vm(const_pointer_ptr ptr);
static u64 pointer_size(const_pointer_ptr ptr);
static void* pointer_read(const_pointer_ptr ptr);
static void pointer_write(pointer_ptr ptr, virtual_pointer_ptr virtual_pointer, u64 address);
static u64 pointer_read_type(const_pointer_ptr ptr, u64 virtual_id);

static void pointer_free_internal(pointer_ptr ptr);

/* free */
static u64 vm_types_init(u64 id, const type_methods_definitions* data_type);
static void vm_init(void);
static void vm_destroy(void);

/* internal */

static u64 virtual_count = TYPE_USER;

static u64 vm_types_init(u64 id, const type_methods_definitions* data_type) {
    known_types_ptr next = sys_memory->alloc(sizeof(struct known_types));
    next->id = id == TYPE_NULL || id >= TYPE_USER ? virtual_count++ : id;
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
        known_types_ptr prev = vm_types->next;
        sys_memory->free(vm_types, sizeof(struct known_types));
        vm_types = prev;
    }
#ifdef USE_MEMORY_DEBUG_INFO
    global_statistics();
#endif
}

static void pointer_free_internal(pointer_ptr ptr) {
    const_known_types_ptr virtual_ptr = default_types + ptr->id;
    if (virtual_ptr->free) {
        virtual_ptr->free(ptr);
    }
}

static pointer_ptr pointer_alloc(u64 size, u64 id) {
    pointer_ptr ptr = sys_memory->alloc(POINTER_SIZE);
    if (size != 0) {
        ptr->data = sys_memory->alloc(size);
        ptr->size = size;
    }
    ptr->id = id;
    return ptr;
}

static void pointer_realloc(pointer_ptr ptr, u64 size) {
    if (ptr != 0 && ptr->data != 0) {
        ptr->data = sys_memory->realloc(ptr->data, ptr->size, size);
        ptr->size = size;
    }
}

u64 pointer_register_type(u64 id, const type_methods_definitions* data_type) {
    return vm_types_init(id, data_type);
}

static void pointer_free(u64 ptr) {
    if (ptr == 0) {
        return;
    }
    pointer_ptr data_ptr = virtual->read(ptr);
    if (data_ptr == 0) {
        return;
    }
    pointer_free_internal(data_ptr);
}

static void pointer_release(pointer_ptr ptr) {
    if (ptr == 0) {
        return;
    }
    void* data_ptr = ptr->data;
    u64 size = ptr->size;
    if (data_ptr != 0 && size != 0) {
        sys_memory->free(data_ptr, size);
    }
    virtual->free(ptr);
    sys_memory->free(ptr, POINTER_SIZE);
}

static u64 pointer_address(const_pointer_ptr ptr) {
    u64 address = 0;
    if (ptr) {
        address = ptr->address;
    }
    return address;
}

static virtual_pointer_ptr pointer_vm(const_pointer_ptr ptr) {
    virtual_pointer_ptr virtual_pointer = 0;
    if (ptr) {
        virtual_pointer = ptr->virtual_pointer;
    }
    return virtual_pointer;
}

static u64 pointer_size(const_pointer_ptr ptr) {
    u64 size = 0;
    if (ptr) {
        size = ptr->size;
    }
    return size;
}

static void* pointer_read(const_pointer_ptr ptr) {
    void* data_ptr = 0;
    if (ptr) {
        data_ptr = ptr->data;
    }
    return data_ptr;
}

static void pointer_write(pointer_ptr ptr, virtual_pointer_ptr virtual_pointer, u64 address) {
    if (ptr) {
        ptr->virtual_pointer = virtual_pointer;
        ptr->address = address;
    }
}

static u64 pointer_read_type(const_pointer_ptr ptr, u64 virtual_id) {
    u64 id = 0;
    if (ptr && ptr->id == virtual_id) {
        id = virtual_id;
    }
    return id;
}

/* implementation */
static void pointer_init(u64 size) {
    virtual->init(&vm, size);
    default_types = sys_memory->alloc(virtual_count * sizeof(struct known_types));
#ifndef ATTRIBUTE
    data_init();
    file_init();
    object_init();
    stack_init();
    string_init();
    string_pointer_init();
    user_init();
#endif
    known_types_ptr current = vm_types;
    while (current->next != 0) {
        known_types_ptr prev = current->next;
        u64 index = current->id;
        if (index > 0 && index < virtual_count) {
            default_types[index] = *current;
        }
        current = prev;
    }
}

static void pointer_destroy(void) {
    sys_memory->free(default_types, virtual_count * sizeof(struct known_types));
    virtual->destroy(&vm);
}

static void pointer_gc(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    virtual->dump_ref();
    virtual->dump();
#endif
    virtual->enumerator_init();
    u64 ptr = 0;
    while ((ptr = virtual->enumerator_next()) != 0) {
        pointer_ptr data_ptr = virtual->read(ptr);
        pointer_free_internal(data_ptr);
    }
    virtual->enumerator_destroy();
}

#ifdef USE_MEMORY_DEBUG_INFO
static void pointer_dump(pointer_ptr ptr) {
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

const pointer_methods PRIVATE_API(pointer_methods_definitions) = {
    .init = pointer_init,
    .destroy = pointer_destroy,
    .gc = pointer_gc,
    .alloc = pointer_alloc,
    .realloc = pointer_realloc,
    .register_type = pointer_register_type,
    .free = pointer_free,
    .release = pointer_release,
    .address = pointer_address,
    .virtual = pointer_vm,
    .size = pointer_size,
    .read = pointer_read,
    .read_type = pointer_read_type,
    .write = pointer_write,
#ifdef USE_MEMORY_DEBUG_INFO
    .dump = pointer_dump,
    .dump_ref = pointer_dump_ref,
#endif
};
