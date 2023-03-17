#include "playground/memory/api/v3/memory.h"

#include "common/lcg.h"

extern const struct memory_allocator memory_allocator_v6;

void use(const struct memory_allocator* allocator) {
    u64 index = 21;
    // initializes memory pool
    allocator->init(index);
    // allocation size aligned to 8 byte boundaries (64-bit pointers)
    u64 _size1 = 2;
    // allocates memory block
    void** _ptr1 = allocator->alloc(_size1);
    for (u64 i = 0; i < _size1; i++) {
        // generates random values
        *(_ptr1 + i) = (void*)lcg_parkmiller_64();
    }
    // releases memory block
    allocator->free(_ptr1);
    // allocation size aligned to 8 byte boundaries (64-bit pointers)
    u64 _size2 = 3;
    // allocates memory block
    void** _ptr2 = allocator->alloc(_size2);
    for (u64 i = 0; i < _size2; i++) {
        *(_ptr2 + i) = (void*)lcg_parkmiller_64();
    }
    // releases memory block
    allocator->free(_ptr2);
    // allocation size aligned to 8 byte boundaries (64-bit pointers)
    u64 _size3 = 16;
    // allocates memory block
    void** _ptr3 = allocator->alloc(_size3);
    for (u64 i = 0; i < _size3; i++) {
        *(_ptr3 + i) = (void*)lcg_parkmiller_64();
    }
    // releases memory block
    allocator->free(_ptr3);
    // allocation size aligned to 8 byte boundaries (64-bit pointers)
    u64 _size4 = 21;
    // allocates memory block
    void** _ptr4 = allocator->alloc(_size4);
    for (u64 i = 0; i < _size4; i++) {
        *(_ptr4 + i) = (void*)lcg_parkmiller_64();
    }
    // releases memory block
    allocator->free(_ptr4);
    // destroys memory pool
    allocator->destroy();
}

int main(void) {
    use(&memory_allocator_v6);
    return 0;
}
