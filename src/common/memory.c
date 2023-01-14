#include "common/memory.h"
#include "std/common.h"

void* memory_alloc(size_t nmemb, size_t size) {
    return calloc(nmemb, size);
}

void memory_free(void* ptr) {
    free(ptr);
}