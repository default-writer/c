/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   February 8, 2025 at 8:46:13 PM GMT+3
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

#include "sys/memory/memory_v1.h"

#include "sys/list/list_v1.h"

#include "vm/v1/pointer/pointer_v1.h"

/* macros */
#define DEFAULT_SIZE 0x8 /* 8 */
#define PTR_SIZE sizeof(void*) /* size of a pointer */
#define VM_DATA_SIZE sizeof(struct vm_data)
#define ALLOC_SIZE(size) (size * PTR_SIZE)

#ifndef USE_GC
#define VM_POINTER_SIZE sizeof(struct vm_pointer)
/* pointer definition */
struct vm_pointer {
    pointer_ptr* ptr;
    struct vm_data* vm;
};
#endif

/* private */
struct vm {
    struct vm_data* head;
    struct vm_data* tail;
};

#ifndef USE_GC
static stack_ptr* cache;
#endif

static struct vm vm_definition_v1;

static struct vm* vm = &vm_definition_v1;

struct vm_data {
    pointer_ptr* sp; /* stack pointer */
    pointer_ptr* bp; /* base pointer */
    struct vm_data* prev;
    struct vm_data* next;
    u64 offset;
    u64 size;
};

struct vm_state {
    struct vm_data* vm;
    pointer_ptr* ptr;
};

struct vm_data_enumerator {
    pointer_ptr (*next)(void);
};

static struct vm_state vm_state;
static struct vm_state* state = &vm_state;

/* api */

static void vm_init(struct vm**, u64 size);
static void vm_destroy(struct vm**);
#ifdef USE_MEMORY_DEBUG_INFO
static void vm_dump(void);
static void vm_dump_ref(void);
#endif
static u64 vm_alloc(pointer_ptr ptr);
static void vm_free(const pointer_ptr ptr);
static pointer_ptr vm_read(u64 address);
static pointer_ptr vm_read_type(u64 address, u64 id);
static u64 virtual_enumerator_next(void);

/* internal */

static struct vm_data* vm_init_internal(u64 size, u64 offset);
static pointer_ptr* vm_read_internal(u64 address);
static pointer_ptr* vm_alloc_internal(u64* address, struct vm_data** target);
static void virtual_enumerator_init(void);
static void virtual_enumerator_destroy(void);
static void virtual_enumerator_init_internal(struct vm* ptr);
static void virtual_enumerator_destroy_internal(void);
static void** virtual_enumerator_next_internal(void);
#ifdef USE_MEMORY_DEBUG_INFO
static pointer_ptr virtual_enumerator_pointer_next_internal(void);
#endif

static struct vm_data* vm_init_internal(u64 size, u64 offset) {
    struct vm_data* vm_data_ptr = sys_memory->alloc(VM_DATA_SIZE);
    vm_data_ptr->bp = sys_memory->alloc(ALLOC_SIZE(size));
    vm_data_ptr->sp = vm_data_ptr->bp;
    vm_data_ptr->offset = offset;
    vm_data_ptr->size = size;
    return vm_data_ptr;
}

static pointer_ptr* vm_read_internal(u64 address) {
    pointer_ptr* ptr = 0;
    const struct vm_data* vm_data_ptr = vm->head;
    do {
        if (address > vm_data_ptr->offset && address <= vm_data_ptr->offset + vm_data_ptr->size) {
            ptr = vm_data_ptr->bp + address - vm_data_ptr->offset - 1;
            break;
        }
        vm_data_ptr = vm_data_ptr->next;
    } while (vm_data_ptr != 0);
    return ptr;
}

static pointer_ptr* vm_alloc_internal(u64* address, struct vm_data** target) {
    struct vm_data* vm_data_ptr = vm->tail;
    pointer_ptr* ptr = 0;
#ifndef USE_GC
    struct vm_pointer* list_ptr = sys_list->pop(cache);
    if (list_ptr != 0) {
        ptr = list_ptr->ptr;
        vm_data_ptr = list_ptr->vm;
        sys_memory->free(list_ptr, VM_POINTER_SIZE);
    }
    if (ptr == 0) {
#endif
        if ((u64)(vm_data_ptr->sp - vm_data_ptr->bp) == vm_data_ptr->size) {
            struct vm_data* prev = vm_data_ptr;
            vm_data_ptr = vm_init_internal(vm_data_ptr->size, vm_data_ptr->offset + vm_data_ptr->size);
            vm_data_ptr->prev = prev;
            prev->next = vm_data_ptr;
            vm->tail = vm_data_ptr;
        }
        ptr = vm_data_ptr->sp;
        ++vm_data_ptr->sp;
#ifndef USE_GC
    }
#endif
    *target = vm_data_ptr;
    *address = (u64)(ptr - vm_data_ptr->bp) + vm_data_ptr->offset + 1;
    return ptr;
}

static void virtual_enumerator_init(void) {
    virtual_enumerator_init_internal(vm);
}

static void virtual_enumerator_destroy(void) {
    virtual_enumerator_destroy_internal();
}

static void virtual_enumerator_init_internal(struct vm* ptr) {
    struct vm_data* vm_data_ptr = ptr->head;
    state->vm = vm_data_ptr;
    state->ptr = vm_data_ptr->bp;
}

static void virtual_enumerator_destroy_internal(void) {
    state->vm = 0;
    state->ptr = 0;
}

/* implementation */

static void vm_init(struct vm** ptr, u64 size) {
#ifndef USE_GC
    cache = sys_memory->alloc(PTR_SIZE);
    sys_list->init(cache);
#endif
    struct vm_data* vm_data_ptr = vm_init_internal(size == 0 ? DEFAULT_SIZE : size, 0);
    vm->head = vm_data_ptr;
    vm->tail = vm_data_ptr;
    struct vm* vm_ptr = sys_memory->alloc(sizeof(struct vm));
    vm_ptr->head = vm_data_ptr;
    vm_ptr->tail = vm_data_ptr;
    *ptr = vm_ptr;
}

static void vm_destroy(struct vm** ptr) {
    if (ptr == 0 || *ptr == 0) {
        return;
    }
    struct vm* current = *ptr;
    vm->head = current->head;
    vm->tail = current->tail;
    sys_memory->free(current, sizeof(struct vm));
    *ptr = 0;
#ifndef USE_GC
    struct vm_pointer* vm_pointer_ptr = 0;
    while ((vm_pointer_ptr = sys_list->pop(cache)) != 0) {
        sys_memory->free(vm_pointer_ptr, VM_POINTER_SIZE);
    }
    sys_list->destroy(cache);
    sys_memory->free(cache, PTR_SIZE);
#ifdef USE_MEMORY_CLEANUP
    cache = 0;
#endif
#endif
    struct vm_data* vm_data_ptr = vm->head;
    while (vm_data_ptr != 0) {
        struct vm_data* next = vm_data_ptr->next;
        sys_memory->free(vm_data_ptr->bp, ALLOC_SIZE(vm_data_ptr->size));
        sys_memory->free(vm_data_ptr, VM_DATA_SIZE);
        vm_data_ptr = next;
    }
    vm->head = 0;
    vm->tail = 0;
}

#ifdef USE_MEMORY_DEBUG_INFO
static void vm_dump(void) {
    virtual_enumerator_init_internal(vm);
    pointer_ptr ptr = 0;
    while ((ptr = virtual_enumerator_pointer_next_internal()) != 0) {
        pointer->dump(ptr);
    }
    virtual_enumerator_destroy_internal();
}

static void vm_dump_ref(void) {
    virtual_enumerator_init_internal(vm);
    void** ptr = 0;
    while ((ptr = virtual_enumerator_next_internal()) != 0) {
        pointer->dump_ref(ptr);
    }
    virtual_enumerator_destroy_internal();
}
#endif

static void vm_free(const pointer_ptr ptr) {
    if (!ptr) {
        return;
    }
    const struct vm_data* vm_data_ptr = pointer->vm(ptr);
    pointer_ptr* data = vm_data_ptr->bp - vm_data_ptr->offset - 1 + pointer->address(ptr);
    if (data != 0) {
#ifndef USE_GC
        struct vm_pointer* vm_pointer_ptr = sys_memory->alloc(VM_POINTER_SIZE);
        vm_pointer_ptr->ptr = data;
        vm_pointer_ptr->vm = pointer->vm(ptr);
        sys_list->push(cache, vm_pointer_ptr);
#endif
#ifdef USE_MEMORY_DEBUG_INFO
        printf("  >-: %016llx ! %016llx > %016llx\n", pointer->address(ptr), (u64)ptr, (u64)data);
#endif
        *data = 0;
    }
}

static pointer_ptr vm_read_type(u64 address, u64 id) {
    if (!address) {
        return 0;
    }
    pointer_ptr* data = vm_read_internal(address);
    pointer_ptr ptr = 0;
    if (data == 0) {
        return 0;
    }
    ptr = *data;
    if (ptr == 0) {
        return 0;
    }
    if (!pointer->read_type(ptr, id)) {
        return 0;
    }
#ifdef USE_MEMORY_DEBUG_INFO
    printf("  ?v: %016llx ! %016llx > %016llx\n", address, (u64)ptr, (u64)data);
#endif
    return ptr;
}

static pointer_ptr vm_read(u64 address) {
    if (!address) {
        return 0;
    }
    pointer_ptr* data = vm_read_internal(address);
    pointer_ptr ptr = 0;
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
    return ptr;
}

static u64 vm_alloc(pointer_ptr ptr) {
    if (!ptr) {
        return 0;
    }
    u64 address = 0;
    struct vm_data* target = 0;
    pointer_ptr* data = vm_alloc_internal(&address, &target);
    *data = ptr;
    pointer->write(ptr, target, address);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("  >+: %016llx ! %016llx > %016llx\n", address, (u64)ptr, (u64)data);
#endif
    return address;
}

#ifdef USE_MEMORY_DEBUG_INFO
static pointer_ptr virtual_enumerator_pointer_next_internal(void) {
    pointer_ptr data = 0;
    struct vm_data* vm_data_ptr = state->vm;
    while (data == 0) {
        if (state->ptr == vm_data_ptr->sp) {
            if (vm_data_ptr->next == 0) {
                break;
            }
            vm_data_ptr = vm_data_ptr->next;
            state->vm = vm_data_ptr;
            state->ptr = vm_data_ptr->bp;
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
            const pointer_ptr data_ptr = *ptr;
            if (data_ptr != 0) {
                address = pointer->address(data_ptr);
                break;
            }
        }
    }
    return address;
}

static void** virtual_enumerator_next_internal(void) {
    void** data = 0;
    struct vm_data* vm_data_ptr = state->vm;
    while (data == 0) {
        if (state->ptr == vm_data_ptr->sp) {
            if (vm_data_ptr->next == 0) {
                break;
            }
            vm_data_ptr = vm_data_ptr->next;
            state->vm = vm_data_ptr;
            state->ptr = vm_data_ptr->bp;
        }
        data = (void**)state->ptr++;
    }
    return data;
}

/* public */
const virtual_methods PRIVATE_API(virtual_methods_definitions) = {
    .init = vm_init,
    .destroy = vm_destroy,
    .alloc = vm_alloc,
    .free = vm_free,
    .read = vm_read,
    .read_type = vm_read_type,
    .enumerator_init = virtual_enumerator_init,
    .enumerator_destroy = virtual_enumerator_destroy,
    .enumerator_next = virtual_enumerator_next,
#ifdef USE_MEMORY_DEBUG_INFO
    .dump = vm_dump,
    .dump_ref = vm_dump_ref
#endif
};
