/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   February 21, 2025 at 4:33:49 AM GMT+3
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
    virtual_pointer_ptr ref;
    void* data;
    u64 size;
    u64 address;
    u64 id;
};

/* private */
typedef struct known_types* known_types_ptr;
typedef const struct known_types* const_known_types_ptr;

struct known_types {
    known_types_ptr next;
    u64 id;
    void (*free)(pointer_ptr ptr);
};

/* definition */
static struct known_types known_types_definition = { 0, 0, 0 };

/* definition */
static known_types_ptr known_types = &known_types_definition;

#ifndef ATTRIBUTE
extern void data_init(void);
extern void file_init(void);
extern void object_init(void);
extern void stack_init(void);
extern void string_init(void);
extern void string_pointer_init(void);
extern void user_init(void);
static void init(void) {
    data_init();
    file_init();
    object_init();
    stack_init();
    string_init();
    string_pointer_init();
    user_init();
}
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
static virtual_pointer_ptr pointer_ref(const_pointer_ptr ptr);
static u64 pointer_size(const_pointer_ptr ptr);
static void* pointer_read(const_pointer_ptr ptr);
static void pointer_write(pointer_ptr ptr, virtual_pointer_ptr virtual_pointer, u64 address);
static u64 pointer_read_type(const_pointer_ptr ptr, u64 virtual_id);

static void pointer_free_internal(pointer_ptr ptr);
static void known_types_init(u64 id, const type_methods_definitions* data_type);
static u64 user_types_init(const type_methods_definitions* data_type);

/* free */
static void vm_init(void);
static void vm_destroy(void);

/* internal */

static u64 known_types_counter = TYPE_USER;

static void known_types_init(u64 id, const type_methods_definitions* data_type) {
    known_types_ptr new_type = CALL(sys_memory)->alloc(sizeof(struct known_types));
    new_type->next = known_types;
    known_types = new_type;
    known_types->id = id;
    known_types->free = data_type->free;
}

static u64 user_types_init(const type_methods_definitions* data_type) {
    known_types_ptr new_type = CALL(sys_memory)->alloc(sizeof(struct known_types));
    new_type->next = known_types;
    new_type->id = known_types_counter++;
    new_type->free = data_type->free;
    known_types = new_type;
    return known_types->id;
}

static void INIT vm_init(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    global_statistics();
#endif
}

static void DESTROY vm_destroy(void) {
    while (known_types->next != 0) {
        known_types_ptr prev = known_types->next;
        CALL(sys_memory)->free(known_types, sizeof(struct known_types));
        known_types = prev;
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
    pointer_ptr ptr = CALL(sys_memory)->alloc(POINTER_SIZE);
    if (size != 0) {
        ptr->data = CALL(sys_memory)->alloc(size);
        ptr->size = size;
    }
    ptr->id = id;
    return ptr;
}

static void pointer_realloc(pointer_ptr ptr, u64 size) {
    if (ptr != 0 && ptr->data != 0) {
        ptr->data = CALL(sys_memory)->realloc(ptr->data, ptr->size, size);
        ptr->size = size;
    }
}

void pointer_register_known_type(u64 id, const type_methods_definitions* data_type) {
    known_types_init(id, data_type);
}

u64 pointer_register_user_type(const type_methods_definitions* data_type) {
    return user_types_init(data_type);
}

static void pointer_free(u64 ptr) {
    if (ptr == 0) {
        return;
    }
    pointer_ptr data_ptr = CALL(virtual)->read(ptr);
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
        CALL(sys_memory)->free(data_ptr, size);
    }
    CALL(virtual)->free(ptr);
    CALL(sys_memory)->free(ptr, POINTER_SIZE);
}

static u64 pointer_address(const_pointer_ptr ptr) {
    u64 address = 0;
    if (ptr) {
        address = ptr->address;
    }
    return address;
}

static virtual_pointer_ptr pointer_ref(const_pointer_ptr ptr) {
    virtual_pointer_ptr ref = 0;
    if (ptr) {
        ref = ptr->ref;
    }
    return ref;
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

static void pointer_write(pointer_ptr ptr, virtual_pointer_ptr ref, u64 address) {
    if (ptr) {
        ptr->ref = ref;
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
    CALL(virtual)->init(&vm, size);
#ifndef ATTRIBUTE
    init();
#endif
    default_types = CALL(sys_memory)->alloc(known_types_counter * sizeof(struct known_types));
    known_types_ptr current = known_types;
    while (current->next != 0) {
        known_types_ptr prev = current->next;
        u64 index = current->id;
        if (index > 0 && index < known_types_counter) {
            default_types[index] = *current;
        }
        current = prev;
    }
}

static void pointer_destroy(void) {
    CALL(sys_memory)->free(default_types, known_types_counter * sizeof(struct known_types));
    CALL(virtual)->destroy(&vm);
}

static void pointer_gc(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(virtual)->dump_ref();
    CALL(virtual)->dump();
#endif
    CALL(virtual)->enumerator_init();
    u64 ptr = 0;
    while ((ptr = CALL(virtual)->enumerator_next()) != 0) {
        pointer_ptr data_ptr = CALL(virtual)->read(ptr);
        pointer_free_internal(data_ptr);
    }
    CALL(virtual)->enumerator_destroy();
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
    .register_known_type = pointer_register_known_type,
    .register_user_type = pointer_register_user_type,
    .free = pointer_free,
    .release = pointer_release,
    .address = pointer_address,
    .ref = pointer_ref,
    .size = pointer_size,
    .read = pointer_read,
    .read_type = pointer_read_type,
    .write = pointer_write,
#ifdef USE_MEMORY_DEBUG_INFO
    .dump = pointer_dump,
    .dump_ref = pointer_dump_ref,
#endif
};

const pointer_methods* _pointer() {
    return &PRIVATE_API(pointer_methods_definitions);
}
