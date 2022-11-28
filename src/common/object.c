#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api.h"
#include "data.h"

/* initializes the new context's head element */
/* as a result, new memory block will be allocated */
/* current context pointer set to zero */
void list_init(struct list_data** const current) {
    /* sets current context's head element */
    *current = NEW(sizeof(struct list_data));
    /* sets current context's counter to zero */
}

/* destroys the memory stack */
/* frees all memory elements */
/* as a result, memory will be freed */
void list_destroy(struct list_data** const current) {
    /* get current context's head */
    /* assigns currently selected item pointer to temporary */
    struct list_data* tmp = *current;
    /* if not already freed */
    if (tmp != 0) {
        /* until we found element with no parent (previous) node */
        do {
            /* gets temporary pointer value */
            struct list_data* ptr = tmp;
            /* gets prev pointer value */
            struct list_data* prev = tmp->prev;
#ifdef USE_MEMORY_CLEANUP
            /* zero all pointers */
            memset(ptr, 0, sizeof(struct list_data));
#endif
            /* free temporary pointer value */
            FREE(ptr);
            /* advances temporary pointer value to the next item */
            tmp = prev;
        } while (tmp != 0);
        /* all stack items are processed */
        *current = 0;
    }
}