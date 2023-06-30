#include "common/alloc.h"
#include "playground/memory/api/memory.h"

/* macros */
#define DEFAULT_SIZE 0x0 /* 0 */
#define PTR_SIZE sizeof(void*) /* size of a pointer */
#define ALLOC_SIZE(size) ((size + 3) * PTR_SIZE)

/* global allocated memory */
static void* memory = 0;
static void** ptr = 0;
static void* memory_alloc_internal(void** next, void* prev, u64 size);

static void memory_init(void);
static void memory_destroy(void);
static void* memory_alloc(u64 size);
static void memory_free(void* data, u64 size);

static void* memory_alloc_internal(void** next, void* prev, u64 size) {
    void** tmp = global_alloc(ALLOC_SIZE(size)); //
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
    global_free(memory, ALLOC_SIZE(DEFAULT_SIZE));
    memory = 0;
    ptr = 0;
}

static void* memory_alloc(u64 size) {
    ptr = memory_alloc_internal(ptr - 1, ptr, size);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("  0+: %016llx > %016llx\n", (u64)ptr, (u64)(*ptr));
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
    printf("  0-: %016llx ! %16lld\n", (u64)last, size);
#endif
    global_free(head - 2, ALLOC_SIZE(size));
}

const union memory_allocator_api memory_allocator_v3 = {
    .v1.init = memory_init,
    .v1.destroy = memory_destroy,
    .v1.alloc = memory_alloc,
    .v1.free = memory_free
};