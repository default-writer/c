#include "playground/list/list.h"
#include "std/common.h"

#define MAX_MEMORY 0xffff // 64K bytes

/*private */

// global allocated memory
static void* list = 0;
static void** ptr = 0;

static void list_init() {
    ptr = &list;
    *ptr = calloc(1, MAX_MEMORY);
    ptr = *ptr;
}

static void list_destroy() {
    ptr = 0;
    free(list);
    list = 0;
}

static void list_push(void* data) {
    void** tmp = ptr;
    ++ptr;
    *tmp = data;
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   +: 0x%016llx >0x%016llx\n", (u64)tmp, (u64)*tmp);
#endif
}

static void* list_pop() {
    --ptr;
    void** tmp = ptr;
    void* data = *tmp;
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   -: 0x%016llx >0x%016llx\n", (u64)tmp, (u64)*tmp);
#endif
#ifdef USE_MEMORY_CLEANUP
    *tmp = 0;
#endif
    return data;
}

static void* list_peek() {
    void** tmp = ptr - 1;
    void* data = *tmp;
#ifdef USE_MEMORY_DEBUG_INFO
    printf("   *: 0x%016llx >0x%016llx\n", (u64)tmp, (u64)*tmp);
#endif
    return data;
}

/* public */

const struct list list_v1 = {
    .init = list_init,
    .destroy = list_destroy,
    .push = list_push,
    .pop = list_pop,
    .peek = list_peek
};