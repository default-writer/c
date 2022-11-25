#ifndef _ALLOC_H_
#define _ALLOC_H_

#ifdef DEBUG

/* address type (for debugging printf function) */
typedef long long unsigned int ADDR;

/* Define a custom `malloc` function. */
void* _alloc(size_t nmemb, size_t size)
{
    void* ptr = calloc(nmemb, size);
    printf("!alloc: 0x%llx :%llx\n", (ADDR)ptr, (ADDR)size);
    return ptr;
}

/* Define a custom `free` function. */
void _free(void* ptr)
{
    if (ptr != 0) {
        printf("!free: 0x%llx\n", (ADDR)ptr);
    }
    free(ptr);
}

/* Override the default `malloc` function used by Rexo with ours. */
#define _LIST_ALLOC _alloc

/* Override the default `free` function used by Rexo with ours. */
#define _LIST_FREE _free

#else

#define _LIST_ALLOC calloc
#define _LIST_FREE free

#endif

#define ALLOC_N(n, type, size) (struct type*)_LIST_ALLOC(n, size)
#define ALLOC(type, size) (struct type*)_LIST_ALLOC(1, size)
#define NEW(size) _LIST_ALLOC(1, size)
#define FREE(ptr) _LIST_FREE(ptr)

#endif