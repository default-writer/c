#ifndef _ALLOC_H_
#define _ALLOC_H_

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void* _list_alloc(size_t nmemb, size_t size);
void _list_free(void* ptr);

#endif // _ALLOC_H_