#include "common/alloc.h"
#include "list-micro/data.h"

/* private */
static struct list_data* list_next(struct list_data* ptr);
static void* list_data(struct list_data* ptr);
static void list_delete(struct list_data* ptr);
static void list_push(struct list_data** current, void* payload);
static void* list_pop(struct list_data** current);
static void* list_peek(struct list_data** current);
static void list_init(struct list_data** current);
static void list_destroy(struct list_data** current);
#ifdef USE_MEMORY_DEBUG_INFO
static void list_print_head(struct list_data** current);
static void list_print(struct list_data** current);
#endif

/* size of a memory block to allocate */
static const size_t _size = sizeof(struct list_data);

/* allocates memory pointer */
static struct list_data* _new(void) {
    /* returns list object */
    return global_alloc(_size);
}

/* releases memory pointer */
static void _delete(struct list_data* ptr) {
    /* releases the pointer */
    global_free(ptr, _size);
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

/* pushes the memory pointer */
static void list_push(struct list_data** current, void* payload) {
    const struct list_data* tmp = *current;
    /* checks if pointer is not null */
    if (tmp != 0) {
        /* creates empty data chunk */
        struct list_data* item = _new();
        /* writes data into allocated memory buffer */
        item->data = payload;
        /* assigns item's next pointer to current pointer */
        item->next = *current;
        /* advances position of head pointer to the new head */
        *current = item;
    }
}

/* pop existing element at the top of the stack/queue/list */
static void* list_pop(struct list_data** current) {
    const struct list_data* tmp = *current;
    /* checks if pointer is not null */
    if (tmp != 0) {
        /* gets the current memory pointer */
        struct list_data* ptr = *current;
        /* gets next pointer */
        struct list_data* next = list_next(ptr);
        /* if we call method on empty stack, do not return head element, return null element by convention */
        if (next == 0) {
            /* returns default element as null element */
            return 0;
        }
        /* rewinds head pointer to next pointer value */
        *current = next;
        /* gets temporary pointer value */
        void* payload = list_data(ptr);
        /* releases memory */
        list_delete(ptr);
        /* returns removed element */
        return payload;
    }
    /* if we call method on empty stack, do not return head element, return null element by convention */
    return 0;
}

/* peeks existing element at the top of the stack/queue/list */
static void* list_peek(struct list_data** current) {
    const struct list_data* tmp = *current;
    /* checks if pointer is not null */
    if (tmp != 0) {
        /* gets the current memory pointer */
        struct list_data* ptr = *current;
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

#ifdef USE_MEMORY_DEBUG_INFO

/* prints head on current context (stack) */
static void list_print_head(struct list_data** current) {
    /* get current context's head */
    struct list_data* ptr = *current;
    /* visualize item */
    printf("   *: %016llx > %016llx\n", (u64)ptr, (u64)list_data(ptr));
}

/* prints all stack trace to output */
static void list_print(struct list_data** current) {
    /* sets the counter */
    int i = 0;
    /* assigns current's head pointer to the temporary */
    struct list_data* tmp = *current;
    if (tmp != 0) {
        /* until we found root element (element with no previous element reference) */
        do {
            /* debug output of memory dump */
            printf("%4d: %016llx * %016llx\n", ++i, (u64)tmp, (u64)list_data(tmp));
            /* remember temporary's prior pointer value to temporary */
            tmp = list_next(tmp);
        } while (tmp != 0 /*root*/);
    }
    /* stop on root element */
}

#endif

/* public */

const struct list list_micro_definition = {
    /* generic methods */
    .init = list_init,
    .destroy = list_destroy,
    /* list methods */
    .push = list_push,
    .pop = list_pop,
    .peek = list_peek,
#ifdef USE_MEMORY_DEBUG_INFO
    .print_head = list_print_head,
    .print = list_print
#endif
};
