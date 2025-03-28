/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 28, 2025 at 3:10:36 PM GMT+3
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

#define VM_TYPE_SIZE sizeof(vm_type)

#include "system/api/api_v1.h"
#include "system/list/list_v1.h"
#include "system/memory/memory_v1.h"

#ifdef USE_MEMORY_DEBUG_INFO
#include "virtual/pointer/pointer_v1.h"
#endif

#include "virtual/virtual/virtual_v1.h"

/* macros */
#define DEFAULT_SIZE 0x8 /* 8 */
#define PTR_SIZE sizeof(void*) /* size of a pointer */
#define PTR_ARRAY_SIZE(size) (size * PTR_SIZE)
#define POINTER_TYPE_SIZE sizeof(pointer_type)
#define KNOWN_TYPES_TYPE_SIZE sizeof(known_types_type)
#define KNOWN_TYPES_TYPE_ARRAY_SIZE(size) (size * KNOWN_TYPES_TYPE_SIZE)
#define POINTER_TYPE_SIZE sizeof(pointer_type)

/* internal */
#include "internal/internal_v1.h"
#include "internal/private_v1.h"

/* private */
typedef struct vm_state {
    virtual_pointer_ptr vptr;
    pointer_ptr* ref;
} vm_state_type;

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

static vm_ptr vm_list = 0;
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

/* api */
static const_vm_ptr pointer_init(u64 size);
static void pointer_destroy(void);
static void pointer_gc(void);

static void pointer_register_known_type(u64 type_id, type_methods_definitions_ptr data_type);
static void pointer_register_user_type(type_methods_definitions_ptr data_type);

static u64 pointer_copy(const_void_ptr data, u64 size, u64 offset, u64 type_id);
static u64 pointer_alloc(const_void_ptr data, u64 size, u64 type_id);
static u64 pointer_release(u64 ptr);
static u64 pointer_free(u64 address, u64 type_id);
static const_void_ptr pointer_read(u64 address, u64 type_id);

#ifdef USE_MEMORY_DEBUG_INFO
static void virtual_dump(const_vm_ptr vm);
static void virtual_dump_ref(const_vm_ptr vm);
#endif

/* internal */
static void pointer_ref_enumerator_init_internal(struct vm_state* ptr, const_vm_ptr vm);
static void pointer_ref_enumerator_destroy_internal(struct vm_state* state);
static pointer_ptr* pointer_ref_enumerator_next_internal(struct vm_state* state);
static pointer_ptr pointer_enumerator_next_internal(struct vm_state* state);
static u64 pointer_next_internal(struct vm_state* state, const_vm_ptr vm);

/* internal */
static u64 pointer_alloc_internal(const_void_ptr data, const_pointer_ptr* tmp, u64 size, u64 type_id);
static void known_types_init_internal(u64 type_id, type_methods_definitions_ptr data_type);
static void user_types_init_internal(type_methods_definitions_ptr data_type);
static u64 check_existing_type_internal(known_types_ptr current, const_type_methods_definitions_ptr data_type);

/* vm */
static INIT void init(void);
static DESTROY void destroy(void);

/* internal */
static u64 known_types_counter = TYPE_USER;

/* implementation */
static void INIT init(void) {
    /* NOLINT: no constructor logic */
}

static void DESTROY destroy(void) {
    while (known_types->next != 0) {
        known_types_ptr prev = known_types->next;
        api->free(known_types);
        known_types = prev;
    }
}

static const_vm_ptr pointer_init(u64 size) {
#ifdef USE_MEMORY_DEBUG_INFO
    init_statistics();
#endif
    if (vm_list != 0) {
        return &vm_list;
    }
    CALL(system)->init(&vm_list, size);
    register_known_types();
    default_types = api->alloc(known_types_counter, KNOWN_TYPES_TYPE_SIZE);
    known_types_ptr current = known_types;
    while (current->next != 0) {
        known_types_ptr prev = current->next;
        u64 index = current->type_id;
        if (index > 0 && index < known_types_counter) {
            default_types[index] = *current;
        }
        current = prev;
    }
    return &vm_list;
}

static void pointer_destroy(void) {
    if (vm_list == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return;
    }
    api->free(default_types);
    CALL(system)->destroy(&vm_list);
    vm_list = 0;
#ifdef USE_MEMORY_DEBUG_INFO
    result_statistics();
#endif
}

static void pointer_gc(void) {
    if (vm_list == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return;
    }
#ifdef USE_MEMORY_DEBUG_INFO
    virtual_dump_ref(&vm_list);
    virtual_dump(&vm_list);
#endif
    struct vm_state state;
    pointer_ref_enumerator_init_internal(&state, &vm_list);
    u64 ptr = 0;
    while ((ptr = pointer_next_internal(&state, &vm_list)) != 0) {
        pointer_release(ptr);
    }
    pointer_ref_enumerator_destroy_internal(&state);
}

static void pointer_register_known_type(u64 type_id, type_methods_definitions_ptr data_type) {
    known_types_init_internal(type_id, data_type);
}

static void pointer_register_user_type(type_methods_definitions_ptr data_type) {
    user_types_init_internal(data_type);
}

/* internal */
static u64 pointer_alloc_internal(const_void_ptr data, const_pointer_ptr* tmp, u64 size, u64 type_id) {
    u64 address = CALL(virtual)->alloc(&vm_list, size, type_id);
    *tmp = CALL(virtual)->read(&vm_list, address, type_id);
    safe_pointer_ptr safe_ptr;
    safe_ptr.const_ptr = *tmp;
    pointer_ptr ptr = safe_ptr.ptr;
    ptr->data = data;
    return address;
}

static void known_types_init_internal(u64 type_id, type_methods_definitions_ptr data_type) {
    if (vm_list == 0) {
        return;
    }
    if (check_existing_type_internal(known_types, data_type) == 0) {
        return;
    }
    known_types_ptr obj = api->alloc(1, KNOWN_TYPES_TYPE_SIZE);
    obj->type_id = type_id;
    obj->next = known_types;
    obj->methods = data_type;
    data_type->type_id = type_id;
    known_types = obj;
}

static void user_types_init_internal(type_methods_definitions_ptr data_type) {
    if (vm_list == 0) {
        return;
    }
    if (check_existing_type_internal(known_types, data_type) == 0) {
        return;
    }
    u64 type_id = known_types_counter;
    known_types_counter++;
    known_types_init_internal(type_id, data_type);
}

static u64 check_existing_type_internal(known_types_ptr current, const_type_methods_definitions_ptr data_type) {
    while (current->next != 0) {
        known_types_ptr prev = current->next;
        if (current->methods == data_type) {
            return FALSE;
        }
        current = prev;
    }
    return TRUE;
}

/* public */
static u64 pointer_alloc(const_void_ptr data, u64 size, u64 type_id) {
    if (vm_list == 0) {
        ERROR_VM_NOT_INITIALIZED(vm);
        return FALSE;
    }
    if (size == 0) {
        ERROR_INVALID_ARGUMENT(size == 0);
        return FALSE;
    }
    if (type_id == 0) {
        ERROR_INVALID_TYPEID(type_id == 0);
        return FALSE;
    }
    const_pointer_ptr tmp;
    u64 address = pointer_alloc_internal(data, &tmp, size, type_id);
    return address;
}

static u64 pointer_copy(const_void_ptr data, u64 size, u64 offset, u64 type_id) {
    if (vm_list == 0) {
        ERROR_VM_NOT_INITIALIZED(vm);
        return FALSE;
    }
    if (data == 0) {
        ERROR_INVALID_POINTER(data == 0);
        return FALSE;
    }
    if (size == 0) {
        ERROR_INVALID_ARGUMENT(size == 0);
        return FALSE;
    }
    if (type_id == 0) {
        ERROR_INVALID_TYPEID(type_id == 0);
        return FALSE;
    }
    void* new_data = CALL(system_memory)->alloc(size);
    const_pointer_ptr tmp;
    u64 address = pointer_alloc_internal(new_data, &tmp, size, type_id);
    api->memcpy(new_data, data, size);
    u64 data_size = size;
    if (offset > 0 && offset < data_size) {
        safe_void_ptr safe_ptr;
        safe_ptr.const_ptr = tmp->data;
        pointer_ptr ptr = safe_ptr.ptr;
        void* data_ptr = ptr;
        if (data_ptr != 0) {
            u8* bytes = data_ptr;
            bytes[offset] = 0;
        }
    }
    return address;
}

static u64 pointer_free(u64 address, u64 type_id) {
    if (vm_list == 0) {
        ERROR_VM_NOT_INITIALIZED(vm);
        return FALSE;
    }
    if (address == 0) {
        ERROR_INVALID_ADDRESS(address == 0);
        return FALSE;
    }
    if (type_id == 0) {
        ERROR_INVALID_TYPEID(type_id == 0);
        return FALSE;
    }
    const_pointer_ptr const_ptr = CALL(virtual)->read(&vm_list, address, type_id);
    if (const_ptr == 0) {
        return FALSE;
    }
    safe_pointer_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr;
    pointer_ptr ptr = safe_ptr.ptr;
    u64 data_size = const_ptr->public.size;
    if (data_size != 0) {
        CALL(system_memory)->free(ptr->data, data_size);
    }
    CALL(virtual)->free(&vm_list, address);
    memset(ptr, 0, POINTER_TYPE_SIZE); /* NOLINT: sizeof(pointer_type) */
    api->free(ptr);
    return TRUE;
}

static u64 pointer_release(u64 address) {
    if (vm_list == 0) {
        ERROR_VM_NOT_INITIALIZED(vm);
        return FALSE;
    }
    if (address == 0) {
        ERROR_INVALID_ADDRESS(address == 0);
        return FALSE;
    }
    u64 type_id = CALL(virtual)->type(&vm_list, address);
    if (type_id == 0) {
        ERROR_INVALID_TYPEID(type_id == 0);
        return FALSE;
    }
    if (type_id > 0 && type_id <= known_types_counter - 1) {
        const_type_methods_definitions_ptr methods = default_types[type_id].methods;
        methods->destructor(address);
    }
    return TRUE;
}

static const_void_ptr pointer_read(u64 address, u64 type_id) {
    if (vm_list == 0) {
        ERROR_VM_NOT_INITIALIZED(vm);
        return FALSE;
    }
    if (address == 0) {
        ERROR_INVALID_ADDRESS(address == 0);
        return NULL_PTR;
    }
    if (type_id == 0) {
        ERROR_INVALID_TYPEID(type_id == 0);
        return NULL_PTR;
    }
    const_pointer_ptr const_ptr = CALL(virtual)->read(&vm_list, address, type_id);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER(const_ptr == 0);
        return NULL_PTR;
    }
    const_void_ptr data = const_ptr->data;
    return data;
}

#ifdef USE_MEMORY_DEBUG_INFO
static void pointer_dump(pointer_ptr ptr) {
    printf("  p^: %016llx > %016llx\n", (u64)ptr, (u64)ptr->data);
}

static void pointer_dump_ref(pointer_ptr* ptr) {
    if (*ptr == 0) {
        return;
    }
    printf("  p&: %016llx > %016llx\n", (u64)ptr, (u64)*ptr);
}
#endif

/* code */
static void pointer_ref_enumerator_init_internal(struct vm_state* state, const_vm_ptr vm) {
    virtual_pointer_ptr vptr = (*vm)->next;
    state->vptr = vptr;
    state->ref = vptr->bp;
}

static void pointer_ref_enumerator_destroy_internal(struct vm_state* state) {
    state->vptr = 0;
    state->ref = 0;
}

/* implementation */
#ifdef USE_MEMORY_DEBUG_INFO
static void virtual_dump(const_vm_ptr vm) {
    struct vm_state state;
    pointer_ref_enumerator_init_internal(&state, vm);
    pointer_ptr ptr = 0;
    while ((ptr = pointer_enumerator_next_internal(&state)) != 0) {
        pointer_dump(ptr);
    }
    pointer_ref_enumerator_destroy_internal(&state);
}

static void virtual_dump_ref(const_vm_ptr vm) {
    struct vm_state state;
    pointer_ref_enumerator_init_internal(&state, vm);
    pointer_ptr* ref = 0;
    while ((ref = pointer_ref_enumerator_next_internal(&state)) != 0) {
        pointer_dump_ref(ref);
    }
    pointer_ref_enumerator_destroy_internal(&state);
}

static pointer_ptr pointer_enumerator_next_internal(struct vm_state* state) {
    pointer_ptr data = 0;
    virtual_pointer_ptr vptr = state->vptr;
    while (data == 0) {
        if (state->ref == vptr->sp) {
            if (vptr->next == 0) {
                break;
            }
            vptr = vptr->next;
            state->vptr = vptr;
            state->ref = vptr->bp;
        }
        data = *state->ref++;
    }
    return data;
}
#endif

static u64 pointer_next_internal(struct vm_state* state, const_vm_ptr vm) {
    u64 address = 0;
    pointer_ptr* ref;
    while ((ref = pointer_ref_enumerator_next_internal(state)) != 0) {
        if (*ref != 0) {
            const_pointer_ptr const_ptr = *ref;
            if (const_ptr != 0) {
                safe_pointer_ptr safe_ptr;
                safe_ptr.const_ptr = const_ptr;
                pointer_ptr ptr = safe_ptr.ptr;
                const_pointer_public_ptr public_ptr = &ptr->public;
                address = public_ptr->address;
                break;
            }
        }
    }
    return address;
}

static pointer_ptr* pointer_ref_enumerator_next_internal(struct vm_state* state) {
    pointer_ptr* ref = 0;
    virtual_pointer_ptr vptr = state->vptr;
    while (ref == 0) {
        if (state->ref == vptr->sp) {
            if (vptr->next == 0) {
                break;
            }
            vptr = vptr->next;
            state->vptr = vptr;
            state->ref = vptr->bp;
        }
        ref = state->ref++;
    }
    return ref;
}

/* public */
const virtual_pointer_methods PRIVATE_API(virtual_pointer_methods_definitions) = {
    .alloc = pointer_alloc,
    .copy = pointer_copy,
    .read = pointer_read,
    .free = pointer_free
};

const virtual_vm_methods PRIVATE_API(virtual_vm_methods_definitions) = {
    .init = pointer_init,
    .destroy = pointer_destroy,
    .gc = pointer_gc,
    .register_known_type = pointer_register_known_type,
    .register_user_type = pointer_register_user_type,
    .release = pointer_release
};

const virtual_pointer_methods* pointer = &PRIVATE_API(virtual_pointer_methods_definitions);
const virtual_pointer_methods* CALL(pointer) {
    return pointer;
}

const virtual_vm_methods* vm = &PRIVATE_API(virtual_vm_methods_definitions);
const virtual_vm_methods* CALL(vm) {
    return vm;
}
