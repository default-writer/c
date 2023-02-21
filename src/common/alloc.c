#include "common/alloc.h"

#ifdef USE_MEMORY_ALLOC
#include "playground/memory/memory.h"
#endif

void* _list_alloc(u64 size) {
    void* ptr = 0;
    if (size != 0) {
#ifdef USE_MEMORY_ALLOC
        ptr = memory_alloc(nmemb, size);
#else
        ptr = calloc(1, size);
#endif
#ifdef USE_MEMORY_DEBUG_INFO
        printf("   +: 0x%016llx !  %16lld\n", (u64)ptr, size);
#endif
    }
    return ptr;
}

void _list_free(void* ptr, u64 size) {
    if (ptr != 0) {
#ifdef USE_MEMORY_DEBUG_INFO
        printf("   -: 0x%016llx !  %16lld\n", (u64)ptr, size);
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
}

void* _list_realloc(void* ptr, u64 size) {
    void* data = 0;
    if (ptr != 0 && size != 0) {
        data = realloc(ptr, size);
    }
    return data;
}
