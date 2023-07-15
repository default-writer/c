#include "common/alloc.h"
#include "playground/memory/api/memory.h"

extern const union memory_allocator_api memory_allocator_v1;
extern const union memory_allocator_api memory_allocator_v2;
extern const union memory_allocator_api memory_allocator_v3;

void use(const union memory_allocator_api* allocator) {
    /* initializes memory pool */
    allocator->v1.init();
    /* allocation size aligned to 8 byte boundaries (64-bit pointers) */
    u64 size = 2;
    /* allocates memory block */
    void** ptr = allocator->v1.alloc(size);
    for (u64 i = 0; i < size; i++) {
        *(ptr + i) = (void*)0xdeadbeefdeadbeef;
    }
    /* allocation size aligned to 8 byte boundaries (64-bit pointers) */
    u64 size2 = 3;
    /* allocates memory block */
    void** ptr2 = allocator->v1.alloc(size2);
    for (u64 i = 0; i < size2; i++) {
        *(ptr2 + i) = (void*)0xdeadbeefdeadbeef;
    }
    /* allocation size aligned to 8 byte boundaries (64-bit pointers) */
    u64 size3 = 16;
    /* allocates memory block */
    void** ptr3 = allocator->v1.alloc(size3);
    for (u64 i = 0; i < size3; i++) {
        *(ptr3 + i) = (void*)0xdeadbeefdeadbeef;
    }
    /* releases memory block */
    allocator->v1.free(ptr3, size3);
    /* releases memory block */
    allocator->v1.free(ptr2, size2);
    /* releases memory block */
    allocator->v1.free(ptr, size);
    /* destroys memory pool */
    allocator->v1.destroy();
}

static void INIT init() {
#ifdef USE_MEMORY_DEBUG_INFO
    global_statistics();
#endif
}

static void DESTROY destroy() {
#ifdef USE_MEMORY_DEBUG_INFO
    global_statistics();
#endif
}

int main(void) {
#ifndef ATTRIBUTE
    init();
#endif
    use(&memory_allocator_v1);
    use(&memory_allocator_v2);
    use(&memory_allocator_v3);
#ifndef ATTRIBUTE
    destroy();
#endif
    return 0;
}
