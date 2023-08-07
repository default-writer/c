/*
 *
 * MIT License
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
#include "vm/v2/vm_v2.h"
#include "common/alloc.h"
#include "list-micro/data.h"
#include "pointer/v2/pointer_v2.h"

/* macros */
#define DEFAULT_SIZE 0x0 /* 0 */
#define PTR_SIZE sizeof(void*) /* size of a pointer */
#define VM_DATA_SIZE sizeof(struct vm_data)
#define ALLOC_SIZE(size) (size * PTR_SIZE)

/* definition */

/* definition */
extern const struct memory memory_definition;

/* definition */
static const struct memory* memory = &memory_definition;

/* implementation */

/* private */

extern const struct pointer_methods pointer_methods_definition;
extern const struct list_methods list_methods_definition;
extern const struct file_methods file_methods_definition;
extern const struct debug_methods debug_methods_definition;

static const struct pointer_methods* pointer = &pointer_methods_definition;
static const struct list_methods* list = &list_methods_definition;
static const struct file_methods* file = &file_methods_definition;
static const struct debug_methods* debug = &debug_methods_definition;

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

static void vm_init(struct vm_data** current, u64 size);
static void vm_destroy(struct vm_data** current);
#ifdef USE_MEMORY_DEBUG_INFO
static void vm_dump(struct vm_data* vm_ptr);
static void vm_dump_ref(struct vm_data* vm_ptr);
#endif
static struct pointer* vm_free(struct vm_data** current, u64 address);
static struct pointer* vm_read(struct vm_data** current, u64 address);
static u64 vm_write(struct vm_data** current, struct pointer* data);
#ifdef USE_MEMORY_DEBUG_INFO
static struct pointer* vm_data_enumerator_next(void);
static void** vm_data_enumerator_next_ref(void);
#endif

/* internal */

static struct vm_data* vm_init_internal(u64 size, u64 offset);
static void** vm_read_internal(struct vm_data** current, u64 address);
static u64 vm_alloc_internal(struct vm_data** current);
static void* to_real_address_internal(struct vm_data* vm, u64 address);
static u64 to_virtual_address_internal(const struct vm_data* vm, struct pointer** ptr);
#ifdef USE_MEMORY_DEBUG_INFO
static void vm_enumerator_init_internal(struct vm_data* vm);
static void vm_enumerator_destroy_internal(void);
#endif

static struct vm_data* vm_init_internal(u64 size, u64 offset) {
    struct vm_data* vm = memory->alloc(VM_DATA_SIZE);
    vm->bp = memory->alloc(ALLOC_SIZE(size));
    vm->sp = vm->bp;
    vm->offset = offset;
    vm->size = size;
    return vm;
}

static void** vm_read_internal(struct vm_data** current, u64 address) {
    void** ptr = 0;
    if (address != 0) {
        struct vm_data* vm = *current;
        while (address <= vm->offset) {
            vm = vm->prev;
        }
        if (vm != 0) {
            if (address > vm->offset + vm->size) {
                return 0;
            }
            ptr = to_real_address_internal(vm, address);
        }
    }
    return ptr;
}

static u64 vm_alloc_internal(struct vm_data** current) {
    struct vm_data* vm = *current;
    u64 address = 0;
    struct pointer** ptr;
    ptr = vm->sp;
    address = to_virtual_address_internal(vm, ptr);
    while (address > vm->offset + vm->size) {
        if (vm->next == 0) {
            vm->next = vm_init_internal(vm->size, vm->offset + vm->size);
            vm->next->prev = vm;
        }
        vm = vm->next;
    }
    ptr = vm->sp;
    ++vm->sp;
    address = to_virtual_address_internal(vm, ptr);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("  >+: %016llx > %016llx\n", (u64)ptr, address);
#endif
    *current = vm;
    return address;
}

static u64 to_virtual_address_internal(const struct vm_data* vm, struct pointer** ptr) {
    return (u64)(ptr - vm->bp) + vm->offset + 1;
}

static void* to_real_address_internal(struct vm_data* vm, u64 address) {
    return vm->bp + address - vm->offset - 1;
}

#ifdef USE_MEMORY_DEBUG_INFO
static void vm_enumerator_init_internal(struct vm_data* vm) {
    state->vm = vm;
    state->ptr = vm->bp;
}

static void vm_enumerator_destroy_internal(void) {
    state->vm = 0;
    state->ptr = 0;
}
#endif

/* implementation */

static void vm_init(struct vm_data** current, u64 size) {
    *current = vm_init_internal(size, 0);
}

static void vm_destroy(struct vm_data** current) {
    struct vm_data* vm = *current;
    while (vm->prev != 0) {
        vm = vm->prev;
    }
    while (vm != 0) {
        struct vm_data* next = vm->next;
        memory->free(vm->bp, ALLOC_SIZE(vm->size));
        memory->free(vm, VM_DATA_SIZE);
        vm = next;
    }
}

#ifdef USE_MEMORY_DEBUG_INFO
static void vm_dump(struct vm_data* vm_ptr) {
    if (vm_ptr == 0) {
        return;
    }
    while (vm_ptr->prev != 0) {
        vm_ptr = vm_ptr->prev;
    }
    vm_enumerator_init_internal(vm_ptr);
    struct pointer* ptr = 0;
    while ((ptr = vm_data_enumerator_next()) != 0) {
        debug->dump(ptr);
    }
    vm_enumerator_destroy_internal();
}

static void vm_dump_ref(struct vm_data* vm_ptr) {
    if (vm_ptr == 0) {
        return;
    }
    while (vm_ptr->prev != 0) {
        vm_ptr = vm_ptr->prev;
    }
    vm_enumerator_init_internal(vm_ptr);
    void** ptr = 0;
    while ((ptr = vm_data_enumerator_next_ref()) != 0) {
        debug->dump_ref(ptr);
    }
    vm_enumerator_destroy_internal();
}
#endif

static struct pointer* vm_free(struct vm_data** current, u64 address) {
    struct pointer* data = 0;
    if (address != 0) {
        void** ptr = vm_read_internal(current, address);
        if (ptr != 0) {
            data = *ptr;
#ifdef VM_DEBUG_INFO
            printf("  >-: %016llx ! %016llx > %016llx\n", address, (u64)data, (u64)ptr);
#endif
            *ptr = 0;
        }
    }
    return data;
}

static struct pointer* vm_read(struct vm_data** current, u64 address) {
    struct pointer* data = 0;
    if (address != 0) {
        void** ptr = vm_read_internal(current, address);
        if (ptr != 0) {
            data = *ptr;
#ifdef VM_DEBUG_INFO
            printf("  <v: %016llx ! %016llx > %016llx\n", address, (u64)data, (u64)ptr);
#endif
        }
    }
    return data;
}

static u64 vm_write(struct vm_data** current, struct pointer* data) {
    u64 address = vm_alloc_internal(current);
    if (address != 0) {
        struct vm_data* vm = *current;
        void** ptr = to_real_address_internal(vm, address);
        if (ptr != 0) {
            *ptr = data;
#ifdef VM_DEBUG_INFO
            printf("  >v: %016llx ! %016llx > %016llx\n", address, (u64)data, (u64)ptr);
#endif
        }
    }
    return address;
}

#ifdef USE_MEMORY_DEBUG_INFO
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

static void** vm_data_enumerator_next_ref(void) {
    void** data = 0;
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
        data = (void**)state->ptr++;
    }
    return data;
}
#endif

/* public */

const struct vm_methods vm_methods_definition = {
    .init = vm_init,
    .destroy = vm_destroy,
    .free = vm_free,
    .read = vm_read,
    .write = vm_write,
#ifdef USE_MEMORY_DEBUG_INFO
    .dump = vm_dump,
    .dump_ref = vm_dump_ref
#endif
};
