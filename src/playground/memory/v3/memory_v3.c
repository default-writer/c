#include "playground/memory/memory.h"

#define MAX_MEMORY 0xffff // 64K bytes

// global allocated memory
static void* memory = 0;
static void** ptr = 0;

static void memory_init(void);
static void memory_destroy(void);
static void* memory_alloc(u64 size);
static void memory_free(void* data, u64 size);

static void* alloc(void** prev, void* next, u64 size) {
    void** tmp = calloc(1, (size + 3) * sizeof(void*)); //
    prev = *prev;
    *prev = tmp;

    *tmp = next;
    *(tmp + 1) = tmp + size + 2;
    *(tmp + 2) = prev;

    return tmp + 2;
}

static void memory_init(void) {
    ptr = &memory;
    *ptr = ptr;
    ptr = alloc(ptr, 0, 0);
    // some ideas:

    // while (*tmp != 0) {
    //     tmp = *tmp;
    // }
}

static void memory_destroy(void) {
    free(memory);
    memory = 0;
    ptr = 0;
}

static void* memory_alloc(u64 size) {
    ptr = alloc(ptr - 1, ptr, size);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("  0+: 0x%016llx >0x%016llx\n", (u64)ptr, (u64)(*ptr));
#endif
    return ptr;
}

// releases global memory
static void memory_free(void* data, u64 size) {
    CLEAN(size)
    void** tmp = data;
    size = (u64)((void**)(*(tmp - 1)) - (void**)data);
    ptr = (void**)(*(tmp - 2));
#ifdef USE_MEMORY_CLEANUP
    memset(tmp - 2, 0, size + 3); // NOLINT
#endif
    free(tmp - 2);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("  0-: 0x%016llx !  %16lld\n", (u64)ptr, size);
#endif
}

const struct memory_allocator memory_allocator_v3 = {
    .init = memory_init,
    .destroy = memory_destroy,
    .alloc = memory_alloc,
    .free = memory_free
};
