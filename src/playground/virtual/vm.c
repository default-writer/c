#include "common/alloc.h"

// #include "playground/list/v2/list.h"
#include "list-micro/data.h"
#include "playground/virtual/vm.h"

/* list definition */
// extern const struct list list_v2;
extern const struct list list_micro_definition;

/*private */

// static const struct list* list = &list_v2;
static const struct list* list = &list_micro_definition;
static struct list_data** cache;
// static struct vm_data** current;

/* api */

static void vm_init(struct vm_data** current, u64 size);
static void vm_destroy(struct vm_data** current);
static void* vm_free(struct vm_data** current, u64 address);
static void* vm_read(struct vm_data** current, u64 address);
static u64 vm_write(struct vm_data** current, void* value);

/* internal */

static void** vm_read_internal(struct vm_data** current, u64 address);
static u64 vm_alloc_internal(struct vm_data** current);
static void* to_real_address_internal(struct vm_data* vm, u64 address);
static u64 to_virtual_address_internal(struct vm_data* vm, void** ptr);

/* public */
struct enumerator_data* vm_enumerator_init(struct vm_data** current);
void vm_enumerator_destroy(struct enumerator_data* data);

/* implementation */

// static u64 address_space = 0x0;

static u64 to_virtual_address_internal(struct vm_data* vm, void** ptr) {
    return (u64)(ptr - vm->bp) + vm->address_space + 1;
}

static void* to_real_address_internal(struct vm_data* vm, u64 address) {
    return vm->bp + address - vm->address_space - 1;
}

static struct vm_data* vm_init_internal(u64 size, u64 address_space) {
    struct vm_data* vm = _list_alloc(sizeof(struct vm_data));
    vm->bp = _list_alloc(size * sizeof(void*));
    vm->sp = vm->bp;
    vm->address_space = address_space;
    vm->size = size;
    return vm;
}

static void vm_init(struct vm_data** current, u64 size) {
    cache = _list_alloc(sizeof(void*));
    list->init(cache);
    *current = vm_init_internal(size, 0);
}

static void vm_destroy(struct vm_data** current) {
    list->destroy(cache);
    _list_free(cache, sizeof(void*));
#ifdef USE_MEMORY_CLEANUP
    cache = 0;
#endif
    struct vm_data* vm = *current;
    while (vm->next != 0) {
        vm = vm->next;
    }
    while (vm != 0) {
        struct vm_data* prev = vm->prev;
        _list_free(vm->bp, vm->size * sizeof(void*));
        _list_free(vm, sizeof(struct vm_data));
        vm = prev;
    }
}

static u64 vm_alloc_internal(struct vm_data** current) {
    struct vm_data* pointer = *current;
    u64 address = 0;
    void** ptr = list->pop(cache);
    if (ptr == 0) {
        address = to_virtual_address_internal(pointer, pointer->sp);
        while (address > pointer->address_space + pointer->size) {
            if (pointer->next == 0) {
                pointer->next = vm_init_internal(pointer->size, pointer->address_space + pointer->size);
                pointer->next->prev = pointer;
            }
            pointer = pointer->next;
        }
        ptr = pointer->sp++;
    }
    address = to_virtual_address_internal(pointer, ptr);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("  >+: 0x%016llx >0x%016llx\n", (u64)ptr, address);
#endif
    *current = pointer;
    return address;
}

static void* vm_free(struct vm_data** current, u64 address) {
    void* data = 0;
    if (address != 0) {
        void** ptr = vm_read_internal(current, address);
        if (ptr != 0) {
#ifdef USE_MEMORY_DEBUG_INFO
            printf("  >-: 0x%016llx >0x%016llx\n", (u64)ptr, address);
#endif
            list->push(cache, ptr);
            data = *ptr;
            *ptr = 0;
        }
    }
    return data;
}

static void* vm_read(struct vm_data** current, u64 address) {
    void* data = 0;
    if (address != 0) {
        void** ptr = vm_read_internal(current, address);
        if (ptr != 0) {
            data = *ptr;
        }
    }
    return data;
}

static void** vm_read_internal(struct vm_data** current, u64 address) {
    void** ptr = 0;
    if (address != 0) {
        struct vm_data* pointer = *current;
        while (address <= pointer->address_space) {
            pointer = pointer->prev;
        }
        ptr = to_real_address_internal(pointer, address);
    }
    return ptr;
}

static u64 vm_write(struct vm_data** current, void* value) {
    u64 address = vm_alloc_internal(current);
    if (address != 0) {
        struct vm_data* pointer = *current;
        void** ptr = to_real_address_internal(pointer, address);
        if (ptr != 0) {
            *ptr = value;
        }
    }
    return address;
}

static void* vm_data_enumerator_next(struct vm_data** current, struct enumerator_data* enumerator) {
    struct vm_data* vm = *current;
    void* data = 0;
    while (data == 0) {
        if (enumerator->current == vm->sp) {
            if (vm->next == 0) {
                break;
            }
            vm = vm->next;
            *current = vm;
            enumerator->current = vm->bp;
        }
        data = enumerator->current++;
    }
    enumerator->value = data;
    return enumerator->value;
}

/* public */

struct enumerator_data* vm_enumerator_init(struct vm_data** current) {
    struct vm_data* vm = *current;
    struct enumerator_data* data = _list_alloc(sizeof(struct enumerator_data));
    data->current = vm->bp;
    return data;
}

void vm_enumerator_destroy(struct enumerator_data* enumerator) {
    _list_free(enumerator, sizeof(struct enumerator_data));
}

const struct vm_data_enumerator vm_data_enumerator_definition = {
    .next = vm_data_enumerator_next
};

const struct vm vm_definition = {
    .init = vm_init,
    .destroy = vm_destroy,
    //.alloc = vm_alloc,
    .free = vm_free,
    .read = vm_read,
    .write = vm_write
};
