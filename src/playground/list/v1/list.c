#include "playground/list/v1/list.h"
#include "std/common.h"

#define MAX_MEMORY 0xffff // 64K bytes

/*private */

// global allocated memory
static void** ptr = 0;
static void* base = 0;

static void list_init() {
    base = ptr = calloc(1, MAX_MEMORY);
}

static void list_destroy() {
    free(base);
    ptr = 0;
    base = 0;
}

static void list_push(void* data) {
    *ptr++ = data;
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   +: 0x%016llx >0x%016llx\n", (u64)ptr - 1, (u64) * (ptr - 1));
#endif
}

static void* list_pop() {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   -: 0x%016llx >0x%016llx\n", (u64)ptr - 1, (u64) * (ptr - 1));
#endif
    return *--ptr;
}

static void* list_peek() {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   *: 0x%016llx >0x%016llx\n", (u64)ptr - 1, (u64) * (ptr - 1));
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