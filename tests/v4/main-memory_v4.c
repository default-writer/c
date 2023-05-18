#include "common/alloc.h"
#include "common/lcg.h"
#include "playground/memory/api/memory.h"

extern const struct memory_allocator_v3 memory_allocator_v6;

void use(const struct memory_allocator_v3* allocator) {
    u64 index = 21;
    /* initializes memory pool */
    allocator->init(index);
    /* allocation size aligned to 8 byte boundaries (64-bit pointers) */
    u64 _size1 = 2;
    /* allocates memory block */
    void** _ptr1 = allocator->alloc(_size1);
    for (u64 i = 0; i < _size1; i++) {
        /* generates random values */
        *(_ptr1 + i) = (void*)lcg_parkmiller_64();
    }
    /* releases memory block */
    allocator->free(_ptr1);
    /* allocation size aligned to 8 byte boundaries (64-bit pointers) */
    u64 _size2 = 3;
    /* allocates memory block */
    void** _ptr2 = allocator->alloc(_size2);
    for (u64 i = 0; i < _size2; i++) {
        *(_ptr2 + i) = (void*)lcg_parkmiller_64();
    }
    /* releases memory block */
    allocator->free(_ptr2);
    /* allocation size aligned to 8 byte boundaries (64-bit pointers) */
    u64 _size3 = 16;
    /* allocates memory block */
    void** _ptr3 = allocator->alloc(_size3);
    for (u64 i = 0; i < _size3; i++) {
        *(_ptr3 + i) = (void*)lcg_parkmiller_64();
    }
    /* releases memory block */
    allocator->free(_ptr3);
    /* allocation size aligned to 8 byte boundaries (64-bit pointers) */
    u64 _size4 = 21;
    /* allocates memory block */
    void** _ptr4 = allocator->alloc(_size4);
    for (u64 i = 0; i < _size4; i++) {
        *(_ptr4 + i) = (void*)lcg_parkmiller_64();
    }
    /* releases memory block */
    allocator->free(_ptr4);
    /* allocation size aligned to 8 byte boundaries (64-bit pointers) */
    u64 _size5 = 22;
    /* allocates memory block */
    void** _ptr5 = allocator->alloc(_size5);
    for (u64 i = 0; i < _size5; i++) {
        *(_ptr5 + i) = (void*)lcg_parkmiller_64();
    }
    /* releases memory block */
    allocator->free(_ptr5);
    /* allocation size aligned to 8 byte boundaries (64-bit pointers) */
    u64 _size6 = 21;
    /* allocates memory block */
    void** _ptr6 = allocator->alloc(_size6);
    for (u64 i = 0; i < _size6; i++) {
        *(_ptr6 + i) = (void*)lcg_parkmiller_64();
    }
    /* releases memory block */
    allocator->free(_ptr6);
    /* allocation size aligned to 8 byte boundaries (64-bit pointers) */
    u64 _size7 = 20;
    /* allocates memory block */
    void** _ptr7 = allocator->alloc(_size7);
    for (u64 i = 0; i < _size7; i++) {
        *(_ptr7 + i) = (void*)lcg_parkmiller_64();
    }
    /* releases memory block */
    allocator->free(_ptr7);
    /* destroys memory pool */
    allocator->destroy();
}

int main(void) {
    use(&memory_allocator_v6);
#ifdef USE_MEMORY_DEBUG_INFO
    global_statistics();
#endif
    return 0;
}
