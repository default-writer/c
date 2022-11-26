#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "alloc.h"
#include "typedefs.h"

void* _list_alloc(size_t nmemb, size_t size)
{
    void* ptr = calloc(nmemb, size);
    printf("!alloc: 0x%llx :%llx\n", (ADDR)ptr, (ADDR)size);
    return ptr;
}

void _list_free(void* ptr)
{
    if (ptr != 0) {
        printf("!free: 0x%llx\n", (ADDR)ptr);
    }
    free(ptr);
}
