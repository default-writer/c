#include "common/alloc.h"
#include "common/memory.h"
#include "std/common.h"

void* _list_alloc(size_t nmemb, size_t size) {
    void* ptr = memory_alloc(nmemb, size);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   +: 0x%016llx !  %16lld\n", (u64)ptr, (u64)size);
#endif
    return ptr;
}

void _list_free(void* ptr, size_t size) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   -: 0x%016llx !  %16lld\n", (u64)ptr, (u64)size);
#endif
#ifdef USE_MEMORY_CLEANUP
    memset((void*)(u8*)ptr, 0, size);
#endif
    memory_free(ptr, size);
}

void* _list_realloc(void* ptr, size_t size) {
    void* current = realloc(ptr, size);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   &: 0x%016llx !  %16lld\n", (u64)ptr, (u64)size);
#endif
    return current;
}