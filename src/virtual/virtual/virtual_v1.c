/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 28, 2025 at 1:44:35 PM GMT+3
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

#include "virtual_v1.h"

#define USING_SYSTEM_ERROR_API
#include "system/error/error_v1.h"

#include "system/api/api_v1.h"
#include "system/list/list_v1.h"
#include "system/memory/memory_v1.h"

/* macros */
#define VM_TYPE_SIZE sizeof(vm_type)
#define DEFAULT_SIZE 0x8 /* 8 */
#define VIRTUAL_POINTER_TYPE_SIZE sizeof(virtual_pointer_type)
#define PTR_SIZE sizeof(void*) /* size of a pointer */
#define PTR_ARRAY_SIZE(size) (size * PTR_SIZE)
#define POINTER_TYPE_SIZE sizeof(pointer_type)
#define KNOWN_TYPES_TYPE_SIZE sizeof(known_types_type)
#define KNOWN_TYPES_TYPE_ARRAY_SIZE(size) (size * KNOWN_TYPES_TYPE_SIZE)

#ifndef USE_GC
#define VM_POINTER_TYPE_SIZE sizeof(vm_pointer_type)
#endif

/* internal */
#include "internal/internal_v1.h"
#include "internal/private_v1.h"

/* private */
#ifndef USE_GC
static stack_ptr* cache;
#endif

typedef struct vm_pointer* vm_pointer_ptr;
typedef struct vm_pointer {
    virtual_pointer_ptr vptr;
    u64 offset;
} vm_pointer_type;

typedef struct virtual_pointer_enumerator {
    pointer_ptr (*next)(void);
} virtual_pointer_enumerator_type;

/* api */
static void virtual_init(const_vm_ptr vm, u64 size);
static void virtual_destroy(const_vm_ptr vm);

static u64 virtual_alloc(const_vm_ptr vm, u64 size, u64 type_id);
static const_pointer_ptr virtual_read(const_vm_ptr vm, u64 address, u64 type_id);
static u64 virtual_type(const_vm_ptr vm, u64 address);
static u64 virtual_free(const_vm_ptr vm, u64 address);

/* internal */
static u64 virtual_alloc_internal(virtual_pointer_ptr* tail, u64 size, u64 type_id);
static virtual_pointer_ptr virtual_init_internal(u64 size);
static pointer_ptr virtual_read_internal(const_virtual_pointer_ptr vptr, u64 address);

/* code */
static u64 virtual_alloc_internal(virtual_pointer_ptr* tail, u64 size, u64 type_id) {
    pointer_ptr src;
    pointer_ptr* tmp = 0;
    virtual_pointer_ptr vptr;
#ifndef USE_GC
    vm_pointer_ptr item = CALL(system_list)->pop(cache);
    if (item != 0) {
        vptr = item->vptr;
        tmp = &vptr->bp[item->offset];
        api->free(item);
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
    src = api->alloc(1, POINTER_TYPE_SIZE);
    safe_pointer_ptr safe_ptr;
    safe_ptr.const_ptr = src;
    pointer_ptr ptr = safe_ptr.ptr;
    *tmp = ptr;
    ptr->vptr = vptr;
    ptr->public.address = address;
    ptr->public.type = type_id;
    ptr->public.size = size;
#ifdef USE_MEMORY_DEBUG_INFO
    printf("  v+: %016llx ! %016llx > %016llx\n", address, (u64)ptr, (u64)vptr);
#endif
    return address;
}

static virtual_pointer_ptr virtual_init_internal(u64 size) {
    virtual_pointer_ptr vptr = api->alloc(1, VIRTUAL_POINTER_TYPE_SIZE);
    pointer_ptr* ref = api->alloc(size, PTR_SIZE);
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
        return NULL_PTR;
    }
    return ptr;
}

/* implementation */
static void virtual_init(const_vm_ptr vm, u64 size) {
    if (vm == 0 || *vm != 0) {
        return;
    }
#ifndef USE_GC
    cache = api->alloc(1, PTR_SIZE);
    CALL(system_list)->init(cache);
#endif
    vm_ptr vm_list = api->alloc(1, VM_TYPE_SIZE);
    virtual_pointer_ptr vptr = virtual_init_internal(size == 0 ? DEFAULT_SIZE : size);
    vm_list->next = vptr;
    safe_vm_ptr safe_ptr;
    safe_ptr.const_ptr = vm;
    *safe_ptr.ptr = vm_list;
}

static void virtual_destroy(const_vm_ptr vm) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return;
    }
#ifndef USE_GC
    vm_pointer_ptr item;
    while ((item = CALL(system_list)->pop(cache)) != 0) {
        api->free(item);
    }
    CALL(system_list)->destroy(cache);
    api->free(cache);
#ifdef USE_MEMORY_CLEANUP
    cache = 0;
#endif
#endif
    safe_vm_ptr safe_ptr;
    safe_ptr.const_ptr = vm;
    vm_ptr vm_list = *safe_ptr.ptr;
    virtual_pointer_ptr vptr = (*vm)->next;
    while (vptr != 0) {
        virtual_pointer_ptr next = vptr->next;
        api->free(vptr->bp);
        api->free(vptr);
        vptr = next;
    }
    (*vm)->next = 0;
    vm_list->next = 0;
    api->free(vm_list);
    *safe_ptr.ptr = 0;
}

static const_pointer_ptr virtual_read(const_vm_ptr vm, u64 address, u64 type_id) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return NULL_PTR;
    }
    if (address == 0) {
        ERROR_INVALID_ADDRESS(address == 0);
        return NULL_PTR;
    }
    if (type_id == 0) {
        ERROR_INVALID_TYPEID(type_id == 0);
        return NULL_PTR;
    }
    const_virtual_pointer_ptr const_vptr = (*vm)->next;
#ifdef USE_MEMORY_DEBUG_INFO
    pointer_ptr const_ptr = virtual_read_internal(const_vptr, address);
#else
    const_pointer_ptr const_ptr = virtual_read_internal(const_vptr, address);
#endif
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER(const_ptr == 0);
        return FALSE;
    }
    safe_pointer_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr;
    const_pointer_ptr ptr = safe_ptr.ptr;
    const_pointer_public_ptr public_ptr = &ptr->public;
    u64 type = public_ptr->type;
    if (type != type_id) {
        ERROR_INVALID_TYPE(type_id);
        return NULL_PTR;
    }
#ifdef USE_MEMORY_DEBUG_INFO
    const_virtual_pointer_ptr vptr = ptr->vptr;
    printf("  v.: %016llx ! %016llx > %016llx\n", address, (u64)ptr, (u64)vptr);
#endif
    return ptr;
}

static u64 virtual_type(const_vm_ptr vm, u64 address) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    if (address == 0) {
        ERROR_INVALID_ADDRESS(address == 0);
        return FALSE;
    }
    const_virtual_pointer_ptr const_vptr = (*vm)->next;
#ifdef USE_MEMORY_DEBUG_INFO
    pointer_ptr const_ptr = virtual_read_internal(const_vptr, address);
#else
    const_pointer_ptr const_ptr = virtual_read_internal(const_vptr, address);
#endif
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER(ptr == 0);
        return FALSE;
    }
    safe_pointer_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr;
    pointer_ptr ptr = safe_ptr.ptr;
    const_pointer_public_ptr public_ptr = &ptr->public;
    u64 type = public_ptr->type;
#ifdef USE_MEMORY_DEBUG_INFO
    const_virtual_pointer_ptr vptr = ptr->vptr;
    printf("  v?: %016llx ! %016llx > %016llx\n", address, (u64)ptr, (u64)vptr);
#endif
    return type;
}

static u64 virtual_free(const_vm_ptr vm, u64 address) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    if (address == 0) {
        return FALSE;
    }
    const_virtual_pointer_ptr const_vptr = (*vm)->next;
    const_pointer_ptr ptr = virtual_read_internal(const_vptr, address);
    if (ptr == 0) {
        return FALSE;
    }
    virtual_pointer_ptr vptr = ptr->vptr;
    if (address > vptr->offset && address <= vptr->offset + DEFAULT_SIZE) {
        u64 offset = address - vptr->offset - 1;
#ifndef USE_GC
        vm_pointer_ptr item = api->alloc(1, VM_POINTER_TYPE_SIZE);
        item->vptr = vptr;
        item->offset = offset;
        CALL(system_list)->push(cache, item);
#endif
#ifdef USE_MEMORY_DEBUG_INFO
        printf("  v-: %016llx ! %016llx > %016llx\n", address, (u64)ptr, (u64)vptr);
#endif
        vptr->bp[offset] = 0;
    }
    return TRUE;
}

static u64 virtual_alloc(const_vm_ptr vm, u64 size, u64 type_id) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
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
    u64 address = virtual_alloc_internal(&(*vm)->next, size, type_id);
    return address;
}

/* public */
const virtual_methods PRIVATE_API(virtual_methods_definitions) = {
    .alloc = virtual_alloc,
    .read = virtual_read,
    .type = virtual_type,
    .free = virtual_free
};

const virtual_system_methods PRIVATE_API(virtual_type_methods_definitions) = {
    .init = virtual_init,
    .destroy = virtual_destroy,
};

const virtual_methods* virtual = &PRIVATE_API(virtual_methods_definitions);
const virtual_methods* CALL(virtual) {
    return virtual;
}

const virtual_system_methods* virtual_system = &PRIVATE_API(virtual_type_methods_definitions);
const virtual_system_methods* CALL(system) {
    return virtual_system;
}
