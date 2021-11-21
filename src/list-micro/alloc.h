#ifdef DEBUG

/* address type (for debugging printf function) */
typedef long long unsigned int ADDR;

/* Define a custom `malloc` function. */
void* _alloc(size_t nmemb, size_t size)
{
    void* ptr = calloc(nmemb, size);
    printf("!alloc: 0x%llx :%ld\n", (ADDR)ptr, size);
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

#define ALLOC(size, type) (type*)_LIST_ALLOC(1, sizeof(type))
#define FREE(ptr) _LIST_FREE(ptr)