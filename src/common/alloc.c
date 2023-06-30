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
#ifdef USE_MEMORY_CLEANUP
        global_memset(ptr, 0, size); /* NOLINT */
#endif
#ifdef USE_MEMORY_DEBUG_INFO
        total_free += size;
        printf("   -: %016llx ! %16lld . %16lld : %16lld\n", (u64)ptr, size, total_free, total_alloc);
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

void* global_memset(void* dest, u8 c, size_t count) {
    size_t block_idx = 0;
    size_t blocks = count >> 3;
    size_t bytes_left = count - (blocks << 3);

    u64 c_ull = c
        | (((u64)c) << 8)
        | (((u64)c) << 16)
        | (((u64)c) << 24)
        | (((u64)c) << 32)
        | (((u64)c) << 40)
        | (((u64)c) << 48)
        | (((u64)c) << 56);

    u64* dest_ptr8 = (u64*)dest;
    for (block_idx = 0; block_idx < blocks; block_idx++)
        dest_ptr8[block_idx] = c_ull;

    if (!bytes_left)
        return dest;

    u8* dest_ptr1 = (u8*)&dest_ptr8[block_idx];
    for (block_idx = 0; block_idx < bytes_left; block_idx++)
        dest_ptr1[block_idx] = (u8)c_ull;

    return dest;
}