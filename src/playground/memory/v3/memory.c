/*
    See `memory.md` for extened list of metadata
    
    Simple linear memory allocation works as follows:

    1. allocation will take extra 8 bytes for pointer data
    2. out-of-bounds pointer (-8) got first available address
    3. memory manager assumes that alloc and free follows FIFO order.
    4. to fast realloc *last* allocated block one should free it and alloc with the new size

*/

/*
    |s|_|_|_|s|_|_|_|_|_|_|s|_|_|_|

      start address >|         end address >|
                     |                      |
    +----------------+----------------------+
    |< service area >|< addressable memory >|
    +---------------+-----------------------+
    |< allocated space                     >|
    +---------------------------------------+

*/

#include "playground/memory/memory.h"
#include "std/common.h"

#define MAX_MEMORY 0xfffffff // 256M bytes

// global allocated memory
static void* memory = 0;
static void** ptr = 0;

static void memory_init() {
    ptr = &memory;
    *ptr = calloc(1, MAX_MEMORY);
    ptr = *ptr;
    *ptr = ptr + 1;
}

static void memory_destroy() {
    free(memory);
    memory = 0;
    ptr = 0;
}

static void* memory_alloc(u32 nmemb, u32 size) {
    void** tmp = *ptr;
    ptr += size;
    ++ptr;
    *ptr = ptr + 1;
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   +: 0x%016llx >0x%016llx\n", (u64)tmp, (u64)*(tmp + size));
#endif
    return tmp;
}

// releases global memory
static void memory_free(void* data, u32 size) {
    --ptr;
    ptr -= size;
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   -: 0x%016llx !  %16lld\n", (u64)ptr, (u64)size);
#endif
}

const struct memory_allocator memory_allocator_v2 = {
    .init = memory_init,
    .destroy = memory_destroy,
    .alloc = memory_alloc,
    .free = memory_free
};