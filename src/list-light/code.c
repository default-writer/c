#include "std/api.h"
#include "list-light/data.h"
#include "common/alloc.h"
#include "common/object.h"
#include "common/print.h"

struct list_data* _new() {
    /* external code allocates memory and resets memort block to zero  */
    return _list_alloc(1, size());
}

void _delete(struct list_data* ptr) {
    _list_free(ptr, size());
}

/* gets size of a memory block to allocate */
size_t size() {
    /* returns size of a memory block to allocate */
    return sizeof(struct list_data);
}

/* gets chunk's next item. external code enshures ptr is not 0 */
struct list_data* list_next(struct list_data *ptr) {
    /* external code enshures prt is not 0 */
    return ptr->next;
}

/* gets chunk's payload. external code enshures ptr is not 0 */
void* list_data(struct list_data* ptr) {
    /* external code enshures prt is not 0 */
    return ptr->payload;
}

/* allocates a memory for provided payload  */
/* at current context, data payload stored at allocated memory buffer */
/* as a result, items counter will increase */
void list_push(struct list_data** const current, void* payload) {
    /* stores into pre-allocated value newly allocated memory buffer pointer */
    struct list_data* item = _new();
    /* sets the new data into allocated memory buffer */
    item->payload = payload;
    /* get current context's head */
    struct list_data* head = *current;
    /* assigns item pointer to head's prev pointer value */
    head->prev = item;
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
    /* if we call method on empty stack, element itself is 0 */
    /* if next to element is 0 so it has no parent, called non-list element (root element) */
    /* root elements returns null, i.e. 0 by convention */
    if (ptr == 0) {
        /* returns default element as null element */
        return 0;
    }
    /* gets next pointer */
    struct list_data* next = list_next(ptr);
    /* if we call method on empty stack, element itself is 0 */
    /* if next to element is 0 so it has no parent, called non-list element (root element) */
    /* root elements returns null, i.e. 0 by convention */
    if (next == 0) {
        /* returns default element as null element */
        return 0;
    }
    /* resets prev pointer */
    next->prev = 0;
    /* rewinds head pointer to next pointer value */
    *current = next;
    /* gets temporary pointer value */
    void* payload = list_data(ptr);
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
    const struct list_data* next = list_next(ptr);
    /* if we call method on empty stack, do not return head element, return null element by convention */
    if (next == 0) {
        /* returns default element as null element */
        return 0;
    }
    /* returns actual data */
    return list_data(ptr);
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