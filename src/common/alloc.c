#include "common/alloc.h"

#ifdef USE_MEMORY_ALLOC
#include "playground/memory/memory.h"
#endif

void* _list_alloc(size_t nmemb, size_t size) {
#ifdef USE_MEMORY_ALLOC
    void* ptr = memory_alloc(nmemb, size);
#else

    void* ptr = calloc(nmemb, size);
#endif
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
    memset((void*)(u8*)ptr, 0, size); // NOLINT
#endif
#ifdef USE_MEMORY_ALLOC
    memory_free(ptr, size);
#else
    free(ptr);
#endif
}

void* _list_realloc(void* ptr, size_t size) {
    void* current = realloc(ptr, size);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   &: 0x%016llx !  %16lld\n", (u64)ptr, (u64)size);
#endif
    return current;
}