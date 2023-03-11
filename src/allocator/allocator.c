#include "common/alloc.h"

#include "allocator.h"

void* _alloc(u64 size);
void _free(void* ptr, u64 size);
void* _realloc(void* ptr, u64 size);

void* _alloc(u64 size) {
    CLEAN(size)
    return 0;
}

void _free(void* ptr, u64 size) {
    CLEAN(ptr)
    CLEAN(size)
}

void* _realloc(void* ptr, u64 size) {
    CLEAN(ptr)
    CLEAN(size)
    return 0;
}
