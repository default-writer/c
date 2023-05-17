#include "common/alloc.h"

#ifdef USE_MEMORY_ALLOC
#include "playground/memory/memory.h"
#endif

#ifdef USE_MEMORY_DEBUG_INFO
static u64 global_allocated = 0;
static u64 global_freed = 0;
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
        global_allocated += size;
        printf("   +: 0x%016llx !  %16lld .  %16lld :  %16lld\n", (u64)ptr, size, global_freed, global_allocated);
#endif
    }
    return ptr;
}

void _list_free(void* ptr, u64 size) {
    if (ptr != 0) {
#ifdef USE_MEMORY_DEBUG_INFO
        global_freed += size;
        printf("   -: 0x%016llx !  %16lld .  %16lld :  %16lld\n", (u64)ptr, size, global_freed, global_allocated);
#endif
#ifdef USE_MEMORY_CLEANUP
        memset((void*)(u8*)ptr, 0, size); /* NOLINT */
#endif
#ifdef USE_MEMORY_ALLOC
        memory_free(ptr, size);
#else
        free(ptr);
#endif
    }
}

void* _list_realloc(void* ptr, u64 size, u64 new_size) {
    void* data = 0;
    if (ptr != 0 && new_size > size) {
#ifdef USE_MEMORY_DEBUG_INFO
        global_allocated += new_size - size;
#endif
        data = realloc(ptr, new_size);
    }
    return data;
}
