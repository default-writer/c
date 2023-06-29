#include "playground/vm/v1/vm_v1.h"
#include "common/alloc.h"
#include "list-micro/data.h"
#include "playground/pointer/v1/pointer_v1.h"

/* macros */
#define DEFAULT_SIZE 0x0 /* 0 */
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

static void vm_init(struct vm_data** current, u64 size);
static void vm_destroy(struct vm_data** current);
#ifdef USE_MEMORY_DEBUG_INFO
static void vm_dump(struct vm_data* current);
static void vm_dump_ref(struct vm_data* current);
#endif
static struct pointer* vm_free(const struct vm_data* current, u64 address);
static struct pointer* vm_read(const struct vm_data* current, u64 address);
static u64 vm_write(struct vm_data* current, struct pointer* data);
#ifdef USE_MEMORY_DEBUG_INFO
static struct pointer* vm_data_enumerator_next(void);
static void* vm_data_enumerator_next_ref(void);
#endif

/* internal */

static void* to_real_address_internal(const struct vm_data* current, u64 address);

static struct vm_data* vm_init_internal(u64 size, u64 offset);
static void** vm_read_internal(const struct vm_data* current, u64 address);
static struct pointer** vm_alloc_internal(struct vm_data* current, u64* address, struct vm_data** target);
#ifdef USE_MEMORY_DEBUG_INFO
static void vm_enumerator_init_internal(struct vm_data* current);
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

static void** vm_read_internal(const struct vm_data* current, u64 address) {
    void** ptr = 0;
    if (address != 0) {
        const struct vm_data* vm = current;
        if (vm != 0) {
            ptr = to_real_address_internal(vm, address);
        }
    }
    return ptr;
}

static struct pointer** vm_alloc_internal(struct vm_data* current, u64* address, struct vm_data** target) {
    struct vm_data* vm = current;
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
        while (vm->next != 0) {
            vm = vm->next;
        }
        if ((u64)(vm->sp - vm->bp) == vm->size) {
            struct vm_data* prev = vm;
            vm = vm_init_internal(vm->size, vm->offset + vm->size);
            vm->prev = prev;
            prev->next = vm;
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

static void* to_real_address_internal(const struct vm_data* current, u64 address) {
    const struct vm_data* vm = current;
    void* data = 0;
    if (address > vm->offset && address <= vm->offset + vm->size) {
        data = vm->bp + address - vm->offset - 1;
    } else {
        vm = vm->next;
        while (vm != 0) {
            if (address > vm->offset && address <= vm->offset + vm->size) {
                data = vm->bp + address - vm->offset - 1;
                break;
            }
            vm = vm->next;
        }
    }
    return data;
}

#ifdef USE_MEMORY_DEBUG_INFO
static void vm_enumerator_init_internal(struct vm_data* current) {
    struct vm_data* vm = current;
    if (vm != 0) {
        while (vm->prev != 0) {
            vm = vm->prev;
        }
        state->vm = vm;
        state->ptr = vm->bp;
    }
}

static void vm_enumerator_destroy_internal(void) {
    state->vm = 0;
    state->ptr = 0;
}
#endif

/* implementation */

static void vm_init(struct vm_data** current, u64 size) {
#ifndef USE_GC
    cache = global_alloc(PTR_SIZE);
    list->init(cache);
#endif
    *current = vm_init_internal(size, 0);
}

static void vm_destroy(struct vm_data** current) {
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
    struct vm_data* vm = *current;
    while (vm->prev != 0) {
        vm = vm->prev;
    }
    while (vm != 0) {
        struct vm_data* next = vm->next;
        global_free(vm->bp, ALLOC_SIZE(vm->size));
        global_free(vm, VM_DATA_SIZE);
        vm = next;
    }
}

#ifdef USE_MEMORY_DEBUG_INFO
static void vm_dump(struct vm_data* current) {
    if (current != 0) {
        vm_enumerator_init_internal(current);
        struct pointer* ptr = 0;
        while ((ptr = vm_data_enumerator_next()) != 0) {
            pointer->dump(ptr);
        }
        vm_enumerator_destroy_internal();
    }
}

static void vm_dump_ref(struct vm_data* current) {
    if (current != 0) {
        vm_enumerator_init_internal(current);
        void** ptr = 0;
        while ((ptr = vm_data_enumerator_next_ref()) != 0) {
            pointer->dump_ref(ptr);
        }
        vm_enumerator_destroy_internal();
    }
}
#endif

static struct pointer* vm_free(const struct vm_data* current, u64 address) {
    const struct vm_data* vm = current;
    struct pointer* data = 0;
    if (address != 0) {
        struct pointer** ptr = (struct pointer**)vm_read_internal(vm, address);
        if (ptr != 0) {
#ifndef USE_GC
            struct vm_pointer* vm_ptr = global_alloc(VM_POINTER_SIZE);
            vm_ptr->ptr = ptr;
            vm_ptr->vm = (*ptr)->vm;
            list->push(cache, vm_ptr);
#endif
            data = *ptr;
#ifdef USE_MEMORY_DEBUG_INFO
            printf("  >-: %016llx : %016llx > %016llx\n", address, (u64)data, (u64)ptr);
#endif
            *ptr = 0;
        }
    }
    return data;
}

static struct pointer* vm_read(const struct vm_data* current, u64 address) {
    const struct vm_data* vm = current;
    struct pointer* data = 0;
    if (address != 0) {
        void** ptr = vm_read_internal(vm, address);
        if (ptr != 0) {
            data = *ptr;
#ifdef USE_MEMORY_DEBUG_INFO
            printf("  <v: %016llx : %016llx > %016llx\n", address, (u64)data, (u64)ptr);
#endif
        }
    }
    return data;
}

static u64 vm_write(struct vm_data* current, struct pointer* data) {
    struct vm_data* vm = current;
    u64 address = 0;
    struct vm_data* target = 0;
    struct pointer** ptr = vm_alloc_internal(vm, &address, &target);
    if (ptr != 0) {
        *ptr = data;
        data->vm = target;
#ifdef USE_MEMORY_DEBUG_INFO
        printf("  >v: %016llx : %016llx > %016llx\n", address, (u64)data, (u64)ptr);
#endif
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

const struct vm vm_definition = {
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
