#ifndef _COMMON_ALLOC_H_
#define _COMMON_ALLOC_H_

#include "std/common.h"

void* _list_alloc(u64 size);
void _list_free(void* ptr, u64 size);
void* _list_realloc(void* ptr, u64 size, u64 new_size);

#endif /* _COMMON_ALLOC_H_ */
