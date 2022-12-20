#ifndef _COMMON_ALLOC_H_
#define _COMMON_ALLOC_H_

#include "std/common.h"

void* _list_alloc(size_t nmemb, size_t size);
void _list_free(void* ptr, size_t size);
void* _list_realloc(void *ptr, size_t size);

#endif // _COMMON_ALLOC_H_