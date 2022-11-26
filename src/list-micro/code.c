#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "api.h"

struct list_data
{
    /* points to previous node */
    struct list_data* prev;
    /* payload */
    void*  payload;
};

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
void list_push(struct list_data** const current, void* payload) {
    /* stores into pre-allocated value newly allocated memory buffer pointer */
    struct list_data* item = NEW(sizeof(struct list_data));
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
void* list_pop(struct list_data** const current) {
    /* get current context's head */
    struct list_data* head = *current;
    /* if we call method on empty stack, do not return head element, return null element by convention */
    if (head == 0 || head->prev == 0) {
        /* returns default element as null element */
        return 0;
    }
    /* gets previos pointer */
    struct list_data* prev = head->prev;
    /* rewinds head pointer to previous pointer value */
    *current = prev;
    /* assigns current stack head pointer to temporary */
    struct list_data* ptr = head;
    /* gets temporary pointer value */
    void* payload = ptr->payload;
    /* detouches the pointer from the list */
#ifdef USE_MEMORY_CLEANUP
    ptr->prev = 0;
    ptr->payload = 0;
#endif
    /* free temporary pointer value */
    FREE(ptr);
    /* returns removed element */
    return payload;
}

// print head on current context (stack)
void list_print_head(struct list_data** const current) {
    // get current context's head
    struct list_data* tmp = *current;
    // visualise item
    printf("alloc: 0x%llx 0x%llx\n", (ADDR)tmp, (ADDR)tmp->payload);
}

// print all stack trace to output
// in a single loop, print out all elements except root element (which does not have a payload)
// as a result, all stack will be printed in last-to-first order (reverse)
void list_print(struct list_data** const current) {
    // get current context's head
    struct list_data* head = *current;
    // sets the counter
    int i = 0; 
    // assigns current's head pointer to the temporary
    struct list_data* tmp = head;
    if (tmp != 0)
    {
        // until we found root element (element with no previous element reference)
        do {
            // debug output of memory dump
            printf("%d: 0x%llx 0x%llx\n", ++i, (ADDR)tmp, (ADDR)tmp->payload);
            // remember temprary's prior pointer value to temporary
            tmp = tmp->prev;
        } while (tmp != 0/*root*/);
    }
    // stop on root element
}

const struct list list_definition_private =
{
    .push = list_push, // immutable function
    .pop = list_pop, // immutable function
    .init = list_init, // immutable function
    .destroy = list_destroy, // immutable function
};

const struct list_class list_definition =
{
    .self = &list_definition_private // immutable definition
};
