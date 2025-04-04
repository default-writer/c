/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 3, 2025 at 3:47:40 PM GMT+3
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

#include "vm_v1.h"

#define USING_SYSTEM_ERROR_API
#include "system/error/error_v1.h"

#include "system/list/list_v1.h"
#include "system/memory/memory_v1.h"
#include "system/os/os_v1.h"

#include "virtual/virtual/virtual_v1.h"

/* macros */
#define DEFAULT_SIZE 0x8 /* 8 */
#define PTR_SIZE sizeof(void_ptr) /* size of a pointer */
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

CVM_EXPORT extern void data_init(const_vm_ptr cvm);
CVM_EXPORT extern void file_init(const_vm_ptr cvm);
CVM_EXPORT extern void object_init(const_vm_ptr cvm);
CVM_EXPORT extern void stack_init(const_vm_ptr cvm);
CVM_EXPORT extern void string_init(const_vm_ptr cvm);
CVM_EXPORT extern void string_pointer_init(const_vm_ptr cvm);
CVM_EXPORT extern void user_init(const_vm_ptr cvm);

static void register_known_types(const_vm_ptr cvm) {
    data_init(cvm);
    file_init(cvm);
    object_init(cvm);
    stack_init(cvm);
    string_init(cvm);
    string_pointer_init(cvm);
    user_init(cvm);
}

#ifdef USE_MEMORY_DEBUG_INFO
static void vm_dump(pointer_ptr ptr);
static void vm_dump_ref(pointer_ptr* ptr);
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

/* public */
static const_vm_ptr vm_init(u64 size);
static void vm_gc(const_vm_ptr cvm);
static u64 vm_release(const_vm_ptr cvm, u64 ptr);
static void vm_destroy(const_vm_ptr cvm);

#ifdef USE_MEMORY_DEBUG_INFO
static void virtual_dump(const_vm_ptr cvm);
static void virtual_dump_ref(const_vm_ptr cvm);
#endif

/* internal */
static void vm_ref_enumerator_init_internal(struct vm_state* ptr, const_vm_ptr cvm);
static void vm_ref_enumerator_destroy_internal(struct vm_state* state);
static pointer_ptr* vm_ref_enumerator_next_internal(struct vm_state* state);
static pointer_ptr vm_enumerator_next_internal(struct vm_state* state);
static u64 vm_next_internal(struct vm_state* state);

/* internal */
static u64 known_types_counter = TYPE_USER;

static const_vm_ptr vm_init(u64 size) {
#ifdef USE_MEMORY_DEBUG_INFO
    init_statistics();
#endif
    const_vm_ptr cvm = CALL(system)->init(size);
    if (cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return NULL_PTR;
    }
    safe_vm_ptr safe_ptr;
    safe_ptr.const_ptr = cvm;
    vm_ptr ptr = *safe_ptr.ptr;
    register_known_types(cvm);
    ptr->default_types = CALL(os)->calloc(known_types_counter, KNOWN_TYPES_TYPE_SIZE);
    known_types_ptr current = (*cvm)->known_types;
    while (current != 0) {
        known_types_ptr prev = current->next;
        u64 type_id = current->methods->type_id;
        if (type_id > 0 && type_id <= known_types_counter) {
            ptr->default_types[type_id - 1] = *current;
        }
        current = prev;
    }
    return cvm;
}

static void vm_gc(const_vm_ptr cvm) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return;
    }
#ifdef USE_MEMORY_DEBUG_INFO
    virtual_dump_ref(cvm);
    virtual_dump(cvm);
#endif
    struct vm_state state;
    vm_ref_enumerator_init_internal(&state, cvm);
    u64 ptr = 0;
    while ((ptr = vm_next_internal(&state)) != 0) {
        vm_release(cvm, ptr);
    }
    vm_ref_enumerator_destroy_internal(&state);
}

static u64 vm_release(const_vm_ptr cvm, u64 address) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (address == 0) {
        ERROR_INVALID_ARGUMENT("address == %lld", address);
        return FALSE;
    }
    u64 type_id = CALL(virtual)->type(cvm, address);
    if (type_id == 0) {
        ERROR_INVALID_TYPE_ID("type_id == %lld", type_id);
        return FALSE;
    }
    if (type_id > 0 && type_id <= known_types_counter) {
        const_type_methods_definitions_ptr methods = (*cvm)->default_types[type_id - 1].methods;
        methods->destructor(cvm, address);
    }
    return TRUE;
}

static void vm_destroy(const_vm_ptr cvm) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return;
    }
    known_types_ptr known_types = (*cvm)->known_types;
    while (known_types != 0) {
        known_types_ptr prev = known_types->next;
        known_types->next = 0;
        CALL(os)->free(known_types);
        known_types = prev;
    }
    CALL(os)->free((*cvm)->default_types);
    CALL(system)->destroy(cvm);
    CALL(error)->clear();
#ifdef USE_MEMORY_DEBUG_INFO
    result_statistics();
#endif
}

#ifdef USE_MEMORY_DEBUG_INFO
static void vm_dump(pointer_ptr ptr) {
    printf("  p^: %016llx > %016llx\n", (u64)ptr, (u64)ptr->data);
}

static void vm_dump_ref(pointer_ptr* ptr) {
    if (*ptr == 0) {
        return;
    }
    printf("  p&: %016llx > %016llx\n", (u64)ptr, (u64)*ptr);
}
#endif

/* code */
static void vm_ref_enumerator_init_internal(struct vm_state* state, const_vm_ptr cvm) {
    virtual_pointer_ptr vptr = (*cvm)->next;
    state->vptr = vptr;
    state->ref = vptr->bp;
}

static void vm_ref_enumerator_destroy_internal(struct vm_state* state) {
    state->vptr = 0;
    state->ref = 0;
}

/* implementation */
#ifdef USE_MEMORY_DEBUG_INFO
static void virtual_dump(const_vm_ptr cvm) {
    struct vm_state state;
    vm_ref_enumerator_init_internal(&state, cvm);
    pointer_ptr ptr = 0;
    while ((ptr = vm_enumerator_next_internal(&state)) != 0) {
        vm_dump(ptr);
    }
    vm_ref_enumerator_destroy_internal(&state);
}

static void virtual_dump_ref(const_vm_ptr cvm) {
    struct vm_state state;
    vm_ref_enumerator_init_internal(&state, cvm);
    pointer_ptr* ref = 0;
    while ((ref = vm_ref_enumerator_next_internal(&state)) != 0) {
        vm_dump_ref(ref);
    }
    vm_ref_enumerator_destroy_internal(&state);
}

static pointer_ptr vm_enumerator_next_internal(struct vm_state* state) {
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

static u64 vm_next_internal(struct vm_state* state) {
    u64 address = 0;
    pointer_ptr* ref;
    while ((ref = vm_ref_enumerator_next_internal(state)) != 0) {
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

static pointer_ptr* vm_ref_enumerator_next_internal(struct vm_state* state) {
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
const virtual_vm_methods PRIVATE_API(virtual_vm_methods_definitions) = {
    .init = vm_init,
    .gc = vm_gc,
    .release = vm_release,
    .destroy = vm_destroy
};

const virtual_vm_methods* PRIVATE_API(vm) = &PRIVATE_API(virtual_vm_methods_definitions);
const virtual_vm_methods* CALL(vm) {
    return PRIVATE_API(vm);
}
