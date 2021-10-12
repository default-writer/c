#define DEBUG
#define LIST_LIGHT

/* #define DIRTY */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef LIST_LIGHT
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

#endif /* LIST_LIGHT */

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
void list_push(struct list** const current, void* payload);
void* list_pop(struct list** const current);
void list_destroy(struct list** const current);

/* list vtable */
const struct list_vtable list_vt = {
    .init = list_init,
    .push = list_push,
    .pop = list_pop,
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
void list_push(struct list** const current, void* payload) {
    /* stores into pre-allocated value newly allocated memory buffer pointer */
    struct list* item = ALLOC(1, struct list);
    /* sets the new data into allocated memory buffer */
    item->payload = payload;
    /* pushes new item on top of the stack in current context */
    /* get current context's head */
    struct list* head = *current;
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
