#include "common/memory.h"
#include "std/common.h"

int main() {
    // initializes memory pool
    memory_init();
    // allocation size aligned to 8 byte boundaries (64-bit pointers)
    u32 size = 8;
    // allocates memory block
    void* ptr = memory_alloc(1, size);
    // will check for service record!!!
    void** sptr = ptr;
    // rewinds the memory pointer "below" the 0 address, i.e. -8
    --sptr;
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   =: 0x%016llx !0x%016llx\n", (u64)sptr, (u64)*sptr);
#endif
    // allocation size aligned to 8 byte boundaries (64-bit pointers)
    u32 size2 = 128;
    // allocates memory block
    void* ptr2 = memory_alloc(1, size2);
    // will check for service record!!!
    void** sptr2 = ptr2;
    // rewinds the memory pointer "below" the 0 address, i.e. -8
    --sptr2;
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   =: 0x%016llx !0x%016llx\n", (u64)sptr2, (u64)*sptr2);
#endif
    // releases memory block
    memory_free(ptr2, size2);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   >: 0x%016llx !0x%016llx\n", (u64)sptr2, (u64)*sptr2);
#endif
    // releases memory block
    memory_free(ptr, size);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   >: 0x%016llx !0x%016llx\n", (u64)sptr, (u64)*sptr);
#endif
    // destroys memory pool
    memory_destroy();
    return 0;
}