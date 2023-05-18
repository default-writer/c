#include "memory_list_v2.h"
#include "common/alloc.h"

/* private */
static struct memory_ref** current;

void memory_list_init(void) {
    current = global_alloc(sizeof(void*));
}

struct memory_ref* memory_list_peek(void) {
    return *current;
}

void memory_list_push(struct memory_ref* ptr) {
    if (ptr != 0) {
        ptr->cache = *current;
    }
    *current = ptr;
}

struct memory_ref* memory_list_pop(void) {
    struct memory_ref* ptr = *current;
    if (ptr != 0) {
        *current = ptr->cache;
    }
    return ptr;
}

void memory_list_destroy(void) {
    global_free(current, sizeof(void*));
#ifdef USE_MEMORY_CLEANUP
    current = 0;
#endif
}
