/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 18, 2025 at 5:45:22 PM GMT+3
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

#define USING_SYSTEM_ERROR_API
#include "system/error/error_v1.h"

#include "system/api/api_v1.h"

#include "system/memory/memory_v1.h"

#include "virtual/api/api_v1.h"
#include "virtual/virtual/virtual_v1.h"

#define POINTER_TYPE_SIZE sizeof(pointer_type)
#define KNOWN_TYPES_TYPE_SIZE sizeof(known_types_type)
#define KNOWN_TYPES_TYPE_ARRAY_SIZE(size) (size * KNOWN_TYPES_TYPE_SIZE)

/* private */
typedef struct pointer {
    const_virtual_pointer_ptr vptr;
    void* data;
    u64 address;
    u64 size;
    u64 type;
} pointer_type;

/* private */
typedef struct known_types* known_types_ptr;
typedef const struct known_types* const_known_types_ptr;

typedef struct known_types {
    u64 type_id;
    known_types_ptr next;
    type_methods_definitions_ptr methods;
} known_types_type;

/* definition */
static known_types_type known_types_definition = { 0, 0, 0 };

/* definition */
static known_types_ptr known_types = &known_types_definition;

CVM_EXPORT extern void data_init(void);
CVM_EXPORT extern void file_init(void);
CVM_EXPORT extern void object_init(void);
CVM_EXPORT extern void stack_init(void);
CVM_EXPORT extern void string_init(void);
CVM_EXPORT extern void string_pointer_init(void);
CVM_EXPORT extern void user_init(void);

static void register_known_types(void) {
    data_init();
    file_init();
    object_init();
    stack_init();
    string_init();
    string_pointer_init();
    user_init();
}

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

static const_vm_ptr pointer_init(u64 size);
static void pointer_destroy(void);
static void pointer_gc(void);

static const_pointer_ptr pointer_alloc(u64 size, u64 type_id);
static const_pointer_ptr pointer_copy(const void* data, u64 size, u64 type_id);
static const_pointer_ptr pointer_copy_guard(const void* data, u64 size, u64 offset, u64 type_id);
static u64 pointer_alloc_guard(const void* data, u64 size, u64 offset, u64 type_id);
static u64 pointer_memcpy(const_pointer_ptr const_ptr, const void* data, u64 size);
static u64 pointer_realloc(const_pointer_ptr ptr, u64 size);
static u64 pointer_free(u64 ptr);
static u64 pointer_release(const_pointer_ptr const_ptr);
static u64 pointer_size(const_pointer_ptr const_ptr);
static void* pointer_data(const_pointer_ptr const_ptr);
static void* pointer_data_guard(const_pointer_ptr const_ptr, u64 offset);
static u64 pointer_guard(const_pointer_ptr const_ptr, u64 offset);
static u64 pointer_get_type(const_pointer_ptr const_ptr);
static u64 pointer_get_address(const_pointer_ptr const_ptr);
static u64 pointer_set(const_pointer_ptr const_ptr, const_virtual_pointer_ptr vptr, u64 address);

static void known_types_init(u64 type_id, type_methods_definitions_ptr data_type);
static void user_types_init(type_methods_definitions_ptr data_type);
static u64 check_existing_type(known_types_ptr current, const_type_methods_definitions_ptr data_type);

/* free */
static INIT void vm_init(void);
static DESTROY void vm_destroy(void);

/* internal */

static u64 known_types_counter = TYPE_USER;

static u64 check_existing_type(known_types_ptr current, const_type_methods_definitions_ptr data_type) {
    while (current->next != 0) {
        known_types_ptr prev = current->next;
        if (current->methods == data_type) {
            return FALSE;
        }
        current = prev;
    }
    return TRUE;
}

static void known_types_init(u64 type_id, type_methods_definitions_ptr data_type) {
    if (vm == 0) {
        return;
    }
    if (check_existing_type(known_types, data_type) == 0) {
        return;
    }
    known_types_ptr obj = system_api->alloc(1, KNOWN_TYPES_TYPE_SIZE);
    obj->type_id = type_id;
    obj->next = known_types;
    obj->methods = data_type;
    data_type->type_id = type_id;
    known_types = obj;
}

static void user_types_init(type_methods_definitions_ptr data_type) {
    if (vm == 0) {
        return;
    }
    if (check_existing_type(known_types, data_type) == 0) {
        return;
    }
    u64 type_id = known_types_counter;
    known_types_counter++;
    known_types_init(type_id, data_type);
}

static void INIT vm_init(void) {
    /* NOLINT: no constructor logic */
}

static void DESTROY vm_destroy(void) {
    while (known_types->next != 0) {
        known_types_ptr prev = known_types->next;
        system_api->free(known_types);
        known_types = prev;
    }
}

static const_pointer_ptr pointer_alloc(u64 size, u64 type) {
    if (vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm);
        return NULL_PTR;
    }
    if (size == 0) {
        ERROR_ARGUMENT_VALUE_NOT_INITIALIZED(size == 0);
        return NULL_PTR;
    }
    if (type == 0) {
        ERROR_ARGUMENT_VALUE_NOT_INITIALIZED(type == 0);
        return NULL_PTR;
    }
    pointer_ptr ptr = CALL(system_memory)->alloc(POINTER_TYPE_SIZE);
    void* data = CALL(system_memory)->alloc(size);
    *ptr = (struct pointer) {
        .data = data,
        .size = size,
        .type = type
    };
    return ptr;
}

static const_pointer_ptr pointer_copy(const void* data, u64 size, u64 type_id) {
    if (vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm);
        return NULL_PTR;
    }
    if (data == 0) {
        ERROR_POINTER_NOT_INITIALIZED(data);
        return NULL_PTR;
    }
    if (size == 0) {
        ERROR_ARGUMENT_VALUE_NOT_INITIALIZED(size == 0);
        return NULL_PTR;
    }
    if (type_id == 0) {
        ERROR_ARGUMENT_VALUE_NOT_INITIALIZED(type_id == 0);
        return NULL_PTR;
    }
    const_pointer_ptr data_ptr = pointer_alloc(size, type_id);
    pointer_memcpy(data_ptr, data, size);
    return data_ptr;
}

static const_pointer_ptr pointer_copy_guard(const void* data, u64 size, u64 offset, u64 type_id) {
    if (vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm);
        return NULL_PTR;
    }
    if (data == 0) {
        ERROR_POINTER_NOT_INITIALIZED(data);
        return NULL_PTR;
    }
    if (size == 0) {
        ERROR_ARGUMENT_VALUE_NOT_INITIALIZED(size == 0);
        return NULL_PTR;
    }
    if (size <= offset) {
        ERROR_ARGUMENT_VALUE_NOT_INITIALIZED(size <= offset);
        return NULL_PTR;
    }
    if (type_id == 0) {
        ERROR_ARGUMENT_VALUE_NOT_INITIALIZED(type_id == 0);
        return NULL_PTR;
    }
    const_pointer_ptr data_ptr = pointer_copy(data, size, type_id);
    pointer_guard(data_ptr, offset);
    return data_ptr;
}

static u64 pointer_alloc_guard(const void* data, u64 size, u64 offset, u64 type_id) {
    if (vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm);
        return FALSE;
    }
    if (data == 0) {
        ERROR_POINTER_NOT_INITIALIZED(data == 0);
        return FALSE;
    }
    if (size == 0) {
        ERROR_ARGUMENT_VALUE_NOT_INITIALIZED(size == 0);
        return FALSE;
    }
    if (size <= offset) {
        ERROR_ARGUMENT_VALUE_NOT_INITIALIZED(size <= offset);
        return FALSE;
    }
    if (type_id == 0) {
        ERROR_ARGUMENT_VALUE_NOT_INITIALIZED(type_id == 0);
        return FALSE;
    }
    const_pointer_ptr data_ptr = pointer_copy(data, size, type_id);
    pointer_guard(data_ptr, offset);
    return CALL(virtual)->alloc(&vm, data_ptr);
}

static u64 pointer_memcpy(const_pointer_ptr const_ptr, const void* data, u64 size) {
    if (vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm);
        return FALSE;
    }
    if (const_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(data);
        return FALSE;
    }
    if (data == 0) {
        return FALSE;
    }
    if (size == 0) {
        ERROR_ARGUMENT_VALUE_NOT_INITIALIZED(size == 0);
        return FALSE;
    }
    void* const_data = const_ptr->data;
    virtual_api->memcpy(const_data, data, size);
    return TRUE;
}

static u64 pointer_realloc(const_pointer_ptr const_ptr, u64 size) {
    if (vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm);
        return FALSE;
    }
    if (const_ptr == 0) {
        return FALSE;
    }
    if (size == 0) {
        ERROR_ARGUMENT_VALUE_NOT_INITIALIZED(size == 0);
        return FALSE;
    }
    if (const_ptr->size >= size) {
        return FALSE;
    }
    safe_pointer_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr;
    pointer_ptr ptr = safe_ptr.ptr;
    if (ptr->data != 0) {
        void* data = CALL(system_memory)->realloc(ptr->data, ptr->size, size);
        ptr->data = data;
    }
    ptr->size = size;
    return ptr->size;
}

void pointer_register_known_type(u64 type_id, type_methods_definitions_ptr data_type) {
    known_types_init(type_id, data_type);
}

void pointer_register_user_type(type_methods_definitions_ptr data_type) {
    user_types_init(data_type);
}

static u64 pointer_free(u64 ptr) {
    if (vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm);
        return FALSE;
    }
    if (ptr == 0) {
        return FALSE;
    }
    const_pointer_ptr data_ptr = CALL(virtual)->read(&vm, ptr);
    if (data_ptr == 0) {
        return FALSE;
    }
    u64 type_id = data_ptr->type;
    if (type_id > 0 && type_id <= known_types_counter - 1) {
        const_type_methods_definitions_ptr methods = default_types[type_id].methods;
        methods->desctructor(data_ptr);
    }
    return TRUE;
}

static u64 pointer_release(const_pointer_ptr const_ptr) {
    if (vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm);
        return FALSE;
    }
    if (const_ptr == 0) {
        return FALSE;
    }
    safe_pointer_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr;
    void* data_ptr = const_ptr->data;
    if (const_ptr->size != 0) {
        CALL(system_memory)->free(data_ptr, const_ptr->size);
        safe_ptr.ptr->data = 0;
    }
    CALL(virtual)->free(&vm, const_ptr->address);
    safe_ptr.ptr->address = 0;
    CALL(system_memory)->free(safe_ptr.ptr, POINTER_TYPE_SIZE);
    return TRUE;
}

static u64 pointer_size(const_pointer_ptr const_ptr) {
    if (vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm);
        return FALSE;
    }
    if (const_ptr == 0) {
        return FALSE;
    }
    return const_ptr->size;
}

static void* pointer_data(const_pointer_ptr const_ptr) {
    if (vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm);
        return NULL_PTR;
    }
    if (const_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(const_ptr == 0);
        return NULL_PTR;
    }
    return const_ptr->data;
}

static void* pointer_data_guard(const_pointer_ptr const_ptr, u64 offset) {
    if (vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm);
        return NULL_PTR;
    }
    if (const_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(const_ptr == 0);
        return NULL_PTR;
    }
    if (offset >= const_ptr->size) {
        return NULL_PTR;
    }
    void* data_ptr = const_ptr->data;
    if (data_ptr != 0) {
        u8* data = data_ptr;
        data[offset] = 0;
    }
    return data_ptr;
}

static u64 pointer_guard(const_pointer_ptr const_ptr, u64 offset) {
    if (vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm);
        return FALSE;
    }
    u8* data = pointer_data(const_ptr);
    if (data == 0) {
        return FALSE;
    }
    data[offset] = 0;
    return TRUE;
}

static u64 pointer_get_type(const_pointer_ptr const_ptr) {
    if (vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm);
        return FALSE;
    }
    if (const_ptr == 0) {
        return FALSE;
    }
    return const_ptr->type;
}

static u64 pointer_get_address(const_pointer_ptr const_ptr) {
    if (vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm);
        return FALSE;
    }
    if (const_ptr == 0) {
        return FALSE;
    }
    return const_ptr->address;
}

/* implementation */
static const_vm_ptr pointer_init(u64 size) {
#ifdef USE_MEMORY_DEBUG_INFO
    init_statistics();
#endif
    if (vm != 0) {
        return &vm;
    }
    CALL(virtual)->init(&vm, size);
    register_known_types();
    default_types = CALL(system_memory)->alloc(KNOWN_TYPES_TYPE_ARRAY_SIZE(known_types_counter));
    known_types_ptr current = known_types;
    while (current->next != 0) {
        known_types_ptr prev = current->next;
        u64 index = current->type_id;
        if (index > 0 && index < known_types_counter) {
            default_types[index] = *current;
        }
        current = prev;
    }
    return &vm;
}

static u64 pointer_set(const_pointer_ptr const_ptr, const_virtual_pointer_ptr vptr, u64 address) {
    if (vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm);
        return FALSE;
    }
    if (const_ptr == 0) {
        return FALSE;
    }
    safe_pointer_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr;
    safe_ptr.ptr->address = address;
    safe_ptr.ptr->vptr = vptr;
    return address;
}

static void pointer_destroy(void) {
    if (vm == 0) {
        return;
    }
    CALL(system_memory)->free(default_types, KNOWN_TYPES_TYPE_ARRAY_SIZE(known_types_counter));
    CALL(virtual)->destroy(&vm);
    vm = 0;
#ifdef USE_MEMORY_DEBUG_INFO
    result_statistics();
#endif
}

static void pointer_gc(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    CALL(virtual)->dump_ref(&vm);
    CALL(virtual)->dump(&vm);
#endif
    CALL(virtual)->enumerator_init(&vm);
    u64 ptr = 0;
    while ((ptr = CALL(virtual)->enumerator_next(&vm)) != 0) {
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
    .alloc_guard = pointer_alloc_guard,
    .memcpy = pointer_memcpy,
    .realloc = pointer_realloc,
    .register_known_type = pointer_register_known_type,
    .register_user_type = pointer_register_user_type,
    .free = pointer_free,
    .release = pointer_release,
    .size = pointer_size,
    .data = pointer_data,
    .data_guard = pointer_data_guard,
    .guard = pointer_guard,
    .get_type = pointer_get_type,
    .set = pointer_set,
    .get_address = pointer_get_address,
#ifdef USE_MEMORY_DEBUG_INFO
    .dump = pointer_dump,
    .dump_ref = pointer_dump_ref,
#endif
};

const virtual_pointer_methods* pointer = &PRIVATE_API(virtual_pointer_methods_definitions);

const virtual_pointer_methods* CALL(pointer) {
    return pointer;
}
