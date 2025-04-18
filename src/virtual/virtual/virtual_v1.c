/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 9, 2025 at 4:06:17 PM GMT+3
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

#include "virtual_v1.h"

#define USING_SYSTEM_ERROR_API
#include "system/error/error_v1.h"

#include "system/memory/memory_v1.h"
#include "system/os/os_v1.h"

#include "virtual/list/list_v1.h"

/* macros */
#define VM_TYPE_SIZE sizeof(vm_type)
#define DEFAULT_SIZE 0x8 /* 8 */
#define VIRTUAL_POINTER_TYPE_SIZE sizeof(virtual_pointer_type)
#define POINTER_TYPE_SIZE sizeof(pointer_type)
#define KNOWN_TYPES_TYPE_SIZE sizeof(known_types_type)
#define KNOWN_TYPES_TYPE_ARRAY_SIZE(size) (size * KNOWN_TYPES_TYPE_SIZE)

#ifndef USE_GC
#define VM_POINTER_TYPE_SIZE sizeof(vm_pointer_type)
#endif

/* internal */
#include "internal/pointer_type_v1.h"
#include "internal/virtual_pointer_type_v1.h"
#include "internal/vm_type_v1.h"

/* private */
typedef struct vm_pointer* vm_pointer_ptr;
typedef struct vm_pointer {
    virtual_pointer_ptr vptr;
    u64 offset;
} vm_pointer_type;

typedef struct virtual_pointer_enumerator {
    pointer_ptr (*next)(void);
} virtual_pointer_enumerator_type;

/* public */
static const_vm_ptr virtual_init(u64 size);
static void virtual_destroy(const_vm_ptr cvm);

static u64 virtual_alloc(const_vm_ptr cvm, u64 size, u64 type_id);
static const_pointer_ptr virtual_read(const_vm_ptr cvm, u64 address);
static u64 virtual_type(const_vm_ptr cvm, u64 address);
static u64 virtual_free(const_vm_ptr cvm, u64 address);

/* internal */
static u64 virtual_alloc_internal(const_vm_ptr cvm, u64 size, u64 type_id);
static virtual_pointer_ptr virtual_init_internal(u64 size);
static pointer_ptr virtual_read_internal(const_virtual_pointer_ptr vptr, u64 address);

/* code */
static u64 virtual_alloc_internal(const_vm_ptr cvm, u64 size, u64 type_id) {
    safe_virtual_pointer_ptr s;
    s.const_ptr = &(*cvm)->next;
    virtual_pointer_ptr* tail = s.ptr;
    pointer_ptr src;
    pointer_ptr* tmp = 0;
    virtual_pointer_ptr vptr;
#ifndef USE_GC
    vm_pointer_ptr item = (vm_pointer_ptr)CALL(list)->pop(cvm, (*cvm)->cache);
    if (item != 0) {
        vptr = item->vptr;
        tmp = &vptr->bp[item->offset];
        CALL(os)->free(item);
    }
    if (tmp == 0) {
#endif
        vptr = *tail;
        if ((u64)(vptr->sp - vptr->bp) == DEFAULT_SIZE) {
            virtual_pointer_ptr prev = vptr;
            vptr = virtual_init_internal(DEFAULT_SIZE);
            vptr->next = prev;
            vptr->offset = prev->offset + DEFAULT_SIZE;
            *tail = vptr;
        }
        tmp = vptr->sp;
        ++vptr->sp;
#ifndef USE_GC
    }
#endif
    u64 address = (u64)(tmp - vptr->bp) + vptr->offset + 1;
    src = CALL(os)->calloc(1, POINTER_TYPE_SIZE);
    safe_pointer_ptr safe_ptr;
    safe_ptr.const_ptr = src;
    pointer_ptr ptr = safe_ptr.ptr;
    *tmp = ptr;
    ptr->vptr = vptr;
    ptr->public.address = address;
    ptr->public.type = type_id;
    ptr->public.size = size;
#ifdef USE_MEMORY_DEBUG_INFO
#ifdef USE_MEMORY_DEBUG_INFO
#ifdef USE_TTY
    const char* start = "\x1b[34m";
    const char* end = "\x1b[0m";
    fprintf(stderr, "%s[  v+ ]%s: %016llx ! %016llx > %016llx\n", start, end, address, (u64)ptr, (u64)vptr); /* NOLINT */
#else
    fprintf(stderr, "  v+ : %016llx ! %016llx > %016llx\n", address, (u64)ptr, (u64)vptr); /* NOLINT */
#endif
#endif
#endif
    return address;
}

static virtual_pointer_ptr virtual_init_internal(u64 size) {
    virtual_pointer_ptr vptr = CALL(os)->calloc(1, VIRTUAL_POINTER_TYPE_SIZE);
    pointer_ptr* ref = CALL(os)->calloc(size, PTR_SIZE);
    vptr->bp = ref;
    vptr->sp = ref;
    return vptr;
}

static pointer_ptr virtual_read_internal(const_virtual_pointer_ptr vptr, u64 address) {
    pointer_ptr ptr = 0;
    do {
        if (address > vptr->offset && address <= vptr->offset + DEFAULT_SIZE) {
            u64 offset = address - vptr->offset - 1;
            ptr = vptr->bp[offset];
            break;
        }
        vptr = vptr->next;
    } while (vptr != 0);
    if (ptr == 0) {
        ERROR_INVALID_VALUE("ptr == %p, address == %lld", (const_void_ptr)ptr, address);
        return NULL_PTR;
    }
    return ptr;
}

static vm_ptr vm[VM_PTR_ARRAY_SIZE];

static vm_ptr* get_free_ptr(void) {
    for (int i = 0; i < VM_PTR_ARRAY_SIZE; i++) {
        if (vm[i] == 0) {
            return vm + i;
        }
    }
    return NULL_PTR;
}

/* implementation */
static const_vm_ptr virtual_init(u64 size) {
    vm_ptr* ptr = get_free_ptr();
    if (ptr == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)ptr);
        return NULL_PTR;
    }
    *ptr = CALL(os)->calloc(1, VM_TYPE_SIZE);
    safe_vm_ptr safe_ptr;
    safe_ptr.ptr = ptr;
    const_vm_ptr cvm = safe_ptr.const_ptr;
    virtual_pointer_ptr vptr = virtual_init_internal(size == 0 ? DEFAULT_SIZE : size);
    (*ptr)->next = vptr;
#ifndef USE_GC
    (*ptr)->cache = CALL(list)->init(cvm);
#endif
    return cvm;
}

static void virtual_destroy(const_vm_ptr cvm) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return;
    }
    safe_vm_ptr safe_ptr;
    safe_ptr.const_ptr = cvm;
    vm_ptr ptr = *safe_ptr.ptr;
#ifndef USE_GC
    vm_pointer_ptr item;
    while ((item = (vm_pointer_ptr)CALL(list)->pop(cvm, (*cvm)->cache)) != 0) {
        CALL(os)->free(item);
    }
    CALL(list)->destroy(cvm, (*cvm)->cache);
    ptr->cache = 0;
#endif
    virtual_pointer_ptr vptr = (*cvm)->next;
    while (vptr != 0) {
        virtual_pointer_ptr next = vptr->next;
        CALL(os)->free(vptr->bp);
        CALL(os)->free(vptr);
        vptr = next;
    }
    ptr->next = 0;
    CALL(os)->free(ptr);
    *safe_ptr.ptr = 0;
}

static const_pointer_ptr virtual_read(const_vm_ptr cvm, u64 address) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return NULL_PTR;
    }
    if (address == 0) {
        ERROR_INVALID_ARGUMENT("address == %lld", address);
        return NULL_PTR;
    }
    const_virtual_pointer_ptr const_vptr = (*cvm)->next;
    const_pointer_ptr const_ptr = virtual_read_internal(const_vptr, address);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld", (const_void_ptr)const_ptr, address);
        return FALSE;
    }
#ifdef USE_MEMORY_DEBUG_INFO
    safe_pointer_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr;
    const_pointer_ptr ptr = safe_ptr.ptr;
    const_virtual_pointer_ptr vptr = ptr->vptr;
#ifdef USE_TTY
    const char* start = "\x1b[34m";
    const char* end = "\x1b[0m";
    fprintf(stderr, "%s[  v. ]%s: %016llx ! %016llx > %016llx\n", start, end, address, (u64)ptr, (u64)vptr); /* NOLINT */
#else
    fprintf(stderr, "  v. : %016llx ! %016llx > %016llx\n", address, (u64)ptr, (u64)vptr); /* NOLINT */
#endif
#endif
    return const_ptr;
}

static u64 virtual_type(const_vm_ptr cvm, u64 address) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (address == 0) {
        ERROR_INVALID_ARGUMENT("address == %lld", address);
        return FALSE;
    }
    const_virtual_pointer_ptr const_vptr = (*cvm)->next;
    const_pointer_ptr const_ptr = virtual_read_internal(const_vptr, address);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld", (const_void_ptr)const_ptr, address);
        return FALSE;
    }
    safe_pointer_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr;
    pointer_ptr ptr = safe_ptr.ptr;
    const_pointer_public_ptr public_ptr = &ptr->public;
    u64 type = public_ptr->type;
#ifdef USE_MEMORY_DEBUG_INFO
    const_virtual_pointer_ptr vptr = ptr->vptr;
#ifdef USE_MEMORY_DEBUG_INFO
#ifdef USE_TTY
    const char* start = "\x1b[34m";
    const char* end = "\x1b[0m";
    fprintf(stderr, "%s[  v? ]%s: %016llx ! %016llx > %016llx\n", start, end, address, (u64)ptr, (u64)vptr); /* NOLINT */
#else
    fprintf(stderr, "  v? : %016llx ! %016llx > %016llx\n", address, (u64)ptr, (u64)vptr); /* NOLINT */
#endif
#endif
#endif
    return type;
}

static u64 virtual_free(const_vm_ptr cvm, u64 address) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (address == 0) {
        return FALSE;
    }
    const_virtual_pointer_ptr const_vptr = (*cvm)->next;
    const_pointer_ptr ptr = virtual_read_internal(const_vptr, address);
    if (ptr == 0) {
        ERROR_INVALID_VALUE("ptr == %p, size == %lld", (const_void_ptr)ptr, address);
        return FALSE;
    }
    virtual_pointer_ptr vptr = ptr->vptr;
    if (address > vptr->offset && address <= vptr->offset + DEFAULT_SIZE) {
        u64 offset = address - vptr->offset - 1;
#ifndef USE_GC
        vm_pointer_ptr item = CALL(os)->calloc(1, VM_POINTER_TYPE_SIZE);
        item->vptr = vptr;
        item->offset = offset;
        CALL(list)->push(cvm, (*cvm)->cache, item);
#endif
#ifdef USE_MEMORY_DEBUG_INFO
#ifdef USE_MEMORY_DEBUG_INFO
#ifdef USE_TTY
        const char* start = "\x1b[34m";
        const char* end = "\x1b[0m";
        fprintf(stderr, "%s[  v- ]%s: %016llx ! %016llx > %016llx\n", start, end, address, (u64)ptr, (u64)vptr); /* NOLINT */
#else
        fprintf(stderr, "  v- : %016llx ! %016llx > %016llx\n", address, (u64)ptr, (u64)vptr); /* NOLINT */
#endif
#endif
#endif
        vptr->bp[offset] = 0;
    }
    return TRUE;
}

static u64 virtual_alloc(const_vm_ptr cvm, u64 size, u64 type_id) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (size == 0) {
        ERROR_INVALID_ARGUMENT("size == %lld", size);
        return FALSE;
    }
    if (type_id == 0) {
        ERROR_INVALID_ARGUMENT("type_id == %lld", type_id);
        return FALSE;
    }
    u64 address = virtual_alloc_internal(cvm, size, type_id);
    return address;
}

/* public */
const virtual_methods PRIVATE_API(virtual_methods_definitions) = {
    .alloc = virtual_alloc,
    .read = virtual_read,
    .type = virtual_type,
    .free = virtual_free
};

const virtual_methods* PRIVATE_API(virtual) = &PRIVATE_API(virtual_methods_definitions);
const virtual_methods* CALL(virtual) {
    return PRIVATE_API(virtual);
}

const virtual_system_methods PRIVATE_API(virtual_system_methods_definitions) = {
    .init = virtual_init,
    .destroy = virtual_destroy,
};

const virtual_system_methods* PRIVATE_API(system) = &PRIVATE_API(virtual_system_methods_definitions);
const virtual_system_methods* CALL(system) {
    return PRIVATE_API(system);
}
