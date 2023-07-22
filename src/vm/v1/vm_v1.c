/*
 *
 *  MIT License
 *
 * Copyright (c) 2023 Artur Mustafin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
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
 */
#include "vm/v1/vm_v1.h"
#include "common/alloc.h"
#include "list-micro/data.h"
#include "pointer/v1/pointer_v1.h"

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

#ifndef USE_GC
extern const struct list list_micro_definition;
static const struct list* list = &list_micro_definition;
static struct list_data** cache;
#endif
static struct vm_data* vm_data;
static struct vm_data* head = 0;
static struct vm_data* tail = 0;

struct vm {
    struct vm_data* head;
    struct vm_data* tail;
};

extern const struct pointer_methods pointer_methods_definition;
static const struct pointer_methods* pointer = &pointer_methods_definition;

struct vm_data {
    struct pointer** sp; /* stack pointer */
    struct pointer** bp; /* base pointer */
    struct vm_data* prev;
    struct vm_data* next;
    u64 offset;
    u64 size;
};

#ifdef USE_MEMORY_DEBUG_INFO
struct vm_state {
    struct vm_data* vm;
    struct pointer** ptr;
};

struct vm_data_enumerator {
    struct pointer* (*next)(void);
};

static struct vm_state vm_state;
static struct vm_state* state = &vm_state;
#endif

/* api */

static struct vm* vm_init(u64 size);
static void vm_destroy(struct vm**);
#ifdef VM_DEBUG_INFO
static void vm_dump(void);
static void vm_dump_ref(void);
#endif
static u64 vm_alloc(struct pointer* ptr);
static void vm_free(struct pointer* ptr);
static struct pointer* vm_read(u64 address, u64 typeid);
#ifdef VM_DEBUG_INFO
static struct pointer* vm_data_enumerator_next(void);
static void* vm_data_enumerator_next_ref(void);
#endif

/* internal */

static struct vm_data* vm_init_internal(u64 size, u64 offset);
static struct pointer** vm_read_internal(u64 address);
static struct pointer** vm_alloc_internal(u64* address, struct vm_data** target);
#ifdef VM_DEBUG_INFO
static void vm_enumerator_init_internal(void);
static void vm_enumerator_destroy_internal(void);
#endif

static struct vm_data* vm_init_internal(u64 size, u64 offset) {
    struct vm_data* vm = global_alloc(VM_DATA_SIZE);
    vm->bp = global_alloc(ALLOC_SIZE(size));
    vm->sp = vm->bp;
    vm->offset = offset;
    vm->size = size;
    return vm;
}

static struct pointer** vm_read_internal(u64 address) {
    struct pointer** ptr = 0;
    const struct vm_data* vm = head;
    do {
        if (address > vm->offset && address <= vm->offset + vm->size) {
            ptr = vm->bp + address - vm->offset - 1;
            break;
        }
        vm = vm->next;
    } while (vm != 0);
    return ptr;
}

static struct pointer** vm_alloc_internal(u64* address, struct vm_data** target) {
    struct vm_data* vm = tail;
    struct pointer** ptr = 0;
#ifndef USE_GC
    struct vm_pointer* vm_ptr = list->pop(cache);
    if (vm_ptr != 0) {
        ptr = vm_ptr->ptr;
        vm = vm_ptr->vm;
        global_free(vm_ptr, VM_POINTER_SIZE);
    }
    if (ptr == 0) {
#endif
        if ((u64)(vm->sp - vm->bp) == vm->size) {
            struct vm_data* prev = vm;
            vm = vm_init_internal(vm->size, vm->offset + vm->size);
            vm->prev = prev;
            prev->next = vm;
            tail = vm;
        }
        ptr = vm->sp;
        ++vm->sp;
#ifndef USE_GC
    }
#endif
    *target = vm;
    *address = (u64)(ptr - vm->bp) + vm->offset + 1;
    return ptr;
}

#ifdef VM_DEBUG_INFO
static void vm_enumerator_init_internal(void) {
    struct vm_data* vm = head;
    state->vm = vm;
    state->ptr = vm->bp;
}

static void vm_enumerator_destroy_internal(void) {
    state->vm = 0;
    state->ptr = 0;
}
#endif

/* implementation */

static struct vm* vm_init(u64 size) {
#ifndef USE_GC
    cache = global_alloc(PTR_SIZE);
    list->init(cache);
#endif
    vm_data = vm_init_internal(size == 0 ? DEFAULT_SIZE : size, 0);
    head = vm_data;
    tail = vm_data;
    struct vm* vm = global_alloc(sizeof(struct vm));
    vm->head = vm_data;
    vm->tail = vm_data;
    return vm;
}

static void vm_destroy(struct vm** ptr) {
    if (ptr != 0 && *ptr != 0) {
        struct vm* current = *ptr;
        head = current->head;
        tail = current->tail;
        global_free(current, sizeof(struct vm));
        *ptr = 0;
    }
#ifndef USE_GC
    struct vm_pointer* vm_ptr = 0;
    while ((vm_ptr = list->pop(cache)) != 0) {
        global_free(vm_ptr, VM_POINTER_SIZE);
    }
    list->destroy(cache);
    global_free(cache, PTR_SIZE);
#ifdef USE_MEMORY_CLEANUP
    cache = 0;
#endif
#endif
    struct vm_data* vm = head;
    while (vm != 0) {
        struct vm_data* next = vm->next;
        global_free(vm->bp, ALLOC_SIZE(vm->size));
        global_free(vm, VM_DATA_SIZE);
        vm = next;
    }
    head = 0;
    tail = 0;
}

#ifdef VM_DEBUG_INFO
static void vm_dump(void) {
    vm_enumerator_init_internal();
    struct pointer* ptr = 0;
    while ((ptr = vm_data_enumerator_next()) != 0) {
        pointer->dump(ptr);
    }
    vm_enumerator_destroy_internal();
}

static void vm_dump_ref(void) {
    vm_enumerator_init_internal();
    void** ptr = 0;
    while ((ptr = vm_data_enumerator_next_ref()) != 0) {
        pointer->dump_ref(ptr);
    }
    vm_enumerator_destroy_internal();
}
#endif

static void vm_free(struct pointer* ptr) {
    if (!ptr) {
        return;
    }
    struct pointer** data = vm_read_internal(ptr->address);
    if (data != 0) {
        ptr = *data;
#ifndef USE_GC
        struct vm_pointer* vm_ptr = global_alloc(VM_POINTER_SIZE);
        vm_ptr->ptr = data;
        vm_ptr->vm = ptr->vm;
        list->push(cache, vm_ptr);
#endif
#ifdef VM_DEBUG_INFO
        printf("  >-: %016llx ! %016llx > %016llx\n", address, (u64)ptr, (u64)data);
#endif
        *data = 0;
    }
}

static struct pointer* vm_read(u64 address, u64 typeid) {
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
    if (ptr->typeid != typeid) {
        return 0;
    }
#ifdef VM_DEBUG_INFO
    printf("  <v: %016llx ! %016llx > %016llx\n", address, (u64)ptr, (u64)data);
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
    ptr->vm = target;
    ptr->address = address;
#ifdef VM_DEBUG_INFO
    printf("  >v: %016llx ! %016llx > %016llx\n", address, (u64)ptr, (u64)data);
#endif
    return address;
}

#ifdef VM_DEBUG_INFO
static struct pointer* vm_data_enumerator_next(void) {
    struct pointer* data = 0;
    struct vm_data* vm = state->vm;
    while (data == 0) {
        if (state->ptr == vm->sp) {
            if (vm->next == 0) {
                break;
            }
            vm = vm->next;
            state->vm = vm;
            state->ptr = vm->bp;
        }
        data = *state->ptr++;
    }
    return data;
}

static void* vm_data_enumerator_next_ref(void) {
    void* data = 0;
    struct vm_data* vm = state->vm;
    while (data == 0) {
        if (state->ptr == vm->sp) {
            if (vm->next == 0) {
                break;
            }
            vm = vm->next;
            state->vm = vm;
            state->ptr = vm->bp;
        }
        data = state->ptr++;
    }
    return data;
}
#endif

/* public */

const struct vm_methods vm_methods_definition = {
    .init = vm_init,
    .destroy = vm_destroy,
    .alloc = vm_alloc,
    .free = vm_free,
    .read = vm_read,
#ifdef VM_DEBUG_INFO
    .dump = vm_dump,
    .dump_ref = vm_dump_ref
#endif
};
