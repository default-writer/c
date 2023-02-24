#include "playground/memory/memory.h"

#define MAX_MEMORY 0xffff // 64K bytes

// global allocated memory
static void* memory = 0;
static void** ptr = 0;

static void memory_init(void);
static void memory_destroy(void);
static void* memory_alloc(u64 size);
static void memory_free(void* data, u64 size);

static void* alloc(void** next, void* prev, u64 size) {
    void** tmp = calloc(1, (size + 3) * sizeof(void*)); //
    next = *next;
    *next = tmp;

    *tmp = prev;
    *(tmp + 1) = tmp + size + 2;
    *(tmp + 2) = next;

    return tmp + 2;
}

static u64 offset(void* data) {
    void** head = data;
    void** next = *(head - 1);
    return (u64)(next - head);
}

static void memory_init(void) {
    ptr = &memory;
    *ptr = ptr;
    ptr = alloc(ptr, 0, 0);
}

static void memory_destroy(void) {
    free(memory);
    memory = 0;
    ptr = 0;
}

static void* memory_alloc(u64 size) {
    ptr = alloc(ptr - 1, ptr, size);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("  0+: 0x%016llx >0x%016llx\n", (u64)ptr, (u64)(*ptr));
#endif
    return ptr;
}

// releases global memory
static void memory_free(void* data, u64 size) {
    void** head = data;
    void** next = *(head - 1);
    void** last = *(head - 2);
    if (*next == 0) {
        next = last + offset(last);
        *next = 0;
    }
    CLEAN(size)
    size = offset(data);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("  0-: 0x%016llx !  %16lld\n", (u64)last, size);
#endif
#ifdef USE_MEMORY_CLEANUP
    memset(head - 2, 0, (size + 3) * sizeof(void*)); // NOLINT
#endif
    free(head - 2);
}

const struct memory_allocator memory_allocator_v3 = {
    .init = memory_init,
    .destroy = memory_destroy,
    .alloc = memory_alloc,
    .free = memory_free
};
