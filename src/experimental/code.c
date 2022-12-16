#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "std/api.h"
#include "experimental/data.h"
#include "common/alloc.h"
#include "common/object.h"
#include "common/print.h"

/* gets size of a memory block to allocate */
size_t size()
{
    /* returns size of a memory block to allocate */
    return sizeof(struct list_data);
}

struct list_data* _new()
{
    /* external code allocates memory and resets memort block to zero  */
    return _list_alloc(1, size());
}

/* gets chunk's next item. external code enshures ptr is not 0 */
struct list_data* _next(struct list_data *ptr)
{
    /* external code enshures prt is not 0 */
    return ptr->next;
}

/* gets chunk's payload. external code enshures ptr is not 0 */
void* _data(struct list_data* ptr)
{
    /* external code enshures prt is not 0 */
    return ptr->payload;
}

void _delete(struct list_data* ptr)
{
    _list_free(ptr);
}

/* allocates a memory for provided payload  */
/* at current context, data payload stored at allocated memory buffer */
/* as a result, items counter will increase */
void list_push(struct list_data** const current, void* payload) {
    /* stores into pre-allocated value newly allocated memory buffer pointer */
    struct list_data* item = _new();
    /* sets the new data into allocated memory buffer */
    item->payload = payload;
    /* assigns item's next pointer to current pointer */
    item->next = *current;
    /* advances position of head pointer to the new head */
    *current = item;
}

/* pop existing element at the top of the stack/queue/list */
/* at current context, existing head will be removed out of stack */
/* for the new stack header, correcponding values will be fixed */
/* as a result, header will be set to previous position, represented as head's reference to next head */
void* list_pop(struct list_data** const current) {
    /* get current context's head */
    struct list_data* ptr = *current;
    /* if we call method on empty stack, do not return head element, return null element by convention */
    if (ptr == 0) {
        /* returns default element as null element */
        return 0;
    }
    /* gets next pointer */
    struct list_data* next = _next(ptr);
    /* if we call method on empty stack, do not return head element, return null element by convention */
    if (next == 0) {
        /* returns default element as null element */
        return 0;
    }
    /* rewinds head pointer to next pointer value */
    *current = next;
    /* gets temporary pointer value */
    void* payload = _data(ptr);
    /* free temporary pointer value */
    _delete(ptr);
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
    /* gets next pointer */
    const struct list_data* next = _next(ptr);
    /* if we call method on empty stack, do not return head element, return null element by convention */
    if (next == 0) {
        /* returns default element as null element */
        return 0;
    }
    /* returns actual data */
    return _data(ptr);
}

const struct list_methods list_methods_experimental = {
    // generic methods
    .init = list_init,
    .destroy = list_destroy,
    // list methods
    .push = list_push,
    .pop = list_pop,
    .peek = list_peek
};
