#include "common/alloc.h"

#include "list/data.h"

/* private */

/* gets size of a memory block to allocate */
static size_t _size() {
    /* returns size of a memory block to allocate */
    return sizeof(struct list_data);
}

/* allocates memory pointer */
static struct list_data* _new() {
    // returns list object
    return _list_alloc(1, _size());
}

/* releases memory pointer */
static void _delete(struct list_data* ptr) {
    /* releases the pointer */
    _list_free(ptr, _size());
}

/* gets chunk's next item. external code ensures ptr is not 0 */
static struct list_data* list_next(struct list_data* ptr) {
    /* external code ensures prt is not 0 */
    return ptr->next;
}

/* gets chunk's payload. external code ensures ptr is not 0 */
static void* list_data(struct list_data* ptr) {
    /* external code ensures prt is not 0 */
    return ptr->data;
}

/* deletes the data pointer */
static void list_delete(struct list_data* ptr) {
    /* releases the pointer */
    _delete(ptr);
}

/* allocates memory pointer */
static struct list_data* list_alloc(void* payload) {
    /* creates empty data chunk */
    struct list_data* item = _new();
    /* writes data into allocated memory buffer */
    item->data = payload;
    /* returns created data structure */
    return item;
}

/* releases memory pointer */
static void list_free(struct list_data** current) {
    /* gets the current memory pointer */
    struct list_data* tmp = *current;
    /* checks if pointer is not null */
    if (tmp != 0) {
        /* releases memory */
        list_delete(tmp);
        /* resets current pointer to 0 */
        *current = 0;
    }
}

/* pushes new item to existing context */
static struct list_data* list_push(struct list_data** current, struct list_data* item) {
    const struct list_data* tmp = *current;
    /* checks if pointer is not null */
    if (tmp != 0 && item != 0) {
        /* gets the current memory pointer */
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
    /* if we call method on empty stack, do not return head element, return null element by convention */
    return 0;
}

/* pops existing element at the top of the stack/queue/list */
static struct list_data* list_pop(struct list_data** current) {
    const struct list_data* tmp = *current;
    /* checks if pointer is not null */
    if (tmp != 0) {
        /* gets the current memory pointer */
        struct list_data* ptr = *current;
        /* gets next pointer */
        struct list_data* next = list_next(ptr);
        /* root elements returns null, i.e. 0 by convention */
        if (next == 0) {
            /* returns default element as null element */
            return 0;
        }
        /* resets prev pointer */
        next->prev = 0;
#ifdef USE_MEMORY_CLEANUP
        /* points to next node */
        ptr->next = 0;
        /* points to next node */
        ptr->prev = 0;
#endif
        /* rewinds head pointer to next pointer value */
        *current = next;
        /* returns removed element */
        return ptr;
    }
    /* if we call method on empty stack, do not return head element, return null element by convention */
    return 0;
}

/* peeks existing element at the top of the stack/queue/list */
static struct list_data* list_peek(struct list_data** current) {
    const struct list_data* tmp = *current;
    /* checks if pointer is not null */
    if (tmp != 0) {
        /* gets the current memory pointer */
        struct list_data* ptr = *current;
        /* gets next pointer */
        const struct list_data* next = list_next(ptr);
        /* root elements returns null, i.e. 0 by convention */
        if (next == 0) {
            /* returns default element as null element */
            return 0;
        }
        /* returns current pointer */
        return ptr;
    }
    /* if we call method on empty stack, do not return head element, return null element by convention */
    return 0;
}

/* initializes the new context's head element */
static void list_init(struct list_data** current) {
    const struct list_data* tmp = *current;
    /* checks if pointer is null */
    if (tmp == 0) {
        /* sets the current memory pointer */
        *current = _new();
    }
}

/* destroys the memory stack */
static void list_destroy(struct list_data** current) {
    /* gets the current memory pointer */
    struct list_data* tmp = *current;
    /* checks if pointer is not null */
    if (tmp != 0) {
        /* until we found element with no next node (not a list element) */
        do {
            /* gets temporary pointer value */
            struct list_data* ptr = tmp;
            /* gets prev pointer value */
            struct list_data* next = list_next(ptr);
            /* releases memory, should check for 0 before execution */
            list_delete(ptr);
            /* advances temporary pointer value to the next item */
            tmp = next;
        } while (tmp != 0);
        /* resets current pointer to 0 */
        *current = 0;
    }
}

// prints head on current context (stack)
static void list_print_head(struct list_data** current) {
    // get current context's head
    struct list_data* ptr = *current;
    // visualize item
    printf("   *: 0x%016llx >0x%016llx\n", (u64)ptr, (u64)list_data(ptr));
}

// prints all stack trace to output
static void list_print(struct list_data** current) {
    // sets the counter
    int i = 0;
    // assigns current's head pointer to the temporary
    struct list_data* tmp = *current;
    if (tmp != 0) {
        // until we found root element (element with no previous element reference)
        do {
            // debug output of memory dump
            printf("%4d: 0x%016llx *0x%016llx\n", ++i, (u64)tmp, (u64)list_data(tmp));
            // remember temporary's prior pointer value to temporary
            tmp = list_next(tmp);
        } while (tmp != 0 /*root*/);
    }
    // stop on root element
}

/* public */

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
    .print_head = list_print_head,
    .print = list_print
};