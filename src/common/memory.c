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

void memory_init() {
    ptr = &memory;
    *ptr = calloc(1, MAX_MEMORY);
    ptr = *ptr;
    *ptr = ptr + 1;
}

void memory_destroy() {
    free(memory);
    memory = 0;
    ptr = 0;
}

void* memory_alloc_v1(u32 nmemb, u32 size) {
    ptr += size;
    *ptr = ptr;
    ++ptr;
    return ptr;
}

void* memory_alloc_v2(u32 nmemb, u32 size) {
    void** tmp = *ptr;
    ptr += size;
    ++ptr;
    *ptr = ptr + 1;
    return tmp;
}

void memory_free_v1(void* data, u32 size) {
    --ptr;
#ifdef USE_MEMORY_CLEANUP
    *ptr = 0;
#endif
    ptr -= size;
}

// releases global memory
void memory_free_v2(void* data, u32 size) {
#ifdef USE_MEMORY_CLEANUP
// assert data + size == ptr
    void** tmp = (void**)data + size;
    *tmp = 0;
    // assert ptr - data == size
    memset(data, 0, size);
#endif
    --ptr;
    ptr -= size;
}

void* (*memory_alloc)(u32 nmemb, u32 size);
void (*memory_free)(void* data, u32 size);