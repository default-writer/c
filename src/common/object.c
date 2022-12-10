#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "std/api.h"
#include "object.h"
#include "common/alloc.h"

/* initializes the new context's head element */
/* as a result, new memory block will be allocated */
/* current context pointer set to zero */
void list_init(struct list_data** const current, struct list_data* (*_new)()) {
    /* sets current context's head element */
    *current = _new();
}

/* destroys the memory stack */
/* frees all memory elements */
/* as a result, memory will be freed */
void list_destroy(struct list_data** const current, void (*delete)(struct list_data*), struct list_data* (*_next)(struct list_data*)) {
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
            struct list_data* next = _next(ptr);
            /* frees up memory, should check for 0 before execution */
            delete(ptr);
            /* advances temporary pointer value to the next item */
            tmp = next;
        } while (tmp != 0);
        /* all stack items are processed */
        *current = 0;
    }
}