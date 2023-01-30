#include "playground/list/v2/list.h"
#include "std/common.h"

#define MAX_MEMORY 0xffff // 64K bytes

/*private */

static struct list_data* list_new() {
    return calloc(1, sizeof(struct list_data));
}

static void list_delete(struct list_data* pointer) {
    free(pointer);
    pointer = 0;
}

static void list_init(struct list_data* pointer) {
    pointer->base = pointer->ptr = calloc(MAX_MEMORY, sizeof(void*));
    pointer->max = pointer->base + MAX_MEMORY;
}

static void list_destroy(struct list_data* pointer) {
    free(pointer->base);
    pointer->base = pointer->ptr = 0;
}

static void* list_push(struct list_data* pointer, void* data) {
    if (pointer->ptr != pointer->max) {
        *pointer->ptr++ = data;
    }
    return pointer->ptr != pointer->max ? pointer->ptr : 0;
}

static void* list_pop(struct list_data* pointer) {
    return pointer->ptr != pointer->base ? *--pointer->ptr : 0;
}

static void* list_peek(struct list_data* pointer) {
    return pointer->ptr != pointer->base ? *(pointer->ptr - 1) : 0;
}

/* public */

const struct list list_v2 = {
    .new = list_new,
    .delete = list_delete,
    .init = list_init,
    .destroy = list_destroy,
    .push = list_push,
    .pop = list_pop,
    .peek = list_peek
};