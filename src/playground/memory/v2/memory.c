#include "playground/memory/memory.h"

#define MAX_MEMORY 0xffff // 64K bytes

// global allocated memory
static void* memory = 0;
static void** ptr = 0;

static void* memory_alloc(u64 size);
static void memory_free(const void* data, u64 size);
static void memory_init(void);
static void memory_destroy(void);

static void memory_init(void) {
    ptr = &memory;
    *ptr = calloc(1, MAX_MEMORY);
    ptr = *ptr;
    *ptr = ptr + 1;
}

static void memory_destroy(void) {
    free(memory);
    memory = 0;
    ptr = 0;
}

static void* memory_alloc(u64 size) {
    void** tmp = *ptr;
    ptr += size;
    ++ptr;
    *ptr = ptr + 1;
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   +: 0x%016llx >0x%016llx\n", (u64)tmp, (u64)(*(tmp + size)));
#endif
    return tmp;
}

// releases global memory
static void memory_free(const void* data, u64 size) {
    ZEROPTR(data)
    --ptr;
    ptr -= size;
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   -: 0x%016llx !  %16lld\n", (u64)ptr, size);
#endif
}

const struct memory_allocator memory_allocator_v2 = {
    .init = memory_init,
    .destroy = memory_destroy,
    .alloc = memory_alloc,
    .free = memory_free
};