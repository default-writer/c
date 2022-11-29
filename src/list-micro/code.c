#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api.h"
#include "list-micro/data.h"
#include "common/alloc.h"
#include "common/object.h"
#include "common/print.h"

struct list_data* next(struct list_data *ptr)
{
    return ptr->prev;
}

struct list_data* new()
{
    return NEW(sizeof(struct list_data));
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
    memset(ptr, 0, sizeof(struct list_data));
#endif
    /* free temporary pointer value */
    FREE(ptr);
    /* returns removed element */
    return payload;
}

const struct list_methods_micro list_methods_micro = {
    .push = list_push, // immutable function
    .pop = list_pop, // immutable function
    .init = list_init, // immutable function
    .destroy = list_destroy, // immutable function
};

const struct list_class_micro list_class_micro = {
    .methods = &list_methods_micro // immutable definition
};
