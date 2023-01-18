#include "playground/list/list.h"
#include "std/common.h"

#define MAX_MEMORY 0xffff // 64K bytes

/*private */

// global allocated memory
static void** ptr = 0;

static void list_init() {
    ptr = calloc(1, MAX_MEMORY);
}

static void list_destroy() {
    free(ptr);
    ptr = 0;
}

static void list_push(void* data) {
    *ptr++ = data;
}

static void* list_pop() {
    return *--ptr;
}

static void* list_peek() {
    return *(ptr - 1);
}

/* public */

const struct list list_v2 = {
    .init = list_init,
    .destroy = list_destroy,
    .push = list_push,
    .pop = list_pop,
    .peek = list_peek
};