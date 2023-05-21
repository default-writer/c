#include "memory_list_v3.h"
#include "common/alloc.h"

/* macros */
#define PTR_SIZE sizeof(void*) /* size of a pointer */

/* private */
static struct memory_ref** current;

void memory_list_init(void) {
    current = global_alloc(PTR_SIZE);
}

struct memory_ref* memory_list_peek(void) {
    return *current;
}

void memory_list_push(struct memory_ref* ptr) {
    struct memory_ref* current_ptr = *current;
    while (current_ptr != 0 && current_ptr->address_space < ptr->address_space) {
        current_ptr = current_ptr->cache; /* get next element in a list */
    }
    if (ptr != 0) {
        if (current_ptr == 0) {
            ptr->cache = *current;
            *current = ptr;
        } else {
            struct memory_ref* next_ptr = current_ptr->cache; /* get next to current element in a list */
            ptr->cache = next_ptr; /* inject ptr into a list */
            current_ptr->cache = ptr;
        }
    }
#ifdef USE_MEMORY_DEBUG_INFO
    struct memory_ref* tmp_ptr = *current;
    while (tmp_ptr != 0) {
        printf("  c.: %016llx . %016llx > %16lld ! %16lld\n", (u64)tmp_ptr, (u64)tmp_ptr->cache, tmp_ptr->address_space, tmp_ptr->size); /* NOLINT */
        tmp_ptr = tmp_ptr->cache;
    }
#endif
}

struct memory_ref* memory_list_pop(void) {
    struct memory_ref* ptr = *current;
    if (ptr != 0) {
        *current = ptr->cache;
    }
    return ptr;
}

void memory_list_destroy(void) {
    global_free(current, PTR_SIZE);
#ifdef USE_MEMORY_CLEANUP
    current = 0;
#endif
}
