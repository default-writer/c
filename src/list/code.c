#include "std/api.h"
#include "list/data.h"
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
const void* list_data(const struct list_data* ptr) {
    /* external code enshures prt is not 0 */
    return ptr->payload;
}

/* allocates a memory for provided payload  */
/* at current context, data payload stored at allocated memory buffer */
/* as a result, items counter will increase */
struct list_data* list_alloc(const void* payload) {
    /* stores into pre-allocated value newly allocated memory buffer pointer */
    struct list_data* tmp = _new();
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
    struct list_data* ptr = *item;
    /* root elements returns null, i.e. 0 by convention */
    if (ptr == 0) {
        /* returns default element as null element */
        return;
    }
    _delete(ptr);
    *item = 0;
}

/* push new item to existing context */
/* at current context, new item will be added as next element */
/* for the new item, add current head as previous element */
/* as a result, head will advances to new position, represented as new item */
struct list_data* list_push(struct list_data** const current, struct list_data* const item) {
    if (item == 0) {
        return 0;
    }
    /* get current context's head */
    struct list_data* head = *current;
    /* assigns item pointer to head's prev pointer value */
    head->prev = item;
    /* assigns item's next pointer to current pointer */
    item->next = *current;
    /* advances position of head pointer to the new head */
    *current = item;
    /* return previous context's head */
    return head;
}

/* pop existing element at the top of the stack/queue/list */
/* at current context, existing head will be removed out of stack */
/* for the new stack header, correcponding values will be fixed */
/* as a result, header will be set to previous position, represented as head's reference to next head */
struct list_data* list_pop(struct list_data** const current) {
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
    /* points to next node */
    ptr->next = 0;
    /* points to next node */
    ptr->prev = 0;
    /* rewinds head pointer to next pointer value */
    *current = next;
    /* returns removed element */
    return ptr;
}

/* peek existing element at the top of the stack/queue/list */
/* at current context, existing head */
struct list_data* list_peek(struct list_data** const current) {
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
    const struct list_data* next = list_next(ptr);
    /* if we call method on empty stack, element itself is 0 */
    /* if next to element is 0 so it has no parent, called non-list element (root element) */
    /* root elements returns null, i.e. 0 by convention */
    if (next == 0) {
        /* returns default element as null element */
        return 0;
    }
    /* returns current pointer */
    return ptr;
}

const struct list list_definition = {
    // generic methods
    .init = list_init,
    .destroy = list_destroy,
    // list methods
    .alloc = list_alloc,
    .free = list_free,
    .push = list_push,
    .pop = list_pop,
    .peek = list_peek,
};