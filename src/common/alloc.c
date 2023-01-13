#include "common/alloc.h"
#include "std/common.h"

void* _list_alloc(size_t nmemb, size_t size) {
    void* ptr = calloc(nmemb, size);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("+: 0x%016llx !0x%08llx\n", (__u_int64_t)ptr, (__u_int64_t)size);
#endif
    return ptr;
}

void _list_free(void* ptr, size_t size) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("-: 0x%016llx !0x%08llx\n", (__u_int64_t)ptr, (__u_int64_t)size);
#endif
#ifdef USE_MEMORY_CLEANUP
    memset((void*)(__u_int8_t*)ptr, 0, size);
#endif
    free(ptr);
}

void* _list_realloc(void* ptr, size_t size) {
    void* current = realloc(ptr, size);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("&: 0x%016llx !0x%08llx\n", (__u_int64_t)ptr, (__u_int64_t)size);
#endif
    return current;
}