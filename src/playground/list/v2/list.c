#include "playground/list/v2/list.h"

#define DEFAULT_SIZE 0xffff // 64K bytes

/*private */

static struct list_data* list_alloc(u64 size);
static void list_free(struct list_data* pointer);
static void* list_push(struct list_data* pointer, void* data);
static void* list_pop(struct list_data* pointer);
static void* list_peek(struct list_data* pointer);

static struct list_data* list_alloc(u64 size) {
    if (size == 0) {
        size = DEFAULT_SIZE;
    }
    struct list_data* ptr = calloc(1, sizeof(struct list_data));
    ptr->base = calloc(size, sizeof(void*));
    ptr->ptr = ptr->base;
    ptr->max = ptr->base + size;
    return ptr;
}

static void list_free(struct list_data* pointer) {
    free(pointer->base);
    pointer->base = 0;
    pointer->ptr = 0;
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
    .alloc = list_alloc,
    .free = list_free,
    .push = list_push,
    .pop = list_pop,
    .peek = list_peek
};