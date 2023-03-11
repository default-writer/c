#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

#include "std/common.h"

void* _alloc(u64 size);
void _free(void* ptr, u64 size);
void* _realloc(void* ptr, u64 size);

#endif // _ALLOCATOR_H_
