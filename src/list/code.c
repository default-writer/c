#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "std/api.h"
#include "list/data.h"
#include "common/alloc.h"
#include "common/object.h"
#include "common/print.h"

size_t size()
{
    return sizeof(struct list_data);
}

struct list_data* new()
{
    return _list_alloc(1, size());
}

struct list_data* next(struct list_data *ptr)
{
    if (ptr == 0)
    {
        return 0;
    }
    return ptr->prev;
}

void* data(struct list_data* ptr)
{
    if (ptr == 0)
    {
        return 0;
    }
    return ptr->payload;
}

void delete(struct list_data* ptr)
{
    if (ptr != 0)
    {
        _list_free(ptr);
    }
}

/* allocates a memory for provided payload  */
/* at current context, data payload stored at allocated memory buffer */
/* as a result, items counter will increase */
struct list_data* list_alloc(void* payload) {
    /* stores into pre-allocated value newly allocated memory buffer pointer */
    struct list_data* tmp = new();
    /* sets the new data into allocated memory buffer */
    tmp->payload = payload;
    /* returns created data structure */
    return tmp;
}

/* frees up memory assigned for allocation of items at current position */
/* at current context, all data needed to be claimed, will be freed */
/* as a result, all items, starting from specified item, will be deleted */
void list_free(struct list_data** const item) {
    /* assigns currently selected item pointer to temporary */
    /* get current context's head */
    struct list_data* tmp = *item;
    if (tmp != 0)
    {
        delete(tmp);
        *item = 0;
    }
}

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
    /* points to previous node */
    ptr->prev = 0;
    /* points to next node */
    ptr->next = 0;
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

const struct list_methods list_methods = {
    .alloc = list_alloc,
    .push = list_push,
    .pop = list_pop,
    .peek = list_peek,
    .free = list_free,
    .init = list_init,
    .destroy = list_destroy
};

