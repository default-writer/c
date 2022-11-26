#ifndef _ALLOC_H_
#define _ALLOC_H_

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef USE_MEMORY_DEBUG_INFO

/* Define a custom `malloc` function. */
void* _list_alloc(size_t nmemb, size_t size);

/* Define a custom `free` function. */
void _list_free(void* ptr);

/* Override the default `malloc` function used by Rexo with ours. */
#define _LIST_ALLOC _list_alloc

/* Override the default `free` function used by Rexo with ours. */
#define _LIST_FREE _list_free

#else

#define _LIST_ALLOC calloc
#define _LIST_FREE free

#endif

#define ALLOC_N(n, type, size) (struct type*)_LIST_ALLOC(n, size)
#define ALLOC(type, size) (struct type*)_LIST_ALLOC(1, size)
#define NEW(size) _LIST_ALLOC(1, size)
#define FREE(ptr) _LIST_FREE(ptr)

#endif // _ALLOC_H_