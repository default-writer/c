#include "common/alloc.h"

#ifdef USE_MEMORY_ALLOC
#include "playground/memory/memory.h"
#endif

#ifdef USE_MEMORY_DEBUG_INFO
static u64 total_alloc = 0;
static u64 total_free = 0;
#endif

void* global_alloc(u64 size) {
    void* ptr = 0;
    if (size != 0) {
#ifdef USE_MEMORY_ALLOC
        ptr = memory_alloc(nmemb, size);
#else
        ptr = calloc(1, size);
#endif
#ifdef USE_MEMORY_DEBUG_INFO
        total_alloc += size;
        printf("   +: %016llx ! %16lld . %16lld : %16lld\n", (u64)ptr, size, total_free, total_alloc);
#endif
    }
    return ptr;
}

void global_free(void* ptr, u64 size) {
    if (ptr != 0) {
#ifdef USE_MEMORY_DEBUG_INFO
        total_free += size;
        printf("   -: %016llx ! %16lld . %16lld : %16lld\n", (u64)ptr, size, total_free, total_alloc);
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

#ifdef USE_MEMORY_DEBUG_INFO
void global_statistics(void) {
    printf("   !: %16lld : %16lld\n", total_free, total_alloc);
}
#endif

void* global_realloc(void* old_ptr, u64 size, u64 new_size) {
    void* ptr = old_ptr;
    if (ptr != 0 && new_size > size) {
        ptr = realloc(ptr, new_size);
#ifdef USE_MEMORY_DEBUG_INFO
        printf("   -: %016llx ! %16lld . %16lld : %16lld\n", (u64)old_ptr, size, total_free, total_alloc);
        total_alloc += new_size - size;
        printf("   +: %016llx ! %16lld . %16lld : %16lld\n", (u64)ptr, size, total_free, total_alloc);
#endif
    }
    return ptr;
}
