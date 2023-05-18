#include "playground/list/v2/list_v2.h"
#include "common/alloc.h"

/*private */
struct list_data {
    void** ptr;
    void** base;
    void** max;
    u64 size;
};

static struct list_data* list_alloc(u64 size);
static void list_free(struct list_data* pointer);
static void* list_push(struct list_data* pointer, void* data);
static void* list_pop(struct list_data* pointer);
static void* list_peek(struct list_data* pointer);

static struct list_data* list_alloc(u64 size) {
    struct list_data* ptr = global_alloc(sizeof(struct list_data));
    ptr->base = global_alloc(size * sizeof(void*));
    ptr->ptr = ptr->base;
    ptr->max = ptr->base + size;
    ptr->size = size;
    return ptr;
}

static void list_free(struct list_data* pointer) {
    global_free(pointer->base, pointer->size * sizeof(void*));
    pointer->base = 0;
    pointer->ptr = 0;
    pointer->max = 0;
    global_free(pointer, sizeof(struct list_data));
    pointer = 0;
}

static void* list_push(struct list_data* pointer, void* data) {
    if (pointer->ptr != pointer->max) {
        *pointer->ptr++ = data;
    }
    return pointer->ptr != pointer->max ? pointer->ptr : 0;
}

static void* list_pop(struct list_data* pointer) {
    void* tmp = 0;
    if (pointer->ptr != pointer->base) {
        tmp = *--pointer->ptr;
        *pointer->ptr = 0;
    }
    return tmp;
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
