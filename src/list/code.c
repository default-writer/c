#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api.h"
#include "data.h"
#include "common/object.h"
#include "common/print.h"

/* push new item to existing context */
/* at current context, new item will be added as next element */
/* for the new item, add current head as previous element */
/* as a result, head will advances to new position, represented as new item */
struct list_data* list_push(struct list_data** const current, struct list_data** const item) {
    if (item == 0 || *item == 0) {
        return 0;
    }
    /* get current context's head */
    struct list_data* head = *current;
    /* assigns item pointer to head's next pointer value */
    head->next = *item;
    /* assigns item's prev pointer to head pointer */
    (*item)->prev = *current;
    /* advances position of head pointer to the new head */
    *current = *item;
    /* return previous context's head */
    return head;
}

/* allocates a memory for provided payload  */
/* at current context, data payload stored at allocated memory buffer */
/* as a result, items counter will increase */
void list_alloc(struct list_data** const current, void* payload) {
    /* stores into pre-allocated value newly allocated memory buffer pointer */
    struct list_data* tmp = NEW(sizeof(struct list_data));
    /* sets the new data into allocated memory buffer */
    tmp->payload = payload;
    /* pushes new item on top of the stack in current context */
    list_push(current, &tmp);
}

/* pop existing element at the top of the stack/queue/list */
/* at current context, existing head will be removed out of stack */
/* for the new stack header, correcponding values will be fixed */
/* as a result, header will be set to previous position, represented as head's reference to previos head */
struct list_data* list_pop(struct list_data** const current) {
    /* get current context's head */
    struct list_data* head = *current;
    /* if we call method on empty stack, do not return head element, return null element by convention */
    if (head == 0 || head->prev == 0) {
        /* returns default element as null element */
        return 0;
    }
    /* gets previos pointer */
    struct list_data* prev = head->prev;
    /* detouches prev pointer to next to it */
    prev->next = 0;
    /* assigns current stack head pointer to temporary */
    /* gets temporary pointer value */
    struct list_data* ptr = head;
    /* detouches the pointer from the list */
#ifdef USE_MEMORY_CLEANUP
    ptr->prev = 0;
    ptr->next = 0;
#endif
    /* rewinds head pointer to previous pointer value */
    *current = prev;
    /* returns removed element */
    return ptr;
}

/* peek existing element at the top of the stack/queue/list */
/* at current context, existing head */
struct list_data* list_peek(struct list_data** const current) {
    /* get current context's head */
    struct list_data* head = *current;
    /* if we call method on empty stack, do not return head element, return null element by convention */
    if (head == 0 || head->prev == 0) {
        /* returns default element as null element */
        return 0;
    }
    /* assigns current stack head pointer to temporary */
    struct list_data* tmp = head;
    /* returns head element */
    return tmp;
}

/* frees up memory assigned for allocation of items at current position */
/* at current context, all data needed to be claimed, will be freed */
/* as a result, all items, starting from specified item, will be deleted */
void list_free(struct list_data** const current, struct list_data** const item) {
    /* assigns currently selected item pointer to temporary */
    struct list_data* tmp = *item;
    /* until we run out of stack or stop at head element */
    if (tmp != 0) {
#ifdef USE_MEMORY_CLEANUP
        /* zero all pointers */
        tmp->prev = 0;
        tmp->next = 0;
        tmp->payload = 0;
#endif
        /* free temporary pointer value */
        FREE(tmp);
    }
    /* all stack items are processed */
    *item = 0;
}

const struct list_methods list_methods = {
    .alloc = list_alloc,
    .push = list_push,
    .pop = list_pop,
    .peek = list_peek,
    .free = list_free,
    .init = list_init,
    .destroy = list_destroy
};

