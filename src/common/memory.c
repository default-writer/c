#include "common/memory.h"
#include "std/common.h"
#include <stdlib.h>

#define MAX_MEMORY 0xfffffff // 256M bytes

// global allocated memory
static void* memory = 0;
static void** ptr = 0;
/*
    Memory allocation works as follows:

    1. 8 bytes addressed by pointer should be equals to the address itself being referenced
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
    if (memory == 0) {
        memory = calloc(1, MAX_MEMORY);
    }
    ptr = memory;
}

void memory_destroy() {
    if (memory != 0) {
        free(memory);
        memory = 0;
    }
    ptr = memory;
}

void* memory_alloc(u32 nmemb, u32 size) {
    // writes down address of start of free memory pool
    *ptr = ptr;
    // decreases the free address space to the requested amount
    *ptr = (u8*)*ptr + size;
    // moves pointer to the actual starting address
    ++ptr;
    // advances position
    ptr = *(ptr - 1);
    // writes down address
    *(ptr - 1) = ptr;
    // returns free memory block
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   +: 0x%016llx !  %16lld\n", (u64)ptr, (u64)nmemb * size);
#endif
    return ptr;
}

// releases global memory
void memory_free(void* data, u32 size) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   -: 0x%016llx !  %16lld\n", (u64)data, (u64)size);
#endif
    // initializes temporary pointer
    void** _ptr = data;
    // advances pointer to service record
    --_ptr;
    // erases the service record
    *_ptr = 0;
    // increases the free address space to the requested amount
    *ptr = (u8*)*ptr - size;
    // updates current allocation position
    --ptr;
}