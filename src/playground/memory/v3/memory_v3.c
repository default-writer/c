#include "common/alloc.h"
#include "playground/memory/api/memory.h"

#define DEFAULT_SIZE 0x0 /* 0 */

/* global allocated memory */
static void* memory = 0;
static void** ptr = 0;
static void* memory_alloc_internal(void** next, void* prev, u64 size);

static void memory_init(void);
static void memory_destroy(void);
static void* memory_alloc(u64 size);
static void memory_free(void* data, u64 size);

static void* memory_alloc_internal(void** next, void* prev, u64 size) {
    void** tmp = _list_alloc((size + 3) * sizeof(void*)); //
    next = *next;
    *next = tmp;

    *tmp = prev;
    *(tmp + 1) = tmp + size + 2;
    *(tmp + 2) = next;

    return tmp + 2;
}

static u64 offset(const void* data) {
    const void* const* head = data;
    const void* const* next = *(head - 1);
    return (u64)(next - head);
}

static void memory_init(void) {
    ptr = &memory;
    *ptr = ptr;
    ptr = memory_alloc_internal(ptr, 0, DEFAULT_SIZE);
}

static void memory_destroy(void) {
    _list_free(memory, (DEFAULT_SIZE + 3) * sizeof(void*));
    memory = 0;
    ptr = 0;
}

static void* memory_alloc(u64 size) {
    ptr = memory_alloc_internal(ptr - 1, ptr, size);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("  0+: 0x%016llx >0x%016llx\n", (u64)ptr, (u64)(*ptr));
#endif
    return ptr;
}

/* releases global memory */
static void memory_free(void* data, u64 size) {
    void** head = data;
    void** next = *(head - 1);
    void** last = *(head - 2);
    if (*next == 0) {
        ptr = last;
        next = ptr + offset(ptr);
        *next = 0;
    }
    CLEAN(size)
    size = offset(data);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("  0-: 0x%016llx !  %16lld\n", (u64)last, size);
#endif
#ifdef USE_MEMORY_CLEANUP
    memset(head - 2, 0, (size + 3) * sizeof(void*)); /* NOLINT */
#endif
    _list_free(head - 2, (size + 3) * sizeof(void*));
}

const union memory_allocator_api memory_allocator_v3 = {
    .v1.init = memory_init,
    .v1.destroy = memory_destroy,
    .v1.alloc = memory_alloc,
    .v1.free = memory_free
};
