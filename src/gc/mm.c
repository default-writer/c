#include <stdio.h>
#include <stdlib.h>

#include "mm.h"
#include "list-micro/api.h"

/* default mm methods */
const struct mm_context** mm_init();
void* mm_alloc(size_t nmemb, size_t size);
void mm_free(void* payload);
void mm_destroy(const struct mm_context**);

/* mm vtable */
const struct mm_vtable mm_vt = {
    .init = mm_init,
    .alloc = mm_alloc,
    .free = mm_free,
    .destroy = mm_destroy
};

struct mm_context {
    struct list* head;
};

/* initialize memory manager */
const struct mm_context** mm_init() {
    /* initialize current context (stack) */
    struct mm_context* ctx = (struct mm_context*)calloc(1, sizeof(struct mm_context));
    return &ctx;
}

/* allocate memory block */
void* mm_alloc(size_t nmemb, size_t size) {
    return NULL;
}

/* free memory block */
void mm_free(void* ptr) {

}

/* cleanup memory manager */
void mm_destroy(const struct mm_context** ctx) {
    free(&ctx);
}