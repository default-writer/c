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
#include "vm/v2/vm_v2.h"
#include "common/memory.h"
#include "list-micro/data.h"
#include "pointer/v2/pointer_v2.h"

/* macros */
#define DEFAULT_SIZE 0x0 /* 0 */
#define PTR_SIZE sizeof(void*) /* size of a pointer */
#define VM_DATA_SIZE sizeof(struct vm_data)
#define ALLOC_SIZE(size) (size * PTR_SIZE)

/* definition */

/* private */

/* definition */
#if defined(VM_MEMORY_DEBUG_INFO)
extern const struct debug_methods debug_methods_definition;
#endif

/* definition */
#if defined(VM_MEMORY_DEBUG_INFO)
static const struct debug_methods* debug = &debug_methods_definition;
#endif

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
#if defined(VM_MEMORY_DEBUG_INFO)
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
static void* to_real_address_internal(struct vm_data* vm_data_ptr, u64 address);
static u64 to_virtual_address_internal(const struct vm_data* vm_data_ptr, struct pointer** ptr);
#ifdef USE_MEMORY_DEBUG_INFO
static void vm_enumerator_init_internal(struct vm_data* vm_data_ptr);
static void vm_enumerator_destroy_internal(void);
#endif

static struct vm_data* vm_init_internal(u64 size, u64 offset) {
    struct vm_data* vm_data_ptr = memory->alloc(VM_DATA_SIZE);
    vm_data_ptr->bp = memory->alloc(ALLOC_SIZE(size));
    vm_data_ptr->sp = vm_data_ptr->bp;
    vm_data_ptr->offset = offset;
    vm_data_ptr->size = size;
    return vm_data_ptr;
}

static void** vm_read_internal(struct vm_data** current, u64 address) {
    void** ptr = 0;
    if (address != 0) {
        struct vm_data* vm_data_ptr = *current;
        while (address <= vm_data_ptr->offset) {
            vm_data_ptr = vm_data_ptr->prev;
        }
        if (vm_data_ptr != 0) {
            if (address > vm_data_ptr->offset + vm_data_ptr->size) {
                return 0;
            }
            ptr = to_real_address_internal(vm_data_ptr, address);
        }
    }
    return ptr;
}

static u64 vm_alloc_internal(struct vm_data** current) {
    struct vm_data* vm_data_ptr = *current;
    u64 address = 0;
    struct pointer** ptr;
    ptr = vm_data_ptr->sp;
    address = to_virtual_address_internal(vm_data_ptr, ptr);
    while (address > vm_data_ptr->offset + vm_data_ptr->size) {
        if (vm_data_ptr->next == 0) {
            vm_data_ptr->next = vm_init_internal(vm_data_ptr->size, vm_data_ptr->offset + vm_data_ptr->size);
            vm_data_ptr->next->prev = vm_data_ptr;
        }
        vm_data_ptr = vm_data_ptr->next;
    }
    ptr = vm_data_ptr->sp;
    ++vm_data_ptr->sp;
    address = to_virtual_address_internal(vm_data_ptr, ptr);
#ifdef USE_MEMORY_DEBUG_INFO
#if defined(VM_ALLOC_DEBUG_INFO)
    printf("  >+: %016llx > %016llx\n", (u64)ptr, address);
#endif
#endif
    *current = vm_data_ptr;
    return address;
}

static u64 to_virtual_address_internal(const struct vm_data* vm_data_ptr, struct pointer** ptr) {
    return (u64)(ptr - vm_data_ptr->bp) + vm_data_ptr->offset + 1;
}

static void* to_real_address_internal(struct vm_data* vm_data_ptr, u64 address) {
    return vm_data_ptr->bp + address - vm_data_ptr->offset - 1;
}

#if defined(VM_MEMORY_DEBUG_INFO)
static void vm_enumerator_init_internal(struct vm_data* vm_data_ptr) {
    state->vm = vm_data_ptr;
    state->ptr = vm_data_ptr->bp;
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
    struct vm_data* vm_data_ptr = *current;
    while (vm_data_ptr->prev != 0) {
        vm_data_ptr = vm_data_ptr->prev;
    }
    while (vm_data_ptr != 0) {
        struct vm_data* next = vm_data_ptr->next;
        memory->free(vm_data_ptr->bp, ALLOC_SIZE(vm_data_ptr->size));
        memory->free(vm_data_ptr, VM_DATA_SIZE);
        vm_data_ptr = next;
    }
}

#if defined(VM_MEMORY_DEBUG_INFO)
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
#ifdef USE_VM_DEBUG_INFO
#if defined(VM_ALLOC_DEBUG_INFO)
            printf("  >-: %016llx ! %016llx > %016llx\n", address, (u64)data, (u64)ptr);
#endif
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
#ifdef USE_VM_DEBUG_INFO
#if defined(VM_ACCESS_DEBUG_INFO)
            printf("  <v: %016llx ! %016llx > %016llx\n", address, (u64)data, (u64)ptr);
#endif
#endif
        }
    }
    return data;
}

static u64 vm_write(struct vm_data** current, struct pointer* data) {
    u64 address = vm_alloc_internal(current);
    if (address != 0) {
        struct vm_data* vm_data_ptr = *current;
        void** ptr = to_real_address_internal(vm_data_ptr, address);
        if (ptr != 0) {
            *ptr = data;
#ifdef USE_VM_DEBUG_INFO
#if defined(VM_ACCESS_DEBUG_INFO)
            printf("  >v: %016llx ! %016llx > %016llx\n", address, (u64)data, (u64)ptr);
#endif
#endif
        }
    }
    return address;
}

#if defined(VM_MEMORY_DEBUG_INFO)
static struct pointer* vm_data_enumerator_next(void) {
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

static void** vm_data_enumerator_next_ref(void) {
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
#endif

/* public */

const struct virtual_methods virtual_methods_definition = {
    .init = vm_init,
    .destroy = vm_destroy,
    .free = vm_free,
    .read = vm_read,
    .write = vm_write,
#if defined(VM_MEMORY_DEBUG_INFO)
    .dump = vm_dump,
    .dump_ref = vm_dump_ref
#endif
};
