/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 6, 2025 at 12:25:10 AM GMT+3
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

#include "sys/memory/memory_v1.h"

#include "sys/list/list_v1.h"

#include "vm/pointer/pointer_v1.h"

/* macros */
#define DEFAULT_SIZE 0x8 /* 8 */
#define PTR_SIZE sizeof(void*) /* size of a pointer */
#define VM_DATA_SIZE sizeof(struct virtual_pointer)
#define ALLOC_SIZE(size) (size * PTR_SIZE)

#ifndef USE_GC
#define VM_POINTER_SIZE sizeof(struct vm_pointer)
/* pointer definition */
struct vm_pointer {
    pointer_ptr* ptr;
    virtual_pointer_ptr virtual_pointer;
};
#endif

/* private */
struct vm {
    virtual_pointer_ptr head;
    virtual_pointer_ptr tail;
};

#ifndef USE_GC
static stack_ptr* cache;
#endif

static struct vm vm_definition_v1;

static struct vm* vm = &vm_definition_v1;

struct virtual_pointer {
    pointer_ptr* sp; /* stack pointer */
    pointer_ptr* bp; /* base pointer */
    virtual_pointer_ptr prev;
    virtual_pointer_ptr next;
    u64 offset;
    u64 size;
};

struct vm_state {
    virtual_pointer_ptr virtual;
    pointer_ptr* ptr;
};

struct virtual_pointer_enumerator {
    pointer_ptr (*next)(void);
};

static struct vm_state vm_state;
static struct vm_state* state = &vm_state;

/* api */

static void virtual_init(struct vm**, u64 size);
static void virtual_destroy(struct vm**);
#ifdef USE_MEMORY_DEBUG_INFO
static void vm_dump(void);
static void vm_dump_ref(void);
#endif
static u64 virtual_alloc(pointer_ptr ptr);
static void virtual_free(const_pointer_ptr ptr);
static pointer_ptr* virtual_read(u64 address);
static pointer_ptr* virtual_read_type(u64 address, u64 id);
static u64 virtual_enumerator_next(void);

/* internal */

static virtual_pointer_ptr virtual_init_internal(u64 size, u64 offset);
static pointer_ptr* virtual_read_internal(u64 address);
static pointer_ptr* virtual_alloc_internal(u64* address, virtual_pointer_ptr* target);
static void virtual_enumerator_init(void);
static void virtual_enumerator_destroy(void);
static void virtual_enumerator_init_internal(void);
static void virtual_enumerator_destroy_internal(void);
static void** virtual_enumerator_next_internal(void);
#ifdef USE_MEMORY_DEBUG_INFO
static pointer_ptr virtual_enumerator_pointer_next_internal(void);
#endif

static virtual_pointer_ptr virtual_init_internal(u64 size, u64 offset) {
    virtual_pointer_ptr virtual_pointer = CALL(system_memory)->alloc(VM_DATA_SIZE);
    virtual_pointer->bp = CALL(system_memory)->alloc(ALLOC_SIZE(size));
    virtual_pointer->sp = virtual_pointer->bp;
    virtual_pointer->offset = offset;
    virtual_pointer->size = size;
    return virtual_pointer;
}

static pointer_ptr* virtual_read_internal(u64 address) {
    if (vm->head == 0) {
        return 0;
    }
    pointer_ptr* ptr = 0;
    const_virtual_pointer_ptr virtual_pointer = vm->head;
    do {
        if (address > virtual_pointer->offset && address <= virtual_pointer->offset + virtual_pointer->size) {
            ptr = virtual_pointer->bp + address - virtual_pointer->offset - 1;
            break;
        }
        virtual_pointer = virtual_pointer->next;
    } while (virtual_pointer != 0);
    return ptr;
}

static pointer_ptr* virtual_alloc_internal(u64* address, virtual_pointer_ptr* target) {
    virtual_pointer_ptr virtual_pointer = vm->tail;
    pointer_ptr* ptr = 0;
#ifndef USE_GC
    struct vm_pointer* list_ptr = CALL(system_list)->pop(cache);
    if (list_ptr != 0) {
        ptr = list_ptr->ptr;
        virtual_pointer = list_ptr->virtual_pointer;
        CALL(system_memory)->free(list_ptr, VM_POINTER_SIZE);
    }
    if (ptr == 0) {
#endif
        if ((u64)(virtual_pointer->sp - virtual_pointer->bp) == virtual_pointer->size) {
            virtual_pointer_ptr prev = virtual_pointer;
            virtual_pointer = virtual_init_internal(virtual_pointer->size, virtual_pointer->offset + virtual_pointer->size);
            virtual_pointer->prev = prev;
            prev->next = virtual_pointer;
            vm->tail = virtual_pointer;
        }
        ptr = virtual_pointer->sp;
        ++virtual_pointer->sp;
#ifndef USE_GC
    }
#endif
    *target = virtual_pointer;
    *address = (u64)(ptr - virtual_pointer->bp) + virtual_pointer->offset + 1;
    return ptr;
}

static void virtual_enumerator_init(void) {
    if (vm->head == 0) {
        return;
    }
    virtual_enumerator_init_internal();
}

static void virtual_enumerator_destroy(void) {
    virtual_enumerator_destroy_internal();
}

static void virtual_enumerator_init_internal() {
    virtual_pointer_ptr virtual_pointer = vm->head;
    state->virtual = virtual_pointer;
    state->ptr = virtual_pointer->bp;
}

static void virtual_enumerator_destroy_internal(void) {
    state->virtual = 0;
    state->ptr = 0;
}

/* implementation */

static void virtual_init(struct vm** ptr, u64 size) {
#ifndef USE_GC
    cache = CALL(system_memory)->alloc(PTR_SIZE);
    CALL(system_list)->init(cache);
#endif
    virtual_pointer_ptr virtual_pointer = virtual_init_internal(size == 0 ? DEFAULT_SIZE : size, 0);
    vm->head = virtual_pointer;
    vm->tail = virtual_pointer;
    struct vm* vm_ptr = CALL(system_memory)->alloc(sizeof(struct vm));
    vm_ptr->head = virtual_pointer;
    vm_ptr->tail = virtual_pointer;
    *ptr = vm_ptr;
}

static void virtual_destroy(struct vm** ptr) {
    if (ptr == 0 || *ptr == 0) {
        return;
    }
    struct vm* current = *ptr;
    vm->head = current->head;
    vm->tail = current->tail;
    CALL(system_memory)->free(current, sizeof(struct vm));
    *ptr = 0;
#ifndef USE_GC
    struct vm_pointer* vm_pointer_ptr = 0;
    while ((vm_pointer_ptr = CALL(system_list)->pop(cache)) != 0) {
        CALL(system_memory)->free(vm_pointer_ptr, VM_POINTER_SIZE);
    }
    CALL(system_list)->destroy(cache);
    CALL(system_memory)->free(cache, PTR_SIZE);
#ifdef USE_MEMORY_CLEANUP
    cache = 0;
#endif
#endif
    virtual_pointer_ptr virtual_pointer = vm->head;
    while (virtual_pointer != 0) {
        virtual_pointer_ptr next = virtual_pointer->next;
        CALL(system_memory)->free(virtual_pointer->bp, ALLOC_SIZE(virtual_pointer->size));
        CALL(system_memory)->free(virtual_pointer, VM_DATA_SIZE);
        virtual_pointer = next;
    }
    vm->head = 0;
    vm->tail = 0;
}

#ifdef USE_MEMORY_DEBUG_INFO
static void virtual_dump(void) {
    virtual_enumerator_init_internal(vm);
    pointer_ptr ptr = 0;
    while ((ptr = virtual_enumerator_pointer_next_internal()) != 0) {
        CALL(pointer)->dump(ptr);
    }
    virtual_enumerator_destroy_internal();
}

static void virtual_dump_ref(void) {
    virtual_enumerator_init_internal(vm);
    void** ptr = 0;
    while ((ptr = virtual_enumerator_next_internal()) != 0) {
        CALL(pointer)->dump_ref(ptr);
    }
    virtual_enumerator_destroy_internal();
}
#endif

static void virtual_free(const_pointer_ptr ptr) {
    if (!ptr) {
        return;
    }
    const virtual_pointer_ptr virtual_pointer = CALL(pointer)->ref(ptr);
    if (!virtual_pointer) {
        return;
    }
    u64 address = CALL(pointer)->address(ptr);
    pointer_ptr* data = virtual_pointer->bp - virtual_pointer->offset - 1 + address;
#ifndef USE_GC
    struct vm_pointer* vm_pointer_ptr = CALL(system_memory)->alloc(VM_POINTER_SIZE);
    vm_pointer_ptr->ptr = data;
    vm_pointer_ptr->virtual_pointer = CALL(pointer)->ref(ptr);
    CALL(system_list)->push(cache, vm_pointer_ptr);
#endif
#ifdef USE_MEMORY_DEBUG_INFO
    printf("  >-: %016llx ! %016llx > %016llx\n", address, (u64)ptr, (u64)data);
#endif
    *data = 0;
}

static pointer_ptr* virtual_read_type(u64 address, u64 id) {
    if (!address) {
        return 0;
    }
    pointer_ptr* data = virtual_read_internal(address);
    if (data == 0) {
        return 0;
    }
    if (*data == 0) {
        return 0;
    }
    const_pointer_ptr ptr = *data;
    if (!CALL(pointer)->read_type(ptr, id)) {
        return 0;
    }
#ifdef USE_MEMORY_DEBUG_INFO
    printf("  ?v: %016llx ! %016llx > %016llx\n", address, (u64)ptr, (u64)data);
#endif
    return data;
}

static pointer_ptr* virtual_read(u64 address) {
    if (!address) {
        return 0;
    }
    pointer_ptr* data = virtual_read_internal(address);
    const_pointer_ptr ptr = 0;
    if (data == 0) {
        return 0;
    }
    ptr = *data;
    if (ptr == 0) {
        return 0;
    }
#ifdef USE_MEMORY_DEBUG_INFO
    printf("  !v: %016llx ! %016llx > %016llx\n", address, (u64)ptr, (u64)data);
#endif
    return data;
}

static u64 virtual_alloc(pointer_ptr ptr) {
    if (!ptr) {
        return 0;
    }
    u64 address = 0;
    virtual_pointer_ptr target = 0;
    pointer_ptr* data = virtual_alloc_internal(&address, &target);
    *data = ptr;
    CALL(pointer)->write(ptr, target, address);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("  >+: %016llx ! %016llx > %016llx\n", address, (u64)ptr, (u64)data);
#endif
    return address;
}

#ifdef USE_MEMORY_DEBUG_INFO
static pointer_ptr virtual_enumerator_pointer_next_internal(void) {
    pointer_ptr data = 0;
    virtual_pointer_ptr virtual_pointer = state->virtual_pointer;
    while (data == 0) {
        if (state->ptr == virtual_pointer->sp) {
            if (virtual_pointer->next == 0) {
                break;
            }
            virtual_pointer = virtual_pointer->next;
            state->virtual_pointer = virtual_pointer;
            state->ptr = virtual_pointer->bp;
        }
        data = *state->ptr++;
    }
    return data;
}
#endif

static u64 virtual_enumerator_next(void) {
    u64 address = 0;
    void** ptr;
    while ((ptr = virtual_enumerator_next_internal()) != 0) {
        if (*ptr != 0) {
            const_pointer_ptr data_ptr = *ptr;
            if (data_ptr != 0) {
                address = CALL(pointer)->address(data_ptr);
                break;
            }
        }
    }
    return address;
}

static void** virtual_enumerator_next_internal(void) {
    if (vm->head == 0) {
        return 0;
    }
    void** data = 0;
    virtual_pointer_ptr virtual_pointer = state->virtual;
    while (data == 0) {
        if (state->ptr == virtual_pointer->sp) {
            if (virtual_pointer->next == 0) {
                break;
            }
            virtual_pointer = virtual_pointer->next;
            state->virtual = virtual_pointer;
            state->ptr = virtual_pointer->bp;
        }
        data = (void**)state->ptr++;
    }
    return data;
}

/* public */
const virtual_methods PRIVATE_API(virtual_methods_definitions) = {
    .init = virtual_init,
    .destroy = virtual_destroy,
    .alloc = virtual_alloc,
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
