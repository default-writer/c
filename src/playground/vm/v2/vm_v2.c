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
    struct pointer_info* info;
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

/* internal */

static struct vm_data* vm_init_internal(u64 size, u64 address_space);
static struct pointer_info* vm_read_internal(struct vm_data** current, u64 address);
static struct pointer_info* vm_alloc_internal(struct vm_data** current);
#ifdef USE_MEMORY_DEBUG_INFO
static void vm_enumerator_init_internal(struct vm_data* vm);
static void vm_enumerator_destroy_internal(void);
static struct pointer_info* vm_enumerator_next();
#endif

static struct vm_data* vm_init_internal(u64 size, u64 address_space) {
    struct vm_data* vm = _list_alloc(sizeof(struct vm_data));
    vm->bp = _list_alloc(size * sizeof(struct pointer_info));
    vm->sp = vm->bp; /* sp is beyond upper array boundary by 1 */
    vm->address_space = address_space;
    vm->size = size;
    return vm;
}

static struct pointer_info* vm_read_internal(struct vm_data** current, u64 address) {
    struct pointer_info* info = 0;
    if (address != 0) {
        struct vm_data* pointer = *current;
        while (address <= pointer->address_space) {
            pointer = pointer->prev;
        }
        info = pointer->bp + address - pointer->address_space - 1;
    }
    return info;
}

static struct pointer_info* vm_alloc_internal(struct vm_data** current) {
    struct vm_data* pointer = 0;
    struct pointer_info* info = 0;
#ifndef USE_GC
    info = list->pop(cache);
    if (info == 0) {
#endif
        pointer = *current;
        while (pointer->sp == pointer->bp + pointer->size) { /* all memory slots utilized  */
            if (pointer->next == 0) { /* current memory region processed */
                pointer->next = vm_init_internal(pointer->size, pointer->address_space + pointer->size);
                pointer->next->prev = pointer;
            }
            pointer = pointer->next;
        }
        info = pointer->sp++; /* get nex memory slot */
        info->vm = pointer; /* associate with current memory region */
#ifdef USE_MEMORY_DEBUG_INFO
        printf("  >+: 0x%016llx >0x%016llx\n", (u64)info, (u64)(pointer->address_space + pointer->sp - pointer->bp));
#endif
#ifndef USE_GC
    } else {
        pointer = info->vm;
#ifdef USE_MEMORY_DEBUG_INFO
        printf("  >.: 0x%016llx >0x%016llx\n", (u64)info, (u64)(pointer->address_space + pointer->sp - pointer->bp));
#endif
    }
#endif
    *current = pointer;
    return info;
}

#ifdef USE_MEMORY_DEBUG_INFO
static void vm_enumerator_init_internal(struct vm_data* vm) {
    state->vm = vm;
    state->info = vm->bp;
}

static void vm_enumerator_destroy_internal(void) {
    state->vm = 0;
    state->info = 0;
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
    struct pointer_info* info = 0;
    while ((info = vm_enumerator_next()) != 0) {
#ifdef USE_MEMORY_DEBUG_INFO
        if (info != 0 && info->ptr != 0) {
            printf("   ^: 0x%016llx >0x%016llx\n", (u64)info, (u64)info->ptr);
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
            printf("  >-: 0x%016llx >0x%016llx\n", (u64)info, address);
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
    u64 address = 0;
    struct pointer_info* info = vm_alloc_internal(current);
    if (info != 0) {
        info->ptr = value;
        address = info->vm->address_space + (u64)(info->vm->sp - info->vm->bp);
    }
    return address;
}

#ifdef USE_MEMORY_DEBUG_INFO
static struct pointer_info* vm_enumerator_next() {
    struct vm_data* vm = state->vm;
    struct pointer_info* info = state->info;
    struct pointer_info* ptr = 0;
    if (vm != 0) {
        ptr = info;
        while (ptr + 1 == vm->sp && vm->next != 0) { /* end of route */
            vm = vm->next;
            ptr = vm->bp;
        }
        if (ptr + 1 < vm->sp) {
            ++ptr;
        }
        if (ptr == info) {
            vm = vm->next;
        }
        state->vm = vm;
        state->info = ptr;
    }
    return ptr;
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
