#ifndef _COMMON_ALLOC_H_
#define _COMMON_ALLOC_H_

#include "std/common.h"

void* global_alloc(u64 size);
void global_free(void* ptr, u64 size);
void* global_realloc(void* ptr, u64 size, u64 new_size);
#ifdef USE_MEMORY_DEBUG_INFO
void global_statistics(void);
#endif

#endif /* _COMMON_ALLOC_H_ */
