#include "playground/memory/memory.h"
#include "std/common.h"

#define MAX_MEMORY 0xffff // 64K bytes

// global allocated memory
static void* memory = 0;
static void** ptr = 0;

static void memory_init() {
    ptr = &memory;
    *ptr = calloc(1, MAX_MEMORY);
    ptr = *ptr;
}

static void memory_destroy() {
    free(memory);
    memory = 0;
    ptr = 0;
}

static void* memory_alloc(u32 nmemb, u32 size) {
    void** tmp = ptr;
    ptr += size;
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   +: 0x%016llx >0x%016llx\n", (u64)tmp, (u64)ptr);
#endif
    return tmp;
}

static void memory_free(void* data, u32 size) {
    ptr -= size;
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   -: 0x%016llx !  %16lld\n", (u64)ptr, (u64)size);
#endif
}

const struct memory_allocator memory_allocator_v1 = {
    .init = memory_init,
    .destroy = memory_destroy,
    .alloc = memory_alloc,
    .free = memory_free
};