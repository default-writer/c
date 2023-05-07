#include "playground/vm/v2/vm_v2.h"
#include "common/alloc.h"
#include "list-micro/data.h"
#include "playground/pointer/v2/pointer_v2.h"

/* list definition */

struct pointer_info {
    struct pointer* ptr;
    struct vm_data* vm;
};

/* private */

#ifndef USE_GC
extern const struct list list_micro_definition;
static const struct list* list = &list_micro_definition;
static struct list_data** cache;
#endif

struct vm_data {
    struct pointer_info* sp; /* stack pointer */
    struct pointer_info* bp; /* base pointer */
    struct vm_data* prev;
    struct vm_data* next;
    u64 address_space;
    u64 size;
};

#ifdef USE_MEMORY_DEBUG_INFO
struct vm_state {
    struct vm_data* vm;
    struct pointer_info* ptr;
};

struct vm_data_enumerator {
    struct pointer_info* (*next)(void);
};

static struct vm_state vm_state;
static struct vm_state* state = &vm_state;
#endif

/* api */

static void vm_init(struct vm_data** current, u64 size);
static void vm_destroy(struct vm_data** current);
#ifdef USE_MEMORY_DEBUG_INFO
static void vm_memory_dump(struct vm_data* vm_ptr);
#endif
static struct pointer* vm_free(struct vm_data** current, u64 address);
static struct pointer* vm_read(struct vm_data** current, u64 address);
static u64 vm_write(struct vm_data** current, struct pointer* value);
#ifdef USE_MEMORY_DEBUG_INFO
static struct pointer_info* vm_data_enumerator_next();
#endif

/* internal */

static struct vm_data* vm_init_internal(u64 size, u64 address_space);
static struct pointer_info* vm_read_internal(struct vm_data** current, u64 address);
static u64 vm_alloc_internal(struct vm_data** current);
static struct pointer_info* to_real_address_internal(struct vm_data* vm, u64 address);
static u64 to_virtual_address_internal(const struct vm_data* vm, const struct pointer_info* ptr);
#ifdef USE_MEMORY_DEBUG_INFO
static void vm_enumerator_init_internal(struct vm_data* vm);
static void vm_enumerator_destroy_internal(void);
#endif

static struct vm_data* vm_init_internal(u64 size, u64 address_space) {
    struct vm_data* vm = _list_alloc(sizeof(struct vm_data));
    vm->bp = _list_alloc(size * sizeof(struct pointer_info));
    vm->sp = vm->bp;
    vm->address_space = address_space;
    vm->size = size;
    return vm;
}

static struct pointer_info* vm_read_internal(struct vm_data** current, u64 address) {
    struct pointer_info* ptr = 0;
    if (address != 0) {
        struct vm_data* pointer = *current;
        while (address <= pointer->address_space) {
            pointer = pointer->prev;
        }
        ptr = to_real_address_internal(pointer, address);
    }
    return ptr;
}

static u64 vm_alloc_internal(struct vm_data** current) {
    struct vm_data* pointer = *current;
    u64 address = 0;
    struct pointer_info* ptr;
#ifndef USE_GC
    ptr = list->pop(cache);
    if (ptr == 0) {
#endif
        ptr = pointer->sp;
        address = to_virtual_address_internal(pointer, ptr);
        while (address > pointer->address_space + pointer->size) {
            if (pointer->next == 0) {
                pointer->next = vm_init_internal(pointer->size, pointer->address_space + pointer->size);
                pointer->next->prev = pointer;
            }
            pointer = pointer->next;
        }
        ptr = pointer->sp++;
        address = to_virtual_address_internal(pointer, ptr);
#ifdef USE_MEMORY_DEBUG_INFO
        printf("  >+: 0x%016llx >0x%016llx\n", (u64)ptr, address);
#endif
#ifndef USE_GC
    } else {
        address = to_virtual_address_internal(pointer, ptr);
#ifdef USE_MEMORY_DEBUG_INFO
        printf("  >.: 0x%016llx >0x%016llx\n", (u64)ptr, address);
#endif
    }
#endif
    *current = pointer;
    return address;
}

static u64 to_virtual_address_internal(const struct vm_data* vm, const struct pointer_info* ptr) {
    return (u64)(ptr - vm->bp) + vm->address_space + 1;
}

static struct pointer_info* to_real_address_internal(struct vm_data* vm, u64 address) {
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
        _list_free(vm->bp, vm->size * sizeof(struct pointer_info*));
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
    struct pointer_info* data_ptr = 0;
    while ((data_ptr = vm_data_enumerator_next()) != 0) {
#ifdef USE_MEMORY_DEBUG_INFO
        if (data_ptr != 0 && data_ptr->ptr != 0) {
            printf("   ^: 0x%016llx >0x%016llx\n", (u64)data_ptr, (u64)data_ptr->ptr);
        }
#endif
    }
    vm_enumerator_destroy_internal();
}
#endif

static struct pointer* vm_free(struct vm_data** current, u64 address) {
    struct pointer* data = 0;
    if (address != 0) {
        struct pointer_info* info = vm_read_internal(current, address);
        if (info != 0) {
#ifdef USE_MEMORY_DEBUG_INFO
            printf("  >-: 0x%016llx >0x%016llx\n", (u64)info->ptr, address);
#endif
#ifndef USE_GC
            list->push(cache, info);
#endif
            data = info->ptr;
            info->ptr = 0;
        }
    }
    return data;
}

static struct pointer* vm_read(struct vm_data** current, u64 address) {
    struct pointer* data = 0;
    if (address != 0) {
        struct pointer_info* info = vm_read_internal(current, address);
        if (info != 0) {
            data = info->ptr;
        }
    }
    return data;
}

static u64 vm_write(struct vm_data** current, struct pointer* value) {
    u64 address = vm_alloc_internal(current);
    if (address != 0) {
        struct vm_data* pointer = *current;
        struct pointer_info* info = to_real_address_internal(pointer, address);
        if (info != 0) {
            info->ptr = value;
            info->vm = pointer;
            value->offset = (u64)(pointer->sp - pointer->bp - 1);
        }
    }
    return address;
}

#ifdef USE_MEMORY_DEBUG_INFO
static struct pointer_info* vm_data_enumerator_next() {
    struct pointer_info* data = 0;
    struct vm_data* vm = state->vm;
    struct pointer_info* ptr = state->ptr;
    while (data == 0) {
        if (ptr == vm->sp) {
            if (vm->next == 0) {
                break;
            }
            vm = vm->next;
            ptr = vm->bp;
            continue;
        }
        data = ptr++;
    }
    state->ptr = ptr;
    state->vm = vm;
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
    .memory_dump = vm_memory_dump
#endif
};
