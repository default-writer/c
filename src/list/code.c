#define DEBUG
#define LIST

/* #define DIRTY */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef LIST
/* address type (for debugging printf function) */
typedef long long unsigned int ADDR;

/* Define a custom `malloc` function. */
void* _calloc(size_t nmemb, size_t size)
{
    void* ptr = calloc(nmemb, size);
#ifdef DEBUG
    printf("!alloc: 0x%llx :%ld\n", (ADDR)ptr, size);
#endif
    return ptr;
}

/* Define a custom `free` function. */
void _free(void* ptr)
{
    if (ptr != 0) {
#ifdef DEBUG
        printf("!free: 0x%llx\n", (ADDR)ptr);
#endif
    }
    free(ptr);
}

/* Override the default `malloc` function used by Rexo with ours. */
#define _LIST_ALLOC _calloc

/* Override the default `free` function used by Rexo with ours. */
#define _LIST_FREE _free

#endif

#ifndef _LIST_ALLOC
    #include <stdlib.h>
    #define _LIST_ALLOC malloc
#endif

#ifndef _LIST_FREE
    #include <stdlib.h>
    #define _LIST_FREE free
#endif

#define ALLOC(size, type) (type*)_LIST_ALLOC(1, sizeof(type))
#define FREE(ptr) _LIST_FREE(ptr)

#include "api.h"

/* default list methods */
void list_init(struct list** const current);
void list_alloc(struct list** const current, void* payload);
struct list* list_push(struct list** const current, struct list** const item);
struct list* list_pop(struct list** const current);
struct list* list_peek(struct list** const current);
void list_free(struct list** const current, struct list** const item);
void list_destroy(struct list** const current);

/* list vtable */
const struct list_vtable list_vt = {
    .alloc = list_alloc,
    .push = list_push,
    .pop = list_pop,
    .peek = list_peek,
    .free = list_free,
    .init = list_init,
    .destroy = list_destroy
};

/* initializes the new context's head element */
/* as a result, new memory block will be allocated */
/* current context pointer set to zero */
void list_init(struct list** const current) {
    /* sets current context's head element */
    *current = ALLOC(1, struct list);
    /* sets current context's counter to zero */
}

/* allocates a memory for provided payload  */
/* at current context, data payload stored at allocated memory buffer */
/* as a result, items counter will increase */
void list_alloc(struct list** const current, void* payload) {
    /* stores into pre-allocated value newly allocated memory buffer pointer */
    struct list* tmp = ALLOC(1, struct list);
    /* sets the new data into allocated memory buffer */
    tmp->payload = payload;
    /* pushes new item on top of the stack in current context */
    list_push(current, &tmp);
}

/* push new item to existing context */
/* at current context, new item will be added as next element */
/* for the new item, add current head as previous element */
/* as a result, head will advances to new position, represented as new item */
struct list* list_push(struct list** const current, struct list** const item) {
    if (item == 0 || *item == 0) {
        return 0;
    }
    /* get current context's head */
    struct list* head = *current;
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
struct list* list_pop(struct list** const current) {
    /* get current context's head */
    struct list* head = *current;
    /* if we call method on empty stack, do not return head element, return null element by convention */
    if (head == 0 || head->prev == 0) {
        /* returns default element as null element */
        return 0;
    }
    /* gets previos pointer */
    struct list* prev = head->prev;
    /* detouches prev pointer to next to it */
    prev->next = 0;
    /* assigns current stack head pointer to temporary */
    /* gets temporary pointer value */
    struct list* ptr = head;
    /* detouches the pointer from the list */
#ifndef DIRTY
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
struct list* list_peek(struct list** const current) {
    /* get current context's head */
    struct list* head = *current;
    struct list* tmp = head;
    /* if we call method on empty stack, do not return head element, return null element by convention */
    if (head == 0 || head->prev == 0) {
        /* returns default element as null element */
        return 0;
    }
    /* assigns current stack head pointer to temporary */
    tmp = head;
    /* returns head element */
    return tmp;
}

/* frees up memory assigned for allocation of items at current position */
/* at current context, all data needed to be claimed, will be freed */
/* as a result, all items, starting from specified item, will be deleted */
void list_free(struct list** const current, struct list** const item) {
    /* assigns currently selected item pointer to temporary */
    struct list* tmp = *item;
    /* until we run out of stack or stop at head element */
    if (tmp != 0) {
#ifndef DIRTY
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
            ptr->next = 0;
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
