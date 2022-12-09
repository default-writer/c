#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "std/api.h"
#include "list-light/data.h"
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
void list_push(struct list_data** const current, void* payload) {
    /* stores into pre-allocated value newly allocated memory buffer pointer */
    struct list_data* item = new();
    /* sets the new data into allocated memory buffer */
    item->payload = payload;
    /* get current context's head */
    struct list_data* head = *current;
    /* assigns item pointer to head's next pointer value */
    head->next = item;
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
    struct list_data* ptr = *current;
    /* if we call method on empty stack, do not return head element, return null element by convention */
    if (ptr == 0) {
        /* returns default element as null element */
        return 0;
    }
    /* gets previos pointer */
    struct list_data* prev = next(ptr);
    /* if we call method on empty stack, do not return head element, return null element by convention */
    if (prev == 0) {
        /* returns default element as null element */
        return 0;
    }
    /* detouches prev pointer to next to it */
    prev->next = 0;
    /* rewinds head pointer to previous pointer value */
    *current = prev;
    /* gets temporary pointer value */
    void* payload = data(ptr);
    /* free temporary pointer value */
    delete(ptr);
    /* returns removed element */
    return payload;
}

/* peek existing element at the top of the stack/queue/list */
/* at current context, existing head */
void* list_peek(struct list_data** const current) {
    /* get current context's head */
    struct list_data* ptr = *current;
    /* if we call method on empty stack, do not return head element, return null element by convention */
    if (ptr == 0) {
        /* returns default element as null element */
        return 0;
    }
    /* returns actual data */
    return data(ptr);
}

const struct list_methods list_methods_light = {
    // generic methods
    .init = list_init,
    .destroy = list_destroy,
    // list methods
    .push = list_push,
    .pop = list_pop,
    .peek = list_peek
};