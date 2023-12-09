/*
 *
 * Russian's IP Protection License
 *
 * Copyright (c) 2023 Artur Mustafin
 *
 * Permission is hereby granted, free of charge, to any person with citizenship
 * and location in Russia including Crimea and all occupations obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * For the rest of the world it is an order to pay royalties by agreement to the
 * author of the code base for ability to use any part of the project for any
 * purpouse including but not limited to the creative ideas or technologies are
 * being used in this owned intellectual property.
 *
 * It is strictly prohibited to use this code base or any part of it for any purpouse
 * including prohibiting or restricive purpouses agains Russians for any EU citizens
 * or other person with USA citizenship, origin or background including work permit
 * or locations from selected territories or any territory or any other country except
 * Russia considered as breaking basic human rights, freedom of speesh or involved in
 * acts of terrorism in a territory owned, occupied or managed by another country.
 *
 */
#include "common/memory.h"
#include "list-micro/data.h"

#include "pointer/v1/pointer_v1.h"
#include "pointer/v1/virtual_v1.h"

/* macros */
#define DEFAULT_SIZE 0x8 /* 8 */
#define PTR_SIZE sizeof(void*) /* size of a pointer */
#define VM_DATA_SIZE sizeof(struct vm_data)
#define ALLOC_SIZE(size) (size * PTR_SIZE)

#ifndef USE_GC
#define VM_POINTER_SIZE sizeof(struct vm_pointer)
/* pointer definition */
struct vm_pointer {
    struct pointer** ptr;
    struct vm_data* vm;
};
#endif

/* private */
struct vm {
    struct vm_data* head;
    struct vm_data* tail;
};

#ifndef USE_GC
extern const struct list list_micro_definition;
static const struct list* list = &list_micro_definition;
static struct list_data** cache;
#endif

static struct vm vm_definition;

static struct vm* vm = &vm_definition;

struct vm_data {
    struct pointer** sp; /* stack pointer */
    struct pointer** bp; /* base pointer */
    struct vm_data* prev;
    struct vm_data* next;
    u64 offset;
    u64 size;
};

struct vm_state {
    struct vm_data* vm;
    struct pointer** ptr;
};

struct vm_data_enumerator {
    struct pointer* (*next)(void);
};

static struct vm_state vm_state;
static struct vm_state* state = &vm_state;

/* api */

static void vm_init(struct vm**, u64 size);
static void vm_destroy(struct vm**);
#if defined(VM_MEMORY_DEBUG_INFO)
static void vm_dump(void);
static void vm_dump_ref(void);
#endif
static u64 vm_alloc(struct pointer* ptr);
static void vm_free(struct pointer* ptr);
static struct pointer* vm_read(u64 address);
static struct pointer* vm_read_type(u64 address, u64 id);
static u64 virtual_enumerator_next(void);

/* internal */

static struct vm_data* vm_init_internal(u64 size, u64 offset);
static struct pointer** vm_read_internal(u64 address);
static struct pointer** vm_alloc_internal(u64* address, struct vm_data** target);
static void virtual_enumerator_init(void);
static void virtual_enumerator_destroy(void);
static void virtual_enumerator_init_internal(struct vm* ptr);
static void virtual_enumerator_destroy_internal(void);
static void** virtual_enumerator_next_internal(void);
#if defined(VM_ALLOC_DEBUG_INFO)
static struct pointer* virtual_enumerator_pointer_next_internal(void);
#endif

static struct vm_data* vm_init_internal(u64 size, u64 offset) {
    struct vm_data* vm_data_ptr = memory->alloc(VM_DATA_SIZE);
    vm_data_ptr->bp = memory->alloc(ALLOC_SIZE(size));
    vm_data_ptr->sp = vm_data_ptr->bp;
    vm_data_ptr->offset = offset;
    vm_data_ptr->size = size;
    return vm_data_ptr;
}

static struct pointer** vm_read_internal(u64 address) {
    struct pointer** ptr = 0;
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

static struct pointer** vm_alloc_internal(u64* address, struct vm_data** target) {
    struct vm_data* vm_data_ptr = vm->tail;
    struct pointer** ptr = 0;
#ifndef USE_GC
    struct vm_pointer* list_ptr = list->pop(cache);
    if (list_ptr != 0) {
        ptr = list_ptr->ptr;
        vm_data_ptr = list_ptr->vm;
        memory->free(list_ptr, VM_POINTER_SIZE);
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
    virtual_enumerator_init_internal(vm);
}

static void virtual_enumerator_init_internal(struct vm* ptr) {
    struct vm_data* vm_data_ptr = ptr->head;
    state->vm = vm_data_ptr;
    state->ptr = vm_data_ptr->bp;
}

#if defined(VM_MEMORY_DEBUG_INFO)
static void virtual_enumerator_destroy_internal(void) {
    state->vm = 0;
    state->ptr = 0;
}
#endif

/* implementation */

static void vm_init(struct vm** ptr, u64 size) {
#ifndef USE_GC
    cache = memory->alloc(PTR_SIZE);
    list->init(cache);
#endif
    struct vm_data* vm_data_ptr = vm_init_internal(size == 0 ? DEFAULT_SIZE : size, 0);
    vm->head = vm_data_ptr;
    vm->tail = vm_data_ptr;
    struct vm* vm_ptr = memory->alloc(sizeof(struct vm));
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
    memory->free(current, sizeof(struct vm));
    *ptr = 0;
#ifndef USE_GC
    struct vm_pointer* vm_pointer_ptr = 0;
    while ((vm_pointer_ptr = list->pop(cache)) != 0) {
        memory->free(vm_pointer_ptr, VM_POINTER_SIZE);
    }
    list->destroy(cache);
    memory->free(cache, PTR_SIZE);
#ifdef USE_MEMORY_CLEANUP
    cache = 0;
#endif
#endif
    struct vm_data* vm_data_ptr = vm->head;
    while (vm_data_ptr != 0) {
        struct vm_data* next = vm_data_ptr->next;
        memory->free(vm_data_ptr->bp, ALLOC_SIZE(vm_data_ptr->size));
        memory->free(vm_data_ptr, VM_DATA_SIZE);
        vm_data_ptr = next;
    }
    vm->head = 0;
    vm->tail = 0;
}

#if defined(VM_MEMORY_DEBUG_INFO)
static void vm_dump(void) {
    virtual_enumerator_init_internal(vm);
    struct pointer* ptr = 0;
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

static void vm_free(struct pointer* ptr) {
    if (!ptr) {
        return;
    }
    const struct vm_data* vm_data_ptr = pointer->vm(ptr);
    struct pointer** data = vm_data_ptr->bp - vm_data_ptr->offset - 1 + pointer->address(ptr);
    if (data != 0) {
#ifndef USE_GC
        struct vm_pointer* vm_pointer_ptr = memory->alloc(VM_POINTER_SIZE);
        vm_pointer_ptr->ptr = data;
        vm_pointer_ptr->vm = pointer->vm(ptr);
        list->push(cache, vm_pointer_ptr);
#endif
#ifdef USE_VM_DEBUG_INFO
#if defined(VM_ALLOC_DEBUG_INFO)
        printf("  >-: %016llx ! %016llx > %016llx\n", pointer->address(ptr), (u64)ptr, (u64)data);
#endif
#endif
        *data = 0;
    }
}

static struct pointer* vm_read_type(u64 address, u64 id) {
    if (!address) {
        return 0;
    }
    struct pointer** data = vm_read_internal(address);
    struct pointer* ptr = 0;
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
#ifdef USE_VM_DEBUG_INFO
#if defined(VM_ACCESS_DEBUG_INFO)
    printf("  <v: %016llx ! %016llx > %016llx\n", address, (u64)ptr, (u64)data);
#endif
#endif
    return ptr;
}

static struct pointer* vm_read(u64 address) {
    if (!address) {
        return 0;
    }
    struct pointer** data = vm_read_internal(address);
    struct pointer* ptr = 0;
    if (data == 0) {
        return 0;
    }
    ptr = *data;
    if (ptr == 0) {
        return 0;
    }
#ifdef USE_VM_DEBUG_INFO
#if defined(VM_ACCESS_DEBUG_INFO)
    printf("  <v: %016llx ! %016llx > %016llx\n", address, (u64)ptr, (u64)data);
#endif
#endif
    return ptr;
}

static u64 vm_alloc(struct pointer* ptr) {
    if (!ptr) {
        return 0;
    }
    u64 address = 0;
    struct vm_data* target = 0;
    struct pointer** data = vm_alloc_internal(&address, &target);
    *data = ptr;
    pointer->write(ptr, target, address);
#ifdef USE_VM_DEBUG_INFO
#if defined(VM_ALLOC_DEBUG_INFO)
    printf("  >+: %016llx ! %016llx > %016llx\n", address, (u64)ptr, (u64)data);
#endif
#endif
    return address;
}

#if defined(VM_MEMORY_DEBUG_INFO)
static struct pointer* virtual_enumerator_pointer_next_internal(void) {
    struct pointer* data = 0;
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
            const struct pointer* data_ptr = *ptr;
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
const struct vm_default_options vm_default_options_definition = {
    .size = DEFAULT_SIZE
};

const struct virtual_methods virtual_methods_definition = {
    .init = vm_init,
    .destroy = vm_destroy,
    .alloc = vm_alloc,
    .free = vm_free,
    .read = vm_read,
    .read_type = vm_read_type,
    .enumerator_init = virtual_enumerator_init,
    .enumerator_destroy = virtual_enumerator_destroy,
    .enumerator_next = virtual_enumerator_next,
#if defined(VM_MEMORY_DEBUG_INFO)
    .dump = vm_dump,
    .dump_ref = vm_dump_ref
#endif
};
