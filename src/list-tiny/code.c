#define DIRTY

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "api.h"

/* default list methods */
void list_init(struct list** const current);
void list_destroy(struct list** const current);

/* default implementation */
void list_push(struct list** const current, void* payload);
void* list_pop(struct list** const current);
void* list_peek(struct list** const current);

/* list vtable */
const struct list_vtable list_vt = {
    .init = list_init,
    .destroy = list_destroy,
    .push = list_push,
    .peek = list_peek,
    .pop = list_pop
};

/* initializes the new context's head element */
/* as a result, new memory block will be allocated */
/* current context pointer set to zero */
void list_init(struct list** const current) {
    /* sets current context's head element */
    *current = NEW(sizeof(struct list));
    /* sets current context's counter to zero */
}

/* destroys the memory stack */
/* frees all memory elements */
/* as a result, memory will be freed */
void list_destroy(struct list** const current) {
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

/* allocates a memory for provided payload  */
/* at current context, data payload stored at allocated memory buffer */
/* as a result, items counter will increase */
void list_push(struct list** const current, void* payload) {
    /* stores into pre-allocated value newly allocated memory buffer pointer */
    struct list* item = NEW(sizeof(struct list));
    /* sets the new data into allocated memory buffer */
    item->payload = payload;
    /* pushes new item on top of the stack in current context */
    /* assigns item's prev pointer to head pointer */
    item->prev = *current;
    /* advances position of head pointer to the new head */
    *current = item;
}

/* pop existing element at the top of the stack/queue/list */
/* at current context, existing head will be removed out of stack */
/* for the new stack header, correcponding values will be fixed */
/* as a result, header will be set to previous position, represented as head's reference to previos head */
void* list_pop(struct list** const current) {
    /* get current context's head */
    struct list* head = *current;
    /* if we call method on empty stack, do not return head element, return null element by convention */
    if (head == 0 || head->prev == 0) {
        /* returns default element as null element */
        return 0;
    }
    /* gets previos pointer */
    struct list* prev = head->prev;
    /* rewinds head pointer to previous pointer value */
    *current = prev;
    /* assigns current stack head pointer to temporary */
    struct list* ptr = head;
    /* gets temporary pointer value */
    void* payload = ptr->payload;
    /* detouches the pointer from the list */
#ifndef DIRTY
    ptr->prev = 0;
    ptr->payload = 0;
#endif
    /* free temporary pointer value */
    FREE(ptr);
    /* returns removed element */
    return payload;
}

/* peek existing element at the top of the stack/queue/list */
/* at current context, existing head */
void* list_peek(struct list** const current) {
    /* get current context's head */
    struct list* head = *current;
    /* if we call method on empty stack, do not return head element, return null element by convention */
    if (head == 0 || head->prev == 0) {
        /* returns default element as null element */
        return 0;
    }
    /* assigns current stack head pointer to temporary */
    struct list* tmp = head;
    /* returns head element */
    return tmp->payload;
}
