#include "std/common.h"
#include "common/alloc.h"

void* _list_alloc(size_t nmemb, size_t size) {
    void* ptr = calloc(nmemb, size);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("+: 0x%llx :%llx\n", (ADDR)ptr, (ADDR)size);
#endif
    return ptr;
}

void _list_free(void* ptr, size_t size) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("-: 0x%llx\n", (ADDR)ptr);
#endif
#ifdef USE_MEMORY_CLEANUP
    memset((void*)(byte*)ptr, 0, size);
#endif
    free(ptr);
}