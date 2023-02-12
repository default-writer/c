#include "playground/virtual/vm.h"
#include "playground/list/v2/list.h"

#define DEFAULT_SIZE 0xffff // 64K bytes

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

static struct vm_data* vm_init(u64 size) {
    struct vm_data* pointer = calloc(1, sizeof(struct vm_data));
    pointer->base = calloc(1, size * sizeof(void*));
    void* max = (void*)((u64*)pointer->base + size);
    pointer->max = max;
    pointer->size = size;
    pointer->ptr = pointer->base;
    cache = list->alloc(DEFAULT_SIZE);
    return pointer;
}

static void vm_destroy(struct vm_data* pointer) {
    list->free(cache);
    cache = 0;
    free(pointer->base);
#ifdef USE_MEMORY_CLEANUP
    pointer->base = 0;
    pointer->ptr = 0;
    pointer->max = 0;
    pointer->size = 0;
#endif
    free(pointer);
    pointer = 0;
}

u64 vm_alloc(struct vm_data* pointer) {
    u64 offset = 0;
    void* ptr = list->pop(cache);
    if (ptr == 0) {
        ptr = pointer->ptr++;
    }
    if (ptr != pointer->max) {
        offset = to_virtual_address(pointer->base, ptr);
    }
    return offset;
}

void vm_free(struct vm_data* pointer, u64 address) {
    if (address > 0 && address < pointer->size) {
        void** ptr = to_real_address(pointer->base, address);
        if (*ptr != 0) {
            list->push(cache, ptr);
            *ptr = 0;
        }
    }
}

static u64 address_space = 0;

static u64 to_virtual_address(void** base, void** ptr) {
    return (u64)(ptr - base) + 1 + address_space;
}

static void* to_real_address(void** base, u64 address) {
    return base + address - 1 + address_space;
}

static void* vm_read(struct vm_data* pointer, u64 address) {
    void** data = 0;
    if (address > 0 && address < pointer->size) {
        data = to_real_address(pointer->base, address);
        data = *data;
    }
    return data;
}

static void vm_write(struct vm_data* pointer, u64 address, void* value) {
    if (address > 0 && address < pointer->size) {
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
