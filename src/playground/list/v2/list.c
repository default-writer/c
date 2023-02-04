#include "playground/list/v2/list.h"

#define MAX_MEMORY 0xffff // 64K bytes

/*private */

static struct list_data* list_new() {
    struct list_data* ptr = calloc(1, sizeof(struct list_data));
    ptr->base = ptr->ptr = calloc(MAX_MEMORY, sizeof(void*));
    ptr->max = ptr->base + MAX_MEMORY;
    return ptr;
}

static void list_delete(struct list_data* pointer) {
    free(pointer->base);
    pointer->base = pointer->ptr = 0;
    free(pointer);
    pointer = 0;
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
    .push = list_push,
    .pop = list_pop,
    .peek = list_peek
};