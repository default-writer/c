/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 12, 2025 at 9:58:46 PM GMT+3
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

#define USING_MACROS

#include "virtual_v1.h"

#define USING_ERROR_API

#include "system/error/error_v1.h"

#define VM_SIZE sizeof(vm_type)

#include "system/memory/memory_v1.h"

#include "system/list/list_v1.h"

#include "virtual/pointer/pointer_v1.h"

#ifdef USE_MEMORY_DEBUG_INFO
#include <stdio.h>
#endif

/* macros */
#define DEFAULT_SIZE 0x8 /* 8 */
#define PTR_SIZE sizeof(void*) /* size of a pointer */
#define VM_DATA_SIZE sizeof(virtual_pointer_type)
#define ALLOC_SIZE(size) (size * PTR_SIZE)

#ifndef USE_GC
#define VM_POINTER_SIZE sizeof(vm_pointer_type)

/* pointer definition */
typedef struct vm_pointer* vm_pointer_ptr;
typedef struct vm_pointer {
    pointer_ptr* ptr;
    virtual_pointer_ptr vptr;
} vm_pointer_type;
#endif

/* private */
typedef struct vm {
    virtual_pointer_ptr head;
    virtual_pointer_ptr tail;
} vm_type;

#ifndef USE_GC
static stack_ptr* cache;
#endif

typedef struct virtual_pointer {
    pointer_ptr* sp; /* stack pointer */
    pointer_ptr* bp; /* base pointer */
    virtual_pointer_ptr prev;
    virtual_pointer_ptr next;
    u64 offset;
    u64 size;
} virtual_pointer_type;

typedef struct vm_state {
    virtual_pointer_ptr vptr;
    pointer_ptr* ptr;
} vm_state_type;

typedef struct virtual_pointer_enumerator {
    pointer_ptr (*next)(void);
} virtual_pointer_enumerator_type;

static struct vm_state vm_state;
static struct vm_state* state = &vm_state;

/* api */

static void virtual_init(const_vm_ptr vm, u64 size);
static void virtual_destroy(const_vm_ptr vm);
#ifdef USE_MEMORY_DEBUG_INFO
static void vm_dump(void);
static void vm_dump_ref(void);
#endif
static u64 virtual_alloc(const_vm_ptr vm, const_pointer_ptr const_ptr);
static u64 virtual_pointer(const_vm_ptr vm, u64 size, u64 type_id);
static u64 virtual_memcpy(const_vm_ptr vm, u64 size, const void* data, u64 type_id);
static void virtual_free(const_vm_ptr vm, u64 address);
static const_pointer_ptr virtual_read(const_vm_ptr vm, u64 address);
static const_pointer_ptr virtual_read_type(const_vm_ptr vm, u64 address, u64 type_id);
static u64 virtual_enumerator_next(const_vm_ptr vm);

/* internal */

static u64 get_address_internal(pointer_ptr* ptr, const_virtual_pointer_ptr vptr);
static virtual_pointer_ptr virtual_init_internal(u64 size);
static pointer_ptr virtual_read_internal(const_vm_ptr vm, u64 address);
static void virtual_enumerator_init(const_vm_ptr vm);
static void virtual_enumerator_destroy(void);
static void virtual_enumerator_init_internal(const_vm_ptr vm);
static void virtual_enumerator_destroy_internal(void);
static pointer_ptr* virtual_enumerator_next_internal(void);
#ifdef USE_MEMORY_DEBUG_INFO
static pointer_ptr virtual_enumerator_pointer_next_internal(void);
#endif

/* code */

static u64 get_address_internal(pointer_ptr* ptr, const_virtual_pointer_ptr vptr) {
    return (u64)(ptr - vptr->bp) + vptr->offset + 1;
}

static virtual_pointer_ptr virtual_init_internal(u64 size) {
    virtual_pointer_ptr vptr = CALL(system_memory)->alloc(VM_DATA_SIZE);
    pointer_ptr* ptr = CALL(system_memory)->alloc(ALLOC_SIZE(size));
    vptr->bp = ptr;
    vptr->sp = ptr;
    vptr->size = size;
    return vptr;
}

static pointer_ptr virtual_read_internal(const_vm_ptr vm, u64 address) {
    pointer_ptr ptr = 0;
    const_virtual_pointer_ptr vptr = (*vm)->head;
    do {
        if (address > vptr->offset && address <= vptr->offset + vptr->size) {
            ptr = vptr->bp[address - vptr->offset - 1];
            break;
        }
        vptr = vptr->next;
    } while (vptr != 0);
    if (ptr == 0) {
        return NULL_PTR;
    }
    return ptr;
}

static void virtual_enumerator_init(const_vm_ptr vm) {
    if (vm == 0 || *vm == 0) {
        return;
    }
    virtual_enumerator_init_internal(vm);
}

static void virtual_enumerator_destroy(void) {
    virtual_enumerator_destroy_internal();
}

static void virtual_enumerator_init_internal(const_vm_ptr vm) {
    virtual_pointer_ptr vptr = (*vm)->head;
    state->vptr = vptr;
    state->ptr = vptr->bp;
}

static void virtual_enumerator_destroy_internal(void) {
    state->vptr = 0;
    state->ptr = 0;
}

/* implementation */

static void virtual_init(const_vm_ptr vm, u64 size) {
    if (vm == 0 || *vm != 0) {
        return;
    }
#ifndef USE_GC
    cache = CALL(system_memory)->alloc(PTR_SIZE);
    CALL(system_list)->init(cache);
#endif
    vm_ptr ref = CALL(system_memory)->alloc(VM_SIZE);
    virtual_pointer_ptr vptr = virtual_init_internal(size == 0 ? DEFAULT_SIZE : size);
    ref->head = vptr;
    ref->tail = vptr;
    safe_vm_ptr safe_ptr;
    safe_ptr.const_ptr = vm;
    *safe_ptr.ptr = ref;
}

static void virtual_destroy(const_vm_ptr vm) {
    if (vm == 0 || *vm == 0) {
        return;
    }
#ifndef USE_GC
    vm_pointer_ptr item;
    while ((item = CALL(system_list)->pop(cache)) != 0) {
        CALL(system_memory)->free(item, VM_POINTER_SIZE);
    }
    CALL(system_list)->destroy(cache);
    CALL(system_memory)->free(cache, PTR_SIZE);
#ifdef USE_MEMORY_CLEANUP
    cache = 0;
#endif
#endif
    safe_vm_ptr safe_ptr;
    safe_ptr.const_ptr = vm;
    vm_ptr ptr = *safe_ptr.ptr;
    virtual_pointer_ptr vptr = ptr->head;
    while (vptr != 0) {
        virtual_pointer_ptr next = vptr->next;
        CALL(system_memory)->free(vptr->bp, ALLOC_SIZE(vptr->size));
        CALL(system_memory)->free(vptr, VM_DATA_SIZE);
        vptr = next;
    }
    ptr->head = 0;
    ptr->tail = 0;
    CALL(system_memory)->free(ptr, VM_SIZE);
    *safe_ptr.ptr = 0;
}

#ifdef USE_MEMORY_DEBUG_INFO
static void virtual_dump(const_vm_ptr vm) {
    if (vm == 0 || *vm == 0) {
        return;
    }
    virtual_enumerator_init_internal(vm);
    pointer_ptr ptr = 0;
    while ((ptr = virtual_enumerator_pointer_next_internal()) != 0) {
        CALL(pointer)->dump(ptr);
    }
    virtual_enumerator_destroy_internal();
}

static void virtual_dump_ref(const_vm_ptr vm) {
    if (vm == 0 || *vm == 0) {
        return;
    }
    virtual_enumerator_init_internal(vm);
    pointer_ptr* ptr = 0;
    while ((ptr = virtual_enumerator_next_internal()) != 0) {
        CALL(pointer)->dump_ref(ptr);
    }
    virtual_enumerator_destroy_internal();
}
#endif

static void virtual_free(const_vm_ptr vm, u64 address) {
    if (vm == 0 || *vm == 0) {
        return;
    }
    if (address == 0) {
        return;
    }
    const_pointer_ptr const_ptr = virtual_read_internal(vm, address);
    if (const_ptr == 0) {
        return;
    }
    safe_pointer_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr;
    pointer_ptr ptr = safe_ptr.ptr;
    virtual_pointer_ptr vptr = *(virtual_pointer_ptr*)ptr;
    if (address > vptr->offset && address <= vptr->offset + vptr->size) {
        u64 offset = address - vptr->offset - 1;
        pointer_ptr* offset_ptr = vptr->bp + offset;
#ifndef USE_GC
        vm_pointer_ptr item = CALL(system_memory)->alloc(VM_POINTER_SIZE);
        item->ptr = offset_ptr;
        item->vptr = vptr;
        CALL(system_list)->push(cache, item);
#endif
#ifdef USE_MEMORY_DEBUG_INFO
        printf("  >-: %016llx ! %016llx > %016llx\n", address, (u64)ptr, (u64)vptr);
#endif
        CALL(pointer)->set(const_ptr, 0, 0);
        *offset_ptr = 0;
    }
}

static const_pointer_ptr virtual_read_type(const_vm_ptr vm, u64 address, u64 type_id) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return NULL_PTR;
    }
    if (address == 0) {
        ERROR_ADDRESS_NOT_INITIALIZED(address == 0);
        return NULL_PTR;
    }
    const_pointer_ptr const_ptr = virtual_read_internal(vm, address);
    if (const_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(const_ptr == 0);
        return NULL_PTR;
    }
    if (CALL(pointer)->get_type(const_ptr) != type_id) {
        return NULL_PTR;
    }
#ifdef USE_MEMORY_DEBUG_INFO
    safe_pointer_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr;
    pointer_ptr ptr = safe_ptr.ptr;
    virtual_pointer_ptr vptr = *(virtual_pointer_ptr*)ptr;
    printf("  ?v: %016llx ! %016llx > %016llx\n", address, (u64)ptr, (u64)vptr);
#endif
    return const_ptr;
}

static const_pointer_ptr virtual_read(const_vm_ptr vm, u64 address) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return NULL_PTR;
    }
    if (address == 0) {
        ERROR_ADDRESS_NOT_INITIALIZED(address == 0);
        return NULL_PTR;
    }
    const_pointer_ptr const_ptr = virtual_read_internal(vm, address);
    if (const_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(const_ptr == 0);
        return NULL_PTR;
    }
#ifdef USE_MEMORY_DEBUG_INFO
    safe_pointer_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr;
    pointer_ptr ptr = safe_ptr.ptr;
    virtual_pointer_ptr vptr = *(virtual_pointer_ptr*)ptr;
    printf("  !v: %016llx ! %016llx > %016llx\n", address, (u64)ptr, (u64)vptr);
#endif
    return const_ptr;
}

static u64 virtual_alloc(const_vm_ptr vm, const_pointer_ptr const_ptr) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    if (const_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(const_ptr == 0);
        return FALSE;
    }
    u64 address = 0;
    pointer_ptr* ptr = 0;
    virtual_pointer_ptr vptr;
#ifndef USE_GC
    vm_pointer_ptr item = CALL(system_list)->pop(cache);
    if (item != 0) {
        ptr = item->ptr;
        vptr = item->vptr;
        CALL(system_memory)->free(item, VM_POINTER_SIZE);
    }
    if (ptr == 0) {
#endif
        vptr = (*vm)->tail;
        if ((u64)(vptr->sp - vptr->bp) == vptr->size) {
            virtual_pointer_ptr prev = vptr;
            vptr = virtual_init_internal(prev->size);
            vptr->offset = prev->offset + prev->size;
            vptr->prev = prev;
            prev->next = vptr;
            (*vm)->tail = vptr;
        }
        ptr = vptr->sp;
        ++vptr->sp;
#ifndef USE_GC
    }
#endif
    address = get_address_internal(ptr, vptr);
    CALL(pointer)->set(const_ptr, vptr, address);
    safe_pointer_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr;
    *ptr = safe_ptr.ptr;
#ifdef USE_MEMORY_DEBUG_INFO
    printf("  >+: %016llx ! %016llx > %016llx\n", address, (u64)ptr, (u64)vptr);
#endif
    return address;
}

static u64 virtual_pointer(const_vm_ptr vm, u64 size, u64 type_id) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    const_pointer_ptr const_ptr = CALL(pointer)->alloc(size, type_id);
    return virtual_alloc(vm, const_ptr);
}

static u64 virtual_memcpy(const_vm_ptr vm, u64 size, const void* data, u64 type_id) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    const_pointer_ptr data_ptr = CALL(pointer)->alloc(size, type_id);
    CALL(pointer)->memcpy(data_ptr, data, size);
    return virtual_alloc(vm, data_ptr);
}

#ifdef USE_MEMORY_DEBUG_INFO
static pointer_ptr virtual_enumerator_pointer_next_internal(void) {
    pointer_ptr data = 0;
    virtual_pointer_ptr vptr = state->vptr;
    while (data == 0) {
        if (state->ptr == vptr->sp) {
            if (vptr->next == 0) {
                break;
            }
            vptr = vptr->next;
            state->vptr = vptr;
            state->ptr = vptr->bp;
        }
        data = *state->ptr++;
    }
    return data;
}
#endif

static u64 virtual_enumerator_next(const_vm_ptr vm) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    u64 address = 0;
    pointer_ptr* ptr;
    while ((ptr = virtual_enumerator_next_internal()) != 0) {
        if (*ptr != 0) {
            const_pointer_ptr data_ptr = *ptr;
            if (data_ptr != 0) {
                address = CALL(pointer)->get_address(data_ptr);
                break;
            }
        }
    }
    return address;
}

static pointer_ptr* virtual_enumerator_next_internal(void) {
    pointer_ptr* data = 0;
    virtual_pointer_ptr vptr = state->vptr;
    while (data == 0) {
        if (state->ptr == vptr->sp) {
            if (vptr->next == 0) {
                break;
            }
            vptr = vptr->next;
            state->vptr = vptr;
            state->ptr = vptr->bp;
        }
        data = state->ptr++;
    }
    return data;
}

/* public */
const virtual_methods PRIVATE_API(virtual_methods_definitions) = {
    .init = virtual_init,
    .destroy = virtual_destroy,
    .alloc = virtual_alloc,
    .pointer = virtual_pointer,
    .memcpy = virtual_memcpy,
    .free = virtual_free,
    .read = virtual_read,
    .read_type = virtual_read_type,
    .enumerator_init = virtual_enumerator_init,
    .enumerator_destroy = virtual_enumerator_destroy,
    .enumerator_next = virtual_enumerator_next,
#ifdef USE_MEMORY_DEBUG_INFO
    .dump = virtual_dump,
    .dump_ref = virtual_dump_ref
#endif
};

const virtual_methods* virtual = &PRIVATE_API(virtual_methods_definitions);

const virtual_methods* CALL(virtual) {
    return virtual;
}
