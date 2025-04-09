/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 9, 2025 at 4:02:08 PM GMT+3
 *
 */
/*
    Copyright (C) 2022-2047 Artur Mustafin (artur.mustafin@gmail.com)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its
       contributors may be used to endorse or promote products derived from
       this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "std/data.h"

#include "vm_v1.h"

#define USING_SYSTEM_ERROR_API
#include "system/error/error_v1.h"

#include "system/memory/memory_v1.h"
#include "system/os/os_v1.h"

#include "virtual/list/list_v1.h"
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
#include "internal/pointer_type_v1.h"
#include "internal/virtual_pointer_type_v1.h"
#include "internal/vm_type_v1.h"

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

static void register_known_types(const_vm_ptr cvm);
static void unregister_known_types(const_vm_ptr cvm);

static void register_known_types(const_vm_ptr cvm) {
    data_init(cvm);
    file_init(cvm);
    object_init(cvm);
    stack_init(cvm);
    string_init(cvm);
    string_pointer_init(cvm);
    user_init(cvm);
}

static void unregister_known_types(const_vm_ptr cvm) {
    CALL(memory)->free((*cvm)->known_types, KNOWN_TYPES_TYPE_ARRAY_SIZE((*cvm)->known_types_capacity));
}

/* public */
static void vm_dump_ref(const_vm_ptr cvm);
static void vm_dump_ref_stack(const_vm_ptr cvm, stack_ptr stack);

/* internal */
static void vm_dump_ref_internal(const_vm_ptr cvm, pointer_ptr* ptr, stack_ptr stack);
static void virtual_dump_ref_internal(const_vm_ptr cvm, stack_ptr stack);

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

/* internal */
static void vm_ref_enumerator_init_internal(struct vm_state* ptr, const_vm_ptr cvm);
static void vm_ref_enumerator_destroy_internal(struct vm_state* state);
static pointer_ptr* vm_ref_enumerator_next_internal(struct vm_state* state);
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
    ptr->known_types_capacity = TYPE_USER - 1;
    ptr->known_types = CALL(memory)->alloc(KNOWN_TYPES_TYPE_ARRAY_SIZE((*cvm)->known_types_capacity));
    register_known_types(cvm);
    return cvm;
}

static void vm_gc(const_vm_ptr cvm) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return;
    }
#ifdef USE_MEMORY_DEBUG_INFO
    virtual_dump_ref_internal(cvm, 0);
#endif
    struct vm_state state;
    vm_ref_enumerator_init_internal(&state, cvm);
    u64 ptr = 0;
    while ((ptr = vm_next_internal(&state)) != 0) {
        vm_release(cvm, ptr);
    }
    vm_ref_enumerator_destroy_internal(&state);
}

static void vm_dump_ref(const_vm_ptr cvm) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return;
    }
    virtual_dump_ref_internal(cvm, 0);
}

static void vm_dump_ref_stack(const_vm_ptr cvm, stack_ptr stack) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return;
    }
    virtual_dump_ref_internal(cvm, stack);
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
#ifdef USE_GC
    if (type_id > 0 && type_id <= (*cvm)->known_types_capacity) {
        const_type_methods_definitions_ptr methods = (*cvm)->known_types[type_id - 1].methods;
        methods->destructor(cvm, address);
    }
#endif
    return TRUE;
}

static void vm_destroy(const_vm_ptr cvm) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return;
    }
    unregister_known_types(cvm);
    CALL(system)->destroy(cvm);
    CALL(error)->clear();
#ifdef USE_MEMORY_DEBUG_INFO
    result_statistics();
#endif
}

static void vm_dump_ref_internal(const_vm_ptr cvm, pointer_ptr* ptr, stack_ptr stack) {
    if (*ptr == 0) {
        return;
    }
    if (stack != NULL_PTR) {
        CALL(list)->push(cvm, stack, *ptr);
    }
#ifdef USE_MEMORY_DEBUG_INFO
#ifdef USE_TTY
    const char* start = "\x1b[34m";
    const char* end = "\x1b[0m";
    fprintf(stderr, "%s[  p& ]%s: %016llx > %016llx\n", start, end, (u64)ptr, (u64)*ptr); /* NOLINT */
#else
    fprintf(stderr, "  p& : %016llx > %016llx\n", (u64)ptr, (u64)*ptr); /* NOLINT */
#endif
#endif
}

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
static void virtual_dump_ref_internal(const_vm_ptr cvm, stack_ptr stack) {
    struct vm_state state;
    vm_ref_enumerator_init_internal(&state, cvm);
    pointer_ptr* ref = 0;
    while ((ref = vm_ref_enumerator_next_internal(&state)) != 0) {
        vm_dump_ref_internal(cvm, ref, stack);
    }
    vm_ref_enumerator_destroy_internal(&state);
}

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
    .destroy = vm_destroy,
    .dump_ref = vm_dump_ref,
    .dump_ref_stack = vm_dump_ref_stack
};

const virtual_vm_methods* PRIVATE_API(vm) = &PRIVATE_API(virtual_vm_methods_definitions);
const virtual_vm_methods* CALL(vm) {
    return PRIVATE_API(vm);
}
