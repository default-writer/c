#include "common/alloc.h"

#include "playground/list/v2/list.h"
#include "playground/virtual/vm.h"

/* list definition */
extern const struct list list_v2;

/*private */
static const struct list* list = &list_v2;
static struct list_data* cache;

/* api */

static struct vm_data* vm_init(u64 size);
static void vm_destroy(struct vm_data* pointer);
u64 vm_alloc(struct vm_data* pointer);
void vm_free(struct vm_data* pointer, u64 address);
static void* vm_read(struct vm_data* pointer, u64 address);
static void vm_write(struct vm_data* pointer, u64 address, void* value);

/* internal */

static void* to_real_address(void** base, u64 address);
static u64 to_virtual_address(void** base, void** ptr);

/* implementation */

static u64 address_space = 0x0;

static u64 to_virtual_address(void** base, void** ptr) {
    return (u64)(ptr - base) + address_space + 1;
}

static void* to_real_address(void** base, u64 address) {
    return base + address - address_space - 1;
}

static struct vm_data* vm_init(u64 size) {
    struct vm_data* pointer = _list_alloc(1, sizeof(struct vm_data));
    pointer->base = _list_alloc(size, sizeof(void*));
    pointer->max = (void*)((u64*)pointer->base + size);
    pointer->ptr = pointer->base;
    pointer->size = size;
    cache = list->alloc(size);
    return pointer;
}

static void vm_destroy(struct vm_data* pointer) {
    list->free(cache);
    cache = 0;
    _list_free(pointer->base, pointer->size * sizeof(void*));
    pointer->base = 0;
    pointer->max = 0;
    pointer->ptr = 0;
    pointer->size = 0;
    _list_free(pointer, sizeof(struct vm_data));
    pointer = 0;
}

u64 vm_alloc(struct vm_data* pointer) {
    u64 address = 0;
    void** ptr = list->pop(cache);
    if (pointer->ptr != pointer->max) {
        if (ptr == 0) {
            ptr = pointer->ptr++;
        }
        if (ptr != pointer->max) {
            address = to_virtual_address(pointer->base, ptr);
#ifdef USE_MEMORY_DEBUG_INFO
            printf("   +: 0x%016llx >0x%016llx\n", (u64)ptr, address);
#endif
        }
    }
    return address;
}

void vm_free(struct vm_data* pointer, u64 address) {
    if (address > address_space && address - address_space < pointer->size) {
        void** ptr = to_real_address(pointer->base, address);
        if (*ptr != 0) {
#ifdef USE_MEMORY_DEBUG_INFO
            printf("   -: 0x%016llx >0x%016llx\n", (u64)ptr, address);
#endif
            list->push(cache, ptr);
            *ptr = 0;
        }
    }
}

static void* vm_read(struct vm_data* pointer, u64 address) {
    void** data = 0;
    if (address > address_space && address - address_space < pointer->size) {
        data = to_real_address(pointer->base, address);
        data = *data;
    }
    return data;
}

static void vm_write(struct vm_data* pointer, u64 address, void* value) {
    if (address > address_space && address - address_space < pointer->size) {
        void** ptr = to_real_address(pointer->base, address);
        *ptr = value;
    }
}

/* public */

const struct vm vm_definition = {
    .init = vm_init,
    .destroy = vm_destroy,
    .alloc = vm_alloc,
    .free = vm_free,
    .read = vm_read,
    .write = vm_write
};
