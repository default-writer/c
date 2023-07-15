#include "playground/list/v1/list_v1.h"
#include "common/alloc.h"

#define MAX_MEMORY 0xffff /* 64K bytes */

/* private */

/* global allocated memory */
static void** ptr = 0;
static void* base = 0;

static void list_init(void) {
    base = ptr = global_alloc(MAX_MEMORY);
}

static void list_destroy(void) {
    global_free(base, MAX_MEMORY);
    ptr = 0;
    base = 0;
}

static void list_push(void* data) {
    *ptr++ = data;
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   >: %016llx > %016llx\n", (u64)ptr - 1, (u64) * (ptr - 1));
#endif
}

static void* list_pop(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   <: %016llx > %016llx\n", (u64)ptr - 1, (u64) * (ptr - 1));
#endif
    return *--ptr;
}

static void* list_peek(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   *: %016llx > %016llx\n", (u64)ptr - 1, (u64) * (ptr - 1));
#endif
    return *(ptr - 1);
}

/* public */

const struct list list_v1 = {
    .init = list_init,
    .destroy = list_destroy,
    .push = list_push,
    .pop = list_pop,
    .peek = list_peek
};
