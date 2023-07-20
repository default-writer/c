#ifndef _COMMON_ALLOC_H_
#define _COMMON_ALLOC_H_

#include "std/common.h"

void* global_alloc(u64 size);
void global_free(void* ptr, u64 size);
void* global_realloc(void* ptr, u64 size, u64 new_size);
void* global_memset(void* dest, u8 c, size_t count);
#ifdef USE_MEMORY_DEBUG_INFO
void global_statistics(void);
#endif

struct memory_info {
    u64 used_memory;
    u64 free_memory;
    u64 maximum_used;
    u64 allocated_memory;
};

const struct memory_info* global_memory_info(void);

#endif /* _COMMON_ALLOC_H_ */
