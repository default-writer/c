#include "common/memory.h"
#include "std/common.h"

#define MAX_MEMORY 0xfffffff // 256M bytes

// global allocated memory
static void* memory = 0;
static void** ptr = 0;

/*
    Linear memory allocation works as follows:

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

void memory_init_v1() {
    ptr = &memory;
    *ptr = calloc(1, MAX_MEMORY);
    ptr = *ptr;
}

void memory_destroy_v1() {
    free(memory);
    memory = 0;
    ptr = 0;
}
void memory_init_v2() {
    ptr = &memory;
    *ptr = calloc(1, MAX_MEMORY);
    ptr = *ptr;
    *ptr = ptr + 1;
}

void memory_destroy_v2() {
    free(memory);
    memory = 0;
    ptr = 0;
}

void* memory_alloc_v1(u32 nmemb, u32 size) {
    void** tmp = ptr;
    ptr += size;
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   +: 0x%016llx >0x%016llx\n", (u64)tmp, (u64)(ptr + size));
#endif
    return tmp;
}

void* memory_alloc_v2(u32 nmemb, u32 size) {
    void** tmp = *ptr;
    ptr += size;
    ++ptr;
    *ptr = ptr + 1;
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   +: 0x%016llx >0x%016llx\n", (u64)tmp, (u64)*(ptr + size));
#endif
    return tmp;
}

void memory_free_v1(void* data, u32 size) {
    ptr-=size;
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   -: 0x%016llx !  %16lld\n", (u64)ptr, (u64)size);
#endif
}

// releases global memory
void memory_free_v2(void* data, u32 size) {
    --ptr;
    ptr -= size;
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   -: 0x%016llx !  %16lld\n", (u64)ptr, (u64)size);
#endif
}

const struct memory_allocator memory_allocator_v1 = {
    .init = memory_init_v1,
    .destroy = memory_destroy_v1,
    .alloc = memory_alloc_v1,
    .free = memory_free_v1
};

const struct memory_allocator memory_allocator_v2 = {
    .init = memory_init_v2,
    .destroy = memory_destroy_v2,
    .alloc = memory_alloc_v2,
    .free = memory_free_v2
};
