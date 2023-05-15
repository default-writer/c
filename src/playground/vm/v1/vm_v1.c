#include "common/alloc.h"
#include "list-micro/data.h"
#include "playground/pointer/pointer.h"
#include "playground/vm/vm.h"

/* list definition */

/* private */

#ifndef USE_GC
extern const struct list list_micro_definition;
static const struct list* list = &list_micro_definition;
static struct list_data** cache;
#endif
extern const struct pointer_methods pointer_methods_definition;
static const struct pointer_methods* pointer = &pointer_methods_definition;

struct vm_data {
    struct pointer** sp; /* stack pointer */
    struct pointer** bp; /* base pointer */
    struct vm_data* prev;
    struct vm_data* next;
    u64 address_space;
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
static void vm_memory_dump(struct vm_data* vm_ptr);
static void vm_memory_dump_ref(struct vm_data* vm_ptr);
#endif
static struct pointer* vm_free(struct vm_data** current, u64 address);
static struct pointer* vm_read(struct vm_data** current, u64 address);
static u64 vm_write(struct vm_data** current, struct pointer* value);
#ifdef USE_MEMORY_DEBUG_INFO
static struct pointer* vm_data_enumerator_next(void);
static struct pointer* vm_data_enumerator_next_ref(void);
#endif

/* internal */

static struct vm_data* vm_init_internal(u64 size, u64 address_space);
static void** vm_read_internal(struct vm_data** current, u64 address);
static u64 vm_alloc_internal(struct vm_data** current);
static void* to_real_address_internal(struct vm_data* vm, u64 address);
static u64 to_virtual_address_internal(const struct vm_data* vm, struct pointer** ptr);
#ifdef USE_MEMORY_DEBUG_INFO
static void vm_enumerator_init_internal(struct vm_data* vm);
static void vm_enumerator_destroy_internal(void);
#endif

static struct vm_data* vm_init_internal(u64 size, u64 address_space) {
    struct vm_data* vm = _list_alloc(sizeof(struct vm_data));
    vm->bp = _list_alloc(size * sizeof(void*));
    vm->sp = vm->bp;
    vm->address_space = address_space;
    vm->size = size;
    return vm;
}

static void** vm_read_internal(struct vm_data** current, u64 address) {
    void** ptr = 0;
    if (address != 0) {
        struct vm_data* vm = *current;
        while (address <= vm->address_space) {
            vm = vm->prev;
        }
        ptr = to_real_address_internal(vm, address);
    }
    return ptr;
}

static u64 vm_alloc_internal(struct vm_data** current) {
    struct vm_data* vm = *current;
    u64 address = 0;
    struct pointer** ptr;
#ifndef USE_GC
    ptr = list->pop(cache);
    if (ptr == 0) {
#endif
        ptr = vm->sp;
        address = to_virtual_address_internal(vm, ptr);
        while (address > vm->address_space + vm->size) {
            if (vm->next == 0) {
                vm->next = vm_init_internal(vm->size, vm->address_space + vm->size);
                vm->next->prev = vm;
            }
            vm = vm->next;
        }
        ptr = vm->sp;
        ++vm->sp;
        address = to_virtual_address_internal(vm, ptr);
#ifdef USE_MEMORY_DEBUG_INFO
        printf("  >+: 0x%016llx >0x%016llx\n", (u64)ptr, address);
#endif
#ifndef USE_GC
    } else {
        address = to_virtual_address_internal(vm, ptr);
#ifdef USE_MEMORY_DEBUG_INFO
        printf("  >.: 0x%016llx >0x%016llx\n", (u64)ptr, address);
#endif
    }
#endif
    *current = vm;
    return address;
}

static u64 to_virtual_address_internal(const struct vm_data* vm, struct pointer** ptr) {
    return (u64)(ptr - vm->bp) + vm->address_space + 1;
}

static void* to_real_address_internal(struct vm_data* vm, u64 address) {
    return vm->bp + address - vm->address_space - 1;
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
#ifndef USE_GC
    cache = _list_alloc(sizeof(void*));
    list->init(cache);
#endif
    *current = vm_init_internal(size, 0);
}

static void vm_destroy(struct vm_data** current) {
#ifndef USE_GC
    list->destroy(cache);
    _list_free(cache, sizeof(void*));
#ifdef USE_MEMORY_CLEANUP
    cache = 0;
#endif
#endif
    struct vm_data* vm = *current;
    while (vm->prev != 0) {
        vm = vm->prev;
    }
    while (vm != 0) {
        struct vm_data* next = vm->next;
        _list_free(vm->bp, vm->size * sizeof(void*));
        _list_free(vm, sizeof(struct vm_data));
        vm = next;
    }
}

#ifdef USE_MEMORY_DEBUG_INFO
static void vm_memory_dump(struct vm_data* vm_ptr) {
    while (vm_ptr->prev != 0) {
        vm_ptr = vm_ptr->prev;
    }
    vm_enumerator_init_internal(vm_ptr);
    struct pointer* data_ptr = 0;
    while ((data_ptr = vm_data_enumerator_next()) != 0) {
#ifdef USE_MEMORY_DEBUG_INFO
        pointer->dump(data_ptr);
#endif
    }
    vm_enumerator_destroy_internal();
}

static void vm_memory_dump_ref(struct vm_data* vm_ptr) {
    while (vm_ptr->prev != 0) {
        vm_ptr = vm_ptr->prev;
    }
    vm_enumerator_init_internal(vm_ptr);
    struct pointer* data_ptr = 0;
    while ((data_ptr = vm_data_enumerator_next_ref()) != 0) {
#ifdef USE_MEMORY_DEBUG_INFO
        pointer->dump_ref(data_ptr);
#endif
    }
    vm_enumerator_destroy_internal();
}
#endif

static struct pointer* vm_free(struct vm_data** current, u64 address) {
    struct pointer* data = 0;
    if (address != 0) {
        void** ptr = vm_read_internal(current, address);
        if (ptr != 0) {
#ifdef USE_MEMORY_DEBUG_INFO
            printf("  >-: 0x%016llx >0x%016llx\n", (u64)ptr, address);
#endif
#ifndef USE_GC
            list->push(cache, ptr);
#endif
            data = *ptr;
            *ptr = 0;
        }
    }
    return data;
}

static struct pointer* vm_read(struct vm_data** current, u64 address) {
    struct pointer* data = 0;
    if (address != 0) {
        void** ptr = vm_read_internal(current, address);
#ifdef USE_MEMORY_DEBUG_INFO
        printf("  >*: 0x%016llx >0x%016llx\n", (u64)ptr, address);
#endif
        if (ptr != 0) {
            data = *ptr;
        }
    }
    return data;
}

static u64 vm_write(struct vm_data** current, struct pointer* value) {
    u64 address = vm_alloc_internal(current);
    if (address != 0) {
        struct vm_data* vm = *current;
        void** ptr = to_real_address_internal(vm, address);
        if (ptr != 0) {
            *ptr = value;
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
        data = (struct pointer*)state->ptr++;
    }
    return data;
}

static struct pointer* vm_data_enumerator_next_ref(void) {
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
#endif

/* public */

const struct vm vm_definition = {
    .init = vm_init,
    .destroy = vm_destroy,
    .free = vm_free,
    .read = vm_read,
    .write = vm_write,
#ifdef USE_MEMORY_DEBUG_INFO
    .memory_dump = vm_memory_dump,
    .memory_dump_ref = vm_memory_dump_ref
#endif
};
