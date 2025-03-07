/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 7, 2025 at 3:18:36 PM GMT+3
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

#include "system/memory/memory_v1.h"

#include "virtual/api/api_v1.h"
#include "virtual/virtual/virtual_v1.h"

#include <stdio.h>

#define DEFAULT_SIZE 0x100
#define POINTER_SIZE sizeof(pointer_type)
#define KNOWN_TYPES_SIZE sizeof(known_types_type)

/* private */
typedef struct pointer {
    public_pointer_type pointer;
    u64 size;
    u64 id;
    void* data;
} pointer_type;

/* private */
typedef struct known_types* known_types_ptr;
typedef const struct known_types* const_known_types_ptr;

typedef struct known_types {
    known_types_ptr next;
    u64 id;
    desctructor desctructor;
} known_types_type;

/* definition */
static known_types_type known_types_definition = { 0, 0, 0 };

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

static vm_ptr vm = 0;
static known_types_ptr default_types;

#ifdef USE_MEMORY_DEBUG_INFO
static void pointer_dump(pointer_ptr ptr);
static void pointer_dump_ref(pointer_ptr* ptr);
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
static const_pointer_ptr pointer_copy(const void* data, u64 size, u64 id);
static const_pointer_ptr pointer_copy_guard(const void* data, u64 size, u64 offset, u64 id);
static void pointer_memcpy(const_pointer_ptr const_ptr, const void* data, u64 size);
static void pointer_realloc(pointer_ptr ptr, u64 size);
static void pointer_free(u64 ptr);
static void pointer_release(const_pointer_ptr const_ptr);
static u64 pointer_size(const_pointer_ptr const_ptr);
static void* pointer_read(const_pointer_ptr const_ptr);
static void* pointer_read_guard(const_pointer_ptr const_ptr, u64 offset);
static void pointer_guard(const_pointer_ptr const_ptr, u64 offset);
static void pointer_write(pointer_ptr ptr, virtual_pointer_ptr vptr, u64 address);
static u64 pointer_read_type(const_pointer_ptr const_ptr, u64 virtual_id);

static void known_types_init(u64 id, const type_methods_definitions* data_type);
static u64 user_types_init(const type_methods_definitions* data_type);

/* free */
static void vm_init(void);
static void vm_destroy(void);

/* internal */

static u64 known_types_counter = TYPE_USER;

static void known_types_init(u64 id, const type_methods_definitions* data_type) {
    known_types_ptr new_type = CALL(system_memory)->alloc(KNOWN_TYPES_SIZE);
    new_type->next = known_types;
    new_type->id = id;
    new_type->desctructor = data_type->desctructor;
    known_types = new_type;
}

static u64 user_types_init(const type_methods_definitions* data_type) {
    known_types_init(known_types_counter, data_type);
    return known_types_counter++;
}

static void INIT vm_init(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    global_statistics();
#endif
}

static void DESTROY vm_destroy(void) {
    while (known_types->next != 0) {
        known_types_ptr prev = known_types->next;
        CALL(system_memory)->free(known_types, KNOWN_TYPES_SIZE);
        known_types = prev;
    }
#ifdef USE_MEMORY_DEBUG_INFO
    global_statistics();
#endif
}

static pointer_ptr pointer_alloc(u64 size, u64 id) {
    pointer_ptr ptr = CALL(system_memory)->alloc(POINTER_SIZE);
    if (size != 0) {
        ptr->data = CALL(system_memory)->alloc(size);
        ptr->size = size;
    }
    ptr->id = id;
    return ptr;
}

static const_pointer_ptr pointer_copy(const void* data, u64 size, u64 id) {
    const_pointer_ptr data_ptr = pointer_alloc(size, id);
    pointer_memcpy(data_ptr, data, size);
    return data_ptr;
}

static const_pointer_ptr pointer_copy_guard(const void* data, u64 size, u64 offset, u64 id) {
    const_pointer_ptr data_ptr = 0;
    if (size > offset) {
        data_ptr = pointer_copy(data, size, id);
        pointer_guard(data_ptr, offset);
    }
    return data_ptr;
}

static void pointer_memcpy(const_pointer_ptr const_ptr, const void* data, u64 size) {
    virtual_api->memcpy(pointer_read(const_ptr), data, size);
}

static void pointer_realloc(pointer_ptr ptr, u64 size) {
    if (ptr != 0 && ptr->data != 0) {
        ptr->data = CALL(system_memory)->realloc(ptr->data, ptr->size, size);
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
    const_pointer_ptr data_ptr = CALL(virtual)->read(ptr);
    if (data_ptr == 0) {
        return;
    }
    u64 type_id = data_ptr->id;
    const desctructor type_desctructor = default_types[type_id].desctructor;
    type_desctructor(data_ptr);
}

static void pointer_release(const_pointer_ptr const_ptr) {
    if (const_ptr == 0) {
        return;
    }
    safe_pointer_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr;
    void* data_ptr = const_ptr->data;
    if (data_ptr != 0 && const_ptr->size != 0) {
        CALL(system_memory)->free(data_ptr, const_ptr->size);
        safe_ptr.public.ptr->data = 0;
    }
    CALL(virtual)->free(const_ptr->pointer.address);
    safe_ptr.public.ptr->pointer.address = 0;
    CALL(system_memory)->free(safe_ptr.public.ptr, POINTER_SIZE);
}

static u64 pointer_size(const_pointer_ptr const_ptr) {
    u64 size = 0;
    if (const_ptr) {
        size = const_ptr->size;
    }
    return size;
}

static void* pointer_read(const_pointer_ptr const_ptr) {
    void* data_ptr = 0;
    if (const_ptr) {
        data_ptr = const_ptr->data;
    }
    return data_ptr;
}

static void* pointer_read_guard(const_pointer_ptr const_ptr, u64 offset) {
    void* data_ptr = 0;
    if (const_ptr && offset < const_ptr->size) {
        data_ptr = pointer_read(const_ptr);
        if (data_ptr) {
            u8* data = data_ptr;
            data[offset] = 0;
        }
    }
    return data_ptr;
}

static void pointer_guard(const_pointer_ptr const_ptr, u64 offset) {
    u8* data = pointer_read(const_ptr);
    if (data) {
        data[offset] = 0;
    }
}

static u64 pointer_read_type(const_pointer_ptr const_ptr, u64 virtual_id) {
    u64 id = 0;
    if (const_ptr && const_ptr->id == virtual_id) {
        id = virtual_id;
    }
    return id;
}

/* implementation */
static void pointer_init(u64 size) {
    if (vm != 0) {
        return;
    }
    CALL(virtual)->init(&vm, size);
#ifndef ATTRIBUTE
    init();
#endif
    default_types = CALL(system_memory)->alloc(known_types_counter * KNOWN_TYPES_SIZE);
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
    if (vm == 0) {
        return;
    }
    CALL(system_memory)->free(default_types, known_types_counter * KNOWN_TYPES_SIZE);
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
        pointer_free(ptr);
    }
    CALL(virtual)->enumerator_destroy();
}

#ifdef USE_MEMORY_DEBUG_INFO
static void pointer_dump(pointer_ptr ptr) {
    printf("   ^: %016llx > %016llx\n", (u64)ptr, (u64)ptr->data);
}

static void pointer_dump_ref(pointer_ptr* ptr) {
    if (*ptr == 0) {
        return;
    }
    printf("   &: %016llx > %016llx\n", (u64)ptr, (u64)*ptr);
}
#endif

/* public */

const virtual_pointer_methods PRIVATE_API(virtual_pointer_methods_definitions) = {
    .init = pointer_init,
    .destroy = pointer_destroy,
    .gc = pointer_gc,
    .alloc = pointer_alloc,
    .copy = pointer_copy,
    .copy_guard = pointer_copy_guard,
    .memcpy = pointer_memcpy,
    .realloc = pointer_realloc,
    .register_known_type = pointer_register_known_type,
    .register_user_type = pointer_register_user_type,
    .free = pointer_free,
    .release = pointer_release,
    .size = pointer_size,
    .read = pointer_read,
    .read_guard = pointer_read_guard,
    .guard = pointer_guard,
    .read_type = pointer_read_type,
#ifdef USE_MEMORY_DEBUG_INFO
    .dump = pointer_dump,
    .dump_ref = pointer_dump_ref,
#endif
};

const virtual_pointer_methods* pointer = &PRIVATE_API(virtual_pointer_methods_definitions);

const virtual_pointer_methods* CALL(pointer) {
    return pointer;
}
