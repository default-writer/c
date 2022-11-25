#define DIRTY

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "api.h"

/* initializes the new context's head element */
/* as a result, new memory block will be allocated */
/* current context pointer set to zero */
void object_init(struct list** const current) {
    /* sets current context's head element */
    *current = NEW(sizeof(struct list));
    /* sets current context's counter to zero */
}

/* destroys the memory stack */
/* frees all memory elements */
/* as a result, memory will be freed */
void object_destroy(struct list** const current) {
    /* get current context's head */
    /* assigns currently selected item pointer to temporary */
    struct list* tmp = *current;
    /* if not already freed */
    if (tmp != 0) {
        /* until we found element with no parent (previous) node */
        do {
            /* gets temporary pointer value */
            struct list* ptr = tmp;
            /* gets prev pointer value */
            struct list* prev = tmp->prev;
#ifndef DIRTY
            /* zero all pointers */
            ptr->prev = 0;
            ptr->payload = 0;
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

/* object: vtable */
const struct object_class object_class_definition =
{
    .init = object_init, // immutable function
    .destroy = object_destroy, // immutable function
    .self = &object_class_definition // immutable definition
};
