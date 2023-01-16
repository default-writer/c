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
        ptr = &memory;
        *ptr = calloc(1, MAX_MEMORY);
    }
    ptr = memory;
    *ptr = ptr + 1;
}

void memory_destroy() {
    if (memory != 0) {
        free(memory);
        memory = 0;
    }
    ptr = 0;
}

void* memory_alloc_v1(u32 nmemb, u32 size) {
    ptr = (void*)((u8*)ptr + size);
    *ptr = ptr;
    ++ptr;
    return ptr;
}

void* memory_alloc_v2(u32 nmemb, u32 size) {
    void** _ptr = *ptr;
    ptr = (void*)((u8*)ptr + size);
    ++ptr;
    *ptr = ptr + 1;
    return _ptr;
}

void memory_free_v1(void* data, u32 size) {
    --ptr;
    *ptr = 0;
    ptr = (void*)((u8*)ptr - size);
}

// releases global memory
void memory_free_v2(void* data, u32 size) {
    --ptr;
    *ptr = 0;
    ptr = (void*)((u8*)ptr - size);
}

void* (*memory_alloc)(u32 nmemb, u32 size) = memory_alloc_v2;
void (*memory_free)(void* data, u32 size) = memory_free_v2;