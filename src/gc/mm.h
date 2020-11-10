#include <stddef.h>

/* memory manager: vtable definition */
struct mm_vtable {
    /* initialize mm context */
    const struct mm_context** (*init)();
    /* allocate block of memory */
    void* (*alloc)(size_t nmemb, size_t size);
    /* free block of memory */
    void (*free)(void*);
    /* destroy mm context */
    void (*destroy)(const struct mm_context**);
};

/* queue/list: vtable */
const struct mm_vtable mm_vt;
