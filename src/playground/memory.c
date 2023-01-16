#include "common/memory.h"
#include "std/common.h"

void use() {
    // initializes memory pool
    memory_init();
    // allocation size aligned to 8 byte boundaries (64-bit pointers)
    u32 size = 2;
    // allocates memory block
    void** ptr = memory_alloc(1, size);
    for(u32 i=0; i < size; i++) {
        *(ptr + i) = (void*)0xdeadbeefdeadbeef;
    }
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   +: 0x%016llx !0x%016llx\n", (u64)ptr, (u64)*(ptr + size));
#endif
    // allocation size aligned to 8 byte boundaries (64-bit pointers)
    u32 size2 = 3;
    // allocates memory block
    void** ptr2 = memory_alloc(1, size2);
    for(u32 i=0; i < size2; i++) {
        *(ptr2 + i) = (void*)0xdeadbeefdeadbeef;
    }
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   +: 0x%016llx !0x%016llx\n", (u64)ptr2, (u64)*(ptr2 + size2));
#endif
    // allocation size aligned to 8 byte boundaries (64-bit pointers)
    u32 size3 = 16;
    // allocates memory block
    void** ptr3 = memory_alloc(1, size3);
    for(u32 i=0; i < size3; i++) {
        *(ptr3 + i) = (void*)0xdeadbeefdeadbeef;
    }
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   +: 0x%016llx !0x%016llx\n", (u64)ptr3, (u64)*(ptr3 + size3));
#endif
    //
    // end 
    //
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   -: 0x%016llx !  %16lld\n", (u64)ptr3, (u64)size3);
#endif
    // releases memory block
    memory_free(ptr3, size3);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   -: 0x%016llx !  %16lld\n", (u64)ptr2, (u64)size2);
#endif
    // releases memory block
    memory_free(ptr2, size2);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   -: 0x%016llx !  %16lld\n", (u64)ptr, (u64)size);
#endif
    // releases memory block
    memory_free(ptr, size);
    // destroys memory pool
    memory_destroy();
}

int main() {
    memory_init = memory_init_v1;
    memory_destroy = memory_destroy_v1;
    memory_alloc = memory_alloc_v1;
    memory_free = memory_free_v1;
    use();
    memory_init = memory_init_v2;
    memory_destroy = memory_destroy_v2;
    memory_alloc = memory_alloc_v2;
    memory_free = memory_free_v2;
    use();
    return 0;
}