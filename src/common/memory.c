#include "common/memory.h"
#include "std/common.h"

/*
    Memory allocation works as follows:

    1. first 8 bytes addressed by pointer should actually be equal to address being returned

*/

void* memory_alloc(u32 nmemb, u32 size) {
    return calloc(nmemb, size);
}

void memory_free(void* ptr, u32 size) {
    free(ptr);
}