#ifndef _ALLOC_H_
#define _ALLOC_H_

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void* _list_alloc(size_t nmemb, size_t size);
void _list_free(void* ptr);

#define NEW(size) _list_alloc(1, size)
#define FREE(ptr) _list_free(ptr)

#endif // _ALLOC_H_