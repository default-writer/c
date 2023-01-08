#include "std/common.h"
#include "common/alloc.h"

inline void* _list_alloc(size_t nmemb, size_t size) {
    void* ptr = calloc(nmemb, size);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("+: 0x%016llx !0x%08llx\n", (LPTR)ptr, (LPTR)size);
#endif
    return ptr;
}

inline void _list_free(void* ptr, size_t size) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("-: 0x%016llx !0x%08llx\n", (LPTR)ptr, (LPTR)size);
#endif
#ifdef USE_MEMORY_CLEANUP
    memset((void*)(BYTE*)ptr, 0, size);
#endif
    free(ptr);
}

inline void* _list_realloc(void *ptr, size_t size) {
    void* current = realloc(ptr, size);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("&: 0x%016llx !0x%08llx\n", (LPTR)ptr, (LPTR)size);
#endif
    return current;
}