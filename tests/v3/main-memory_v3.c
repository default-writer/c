#include "playground/memory/api/v2/memory.h"

extern const struct memory_allocator memory_allocator_v5;

void use(const struct memory_allocator* allocator) {
    // initializes memory pool
    allocator->init();
    // allocation size aligned to 8 byte boundaries (64-bit pointers)
    u64 size1 = 2;
    // allocates memory block
    void** ptr1 = allocator->alloc(size1);
    for (u64 i = 0; i < size1; i++) {
        *(ptr1 + i) = (void*)0xdeadbeefdeadbeef;
    }
    // allocation size aligned to 8 byte boundaries (64-bit pointers)
    u64 size2 = 3;
    // allocates memory block
    void** ptr2 = allocator->alloc(size2);
    for (u64 i = 0; i < size2; i++) {
        *(ptr2 + i) = (void*)0xdeadbeefdeadbeef;
    }
    // allocation size aligned to 8 byte boundaries (64-bit pointers)
    u64 size3 = 16;
    // allocates memory block
    void** ptr3 = allocator->alloc(size3);
    for (u64 i = 0; i < size3; i++) {
        *(ptr3 + i) = (void*)0xdeadbeefdeadbeef;
    }
    // releases memory block
    allocator->free(ptr2);
    // releases memory block
    allocator->free(ptr3);
    // allocation size aligned to 8 byte boundaries (64-bit pointers)
    u64 size4 = 16;
    // allocates memory block
    void** ptr4 = allocator->alloc(size4);
    for (u64 i = 0; i < size4; i++) {
        *(ptr4 + i) = (void*)0xdeadbeefdeadbeef;
    }
    // releases memory block
    allocator->free(ptr4);
    // releases memory block
    allocator->free(ptr1);
    // destroys memory pool
    allocator->destroy();
}

int main(void) {
    use(&memory_allocator_v5);
    return 0;
}