#include "std/common.h"
#include "common/alloc.h"
#include "common/object.h"

/* initializes the new context's head element */
/* as a result, new memory block will be allocated */
/* current context pointer set to zero */
void list_init(struct list_data** const current, struct list_data* (*list_new)()) {

    const struct list_data* tmp = *current;
    if (tmp == 0) {
        /* sets current context's head element */
        *current = list_new();
    }
}

/* destroys the memory stack */
/* frees all memory elements */
/* as a result, memory will be freed */
void list_destroy(struct list_data** const current, void (*list_delete)(struct list_data*), struct list_data* (*list_next)(struct list_data*)) {
    /* get current context's head */
    /* assigns currently selected item pointer to temporary */
    struct list_data* tmp = *current;
    /* if not already freed */
    if (tmp != 0) {
        /* until we found element with no next node (not a list element) */
        do {
            /* gets temporary pointer value */
            struct list_data* ptr = tmp;
            /* gets prev pointer value */
            struct list_data* next = list_next(ptr);
            /* frees up memory, should check for 0 before execution */
            list_delete(ptr);
            /* advances temporary pointer value to the next item */
            tmp = next;
        } while (tmp != 0);
        /* all stack items are processed */
        *current = 0;
    }
}