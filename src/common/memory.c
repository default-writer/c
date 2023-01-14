#include "common/memory.h"
#include "list-experimental/data.h"
#include "std/list.h"

#define MEMORY_ALLOC_SIZE 0xffffff
#define MEMORY_N_ELEMENTS 0xff

void* memory_alloc(size_t nmemb, size_t size) {
    return calloc(nmemb, size);
}

void memory_free(void* ptr) {
    free(ptr);
}