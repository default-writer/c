#ifndef _COMMON_MEMORY_H_
#define _COMMON_MEMORY_H_

#include "std/common.h"

void* memory_alloc(size_t __nmemb, size_t __size);
void memory_free(void* ptr);

#endif // _COMMON_MEMORY_H_