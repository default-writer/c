#ifndef _COMMON_MEMORY_H_
#define _COMMON_MEMORY_H_

#include "std/common.h"

void memory_init();
void memory_destroy();

void* memory_alloc(u32 nmemb, u32 size);
void memory_free(void* ptr, u32 size);

#endif // _COMMON_MEMORY_H_