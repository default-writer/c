#include "playground/memory/api/memory.h"

#define MAX_MEMORY 0xffff /* 64K bytes */

/* global allocated memory */
static void* memory = 0;
static void** ptr = 0;

static void memory_init(void);
static void memory_destroy(void);
static void* memory_alloc(u64 size);
static void memory_free(void* data, u64 size);

static void memory_init(void) {
    ptr = &memory;
    *ptr = calloc(1, MAX_MEMORY);
    ptr = *ptr;
}

static void memory_destroy(void) {
    free(memory);
    memory = 0;
    ptr = 0;
}

static void* memory_alloc(u64 size) {
    void** tmp = ptr;
    ptr += size;
#ifdef USE_MEMORY_DEBUG_INFO
    printf("  0+: 0x%016llx >0x%016llx\n", (u64)tmp, (u64)ptr);
#endif
    return tmp;
}

static void memory_free(void* data, u64 size) {
    CLEAN(data)
    ptr -= size;
#ifdef USE_MEMORY_DEBUG_INFO
    printf("  0-: 0x%016llx !  %16lld\n", (u64)ptr, size);
#endif
}

const union memory_allocator_api memory_allocator_v1 = {
    .v1.init = memory_init,
    .v1.destroy = memory_destroy,
    .v1.alloc = memory_alloc,
    .v1.free = memory_free
};
