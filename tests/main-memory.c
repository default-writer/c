#include "playground/memory/memory.h"
#include "std/common.h"

extern const struct memory_allocator memory_allocator_v1;
extern const struct memory_allocator memory_allocator_v2;

void use(const struct memory_allocator* allocator) {
    // initializes memory pool
    allocator->init();
    // allocation size aligned to 8 byte boundaries (64-bit pointers)
    u32 size = 2;
    // allocates memory block
    void** ptr = allocator->alloc(1, size);
    for(u32 i=0; i < size; i++) {
        *(ptr + i) = (void*)0xdeadbeefdeadbeef;
    }
    // allocation size aligned to 8 byte boundaries (64-bit pointers)
    u32 size2 = 3;
    // allocates memory block
    void** ptr2 = allocator->alloc(1, size2);
    for(u32 i=0; i < size2; i++) {
        *(ptr2 + i) = (void*)0xdeadbeefdeadbeef;
    }
    // allocation size aligned to 8 byte boundaries (64-bit pointers)
    u32 size3 = 16;
    // allocates memory block
    void** ptr3 = allocator->alloc(1, size3);
    for(u32 i=0; i < size3; i++) {
        *(ptr3 + i) = (void*)0xdeadbeefdeadbeef;
    }
    // releases memory block
    allocator->free(ptr3, size3);
    // releases memory block
    allocator->free(ptr2, size2);
    // releases memory block
    allocator->free(ptr, size);
    // destroys memory pool
    allocator->destroy();
}

int main() {
    use(&memory_allocator_v1);
    use(&memory_allocator_v2);
    return 0;
}